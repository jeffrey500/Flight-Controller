# Flight-Controller V1.0
<img src="media/Board_front.png" width="49%" alt="CUBEMX"> <img src="media/Board_back.png" width="49%" alt="CUBEMX">
The purpose of this project was to gain experience working on a 4-layer PCB layout with Altium and working with the ARM STM32 family of microcontrollers.  
Communication protocols such as SPI, UART, I2C, CAN, and USB were explored.  
Going forward, this board will serve as a testbed for the testing of control algorithms and can interface with an external flight computer such   
as a jetson nano for tasks such as computer vision and advanced autonomous flight. 

This board will also be designed to optionally run the [Ardupilot](https://ardupilot.org/) firmware for autonomous flight and initial testing.  
This board can also support the DJI Air unit system through the VTX connector.

This board is a part of my interest to building the entire hardware and software stack for a drone (Project Drone Full Stack PDFS).

## Features

| Feature                   | Capability                            |
|---------------------------|---------------------------------------|
| Processor                 | STM32H743VIT6                         |
| Clock Speed               | 480MHz                                |
| Flash Memory              | 2 MB                                  |
| PWM/D-Shot                | 8 (4 TIM2 & 4 TIM4)                   |
| Inertial Measurement Unit | BMI270                                |
| Barometer                 | BMP388                                |
| UART (external)           | 5 (USART1 USART2 USART3 UART4 USART6) |
| SPI  (external)           | 1 (SPI4)                              |
| CAN bus                   | 1                                     |
| Camera Feed               | Digital                               |
| I2C                       | Row 2, Cell 2                         |
| DAC                       | 1                                     |
| Power                     | 7-35V                                 |
| Debugging                 | Serial Debug                          |
| Serial                    | USB-C                                 |
| Logging                   | Micro-SD Card                         |

### Connectors
The external connectors for UART, SPI, CAN, and DAC are JST-GH for secure latching connections.  
The PWM/D-Shot uses 2.54mm pitch male header pins in 3 rows of 8 pins.  
An O3 or O4 DJI Air Unit can connect through the 6-pin JST-SH VTX connector.

## CubeMX
The [STM32CubeMX Software](https://www.st.com/en/development-tools/stm32cubemx.html) was used to determine the power and pinout for the STM32H743VIT6.  
See the [CubeMX .ioc file](/FC.ioc)

<img src="media/CUBEMX.png" width="80%" alt="CUBEMX">

## Schematic
<img src="media/Schematic.png" width="100%" alt="Schematic">

## Board Stackup
Signal Ground Ground Signal (S-G-G-S) was chosen over Signal Ground Power Signal (S-G-P-S) as both signal layers are shielded from each other by a close ground plane.  
Standard copper pour is used for signal layers as well as thorough ground stitching.

| Layer | Purpose |
|-------|---------|
| 1     | Signal  |
| 2     | Ground  |
| 3     | Ground  |
| 4     | Signal  |

### Top and Bottom signal layers respectively with copper pours
<img src="media/Layer1_pour.png" width="46%" alt="Layer1_pour"> <img src="media/Layer4_pour.png" width="49%" alt="Layer4_pour">

### Internal Ground Plane 2 and 3 respectively   
<img src="media/Layer2_pour.png" width="46%" alt="Layer1_pour"> <img src="media/Layer3_pour.png" width="49.4%" alt="Layer4_pour">

## Routing

### Power
Power is routed through the board using thick PCB traces and many vias. The Board uses a LMR16020 step down simple switcher to get 5V, used to power external peripherals such as radio and GPS.   
The TPS7A2033PDBVR low dropout regulator is used to further reduce the voltage from 5V to 3.3V for the STM32H743 and onboard peripherals.  
Such power components were placed in the top right corner to try and isolate the noise they would generate.

### Ground Vias
Anytime a signal traveled between layers, a corresponding ground reference via was placed.

### Signals
Faster or more critical signals were routed first such that they had minimal trace length and travel between layers.   
USB was routed with a differential pair.

Routing Priority: USB, SDMMC, SPI, DAC, CAN, UART, I2C, PWM, GPIO.  

Impedance matching was not done for SDMMC due to a lower speed requirement but length matching (>10mm) was taken into account. 

## Mechanical
Dimensions 41 mm* 55.9 mm. 1.6mm Board thickness. Four M3 mounting holes on the corners of the board. 

## Ardupilot

The Ardupilot bootloader and firmware can be flashed by using an ST-LINK and Serial Debug pins at the bottom right of the board.  

See the attached [Binary Bootloader File](/Ardupilot%20Firmware/AP_Bootloader.bin) and [Binary Firmware File](/Ardupilot%20Firmware/AP_Bootloader.bin).  
See the attached hardware definition files [hwdef.dat](/Ardupilot%20Firmware/hwdef.dat) and [hwdef-bl.dat](/Ardupilot%20Firmware/hwdef-bl.dat) that can be built using the [Ardupilot Repository](https://github.com/Ardupilot/ardupilot).

## Next Iteration
- Input Capacitor Needs higher voltage rating so it can accommodate 6s lipo battery
- Connect both 5.1k usb resistors
- Re-align the SD-card properly
- 2 pin JST connector for main power
- Buzzer on a proper PWM channel or active buzzer
- Reorder the Motor driver pins
- Better Silkscreen
  - PWM pins
  - Align text to be readable horizontally or on the right Purpose of connectors
  - Serial Debug
- Less long-parallel wiring which could resulted in the coupling of signals
  - Better initial planning of component layout
- The LMR16020 better layout, rms rated caps and inductor. Currently, causing high thermal output.

## License
[Flight-Controller V1.0](https://github.com/jeffrey500/Flight-Controller) © 2026 by [Jeffrey Zhu](https://jzhu.ca) is licensed under a [Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License](https://creativecommons.org/licenses/by-nc-sa/4.0/).

[![CC BY-NC-SA 4.0](https://licensebuttons.net/l/by-nc-sa/4.0/88x31.png)](https://creativecommons.org/licenses/by-nc-sa/4.0/)