
 /*
 * File:TGS5042_Read2.ino
 * VERS: 0.1 REV:260613
 * CO400 CO Alarm TGS5042 Carbon Monoxide Sensor Readout Sketch #2
 * 
 * Optimized for Arduino Nano with External 3.0V Analog Reference(AREF)
 * Adds  temperature compensation to fast readout  
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
// Hardware Pin Configuration
const int CO_SENSOR_PIN   = A0;  // Connects to Op-amp Pin 7 (Buffer Out)
const int THERMISTOR_PIN   = A1;  // Connects to Divider junction of the 10k Leg
const int THERMISTOR_GND   = 4;   // Digital Port tied to the Thermistor Ground side

// Measurement Constants
const float V_REF = 3.0;                  // Precision 3.0V tied to AREF pin (Volts)
const float ADC_RESOLUTION = 1023.0; 
const float TIA_GAIN_RESISTOR = 1000000.0; // 1M Ohm feedback resistor (Stage 1)
const float SENSOR_SENSITIVITY = 1.642;    // Your barcode factor (nA/ppm)


// Note: This remains calculated against 5V since the divider hooks to the 5V rail.
const float BASELINE_VOLTAGE = 2.1359; 

void setup() {
  Serial.begin(9600);
  
  // Set Arduino to measure all analog values strictly against the clean 3.0V AREF line
  analogReference(EXTERNAL); 
  
  // Configure the Thermistor Ground Gate to hide in a safe, disconnected state
  pinMode(THERMISTOR_GND, INPUT); 
  pinMode(CO_SENSOR_PIN, INPUT);
  pinMode(THERMISTOR_PIN, INPUT);
  
  Serial.println(F("======================================="));
  Serial.println(F(" TGS5042 System Ready (Low-Power Temp) "));
  Serial.println(F("======================================="));
}

void loop() {
  // --- STEP 1: READ TEMPERATURE (Dynamic Activation Loop) ---
  pinMode(THERMISTOR_GND, OUTPUT);   // Turn on the ground connection
  digitalWrite(THERMISTOR_GND, LOW); // Pull to absolute 0V Ground
  delay(5);                          // Let the analog node settle down smoothly

  int tempAdc = analogRead(THERMISTOR_PIN);
  
  pinMode(THERMISTOR_GND, INPUT);    // Instantly put back into High-Z to turn off the path

  // Calculate PTC resistance from the reading
  float vTemp = (tempAdc / ADC_RESOLUTION) * V_REF;
  float ptcResistance = 10000.0 * (vTemp / (V_REF - vTemp)); // Standard voltage divider math

  // Linear calculation for a baseline 10k PTC (Approximated 0.4% change per °C)
  float currentTemperature = 25.0 + ((ptcResistance - 10000.0) / 40.0);

  // --- STEP 2: READ CARBON MONOXIDE LEVEL ---
  int coAdc = analogRead(CO_SENSOR_PIN);
  float measuredVoltage = (coAdc / ADC_RESOLUTION) * V_REF;
  
  // Isolate gas voltage shift from the static hardware baseline offset
  float gasVoltageShift = measuredVoltage - BASELINE_VOLTAGE;
  
  // Convert voltage shift back to nanoamperes (Current = V / R * 10^9)
  float sensorCurrent_nA = (gasVoltageShift / TIA_GAIN_RESISTOR) * 1000000000.0;
  
  // Compute basic gas output concentration
  float co_ppm = sensorCurrent_nA / SENSOR_SENSITIVITY;
  if (co_ppm < 0.0) co_ppm = 0.0;

  // --- STEP 3: PRINT COMBINED DIAGNOSTICS ---
  Serial.print(F("Temp: "));  Serial.print(currentTemperature, 1);  Serial.print(F("°C"));
  Serial.print(F(" | PTC: ")); Serial.print(ptcResistance / 1000.0, 2); Serial.print(F(" kOhm"));
  Serial.print(F(" | CO Conc: ")); Serial.print(co_ppm, 1); Serial.println(F(" ppm"));

  delay(2000); // Wait 2 seconds before sampling again
}
