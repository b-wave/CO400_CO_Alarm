# First Alert CO Alarm (Model CO400) - Teardown (Work in Progress) 
<p align="center">
  <img src="resources/20260530_110405.jpg" width="225" alt="First Alert CO Alarm">
</p>

## Overview
This device is a sensor and alarm device (ca. 2006) which has reached its end-of-life. I was curious what made it tick and was supprized to find an automated detction warnign system that is a masterwork of low-power engineering. It is not only a humble consumer device but is a minuature automated chemistry lab, with a fuel cell!  
<p align="center">
  <img src="resources/20260531_104031.jpg" width="225" alt="Circuits Assy.">
</p>

## Hardware Description
The hardware has several interesting, some unusual, and some familiar parts.  I did not kow what to expect, i was tinking it would be a simple sensor tied to a beeper, but the device interested me so i decided to do a **bigclivedotcom** - style teardown! It was not as easy as i thought it would be to trace out such a "simple" PCB but it actully took several hours over a few days to complete. And...I am not sure it is 100% so take it with about a pound of salt.
- **TGS-5042** CO Sensor
- **Microchip PIC16F88**Microcontroller
- **MCP6021** Rail-to-Rail Input/Output, 10 MHz Op Amps
- **RE46C107** DC to DC Converter, Voltage Regulator and Piezoelectric Horn Driver
- **Schematics!**
- **Software!** For further investigations 
<p align="center">
  <img src="resources/schematic.jpg" width="225" alt="CO400 Schematic">
</p>

