
# First Alert CO Alarm (Model CO400) - Teardown
*A Work in Progress* 
<p align="center">
  <img src="resources/20260530_110405.jpg" width="225" alt="First Alert CO Alarm">
</p>

## Overview
This device is a sensor and alarm device (ca. 2009) which has reached its end-of-life. I was curious what made it tick and was surprised to find an automated detection and  warning system that is a masterwork of low-power engineering design. It is not only just a humble consumer device but is a minuature automated chemistry lab, with a fuel cell!  It was manufatured by a company called **BRK Brands, Inc.**  So it is likely used in other CO detectors as well. 
<p align="center">
  <img src="resources\20260530_110416.jpg" width="225" alt="Product Label">
</p>
Note: The date: 2009 SEP 7 This device was indicating the five-chirp per minute warning: End-of-life. 

## Hardware Description
The hardware has several interesting, some unusual, and some familiar parts.  <p align="center">
  <img src="resources/20260609_133546.jpg" width="250" alt="CO400 PCB">
</p>

### One moment please.. 
I did not kow what to expect, i was tinking it would be a simple sensor tied to a beeper, but the device interested me so i decided to do a **bigclivedotcom** - style teardown! It was not as easy as i thought it would be to trace out such a "simple" PCB but it actully took several hours over a few days to complete. And...I am not sure it is 100% so take it with about a pound of salt.

### The hardware 
- **TGS-5042** CO Sensor
- **PIC16F88** Microcontroller
- **MCP6042** rail-to rail high performance op amp uss a single supply drawing less than 1 µA
- **RE46C107** DC to DC Converter, Voltage Regulator and Piezoelectric Horn Driver.

### Details and Test Software 
   - **Schematics!** PCB reverse engineering 
   - **Serial Data** Maybe, kind of?
   - **Software** For further investigations

## Figaro TGS5042 Sensor 
<p align="center">
  <img src="resources/20260530_115704.jpg" width="225" alt="TGS-5042 Sensor">
</p>
When I first saw this device I thought it was a battery. There was even about 0.5V between the terminals. This is the sensor.  It is a miniture lab consisting of fuel-cell-type electrochemical sensor that outputs a tiny current strictly linear to the Carbon Monoxide (CO) gas concentration. It turns out seeing ~0.5V on the sensor pins when the board has no power supply is completely normal and expected for this specific component.

### It acts like a Fuel Cell?

The TGS5042 is a fuel cell type sensor. It contains an aqueous alkaline electrolyte and an internal water reservoir. When target gas or residual gases are present, it literally generates its own micro-voltage and current. But it turns out, the electrolyte and water reservoir dry out after about 10-years, which is the state my sensor is in now. The datasheet and manual have lots of good info if you want to see more details.  

### Note the number/barcode:1642.
Each sensor has a printed calibration number (see photo) This stands for 1.642nA/ppm for my sensor. This means that for every 1 part-per-million (ppm) of CO present, this sensor generates exactly 1.642 nanoamps of current.I will discuss reading this sensor in the OP Amp interface circuit description.  

## PIC16F688 Microcontroller Chip

<p align="center">
  <img src="resources/WIN_20260530_15_47_08_Pro.jpg" width="225" alt="TGS-5042 Sensor">
</p>

Seeing this chip on the board is like being handed a secret decoder to the whole thing. While it is unlikely to be able to capture the existing code, we know what each pin does. What does the "D" stamped on TP1 mean? 

### Features 
The PIC16F688 is a 14-pin, 8-bit CMOS microcontroller from Microchip Technology featuring nanoWatt technology for low power consumption - ideal for this device.  It includes 7KB of Flash program memory, 256 bytes of SRAM, and 256 bytes of EEPROM, supporting supply voltages from 2.0V to 5.5V. 

## MCP6042 Op Amp
<p align="center">
  <img src="resources/WIN_20260530_15_49_09_Pro.jpg" width="225" alt="Transimpedance Amplifier">
</p>

### Score!  
When a chip company gets selected to go in a product they get a "socket"  Microchip got two on this board! 
The MCP6042, from Microchip Technology Inc. is a  rail-to rail input and output high performance operational amplifier. The MCP6042 operates with
a single supply voltage as low as 1.4V, while drawing less than 1 µA of quiescent current per amplifier. These devices are also designed to support
rail-to-rail input and output operation. This combination of features supports our  battery-powered application.
The MCP6042 amplifier has a gain-bandwidth product of 14 kHz (typical) and is unity gain stable. These specifications make this op amp ideal 
for low frequency applications, such as sensor conditioning.

### Key specifications include: 
   - Dual Amplifier in an 8-pin DIP Package
   - Low Quiescent Current: <1 µA (600 nA/amplifiertypical)
   - Rail-to-Rail Input/Output
   - Gain Bandwidth Product: 14 kHz (typical)
   - Wide Supply Voltage Range: 1.4V to 6.0V
    
**Note:**  The 1M Resistor and 100nF capacitor, these match the reference circuit from the TGS-5042 Sensor documentaion. 

## The RE46C107  DC to DC Converter, Voltage Regulator and Piezoelectric Horn Driver

I had to remove the  piezoelectric transducer but that 16-pin DIP on th left side of the board is the  RE46C107
<p align="center">
  <img src="resources/20260531_104031.jpg" width="225" alt="RE-46C07">
</p>
 
The **RE46C107** is manufactured R&E International *A Subsidiary of Microchip Technology Inc.*  Is this a third and last "socket" win? Yes I think it is a third win! This is an interesting chip I never heard of. The RE46C107 is intended for use in 3V battery powered products like Smoke Detectors and CO Alarms.
### Alarming
The circuit features a DC-to-DC up-converter and driver circuit suitable for driving a piezoelectric horn. Oh, so that's how they get such a loud siren out of 3V batteries and there is a spec for alarm devices have to be so many dB (loud).  
### Boost it!
A selectable  3.0V or 3.3V A 5 Volt regulator is also provided for microprocessor voltage regulation. This curcuit uses the 3-Volt setting. 
### Hidden in Plain sight 
It also has a LED Driver and low battery detection but for some reason these features were not used in this product. This chip is located under the Piezo Horn, so i removed it to get a look at these circuits. 
<p align="center">
  <img src="resources/WIN_20260530_15_49_50_Pro.jpg" width="225" alt="RE46C07 DC-DC converter Inductor">
</p>

### Scope Out the Supporting Parts
This a microscope picture of the inductor, i was trying to get any indication of the values of the inductor and diode they used  - i assume that it matches the documtation's typical application circuit that use a 10 uH inductor. There is one small signal diode i used a 1N914 are a couple of rectifier diodes, shich were installed with the part numbers down on the board.  The specs called for a Schottky diode (I chose a 1N5818 seems to fit the specs.)  so i just used the same for the other diode. 

    Notes: 
    - Inductor L1 must have maximum peak current rating of at least 1.5A and for best results should have DC resistance of less than 0.3 ohm.
    - Schottky diode D1 must have maximum peak current rating of at least 1.5A and for best results should have forward voltage spec of less 
    than 0.5V at 1 Amp. 

    
### Key Specifications Include:

  - Low Quiescent Current - Low power design feature
  - 10V Boost Converter regulator - That's a lot of volts from a couple of 1.5 V AA batteries
  - Horn Driver - A complementary driver outputs HS and HB connect to the ceramic piezoelectric transducer, with a feedback pin as well
  - Voltage Regulation buck to 3.0V or 3.3V - It is set to 3-Volts (3v0) by a logic input used to set the Vreg output voltage level is tied to Vss (GND).
  - Voltage Regulation for +5 Volts - For logic circuits
  - Low Battery Detection - not used in this circuit
  - LED Driver- not used in this circuit 
## Methods to My Madness:  
How I captured the schematic. Having the datasheets and reference circuits helped. The following two pictures were made to help. 

<p align="center">
  <img src="resources/20260531_104031.jpg" width="225" alt="RE-46C07">
</p> 
Note: the solder side is mirrored and enhanced like X-ray vision:
<p align="center">
  <img src="resources/20260531_104048.jpg" width="225" alt="CO400 PCB">
</p>

### X-ray Vision.
The plan was to overlay these with the component side semi-transparent and the solder side mirrored so I could see the traces. I had no editing Software on hand  that could do layers like this. So I proceed to draw in the component outlines and using a continuity checker and a high contrast enhanced B&W print out, which was scaled up a bit which helped.

### Buzzed it out
I used my multimeter to do point-to-point and began to capture the circuits in KiCad using the reference circuits a guides. Since I started doing this before on the board from the solder side, I sometimes got the pin numbers wrong by starting on the wrong side of the chips! But it was easy to find and correct the errors using the reference documents.

### Board Takeover!
I originally considered reprogrmming the PIC16F688 controller.  The theory is that there is a end-of-life timer that just halts normal operation then there may still be some remaining sensitivity to the CO sensor that i could read.  The initial plan was to unsolder the chip and replace it with a 14-pin socket. this would allow not only to reprogram the chip off of the board but the socket would alow me to jumper into an Arduino and run the sensor that way.

### DFM 
One other minor annoyance tracing the circuits was there are a lot of pads that do not support any particular components. My guess is they are for factory programming, calibration, and test or *Design For Manufacturing* (DFM). Another clue is there are a couple of large holes in the board. (DAT1, DAT2) these are not mounting holes but I suspect they are "datum?" holes for alignment fot a "bed of nails" fixture.  Which brings up another idea - I bet all the signals for *In Circuit Programming* (ICP) are on these pads, along with other importaint signals. 

### Not My Problem
This effort was to see how this thing worked, not to copy or reproduce it.  There are some errors i am aware of on the schematic i will probabily fix, and i still have some questions about power distribution etc. 

### Here is the Schematic
Here is what i came up with so far.  I  may upodate it if there are any other cool bits or mistakes i find, but as i said before this is most certaintly not complete but i never inteded to *copy or reproduce* the detector. Hopefully, it may be usefull to someone - maybe as part of a post-apocalyptic tricorder device?  
<p align="center">
  <img src="resources/schematic.jpg" width="500" alt="CO400 Schematic">
</p>

## Circuit Analysis:  
How the circuits work
*Comming Soon!*

## Testing Methods and Results: 

Including the *Serial data?* I found :  
*Comming Soon!*
