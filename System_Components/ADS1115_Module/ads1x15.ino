// https://electropeak.com/learn/interfacing-ads1015-12-bit-adc-with-arduino/
#include "ADS1X15.h" // library reference at https://github.com/RobTillaart/ADS1X15

ADS1115 ADS(0x48);

void setup()
{
  Serial.begin(115200);
  Serial.println(__FILE__);
  Serial.print("ADS1X15_LIB_VERSION: ");
  Serial.println(ADS1X15_LIB_VERSION);

  ADS.begin();
}

void loop()
{
  if (!ADS.isConnected()) {
    Serial.println("ADS1015 Not Connected.");
  }
  else {
    ADS.setGain(0);

    int16_t val_0 = ADS.readADC(0);
    int16_t val_1 = ADS.readADC(1);
    int16_t val_2 = ADS.readADC(2);
    int16_t val_3 = ADS.readADC(3);

    float f = ADS.toVoltage(1);  // voltage factor

   Serial.print("Analog0: "); Serial.print(val_0); Serial.print('\t'); Serial.print(val_0 * f, 3);
   Serial.print("\tAnalog1: "); Serial.print(val_1); Serial.print('\t'); Serial.print(val_1 * f, 3);
   Serial.print("\tAnalog2: "); Serial.print(val_2); Serial.print('\t'); Serial.print(val_2 * f, 3);
   Serial.print("\tAnalog3: "); Serial.print(val_3); Serial.print('\t'); Serial.print(val_3 * f, 3);
  }
  delay(2000);
}
