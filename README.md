## A NRF51422-based usb host ##

### Overview ###

I've been looking for a project that lets me deep-dive on the USB protocol. I finally settled on trying to take wired devices (mice, keyboards, and joysticks) and expose them over bluetooth. Some of the best mechanical keyboards are USB-only, so i thought it would be useful if you could have a device you connect to those devices that exposed them as a wireless bluetooth decice. This project contains the end result of my experiments: how to connect a MAX3421E USB Host Controller to a NRF51422 Bluetooth SoC over SPI to proxy basically any USB HID devices over bluetooth. 


- [usb_host](https://github.com/dretay/usb_host): 
-- This project contains all the code needed to actually enumerate and query USB devies connected to the MAX3421E. 

### Main Features ###
![](https://raw.githubusercontent.com/dretay/nrf51422_usb_host/master/IMG_4446.jpg)
- 
