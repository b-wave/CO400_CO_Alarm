# First Alert CO Alarm (Model CO400) - Teardown (Work in Progress) 
<p align="center">
  <img src="resources/20260530_110405.jpg" width="225" alt="First Alert CO Alarm">
</p>

## Overview
This device is a sensor and alarm device (ca. 2009) which has reached its end-of-life. I was curious what made it tick and was surprised to find an automated detection and  warning system that is a masterwork of low-power engineering design. It is not only just a humble consumer device but is a minuature automated chemistry lab, with a fuel cell!  It was manufatured by a company called **BRK Brands, Inc.**  So it is likely used in other CO detectors as well. 
<p align="center">
  <img src="resources\20260530_110416.jpg" width="225" alt="Product Label">
</p>
Note: The date 2009 SEP 7. This device was indicating the five-chirp per minute warning: End-of-life. 

## Hardware Description
The hardware has several interesting, some unusual, and some familiar parts.  I did not kow what to expect, i was tinking it would be a simple sensor tied to a beeper, but the device interested me so i decided to do a **bigclivedotcom** - style teardown! It was not as easy as i thought it would be to trace out such a "simple" PCB but it actully took several hours over a few days to complete. And...I am not sure it is 100% so take it with about a pound of salt.
- **TGS-5042** CO Sensor
- **PIC16F88** Microcontroller
- **MCP6042** rail-to rail high performance op amp uss a single supply drawing less than 1 µA
- **RE46C107** DC to DC Converter, Voltage Regulator and Piezoelectric Horn Driver
- Details and Test Software 
   - **Schematics!** PCB reverse engineering 
   - **Serial Data** Maybe, kind of?
   - **Software** For further investigations
 
<p align="center">
  <img src="resources/20260531_104048.jpg" width="250" alt="CO400 PCB">
</p>

## Figaro TGS5042 Sensor 
<p align="center">
  <img src="resources/20260530_115704.jpg" width="225" alt="TGS-5042 Sensor">
</p>
When I first saw this device I thought it was a battery. There was even about 0.5V between the terminals. This is the sensor.  It is a miniture lab consisting of fuel-cell-type electrochemical sensor that outputs a tiny current strictly linear to the Carbon Monoxide (CO) gas concentration. It turns out seeing ~0.5V on the sensor pins when the board has no power supply is completely normal and expected for this specific component.

### It acts like a Fuel Cell battery

The TGS5042 is a fuel cell type sensor. It contains an aqueous alkaline electrolyte and an internal water reservoir. When target gas or residual gases are present, it literally generates its own micro-voltage and current. But it turns out, the electrolyte and water reservoir dry out after about 10-years, which is the state my sensor is in now. The datasheet and manual have lots of good info if you want to see more details.  

### Note the number/barcode:1642.
-Each sensor has a printed calibration number (see photo) This stands for 1.642nA/ppm for my sensor. This means that for every 1 part-per-million (ppm) of CO present, this sensor generates exactly 1.642 nanoamps of current.I will discuss reading this sensor in the OP Amp interface circuit description.  

## PIC16F688 Microcontroller Chip

<p align="center">
  <img src="resources/WIN_20260530_15_47_08_Pro.jpg" width="225" alt="TGS-5042 Sensor">
</p>

Seeing this chip on the board is like being handed a secret decoder to the whole thing. While it is unlikely to be able to capture the existing code, we know what each pin does. What does the "D" stamped on TP1 mean? Possibly "Data" ?!?

The PIC16F688 is a 14-pin, 8-bit CMOS microcontroller from Microchip Technology featuring nanoWatt technology for low power consumption - ideal for this device.  It includes 7KB of Flash program memory, 256 bytes of SRAM, and 256 bytes of EEPROM, supporting supply voltages from 2.0V to 5.5V. 

## MCP6042 Op Amp
<p align="center">
  <img src="resources/WIN_20260530_15_49_09_Pro.jpg" width="225" alt="Transimpedance Amplifier">
</p>
Score!  When a chip company gets selected to go in a product they get a "socket"  Microchip got two on this board! 
The MCP6042, from Microchip Technology Inc. is a  rail-to rail input and output high performance operational amplifier. The MCP6042 operates with
a single supply voltage as low as 1.4V, while drawing less than 1 µA of quiescent current per amplifier. These devices are also designed to support
rail-to-rail input and output operation. This combination of features supports our  battery-powered application.
The MCP6042 amplifier has a gain-bandwidth product of 14 kHz (typical) and are unity gain stable. These specifications make this e op amp ideal 
for low frequency applications, such as sensor conditioning.

### Key specifications include: 
   - Dual Amplifier in an 8-pin DIP Package
   - Low Quiescent Current: 600 nA/amplifier (typical)
   - Rail-to-Rail Input/Output
   - Gain Bandwidth Product: 14 kHz (typical)
   - Wide Supply Voltage Range: 1.4V to 6.0V
    
**Note:**  The 1M Resistor and 100nF capacitor, these match the reference circuit from the TGS-5042 Sensor documentaion. 

## The RE46C107  DC to DC Converter, Voltage Regulator and Piezoelectric Horn Driver

I had to remove the  piezoelectric transducer but that 16-pin DIP on th left side of the board is the  RE46C107
<p align="center">
  <img src="resources/20260531_104031.jpg" width="225" alt="RE-46C07">
</p>
 
The RE46C107 is manufactured R&E International *A Subsidiary of Microchip Technology Inc.*  Is this a third and last "socket" win? Yes I think it is a third win! This is an interesting chip I never heard of. The RE46C107 is intended for use in 3V battery powered products like Smoke Detectors and CO Alarms. The circuit features a DC-to-DC up-converter and driver circuit suitable for driving a piezoelectric horn. Oh, so that's how they get such a loud siren out of 3V batteries and there is a spec for alarm devices have to be so many dB (loud).  A selectable  3.0V or 3.3V regulator is also provided for microprocessor voltage regulation, this curcuit uses the 3-Volt setting. It also has its own LED driver and low battery detection but for some reason these features were not used in this product. This chip is located under the Piezo Horn, so i removed it to get a look at these circuits. 
<p align="center">
  <img src="resources/WIN_20260530_15_49_50_Pro.jpg" width="225" alt="RE46C07 DC-DC converter Inductor">
</p>
This a micoscope picture of the inductor, i was trying to get any indication of the values of the inductor and Diode they used  - i assume that it matches the documtation's typical application circuit that use a 10 uH inductor and a Schottky diode (I chose a 1N5818 seems to fit the specs.)  

    Notes: 
    - Inductor L1 must have maximum peak current rating of at least 1.5A and for best results should have DC resistance of less than 0.3 ohm.
    - Schottky diode D1 must have maximum peak current rating of at least 1.5A and for best results should have forward voltage spec of less 
    than 0.5V at 1 Amp. 

    
### Key Specifications Include:

  - Low Quiescent Current - Low power design feature
  - 10V Boost Converter regulator - That's a lot of volts from a couple of 1.5 V AA batteries
  - Horn Driver - A complementary driver outputs HS and HB connect to the ceramic piezoelectric transducer, with a feedback pin as well
  - Voltage Regulation to 3.0V or 3.3V - It is set to 3-Volts (3v0) by a logic input used to set the Vreg output voltage level is tied to Vss (GND).
  - Low Battery Detection - not used in this circuit

## Methods to My Madness:  
How I captured the schematic. Having the datasheets and reference circuits helped. The following two pictures were made to help. The plan was to overlay these with the component side semi-transparent and the solder side mirrored so I could see the traces. 

<p align="center">
  <img src="resources/20260531_104031.jpg" width="225" alt="RE-46C07">
</p> 
Note: the solder side is mirrored and enhanced like X-ray vision:
<p align="center">
  <img src="resources/20260531_104048.jpg" width="225" alt="CO400 PCB">
</p>
I had no editing Software on hand  that could do layers like this. So I proceed to draw in the component outlines and using a continuity checker and a high contrast enhanced B&W print out, which was scaled up a bit I "Buzzed it out" and began capture in KiCad. since I started doing this before o  the board, I got the pin numbers wrong but that was easy to find the errors using the reference documents. As I stated earlier, this is probably not 100% correct,  I mainly considered the PIC16F688 controller as my initial plan was to unsolder the chip and replace it with a 14-pin socket. One other minor annoyance was there are a lot of pads that do not support any particular components. My guess is they are for factory programming, calibration, and test. Another clue is there are large holes in the board. (DAT1, DAT2) these are not mounting holes but I suspect they are "datum?" holes for alignment fot a "bed of nails" fixture.
<p align="center">
  <img src="resources/schematic.jpg" width="250" alt="CO400 Schematic">
</p>

## Circuit Analysis:  
How the circuits work
*Comming Soon!*

## Testing Methods and Results  Including the *SERIAL DATA* I found :  

*Comming Soon!*
