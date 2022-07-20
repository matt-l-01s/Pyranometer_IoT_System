#include <Arduino.h>
#include <TM1637Display.h>

#define CLK 2
#define DIO 3

TM1637Display display(CLK, DIO);

void setup()
{
  int sensor = A2;
  int sensor2 = A3;
  pinMode(sensor,INPUT);
  pinMode(sensor2, INPUT);
  analogReference(INTERNAL1V1);
  Serial.begin(9600);
}

void loop()
{
  float sensor_voltage, sensor_voltage2;
  float sensitivity = 0.0002021;
  float solar_irradiance, solar_irradiance2;
  float reference_voltage = 1.1;

  int analog_value = analogRead(A2);
  sensor_voltage = (analog_value * reference_voltage) / 1024.0; 
  solar_irradiance = (sensor_voltage * 8 / sensitivity);
  Serial.println(analog_value);
  
  int analog_value2 = analogRead(A3);
  sensor_voltage2 = (analog_value2 * reference_voltage) / 1024.0; 
  solar_irradiance2 = (sensor_voltage2 / sensitivity);
  Serial.println(solar_irradiance2);
  Serial.println(" ");

  delay(1000); 
  
  int k;

  display.setBrightness(0x0f);

  uint8_t data[] = { 0x0, 0x0, 0x0, 0x0 };
  display.setSegments(data);
  display.showNumberDec(solar_irradiance, false);


}
