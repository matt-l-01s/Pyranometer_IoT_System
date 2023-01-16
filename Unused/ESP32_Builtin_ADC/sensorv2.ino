#include "esp_adc_cal.h"
#define Sensor1    32

int ADC_value = 0;
float Voltage = 0.0;
float final_value = 0.0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop()
{
  // put your main code here, to run repeatedly:
  float sensor_voltage;
  float sensitivity = 0.0002021;
  float solar_irradiance;
  float reference_voltage = 1.1;

  int analog_value = analogRead(Sensor1);
  sensor_voltage = (analog_value * reference_voltage) / 1024.0;
  solar_irradiance = (sensor_voltage / sensitivity);
  Serial.println(solar_irradiance);
  delay(1000);
}
