/*

 */
 /*
 * File: TGS5042_Test1.ino
 * VERS: 0.1 REV:260613
 * CO400 CO Alarm TGS5042 Carbon Monoxide Sensor Readout Sketch #2
 * 
 * TGS5042 Functional Tester & Response Monitor
 * Use this to verify if an old sensor reacts to smoke/CO.  
 *
 *  Sensor  TGS5042 + 10k PTC Thermistor Monitor
 * 
 *  NOTE:  PIC16F688 (U2) Must be removed from the circuit card!
 *         Analog signals:
 *           Sensor Output (from U3 Buffer) U2 socket Pin 8 (AN6)
 *           Themistor input pin 11 (AN2) 
 *           Termistor Enable pin 10 (RC0) GND to read (LED 1 will turn on)
 *         Power pins:  ( you can power the board from the Nano as well)
 *          3v3 is on U2 pin 9 (AN7)
 *          5V  power is on U2 pin 9 (AN7)
 *          GND is on U2 pin 14 (Vss) 
 *
 */
const int CO_SENSOR_PIN = A0;      // Op-amp Pin 7 (Buffer Out)
const float V_REF = 3.0;           // Precision 3.0V tied to AREF pin
const float ADC_RESOLUTION = 1023.0;

// Note: This is measured on my C400 Unit.
const float BASELINE_VOLTAGE =  0.298; 

// Note: Calibration from sensor label
const float SENSOR_SENSITIVITY = 1.642; 

float maxPpmDetected = 0.0;

void setup() {
  Serial.begin(9600);
  analogReference(EXTERNAL); // Using the 3.0V line for accuracy
  pinMode(CO_SENSOR_PIN, INPUT);
  
  Serial.println(F("======================================="));
  Serial.println(F("    TGS5042 QUICK FUNCTIONAL TEST      "));
  Serial.println(F("======================================="));
  Serial.println(F("Expose sensor to smoke and check for spikes."));
}

void loop() {
  int rawAdc = analogRead(CO_SENSOR_PIN);
  float measuredVoltage = (rawAdc / ADC_RESOLUTION) * V_REF;
  
  // Calculate voltage change from baseline
  float voltageShift = measuredVoltage - BASELINE_VOLTAGE;
  
  // Voltage shift to current: (V / 1M Ohm) * 10^9 to get nA
  float current_nA = (voltageShift / 1000000.0) * 1000000000.0;
  
  // Current to PPM
  float co_ppm = current_nA / SENSOR_SENSITIVITY;
  if (co_ppm < 0.0) co_ppm = 0.0;

  // Track the absolute highest reading during the test
  if (co_ppm > maxPpmDetected) {
    maxPpmDetected = co_ppm;
  }

  // Print results
  Serial.print(F("Raw ADC: "));   Serial.print(rawAdc);
  Serial.print(F(" | Volts: "));   Serial.print(measuredVoltage, 3);
  Serial.print(F("V | Current CO: ")); Serial.print(co_ppm, 1);
  Serial.print(F(" ppm | PEAK CO: "));  Serial.print(maxPpmDetected, 1);
  Serial.println(F(" ppm"));

  delay(1000); // 1-second updates for snappy testing
}
