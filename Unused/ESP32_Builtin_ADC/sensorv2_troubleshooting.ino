#include "esp_adc_cal.h"
#define Sensor1    32

int ADC_value = 0;
float Voltage = 0.0;
float final_value = 0.0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  esp_adc_cal_characteristics_t adc_chars;
  esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 3130, &adc_chars);
//  adc1_config_width(ADC_WIDTH_BIT_12);
//  adc1_config_channel_atten(ADC1_CHANNEL_5,ADC_ATTEN_DB_0);
 }

void loop()
{
  // put your main code here, to run repeatedly:
  float sensor_voltage;
  float sensitivity = 0.0002021;
  float solar_irradiance;
  float reference_voltage = 3.13;

  //int analog_value = analogRead(Sensor1);
  int analog_value = adc1_get_raw(ADC1_CHANNEL_5);
  
  sensor_voltage = (analog_value * reference_voltage) / 1024.0;
  //sensor_voltage = readADC_Cal(analog_value)
  solar_irradiance = (sensor_voltage / sensitivity); 
  Serial.println(analog_value);
//  Serial.print('\t');
//  Serial.print(sensor_voltage);
//  Serial.print('\t');
//  Serial.println(solar_irradiance);
  delay(1000);
}

uint32_t readADC_Cal(int ADC_Raw)
{
  esp_adc_cal_characteristics_t adc_chars;

  esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 3130, &adc_chars);
  return(esp_adc_cal_raw_to_voltage(ADC_Raw, &adc_chars));
}
