# First Alert CO Alarm (Model CO400) - Teardown (Work in Progress) 
<p align="center">
  <img src="resources/20260530_110405.jpg" width="225" alt="First Alert CO Alarm">
</p>

## Overview
This device is a sensor and alarm device (ca. 2009) which has reached its end-of-life. I was curious what made it tick and was surprised to find an automated detection and  warning system that is a masterwork of low-power engineering design. It is not only just a humble consumer device but is a minuature automated chemistry lab, with a fuel cell!  
<p align="center">
  <img src="resources\20260530_110416.jpg" width="225" alt="Product Label">
</p>

## Hardware Description
The hardware has several interesting, some unusual, and some familiar parts.  I did not kow what to expect, i was tinking it would be a simple sensor tied to a beeper, but the device interested me so i decided to do a **bigclivedotcom** - style teardown! It was not as easy as i thought it would be to trace out such a "simple" PCB but it actully took several hours over a few days to complete. And...I am not sure it is 100% so take it with about a pound of salt.
- **TGS-5042** CO Sensor
- **Microchip PIC16F88**Microcontroller
- **MCP6021** Rail-to-Rail Input/Output, 10 MHz Op Amps
- **RE46C107** DC to DC Converter, Voltage Regulator and Piezoelectric Horn Drivel
- Details and Test Software 
   - **Schematics!** 
   - **Software!** For further investigations
 
<p align="center">
  <img src="resources/schematic.jpg" width="225" alt="CO400 Schematic">
</p>

## Figaro TGS5042 Sensor 
<p align="center">
  <img src="resources/20260530_115704.jpg" width="225" alt="TGS-5042 Sensor">
</p>
When I first saw this device I thought it was a battery. There was even about 0.5V between the terminals. This is the sensor.  It is a miniture lab consisting of fuel-cell-type electrochemical sensor that outputs a tiny current strictly linear to the Carbon Monoxide (CO) gas concentration. It turns out seeing ~0.5V on the sensor pins when the board has no power supply is completely normal and expected for this specific component.

### It acts like a battery

The TGS5042 is a fuel cell type sensor. It contains an aqueous alkaline electrolyte and an internal water reservoir. When target gas or residual gases are present, it literally generates its own micro-voltage and current. But it turns out, the electrolyte and water reservoir dry out after about 10-years, which is the state my sensor is in now. The datasheet and manual have lots of good info if you want to see more details.  

### Note the number/barcode: 
1642. Each sensor has a printed calibration number (see photo) This stands for 1.642nA/ppm for my sensor. This means that for every 1 part-per-million (ppm) of CO present, this sensor generates exactly 1.642 nanoamps of current. I will discuss reading this sensor in the OP Amp interface circuit description.  
