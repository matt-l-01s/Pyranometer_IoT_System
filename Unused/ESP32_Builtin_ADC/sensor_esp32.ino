int sensorPin = 36;    // select the input pin for the potentiometer
  
void setup() {
  // declare the ledPin as an OUTPUT:
  Serial.begin(115200);
  analogSetAttenuation(ADC_0db);
}

void loop() {
  // read the value from the sensor:
  Serial.println(analogRead(sensorPin));
  delay(2000);
}
