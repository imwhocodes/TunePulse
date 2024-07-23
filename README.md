# TunePulse Firmware

[![License](https://img.shields.io/badge/LICENSE-Apache_2.0-blue.svg)](https://github.com/creapunk/TunePulse/blob/main/LICENSE)
[![PlatformIO](https://img.shields.io/badge/PIO-white?logo=platformio&logoColor=%23F5822A)](https://platformio.org/)
[![Discord](https://img.shields.io/discord/1098363068435681290?style=social&logo=discord&label=COMMUNITY)](https://discord.gg/V4aJdTja8v)
[![Ko-fi](https://img.shields.io/badge/Support%20on%20Ko--fi-F16061?style=flat&logo=kofi&logoColor=white&labelColor=%23FF5E5B)](https://ko-fi.com/creapunk)

**TunePulse** is an upcoming open-source firmware designed to

- control a wide range of motors (`DC`, `STEPPERS`, `BLDC`, `PMSM`, `LINEAR`)
- across various control modes (`CLOSED-LOOP`, `FOC`, `POSITION`, `SPEED`, `TORQUE`, etc.)
- featuring compatibility with multiple interfaces (`STEP-DIR`, `UART`, `I2C`, `CAN/CAN-FD`, `USB`)
- and support for various protocols (`GCODE`, `KLIPPER`, etc.).

Its primary goal is to advance the development of closed-loop systems and simplify their integration into existing systems.

> **Note: This firmware is under heavy development and currently has no documentation.
> For more information, join the creapunk community [Discord](https://discord.gg/V4aJdTja8v).**

## SETUP

### Applications:

- STMCubeProgrammer - https://www.st.com/en/development-tools/stm32cubeprog.html
- VSCode - https://code.visualstudio.com
- PlatformIO - (Install as VSCode extension, see image below)  
  ![PIO Extension](./assets/PIO_Extension.png)

### First Upload:

To generate the binary, pull the TunePulse repository from github. Be sure to switch to a branch that has a bootloader available if you want to remove the programmer from the loop. Make sure the PlatformIO extension is installed for building.

Use the command pallete and run the command `PlatformIO: Pick Project Environment`. Be sure to pick the correct target device. Once done, you can run the `PlatformIO: Build` to generate target binaries. These binaries will be in the local directory `.pio/build/target_release/firmware.bin`.

The first binary has to be flashed using the STM Programmer. Attach a USB cable from a computer to the flashing hardware into the port that says ST-LINK. Then connect the single usb-c port to the driver board using the full function usb-c cable (direction matters here, if the next step fails, flip the cable).

To flash, open up STM32CubeProgrammer applciation. In the screenshot below, the connect button is marked 1. Click that and ensure the device connects properly. If it fails, flip the usb-c cable and try again. You will know it is connected if you see hex data fill up the window. Next click the botton on the left labeled 2 in the screenshot below.

![STM32CubeProgrammerWindow1](./assets/STM32Programmer.png)

In this next menu, simply browse for your binary file from the path above, and click the Start Programming button. This should have properly flashed the device and you are all set for further testing.

### DFU from VSCode

Once firmware with DFU boot is uploaded onto the device, the STLink programmer can be removed and you can connect directly over USB-C. This currently is set up to be triggered if the SW1 Button is held down during power up. If done correcly, the blue and green LEDs should be showing, and if you check the device manager on the computer it should have a `DFU in FS Mode` under the `Universal Serial Bus devices` as shown below.

![DFU Boot in Device Manager](./assets/Device_Manager_DFU.png)

Next in VSCode, we can set up flashing to this device with the command: `PlatformIO: Set Project Port (upload/monitor/test)`. By default, if there are no other devices, you will want to choose the option `Custom`, and enter `USB1`. This may be a higher USB number if you have multiple attached simultaneously. You can check what this is through the STM32CubeProgrammer if you choose the USB option of connecting, and refresh the port list.

Once this is completed, simply running `PlatformIO: Upload` will upload the built code straight onto the device.

## Target defines

For specific targets:

```c
#define TARGET_CLN17_V1_5 # CLN17 v1.5
#define TARGET_CLN17_V2 # CLN17 v2
```

For specific features:

```c
// #define FEATURE_CANFD # CAN FD communication
```

## Contribution

See `CONTRIBUTION.md`
