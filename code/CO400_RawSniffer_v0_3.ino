/*
 * File: CO400_RawSniffer_v0.4.1.ino
 * VERS: 0.4  REV: 260614
 *
 * Purpose:
 *   Correct raw-frame logger for First Alert CO400 internal PWM stream.
 *   Measures HIGH and LOW widths separately (mark/space).
 *   Correctly detects:
 *     - rattle pulses
 *     - sync gap
 *     - inter-packet LED-flash gap (~100 ms)
 *     - long idle (~2.5 s)
 *   Decodes bits from HIGH widths only.
 *   Fixes skipped packets by dumping ALL frames on gap events.
 *
 * First packets seen (6/13/26): 
 * ?t=1891 ms  Bits=63  FRAME: 0x55 0xAA 0x40 0x15 0x14 0x4 0x25 | CHK?=0x25
 * ?t=2833 ms  Bits=61  FRAME: 0x55 0xAA 0x4A 0x15 0x54 0x40 0x9 | CHK?=0x9
 * ?t=4762 ms  Bits=61  FRAME: 0x55 0xAA 0x4A 0x15 0x54 0x40 0x9 | CHK?=0x9
 * ?t=4761 ms  Bits=60  FRAME: 0x55 0xAA 0x4A 0x15 0x54 0x40 0x9 | CHK?=0x9
 */

const int DATA_PIN = 2;

// HIGH pulse widths (bit values)
const unsigned long T_BIT0_MIN = 150;
const unsigned long T_BIT0_MAX = 260;

const unsigned long T_BIT1_MIN = 420;
const unsigned long T_BIT1_MAX = 620;

// LOW gaps (structure)
const unsigned long T_RATTLE_MAX = 140;
const unsigned long T_SYNC_MIN   = 380;
const unsigned long T_SYNC_MAX   = 520;

const unsigned long T_GAP_FRAME  = 20000;      // ~20 ms → LED flash gap
const unsigned long T_IDLE_LONG  = 1500000;    // >1.5 s → main idle

enum State { WAIT_IDLE, PREAMBLE, READ_BITS };
volatile State state = WAIT_IDLE;

volatile bool lastLevel = LOW;
volatile unsigned long lastEdge = 0;

volatile unsigned long highWidth = 0;
volatile unsigned long lowWidth  = 0;

volatile bool highReady = false;
volatile bool lowReady  = false;

uint8_t frame[16];
int bitPos = 0;
int bytePos = 0;
int totalBits = 0;

unsigned long lastFrameTime = 0;

void isr() {
  unsigned long now = micros();
  bool level = digitalRead(DATA_PIN);
  unsigned long dt = now - lastEdge;
  lastEdge = now;

  if (lastLevel == LOW && level == HIGH) {
    lowWidth = dt;
    lowReady = true;
  }
  else if (lastLevel == HIGH && level == LOW) {
    highWidth = dt;
    highReady = true;
  }

  lastLevel = level;
}

void dumpFrame() {
  if (bytePos == 0) return;

  unsigned long now = millis();
  unsigned long dt = now - lastFrameTime;
  lastFrameTime = now;

  Serial.print(F("\nΔt=")); // "Δ" does not print for all serial terms
  Serial.print(dt);
  Serial.print(F(" ms  Bits="));
  Serial.print(totalBits);
  Serial.print(F("  FRAME: "));

  for (int i = 0; i < bytePos; i++) {
    Serial.print(F("0x"));
    if(frame[i] < 16) Serial.print(F("0"));
    Serial.print(frame[i], HEX);
    Serial.print(F(" "));
  }

  Serial.print(F("| CHK?=0x"));
  if(frame[bytePos - 1] < 16) Serial.print(F("0"));
  Serial.print(frame[bytePos - 1], HEX);
}

void resetFrame() {
  bitPos = 0;
  bytePos = 0;
  totalBits = 0;
  memset(frame, 0, sizeof(frame));
}

void setup() {
  Serial.begin(9600);
  pinMode(DATA_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(DATA_PIN), isr, CHANGE);

  Serial.println(F("\n=============================================="));
  Serial.println(F("   CO400 RAW PWM FRAME LOGGER v0.4.1 ACTIVE"));
  Serial.println(F("=============================================="));
}

void loop() {

  // LOW widths (structure)
  if (lowReady) {
    noInterrupts();
    unsigned long lw = lowWidth;
    lowReady = false;
    interrupts();

    if (lw >= T_IDLE_LONG) {
      dumpFrame();
      resetFrame();
      state = WAIT_IDLE;
      return;
    }

    if (lw >= T_GAP_FRAME && totalBits >= 8) {
      dumpFrame();
      resetFrame();
      state = PREAMBLE;
      return;
    }

    if (lw >= T_SYNC_MIN && lw <= T_SYNC_MAX) {
      state = READ_BITS;
      return;
    }
  }

  // HIGH widths (bit values)
  if (highReady) {
    noInterrupts();
    unsigned long hw = highWidth;
    highReady = false;
    interrupts();

    if (state == READ_BITS) {
      bool bitValid = false;
      bool bitVal = 0;

      if (hw >= T_BIT0_MIN && hw <= T_BIT0_MAX) {
        bitVal = 0;
        bitValid = true;
      }
      else if (hw >= T_BIT1_MIN && hw <= T_BIT1_MAX) {
        bitVal = 1;
        bitValid = true;
      }

      if (bitValid) {
        totalBits++;

        if (bitVal) frame[bytePos] |= (1 << bitPos);

        bitPos++;

        if (bitPos >= 8) {
          bitPos = 0;
          bytePos++;

          if (bytePos >= sizeof(frame)) {
            dumpFrame();
            resetFrame();
            state = PREAMBLE;
          }
        }
      }
    }
  }
}
