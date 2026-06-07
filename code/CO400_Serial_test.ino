//=============================================================================
//File: CO400_Serial_test.ino
//=============================================================================
// Purpose: To attempt decoding the serial stream (PWM?) at TP1
// 
/* How It Works:
• Idle Detection: Waits for a silence > 2.0 seconds (2.4 Seconds measured)  to reset the buffer (bitCount = 0)
* NOTE: When LED flashes the space between is only ~100mSec so maybe we should do 50 - 100 mSec?  (maybe 1 mSec)
• Pulse Measurement: On every Falling Edge, it measures the preceding High duration.
• 100–350 µs → Records 0. 
• 400–650 µs → Records 1.
• Frame Completion: When the line stays low too long (>1ms gap between bits) or 
* the buffer fills (32 bits), it flags frameReady.
• Output: Prints the raw binary and a Hex representation to the Serial Monitor.
* Test proceedure:
• Upload and open Serial Monitor (115200 baud).
• Observe: Watch the Hex value change when you simulate Low Battery or trigger the End of Life chirp.
• Refine: If you see extra bits at the start (preamble), adjust the code to skip the first N bits before printing.
* The first 2 bits are normal short MARKs followed by  a long (~500uSec) SPACE  then another short 
* ...so the first two or three bits may be ignored.
* especially if the bit count/4 is off by 2-bits
*/ 

#include <Arduino.h>
const uint8_t PIN_SIGNAL = 2;       // Input pin connected to detector data
const unsigned long IDLE_THRESH = 2000000; // 2.0s (2,000,000 µs) to detect frame start
const unsigned long BIT_0_MIN = 100;       // Min width for '0' (200µs target)
const unsigned long BIT_0_MAX = 350;       // Max width for '0'
const unsigned long BIT_1_MIN = 400;       // Min width for '1' (500µs target)
const unsigned long BIT_1_MAX = 650;       // Max width for '1'

volatile unsigned long lastEdgeTime = 0;
volatile byte bitBuffer[32];      // Buffer for up to 32 bits
volatile byte bitCount = 0;
volatile bool frameReady = false;
volatile bool readingFrame = false;

void setup() {
  Serial.begin(115200);
  pinMode(PIN_SIGNAL, INPUT);
  // Trigger on CHANGE to catch both rising and falling edges
  attachInterrupt(digitalPinToInterrupt(PIN_SIGNAL), signalEdge, CHANGE);
  Serial.println("CO Detector Decoder Ready...");
}

void loop() {
  if (frameReady) {
    noInterrupts(); // Critical section to read buffer
    byte count = bitCount;
    byte localBuffer[32];
    for (byte i = 0; i < count; i++) localBuffer[i] = bitBuffer[i];
    bitCount = 0;
    frameReady = false;
    interrupts();

    // Print Binary
    Serial.print("Packet [");
    Serial.print(count);
    Serial.print(" bits]: ");
    for (byte i = 0; i < count; i++) {
      Serial.print(localBuffer[i]);
      if ((i + 1) % 8 == 0 && i < count - 1) Serial.print(" "); // Space every byte
    }
   
    // Print Hex (if divisible by 4 for nibble alignment)
    if (count >= 8) {
      Serial.print(" | Hex: 0x");
      for (byte i = 0; i < count; i += 8) {
        byte val = 0;
        for (byte j = 0; j < 8 && (i+j) < count; j++) {
          val = (val << 1) | localBuffer[i+j];
        }
        if (val < 16) Serial.print("0");
        Serial.print(val, HEX);
      }
    }
    Serial.println();
  }
}

void signalEdge() {
  unsigned long now = micros();
  unsigned long duration = now - lastEdgeTime;
  lastEdgeTime = now;

  // Detect Rising Edge (Start of Pulse)
  if (digitalRead(PIN_SIGNAL) == HIGH) {
    // Check if we just woke up from a long idle (Frame Start)
    if (duration > IDLE_THRESH) {
      bitCount = 0;
      readingFrame = true;
      // Optional: Ignore the first pulse if it's a sync preamble
    }
  }
  // Detect Falling Edge (End of Pulse -> Measure Width)
  else if (readingFrame) {
    if (duration >= BIT_0_MIN && duration <= BIT_0_MAX) {
      if (bitCount < 32) bitBuffer[bitCount++] = 0;
    }
    else if (duration >= BIT_1_MIN && duration <= BIT_1_MAX) {
      if (bitCount < 32) bitBuffer[bitCount++] = 1;
    }
    else {
      // Noise or end of packet (if low space is missing or wrong)
      // If duration is very long here, it's the post-packet idle
      if (duration > 1000) {
        if (bitCount > 0) frameReady = true;
        readingFrame = false;
      }
    }
   
    // Safety: End frame if buffer full
    if (bitCount >= 32) {
      frameReady = true;
      readingFrame = false;
    }
  }
}  

