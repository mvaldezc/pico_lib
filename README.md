# pico_lib

## 1. Description

<img src="readme_misc/_DSF3371_edit.jpg" align="right" width="424" height="283"/>

The primary purpose of this repository is to provide a set of core reusable low level features that enable robotic related functionality on a Raspberry Pico 2. This implementation can be tailored to specific robotic applications.

The set of features include:
* State-machine management
* Low-level communication
* MCU utilities
* Unit tests for core functionality

In progress:
* RTOS-based architecture
* Parallel motor control
* Real-time trajectory generation
* State-Estimation
* Advanced system control methods


#### State Management
State management is based on the [state design pattern](https://en.wikipedia.org/wiki/State_pattern). It handles state transitions based on events. 
 * Functionality:
   * State manager
 * Interfaces for:
   * States
   * Events
   * State transition matrix

#### Communication
Communication is service-based, inspired by [UDS](https://en.wikipedia.org/wiki/Unified_Diagnostic_Services). It provides callbacks for each message id. 
 * Interfaces for:
   * Service Ids
   * Service callbacks
 * Wrappers for:
   * Lock guards
   * I2C slave
   * SPI master
   * DMA access
 * Drivers for:
   * CAN-FD controller and transceiver [MCP251863](https://www.microchip.com/en-us/product/mcp251863)
 * Message data structures
   * FIFO circular buffer

#### Utilities
 * Functionality:
   * MCU software reset
   * Uart Logger
   * MCU read/write flash memory


