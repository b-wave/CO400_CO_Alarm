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

Note:  **2009 SEP 7** This device was indicating the five-chirp per minute warning: End-of-life. 

## Hardware Description
The hardware has several interesting, some unusual, and some familiar parts.  <p align="center">
  <img src="resources/20260609_133546.jpg" width="250" alt="CO400 PCB">
</p>

### One moment please.. 
I did not kow what to expect, i was tinking it would be a simple sensor tied to a beeper, but the device interested me so i decided to do a **bigclivedotcom** - style teardown! It was not as easy as i thought it would be to trace out such a "simple" PCB but it actully took several hours over a few days to complete. And...I am not sure it is 100% so take it with about a pound of salt.

### The hardware 
- **TGS-5042** CO Sensor
- **PIC16F88** Microcontroller
- **MCP6042** Op amp
- **RE46C107** DC to DC Converter, Voltage Regulator and Piezoelectric Horn Driver

### Details and Test Software 
   - **Schematics!** PCB reverse engineering 
   - **Serial Data** Maybe, kind of?
   - **Software** For further investigations

## Figaro TGS5042 Sensor 
<p align="center">
  <img src="resources/20260530_115704.jpg" width="225" alt="TGS-5042 Sensor">
</p>
When I first saw this device I thought it was a battery. There was even about 0.5V between the terminals. This is the sensor.  It is a miniture lab consisting of fuel-cell-type electrochemical sensor that outputs a tiny current strictly linear to the Carbon Monoxide (CO) gas concentration. It turns out seeing ~0.5V on the sensor pins when the board has no power supply is completely normal and expected for this specific component.

### It Has a Fuel Cell?

The **TGS5042** is a fuel cell type sensor. That is, it generates electricity from an electrolyte and a gas. It contains an aqueous alkaline electrolyte and an internal water reservoir. When target gas or residual gases are present, it literally generates its own micro-voltage and current. But it turns out, the electrolyte and water reservoir dry out after about 10-years, which is the state my sensor is in now. The datasheet and manual have lots of good info if you want to see more details.  

### Note the number: 1642
Each sensor has a printed calibration number (see photo) This stands for 1.642nA/ppm for my sensor. This means that for every 1 part-per-million (ppm) of CO present, this sensor generates exactly 1.642 nanoamps of current.I will discuss reading this sensor in the OP Amp interface circuit description.  

## PIC16F688 Microcontroller Chip

<p align="center">
  <img src="resources/WIN_20260530_15_47_08_Pro.jpg" width="225" alt="TGS-5042 Sensor">
</p>

Seeing this chip on the board is like being handed a secret decoder to the whole thing. While it is unlikely to be able to capture the existing code, we know what each pin does. *What does the **"D"** stamped on TP1 mean?* 

### Features 
The PIC16F688 is a 14-pin, 8-bit CMOS microcontroller from Microchip Technology featuring nanoWatt technology for low power consumption - ideal for this device.  Some of the key features/interfaces:
  - It includes 7KB of Flash program memory, 256 bytes of SRAM, and 256 bytes of EEPROM, supporting supply voltages from 2.0V to 5.5V.
  - This chip also supports low-power by switching off things like the Thermistor temperature sensor and the amplifiers to save power.
  - Since it is a saftey device it also provides a power-cycle with an NPN transistor switch (Q1) a PN2222 temporatily taking the battery voltage down trough (R6) controlled by a port (AN3) - proabibily on a watchdog timer?
  - It pulses the horn thru U1 (pin 14) via RC4 as well as controling some built in tests
  - The LED (D4) /Thermistor (TH1) are simultainioisly controlled  by pulling (RC0) Low.
  -  SW1 is the TEST/SILENCE button to input (RC5).
  -  There is another circuit that seems to be a self-test consisting of (D1) (R2) (R12) and (C12) I am not exactly sure what this circuit does but it apparently uses Vbatt and is controlled by (RC3)  and (RC4) which is also the signal that enables the Siren. 

## MCP6042 Op Amp
<p align="center">
  <img src="resources/WIN_20260530_15_49_09_Pro.jpg" width="225" alt="Transimpedance Amplifier">
</p>

### Score!  
When a chip company gets selected to go in a product they get a "socket" **Microchip** got two on this board! 
The **MCP6042**, from **Microchip Technology Inc.** is a  high performance operational amplifier. The **MCP6042** operates with
a single supply voltage as low as 1.4V, while drawing less than 1 µA of quiescent current per amplifier.

The MCP6042 amplifier acts in this device as a Transimpedance Amplifier (TIA). All the current from the sensor is forced to flow through the feedback resistor The output voltage is calculated simply by multiplying the input current by the feedback resistor. The specifications make this op amp ideal impedance characteristics and bandwidth 
for applications, such as sensor conditioning. We will go into the details later in the detailed circuit description. 

### Key specifications include: 
   - Dual Amplifier in an 8-pin DIP Package
   - Low Quiescent Current: <1 µA (600 nA/amplifier typical)
   - Wide Supply Voltage Range: 1.4V to 6.0V
    
**Note:**  The 1M Resistor and 100nF capacitor seen in th photo match the reference circuit from the **TGS-5042** Sensor documentaion. 

## The RE46C107  DC to DC Converter, Voltage Regulator & Piezoelectric Horn Driver Chip

This is kind of a multipurpose specially chip. I had to remove the  piezoelectric transducer to reveal it but that 16-pin DIP on the left side of the board is the **RE46C107**
<p align="center">
  <img src="resources/20260531_104031.jpg" width="225" alt="RE-46C07">
</p>
 
The **RE46C107** is manufactured R&E International *A Subsidiary of Microchip Technology Inc.*  Is this a third and last "socket" *win?* Yes, I think it is a third win! This is an interesting chip I never heard of. The RE46C107 is intended for use in 3V battery powered products like Smoke Detectors and CO Alarms.
### Alarming
The circuit features a DC-to-DC up-converter and driver circuit suitable for driving a piezoelectric horn. Oh, so that's how they get such a loud siren out of 3V batteries and there is a spec for alarm devices have to be so many dB (loud).  
### Boost it!
A selectable  3.0V or 3.3V A 5 Volt regulator is also provided for microprocessor voltage regulation. This curcuit uses the 3.3-Volt setting. 
### Hidden in Plain sight 
It also has a LED Driver and low battery detection but for some reason these features were not used in this product. This chip is located under the Piezo Horn, so i removed it to get a look at these circuits. 
<p align="center">
  <img src="resources/WIN_20260530_15_49_50_Pro.jpg" width="225" alt="RE46C07 DC-DC converter Inductor">
</p>

### Scope Out the Supporting Parts
This a microscope picture of the inductor, i was trying to get any indication of the values of the inductor and diode they used  - i assume that it matches the documtation's typical application circuit that use a 10 uH inductor.  Then there are a couple of rectifier diodes, which were installed with the part numbers down on the board.  The specs called for a Schottky diode (I chose a 1N5818 seems to fit the specs.)  There ia another rectifier diode in the processor test circuits - not related to this chip- so i just used the same for that other diode. 

    Notes: 
    - Inductor L1 must have maximum peak current rating of at least 1.5A and for best results should have DC resistance of less than 0.3 ohm.
    - Schottky diode D1 must have maximum peak current rating of at least 1.5A and for best results should have forward voltage spec of less 
    than 0.5V at 1 Amp. 

    
### Key Specifications Include:

  - Low Quiescent Current - Low power design feature
  - 10V Boost Converter regulator - That's a lot of volts from a couple of 1.5 V AA batteries
  - Horn Driver - A complementary driver outputs HS and HB connect to the ceramic piezoelectric transducer, with a feedback pin as well.
  - Voltage Regulation buck to 3.0V or 3.3V - It is set by a logic input used to set the Vreg output.
  - Voltage Regulation for +5 Volts - For logic circuits
  - Low Battery Detection - not used in this circuit
  - LED Driver- not used in this circuit 
## Methods to My Madness:  
How I captured the schematic. Having the datasheets and reference circuits helped. The following two pictures were made to help. 

<p align="center">
  <img src="resources/20260531_104031.jpg" width="225" alt="RE-46C07">
</p> 
Note: the solder side is mirrored and enhanced - like viewing the board through the circuit side:
<p align="center">
  <img src="resources/20260531_104048.jpg" width="225" alt="CO400 PCB">
</p>

### X-ray Vision.
The plan was to overlay these with the component side semi-transparent and the solder side mirrored so I could see the traces. I had no editing Software on hand  that could do layers like this. So no X-ray vision needed,  I proceed to draw in the component outlines on a printout of the solder side and using continuity check on my DMM The contrast enhanced B&W print out, which was scaled up a bit helped - but an X-ray view would have been better. 

### Buzzed it out
I used my multimeter to do point-to-point and began to capture the circuits in KiCad using the reference circuits a guides. Since I started doing this before on the board from the solder side, I sometimes got the pin numbers wrong by starting on the wrong side of the chips! But it was easy to find and correct the errors using the reference documents. Assignment of component values was done later, with the help of component markings and measurements.  

### Board Takeover!
I originally considered reprogrmming the PIC16F688 controller.  The theory is that there is a end-of-life timer that just halts normal operation then there may still be some remaining sensitivity to the CO sensor that i could read.  The initial plan was to unsolder the chip and replace it with a 14-pin socket. This would allow not only to reprogram the chip off of the board but the socket also would alow me to jumper into an Arduino and run the sensor that way. Alternatively, simply soldering in short jumper wires to the pads after removing the chip would allow me to use a breadboard. 

### DFM & DFT
One other minor annoyance tracing the circuits was there are a lot of pads that do not support any particular components, and some components used long leads so i had to use the board to verify that they were just pads without component leads. My guess is they are for factory programming, calibration, and test or *Design For Manufacturing* (DFM). Another clue is there are a couple of large holes in the board. (DAT1, DAT2) these are not mounting holes but I suspect they are "datum?" holes for alignment pins for a pogo pin or "bed of nails" fixture.  Which brings up another idea - I bet all the signals for *In Circuit Programming* (ICP) are on these pads, along with other importaint signals. I probably should capture these on the schematic somehow,  although they are not labeled. 

### Not My Problem
This effort was to see how this thing worked, not to copy or reproduce it.  There are some errors i am aware of on the schematic i will probabily fix, and i still had some questions about power distribution etc. 

### Here is the Schematic
Here is what i came up with so far.  I  may upodate it if there are any other cool bits or mistakes i find, but as i said before this is most certaintly not complete but i never inteded to *copy or reproduce* the detector. 

Hopefully, it may be usefull to someone - maybe as part of your post-apocalyptic tricorder device project?   

<p align="center">
  <img src="resources/schematic.jpg" width="500" alt="CO400 Schematic">
</p>

~~Schematic TODO: Verify U3 V+ voltage and the voltage @ R17. - this will be critical to the ADC and reading of the sensor voltage.~~ **DONE**

## Circuit Analysis:  
Most of the circuits have been explained in the previous secctions.  The reading of the Figaro TGS5042 sensor (U4) is the heart of this device.  There are a few mathematical fomula are needed that get the %CO from the sensor. Due to regulatory requirements for a saftey device these circuits have Built In System Tests (BIST) to meet those requirements, since the circuits contain them I will also explain these and wil provide some proposed Arduino sketches to help explain/implement this sensor. This is a little technical - it was mostly vibed. There are a couple of *"got-ya things"* that need to be addressed, such as the Reference voltage for the ADC, and temperature compensation. It is basically Ohm's Law but the current resolution for 1% PPM CO is a nano Ampere or about $$\frac{1}{\ 1,000,000,000}$$ of an AMP!  This needs to be converted to a voltage so the ADC on the PIC can read it.  Here are the details:
*Demo Software - Comming Soon!*

## 1. Sensor Current to Output Voltage ($V_{out}$)
The sensor generates a minute current proportional to gas concentration. An operational amplifier or load resistor is used to convert this current into a measurable voltage.  

### 2. Calculating Sensor Current ($I_s$)
To derive the raw sensor current in Amperes (A) from your circuit's measured output voltage, use the following formula:
$$I_s = \frac{V_{out} - 1.0}{1.0 \times 10^6}$$ 

### 3. Carbon Monoxide (CO) Concentration Calculation
To determine the absolute CO gas concentration in parts per million (ppm), divide the sensor output current by the sensor's individual sensitivity coefficient (measured in nA/ppm and found printed on the sensor's barcode).
$$\text{CO Concentration (ppm)} = \frac{\text{Sensor Output Current (nA)}}{\text{Sensor Sensitivity (nA/ppm)}}$$ 

### 4. Figaro Reference Formula
The official Figaro EM5042A Evaluation Module circuit applies a fixed amplification factor (1.0 × 10⁶) resulting in a 1.0V baseline offset in clean air.
$$V_{out} = (\text{Concentration} \times \text{Sensitivity}) + 1.0$$ 

## 5. Microprocessor Measurement Resolution
To evaluate the minimum measurable step of carbon monoxide resolution for our specific analog-to-digital converter (ADC) setup, apply:
$$\text{Resolution} = \frac{C_{max}}{2^M \times B_{min}}$$ 

  Where:
  
    * Cmax = Maximum target CO concentration
    * M = Number of microcontroller ADC bits (e.g., 10-bit, 12-bit)
    * Bmin = Minimum distinct digital bits required


## Interface Circuitry & Theory of Operation

OK with that out of the way, lets dig into some of these circuits. The TGS5042 carbon monoxide sensor generates a minute electrical current directly proportional to gas concentration. To process this signal safely and accurately, the circuit utilizes a dual operational amplifier (MCP6042) split into two distinct functional stages with the integrated Built-In Self-Test (BIST) diagnostics.

### 1. Anti-Polarization Shunt Circuit

 A big deal for just one  resistor. It was verified by reading the color code as measuring it would lead to incorrect result. see Notes below.
 ~~TODO: Need to check this resistor. I may have measured this value and the generated voltage may have interfered with the measurment, it seems pretty specfic~~ **DONE!**
 
* The 100kΩ resistor (R15) connected across the Working Electrode (WE) and Counter Electrode (CE).
* Note: Electrochemical sensors can degrade permanently or suffer severe baseline drift if they hold an electrical bias while powered down. When the main system power is completely off, this resistor acts as a safe drain path. It maintains the potential between WE and CE at exactly 0V, preventing polarization damage.

### 2. Stage 1: Transimpedance Amplifier (TIA)

* Key Components:  MCP6042 Op-Amp (U3A) (First Stage: Pins 1, 2, 3), 1MΩ feedback resistor (R3), 100nF feedback capacitor (2.2kΩ (R4) / 220Ω (R11) ) act as isolation resistors.
* Function: This stage converts the sensor's raw nanoampere (nA) current into a readable voltage.
* Gain Control: The 1MΩ resistor (R3) sets the transimpedance gain. Because of this value, every 1 nA of sensor current translates to exactly 1 mV of voltage deviation at the output (Pin 1) the following circuits are needed.
   * Filtering: The 100nF parallel feedback capacitor acts as a low-pass filter to smooth out high-frequency environmental noise.
   * Protection: The 2.2kΩ and 220Ω inline resistors protect the delicate op-amp inputs against unexpected current spikes.

### Sensor Diagnostic Test 1 (RA5)

* Circuit Path: Microprocessor Digital Pin → Diode (D2) Anode on MCU side → 1MΩ (R5) resistor → Pin 3 (Inverting Input). There small signal diode (D2) is probabily a 1N914 or 1N4148.
* Normal Mode: The MCU configures its digital pin as a High-Z Input (or holds it HIGH). This reverse-biases the diode, isolating the diagnostic branch completely so it does not affect gas readings.
* Self-Test Mode: The MCU drives this pin LOW. This pulls current away from Pin 3 through the 1MΩ resistor. The op-amp immediately compensates by driving its output (Pin 1) upward. The MCU checks for this predictable voltage step-up on the ADC to verify that the first stage op-amp loop is alive and electrically sound.

### 3. Inter-Stage RC Filter

* Key Components: 240Ω series resistor (R14), 100nF capacitor (C8) to Ground.
* Function: Located between the first stage output (Pin 1) and second stage input (Pin 6). This passive low-pass filter acts as a hardware noise barrier. It strips away high-frequency ripple and digital switching noise before the signal enters the ADC buffer.

### 4. Stage 2: Voltage Buffer & Baseline Offset

* Key Components: MCP6042 Op-Amp (U3B) Second Stage: Pins 5, 6, 7, Voltage Divider 590kΩ (R17) to $V_{CC}$ and 440kΩ (R16) to GND.
* Function: This stage isolates the measurement circuit from the microprocessor's ADC load while establishing a stable "clean air" baseline reference voltage.  The analog output is read on (AN6). 
* Voltage Divider (Pin 5): The 590kΩ (R17) and 440kΩ (R16) divider creates a permanent voltage offset on the non-inverting pin. For a 3.3V system, this sets a steady baseline reference point around 1.4V.
   * Note: Electrochemical sensors can sometimes exhibit negative baseline drift or minor reverse currents under specific temperatures or clean-air conditions. Elevating the "zero gas" signal above 0V prevents the output signal from clipping against the ground rail, allowing the ADC to capture both positive gas spikes and negative sensor drift accurately.

### ADC / Buffer Validation Circuit 2 (RA1/AN1)

* Circuit Path: Microprocessor Pin (RA1/AN1) → 10kΩ resistor (R1) → Pin 6 (U3B) The non-inverting input of the Buffer stage.
* Dual Functionality:
1. Buffer Diagnostics: If driven by a digital pin, the MCU can momentarily inject a test voltage into the buffer input. Observing the corresponding shift on the main ADC trace confirms that the second stage, the PCB trace, and the MCU's internal ADC hardware are completely intact.
2. Fast Stabilization Assist: Upon cold-booting, electrochemical sensors require time to settle. The MCU can temporarily configure this pin as an active output to rapidly charge the filter node to its steady-state voltage, sharply reducing the initial sensor warm-up time before flipping the pin back to a passive input state.
   
  NOTE:  Based on the transient response of the 10kΩ injection resistor and the 100nF filter capacitor, a single initialization pulse lasting exactly 560 microseconds (0.56 ms) will perfectly pre-charge the analog filter node to its steady-state clean-air operating baseline, eliminating the slow hardware startup lag.  

## Testing Methods, Notes & Results 

Including the *Serial data?* I found :  
*Comming Soon!*

### Core Power Architecture & Baseline Verifications
The interface board utilizes a Microchip RE46C107 ASIC to manage power distribution, battery step-up operations, and reference voltage rails. 

* **Main System Reference (`VREG`):** Powered at **3.39V** (Configured via `REGSEL` Pin 9 tied HIGH to select the 3.3V power profile). This rail provides a quiet analog reference line for temperature sensing.
* **Piezo/Amplifier Rail (`VO`):** Boosted internally to **4.5V** via the chip's inductive DC-to-DC step-up circuit.
* **Stage 2 Baseline Bias (Pin 5):** Biased to a factory baseline of exactly **0.298V**. This low-offset configuration provides maximum voltage headroom for incoming positive gas spikes while preventing ground-rail clipping.

### Corrected Schematic Values (3.3V System Target)
To manually replicate the verified *in-situ* baseline voltage of ~0.298V without utilizing non-standard values, substitute the theoretical divider values with standard 1% components:

* **Top Divider Resistor (R_top):** Swap out for a standard **487kΩ** (or **470kΩ** as a close alternative).
* **Bottom Divider Resistor (R_bottom):** Swap out for a standard **47kΩ**.
* **Resulting Baseline:** Yields an explicit stable bias of **0.298V**, mirroring the production ASIC profile perfectly.
* 
### Anti-Polarization Shunt Circuit
* **Component:** `100kΩ` Resistor (**BROWN-BLACK-YELLOW**) connected directly across the Working Electrode (**WE**) and Counter Electrode (**CE**).
* **Function:** Electrochemical cells naturally behave like tiny batteries and will drift or suffer permanent degradation if they hold an electrical charge while unpowered. This resistor acts as a safe drain path when the system is off, maintaining a strict 0V potential between the electrodes.
* **Bench-Testing Note:** Do not attempt to measure this resistor in-circuit using a standard Multimeter resistance setting. The active chemistry of the TGS5042 injects a residual voltage into the traces, which skews the meter's test current and produces false, fluctuating resistance readings.

