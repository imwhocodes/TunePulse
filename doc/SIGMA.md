# SIGMA

## Research

Task of designing robust and expandable control system is not trivial. Controller should be able to perform low level computations for fast response control loops, as well as slow and deliberate computations for processing user input and managing changes in the required control law.

There is a lot of similarities with designing a control system of a robot, so in the process of search for the solution a number of research papers on robot architectures were explored.

### ALFA - a language for action

> Gat, E., 1991. ALFA: a language for programming reactive robotic control systems, in: Proceedings. 1991 IEEE International Conference on Robotics and Automation. Presented at the 1991 IEEE International Conference on Robotics and Automation, IEEE Comput. Soc. Press, Sacramento, CA, USA, pp. 1116–1121. https://doi.org/10.1109/ROBOT.1991.131743

ALFA language is a big inspiration for chosen architecture. This language facilitates idea of separate computational **modules**, each managing it's own little and concrete task, which connect with each other via **communication channels**.

#### Modules

Each module consists of a number of **methods**, each describes the transfer function which it computes. Each module contains minimal internal state, it's outputs defined almost entierly by it's inputs, which makes module design and testing very simple.

Discrete changes in system behaviour achieved by swapping different modules in and out of the system.

In theory each method runs in parallel, so that they in essense describe not in which order do which operation, but how data flows from inputs of a module to it's outputs.

#### Channels

Modules communicate between each other and with outside world via means of **communication channels**. A channel is a computational entity which takes multiple inputs and combines them in some predetermined way to produce a single output.

#### Discussion

Use of communication channels allows designer to:
1. lay out overall system structure using channels,
2. abstract out source of signals of interest and focus only on modules' internal design and testing.


### ATLANTIS - A Three-Layer Architecture for Navigating Through Intricate Situations

> Gat, E., n.d. Integrating Planning and Reacting in a Heterogeneous Asynchronous Architecture for Controlling Real-World Mobile Robots.

ATLANTIS robot architecture is a three layer architecture, designed to control rover-like robot for robust navigation through terrain with different obstacles in the way.

Architecture consists of three layers:
1. Controller,
2. Sequencer,
3. Deliberator.

#### Controller

Controller layer implements reactive control system, which is responsible for controlling primitive activities, such as servo control loops and a number of another sensorimotor processes.

This layer is built using ALFA language and fully exploits it's benefits.

#### Sequencer

This layer manages primitive actions by swapping different modules in Controller layer, as well as managing deliberate computations in Deliberator layer.

#### Deliberator

This layer performs deliberate and time consuming computations, required for long term planning and pathfinding.

#### Discussion

In the process of a research following statements, regarding our application of building robust stepper motor control system, were drawn:
1. **Control system should be heterogeneous.** There is no benefit to restrict ourselves to singular code structure to perform different task, such as debug logging, managing user input and fast servo control. Each task should be managed by it's own code.
2. **Control system should be asynchronous.** Slow computations should be performed in parallel with fast ones to allow fast reaction to contingencies.
3. **Control system should be designed bottom up.**

## Control system overview

Based on our research following design decisions were made.

### Controller layer

Low level layer is built after ALFA's ideology of communication channels and computational modules. In our architecture there are following building blocks:
1. **Communication channel.** Main communication mean in controller layer. Single Channel can hold singular value, at any time can be written to by only one Module, and be read from by any number of Modules. Single Module cannot read and write to the same Channel. <!-- It is volatile global variable, which can be referenced by any module and can be written to by any module, however there are (verbal for now) restrictions on which module can write to which channel. Also, due to nature of choosen Channel implementation only one Module at a time should write in the Channel. -->
1. **Selector.** This block can be used to create discrete changes in system behaviour. It can be initialized with a number of Modules, which write to same Channels and perform 'similar' functions. At any moment only one Module can be active, which ensures Channel write rules established earlier. Selector control also implemented via separate Channel, which holds information on which Module should be used.
1. **Computational module.** This block is analogues to computational module in ALFA. 
It performs predefined computations on data from input Channels and writes data to output Channels. The Module should have minimal to no internal state and should not contain any decision making logic.
Also some Modules contain device Drivers, which allows Module to read data from external sensors and control external devices such as PWM motor driver.
1. **Computational block.** This block represents basic building block which can be used inside Module to create complex behaviours and perform calculations. It can't write/read to/from Channels, and can be used only inside Module. PID regulator, velocity estimator, IIR/FIR filter all falls down in this category.

#### Multithreading

Motor control system is a complex system, containing a lot of feedback loops, some of which is slow (such as position control), some of which is fast (such as current control). Since these tasks require different levels of decision-making speed and sensor feedback, it makes sense to split them into separate "threads".

### Sequencer layer

The Sequencer is responsible to control overall system's behaviour by selecting appropriate Controller modes at appropriate times with approptiate setpoints.

The decision has been made to limit the control over the Controller layer to Channels, which will allow the Sequencer to change the setpoints and select the required Selectors' modes of operation.

This decision is motivated by intention to avoid creating special control interfaces to manage Controller. That way Sequencer is able at any point read any Channel for decision making or debug purposes as well as the ability to asyncronously change Controller behaviour while not interrupting it's operation.

## Implementation

### Channels

Channels implemented as global volatile variables, separated in different namespaces. Read and write access to Channels should be performed only in guarded critical code sections, which ensures that data will not be changed while read/write in progress.

For safety reasons each channel is REQUIRED to have default value, which should be always written to a channel if no other source is available, and all Modules reading from the Channel should check for this value to be aware if data is valid or not.

Also for the ease of debugging, each read and write operation to a Channel should be performed with the use of dedicated macro.

### Modules

Every Module is a class defining input and output Channels as well as formulae, connecting inputs to outputs.

To ensure Channel write safety Channels divided into "Write groups", each corresponding to distinct task, i.e. group, carying required voltages of phases A and B: `VOLTG_SIN` and `VOLTG_COS`.

Each Module can belong to only a single Write group.

Also, no matter what, only one write per iteration is allowed to the given channel. So Module could read and write to the same Channel, only and only if this Module is the only one writing to the Channel.

#### Digression
To be able to quickly modify the code in the future, any Module can be replaced with a similar Module with modified logic or Selector, provided that its output Channels AT LEAST contain Channels of the original Module

### Selector

Selector is a special type of Module. It can't perform calculations on its own, however it can contain a group of Modules, belonging to the same Write group. Also Selector has control Channel, which is used to select which Module should be in use at any given time.

Only one Selector per Write group is allowed. This rule guarantees Channels write safety.
