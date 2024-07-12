# SIGMA - S*calable* I*somorphic* G*uidance* M*odular* A*rchitecture*

## Overview

The SIGMA (**S**calable **I**somorphic **G**uidance **M**odular **A**rchitecture) is designed to address the challenge of creating a robust and expandable control system. 

It aims to facilitate and harmonize both low-level computations for fast response control loops and high-level computations for processing input and managing changes in control laws while enabling seamless real-time control across various applications.
 
### Key Concepts

1. **Scalable**: The system can expand and adapt to increasing control system demands without restructuring its core structure.

2. **Isomorphic**: System components are decoupled and has same control interface, allowing flexible usage or omission without causing errors and making system easier to understand and maintain.

3. **Guidance**: Designed for control systems with multiple components working as a unified entity, ensuring real-time coordinated operations within diferent modes.

4. **Modular**: Composed of interchangeable modules, facilitating easy customization and upgrading of the system without disrupting overall functionality.

## Research

Task of designing robust and expandable control system is not trivial. Controller should be able to perform low level computations for fast response control loops, as well as slow and deliberate computations for processing user input and managing changes in the required control law.

There is a lot of similarities with designing a control system of a robot, so in the process of search for the solution a number of research papers on robot architectures were explored.

<details>
  <summary>If you're interested, here are sources, excerpts and conclusions on which we built this architecture</summary>

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

</details>

## SIGMA Architecture Details

Based on our research, the system architecture was divided into two levels:

1. **Controller Layer:**
    - Responsible for low-level computations for fast response control loops.
    - Does not defines any behavor selection logic; it simply computes the given task.
    - Defines the mathematical implementation of algorithms and data interaction.
2. **Sequencer Layer:**
    - Responsible for controlling the overall system behavior and processing input.
    - Defines the operational logic of the algorithms by selecting the Controller modes with the appropriate setpoints.
    - Ensures system synchronyzation by activation of appropriate blocks at the appropriate times.

------

### Controller Layer

The Controller Layer is responsible for all mathematical implementations of the Control System's algorithms and work with external low-level perepherias. Its architecture was partially inspired by ALFA's ideology of communication channels and computational modules. However, certain modifications were made to enhance the code's modularity and reliability and to improve compatibility with real-time systems.

In SIGMA architecture, the Controller Layer consists of five building blocks:

1. **`Communication Channel`:** The primary means of communication in the Controller layer, facilitating interaction between system components.
	- Must be visible/accessable to all parts of the system.
	- Each `Channel` can hold only one value.
	- During each computation cycle, it can be changed only once by a single `Module`.
	- Should have a default value, which is always written to the `Channel` if no other source is available.
2. **`Compute Block`:** A basic building block used for complex calculations, such as executing a PID regulator.
	- Cannot write to or read anything from `Channels`.
	- Should have minimal internal state.
3. **`Compute Module`:** Performs predefined computations on data from input `Channels` and writes results to output `Channels`.
	- Must have a function to update data.
	- Should have minimal internal state.
	- Can use `Computate Blocks` to perform calculations.
	- Should not contain any decision-making logic.
4. **`Driver Module`:** Contains device drivers, enabling interaction with peripherals, external devices, and other systems, such as collecting data from external sensors and driving actuators.
	- Must have a function to update data.
	- May not have input or output `Channels`.
	- Can have any internal logic for processing values.
5. **`Selector Module`:** Allows discrete changes in system behavior by switching between `Modules` that share the same output `Channels`.
	- Can be initialized with any number of `Modules` that write to the same `Channels` (Write Group) and perform similar functions.
	- Has a single input `Channel` that determines which internal `Module` should be used.
	- At any moment, only one of the `Selector's` internal `Modules` can be active.
	- Must have a function to invoke the active internal `Module`.
	- Cannot contain any mathematical operations.
	- If the `Modules` have different `Channels` besides the common ones, unused `Channels` should take on safe values when switching `Modules`.

#### Important Aspects

- **System integrity.** A key rule is that only one write per iteration is allowed to a given `Channel`. This rule ensures code execution safety, increases performance speed, and prevents unpredictable behavior. It also means that a `Module` can read and write to the same `Channel` only if it is the sole module writing to that `Channel` per iteration and it make it once.

- **Predictable execution time.** An important property of the controller layer is that its execution time remains constant for all combinations of `Selector` modes. This does not mean the code will always execute in a fixed time but implies that for the same combination of all `Selector's` states, regardless of other valid input parameters, the code will perform a fixed number of mathematical operations. Knowing the execution time of each module makes it straightforward to calculate the total code execution time, which is extremely useful for real-time operating systems (RTOS).

- **Multithreading.** Complex systems may contain a lot of feedback loops, some of which is slow (such as position control), some of which is fast (such as current control). Since these tasks require different levels of decision-making speed and sensor feedback, it makes sense to split them into separate "threads" - groups of `Channels` with same update frequincy. 

------

### Sequencer layer

The Sequencer is responsible to control overall system's behaviour by selecting appropriate Controller modes at appropriate times with approptiate setpoints.

The decision has been made to limit the control over the Controller layer to Channels, which will allow the Sequencer to change the setpoints and select the required Selectors' modes of operation.

This decision is motivated by intention to avoid creating special control interfaces to manage Controller. That way Sequencer is able at any point read any Channel for decision making or debug purposes as well as the ability to asyncronously change Controller behaviour while not interrupting it's operation.

------

## Implementation [to be updated]

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
