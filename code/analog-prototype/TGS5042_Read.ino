/*
 * File:TGS5042_Read.ino
 * VERS: 0.1 REV:260613
 * TGS5042 Carbon Monoxide Sensor Readout Sketch
 * Optimized for Arduino Nano with External 3.0V Analog Reference(AREF)
 * 
 * This is a quick test - it should read out from the sensor.
 * 
 *  NOTE:  PIC16F688 (U2) Must be removed from the circuit card!
 *         Sensor Output analog signal from U3 Buffer is on U2 socket Pin 8 (AN6)
 *         Power pins:  ( you can power the board from the Nano as well)
 *          3v3 is on U2 pin 9 (AN7)
 *          5V  power is on U2 pin 9 (AN7)
 *          GND is on U2 pin 14 (Vss) 
 *
 */

// Hardware Pin Configuration
const int ANALOG_INPUT_PIN = A0;  // Connects to Stage 2 Output (Op-amp Pin 7)

// Circuit and Sensor Constants
const float V_REF = 3.0;          // External Reference Voltage tied to AREF pin (Volts)
const float ADC_RESOLUTION = 1023.0; 
const float TIA_GAIN_RESISTOR = 1000000.0; // 1M Ohm feedback resistor (Stage 1)

// Hardware 3v3 R17 = 480k R16 = 47k 
// Note: Using the measured value 
const float BASELINE_VOLTAGE = 0.298;  //they were probabily looking for 1/10th 3.3v

// Sensor Calibration Constant
const float SENSOR_SENSITIVITY = 1.642; // From the label on sensor

void setup() {
  Serial.begin(9600);
  
  // CRITICAL: Tells Arduino to use the 3.0V rail on the AREF pin for ADC conversions
  analogReference(EXTERNAL); 
  pinMode(ANALOG_INPUT_PIN, INPUT);
  
  Serial.println(F("======================================="));
  Serial.println(F(" TGS5042 CO Sensor Monitor (VREF 3.0V) "));
  Serial.println(F("======================================="));
}

void loop() {
  // 1. Read the raw ADC value (Mapped against the 3.0V AREF ceiling)
  int rawAdc = analogRead(ANALOG_INPUT_PIN);
  
  // 2. Convert raw digital steps back into measured voltage
  float measuredVoltage = (rawAdc / ADC_RESOLUTION) * V_REF;
  
  // 3. Isolate the gas voltage shift from the hardware baseline
  float gasVoltageShift = measuredVoltage - BASELINE_VOLTAGE;
  
  // 4. Convert the voltage shift back to sensor current (A -> nA)
  float sensorCurrent_nA = (gasVoltageShift / TIA_GAIN_RESISTOR) * 1000000000.0;
  
  // 5. Compute the final carbon monoxide concentration in ppm
  float co_ppm = sensorCurrent_nA / SENSOR_SENSITIVITY;
  
  // Clamp negative drift values gracefully
  if (co_ppm < 0.0) co_ppm = 0.0;

  // 6. Print diagnostics
  Serial.print(F("ADC: "));  Serial.print(rawAdc);
  Serial.print(F(" | V_out: ")); Serial.print(measuredVoltage, 3);
  Serial.print(F("V | Current: ")); Serial.print(sensorCurrent_nA, 2);
  Serial.print(F(" nA | CO Conc: ")); Serial.print(co_ppm, 1);
  Serial.println(F(" ppm"));

  delay(2000);
}
