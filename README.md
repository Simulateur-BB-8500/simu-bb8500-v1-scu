# Description

The **Simulator Control Unit** (SCU) is the central processor of the system. This is the master unit of the simulator which handles the following functions:

* **Read the dashboard inputs** (switchs and levers activated by the train driver).
* **Control the dashboard outputs** (displays, gauges, light indicators, etc.).
* Implement the **simulation algorithms** to recreate the real locomotive behaviour.
* **Send the commands to the SGDU** when a dashboard action requires a soundtrack and/or needs to be transmitted to the game.
* **Receive and process the game data sent by the SGDU**, such as the train speed and the speed limit.

# Hardware

The project is based on a **Nucleo-F746ZG** board.

Excepts for the basic inputs which are directly connected to the MCU, most of the dashboard modules are detected or controlled through a dedicated circuit: in the V1 version of the simulator, there are **32 through-hole PCBs making the interface between the dashboard and the MCU** (power drivers, logic adapters, etc.).

# Embedded software

## Environment

As of version `sw0.1` the embedded software is developed under **Eclipse IDE** version 2020-09 (4.17.0) and **GNU MCU** plugin. The `script` folder contains Eclipse run/debug configuration files and **JLink** scripts to flash the MCU.

> [!WARNING]
> To compile any version under `sw2.0`, the `git_version.sh` script must be patched when `sscanf` function is called: the `SW` prefix must be replaced by `sw` since Git tags have been renamed in this way.

## Target

The Nucleo board is based on the **STM32F746ZG** microcontroller of the STMicroelectronics F7 family. Each hardware revision has a corresponding **build configuration** in the Eclipse project, which sets up the code for the selected board version.

## Structure

The project is organized as follow:

* `drivers` :
    * `device` : MCU **startup** code and **linker** script.
    * `registers` : MCU **registers** address definition.
    * `peripherals` : internal MCU **peripherals** drivers.
    * `components` : external **components** drivers.
    * `utils` : **utility** functions.
* `middleware` :
    * `interface` : **SGDU** interface driver.
    * `simulator` : **locomotive simulation algorithms** implementation.
* `application` : Main **application**.
