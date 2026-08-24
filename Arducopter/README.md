## Ardupilot

The Ardupilot bootloader and firmware can be flashed by using an ST-LINK and Serial Debug pins at the bottom right of the board.  

See the attached [Binary Bootloader File](/Ardupilot%20Firmware/AP_Bootloader.bin) and [Binary Firmware File](/Ardupilot%20Firmware/AP_Bootloader.bin).  
See the attached hardware definition files [hwdef.dat](/Ardupilot%20Firmware/hwdef.dat) and [hwdef-bl.dat](/Ardupilot%20Firmware/hwdef-bl.dat) that can be built using the [Ardupilot Repository](https://github.com/Ardupilot/ardupilot).

## Ardupilot Build
```bash
cd ardupilot/
./waf clean
./waf configure --board MyCustomFC
./waf copter
```