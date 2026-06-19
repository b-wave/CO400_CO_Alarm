# RawSniffer (TP1 PWM Decoder)

Some log fies are incuded. You can look at these logs and see bits changing, possibly in response to the inputs. These are some the active sniffer tool captured and decoded te states from the TP1 data stream. It represents the current direction of the project. 

| STATE | LED Pulses | ALARM Pulses |
| :---- | :----: |:-------: |
| NULL | 0  |  0 |  
|  NORMAL | 1 | 0 |
|  LOW_BATT | 1 | 1 |
| MALFUNCTION | 3 | 3 |
|  ALARM| 8 | 4  |
|  EOL | 5 | 5 |

The data is spotty due to the known flaws in the existing code and the long sample periods and need better code to really make sure. Here are my current guesses looking at the data in these files responding to stimulaton. For the CO, the last two bytes seem to follow the two  automatic self-test spikes and recovery.  We know the thermistor gets polled only every ~48 Sec. I did some heating of the thermistor and saw the 5th and 6th bytes change.   We can't tell the battery polling happens but when the voltage was lowered to 2.0V (to trigger the LOW_BATT alarm) bytes 3 and 4 seemed to change. Oh, and the  ALARM is triggered by holding the test button down for several seconds, first we see a chirp ( which looks like LOW_BATT) then the alarm happens- we can look foe any status bits do they change?  This table is probabily wrong, the one field we know never changes is the first byte (0x55 or 0xAA depending on which sense we make the bits):

| Byte 1 |  Byte 2  | Byte 3 | Byte 4 | Byte 5 |  Byte 6  | Byte 7 | Byte 8 |
| :---- | :----: | :----: | :----: | :----: | :----: | :----: | :----: |
| 0x055 | STATUS? | BATTERY? |  BATTERY? |  TEMP? |  TEMP? |  CO? |  CO? | 
| 0x0AA | STATUS? | BATTERY? |  BATTERY? |  TEMP? |  TEMP? |  CO? |  CO? | 




For older analog-based experiments, see `analog-prototypes/`.

## Future Improvements (Toward a Full Analytical Tool)

The current sniffer sketches demonstrate that the TP1 PWM stream can be captured
and decoded, but they are intentionally lightweight and exploratory. The
following improvements outline a path toward a more robust analytical tool
capable of producing accurate, structured CSV logs suitable for long‑term
analysis and protocol research.

### 1. Interrupt‑Driven Edge Capture
Move pulse detection into a hardware interrupt (`CHANGE` on TP1).  
This prevents missed edges and dramatically improves timing accuracy.

### 2. Timestamp Ring Buffer
Store edge timestamps in a circular buffer inside the ISR.  
Decode frames later in the main loop to avoid blocking capture.

### 3. Frame Synchronization
Detect the start of a new frame using:
- a long inter‑frame gap, or  
- a distinctive preamble pulse  
This ensures clean frame boundaries and prevents partial‑frame decoding.

### 4. Adaptive Bit Classification
Replace fixed thresholds with dynamic timing bands that track:
- average “0” pulse width  
- average “1” pulse width  
This reduces bit‑count errors and improves decoding stability.

### 5. Bit‑Count and Structure Validation
Add sanity checks for:
- expected bit count  
- expected byte count  
- optional checksum guessing  
Invalid frames can be flagged or discarded.

### 6. Confidence Scoring
For each decoded frame, compute:
- timing variance  
- ambiguous pulses  
- checksum validity (if known)  
- frame completeness  
This allows CSV logs to include a “confidence” column.

### 7. Structured CSV Output
Move from free‑form serial prints to structured CSV fields such as:


This makes the output suitable for Python, Excel, and long‑term analysis.

### 8. LED/Horn Correlation
If LED and horn lines are tapped, log their state alongside each frame.  
This helps map internal bytes to visible alarm behavior.

### 9. Trigger‑On‑Change Mode
Only log frames when:
- a byte changes  
- a bit flips  
- LED/horn changes  
- checksum fails  
Useful for long captures without generating huge logs.

## v0.5.0 Roadmap (Next Major Milestone)

Version 0.5.0 represents the transition from a simple TP1 sniffer into a more
structured, timing‑accurate analytical tool. The goal is to stabilize frame
capture, reduce dropped bits, and begin producing meaningful CSV logs suitable
for offline analysis.

### Goals for v0.5.0

- **Reliable frame capture** using interrupt‑driven edge detection  
- **Consistent bit decoding** with adaptive thresholds  
- **Frame alignment** using preamble or inter‑frame gap detection  
- **Structured CSV output** with timestamps and decoded bytes  
- **Optional LED/Horn correlation** if those lines are tapped  
- **Basic validation** (bit count, frame length, simple heuristics)

### Planned Features

#### 1. Interrupt‑Driven Capture
Move TP1 edge detection into a hardware interrupt (`CHANGE` mode).  
This eliminates missed pulses and stabilizes timing.

#### 2. Timestamp Ring Buffer
Store pulse timestamps in a circular buffer inside the ISR.  
Decode frames later in the main loop to avoid blocking capture.

#### 3. Frame Synchronization
Detect the start of a new frame using:
- a long inter‑frame gap, or  
- a distinctive preamble pulse  

This ensures clean frame boundaries.

#### 4. Adaptive Bit Classification
Automatically track average pulse widths for “0” and “1” and adjust thresholds
dynamically. This reduces bit‑count errors and improves decoding robustness.

#### 5. Basic Frame Validation
Add simple checks:
- expected bit count  
- expected byte count  
- optional checksum guessing  

Invalid frames can be flagged or skipped.

#### 6. Structured CSV Output
Replace free‑form serial prints with structured CSV fields:


