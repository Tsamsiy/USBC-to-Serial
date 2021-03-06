# USB typeC to Serial converter with PowerDelivery

[work in progress project]

This Project combines the common USB-UART bridges known in the Arduino community with a more capable power supply.
The USB type C architecture is ideal for this job since it features data connectivity and power delivery all in one connector.

This module is based on the CP2102N and the STUSB4500. 
The CP2102N is the USB-UART bridge. This chip relatively popular and cheaper compared to other products.
The STUSB4500 is a standalone USB PowerDelivery controller. This enables the module to sink voltages greater than 5V when connected to systems that feature PD.

Settings on the PD controller are changed by the on-board ATTiny841 microcontroller.
By pressing the mode-button on the board, one can cycle through a set of predefined voltages.
LED indicators on the board will schow which voltage is present on the VSNK output.

The MCU is also interfaced with the modules own UART interface. This allows the user to customize the PD settings via AT-Commands.
When the module is not in "AT mode", the MCU's UART port is disabled.
This direct connection is also used to change the MCU's firmware without needing an external adapter.

# Hardware V1.0.1
![IMG_20210406_214657](https://user-images.githubusercontent.com/49845327/114308589-cdbf0100-9ae4-11eb-855c-54cde379152d.jpg)
![IMG_20210406_214722](https://user-images.githubusercontent.com/49845327/114308629-e202fe00-9ae4-11eb-9fad-4c9d3fbb45a6.jpg)
note: an error occured while uploading gerbers to the PCB manufacturer's website.
This specific PCB had to be manually altered to make it work

# Current status
- schematics completed
- PCB layout completed
- PCBs ordered
- parsing algorithm functional
- PCB assembly completed
- USB-UART sub-module tested
- MCU sub-module tested
- PD sub-module tested
- mode button firmware fuctional (no AT mode)
- configured TXT and RXT LEDs using Simplicity Studio Xpress configurator

# To do
- reduce size of pasring algorithm
- [next version] add connection between data pairs 1 and 2 on the USB socket
- [next version] correct wrong polarity of TXT and RXT LEDs in schematic
