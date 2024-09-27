# pico_lib

## 1. Description

<img src="readme_misc/_DSF3371_edit.jpg" align="right" width="372" height="248"/>

The primary purpose of this repository is to provide a set of core reusable low level features that enable robotic related functionality on a [Raspberry Pico 2](https://www.raspberrypi.com/products/raspberry-pi-pico-2/). This implementation can be tailored to specific robotic applications.

**Features:**

<details>
<summary>State-machine management</summary>
<br>

State management is based on the [state design pattern](https://en.wikipedia.org/wiki/State_pattern). It handles state transitions based on events. 

 * **Functionality**
   * State manager
 * **Interfaces**
   * States
   * Events
   * State transition matrix

</details>

<details>
<summary>Low-level communication</summary>
<br>

Communication is service-based, inspired by [UDS](https://en.wikipedia.org/wiki/Unified_Diagnostic_Services). It provides callbacks for each message id.

* **Interfaces**
    * Service Ids
    * Service callbacks
* **Wrappers**
    * Lock guards
    * I2C slave
    * SPI master
    * DMA access
* **Drivers**
    * CAN-FD controller and transceiver [MCP251863](https://www.microchip.com/en-us/product/MCP251863)
* **Message data structures**
    * FIFO circular buffer

</details>

<details>
<summary>MCU utilities</summary>
<br>

MCU related functions

 * **Functionality**
   * MCU software reset
   * Uart Logger
   * MCU read/write flash memory

</details>

<details>
<summary>Unit tests for core functionality</summary>
<br>

A set of unit tests for the higher level features of these repo where developed using [GoogleTest](http://google.github.io/googletest/).

</details>
