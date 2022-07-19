#include <Arduino.h>
#include <TM1637Display.h>

#define CLK 2
#define DIO 3

#define TEST_DELAY   2000

TM1637Display display(CLK, DIO);

void setup()
{
}

void loop()
{
  int k;

  display.setBrightness(0x0f);

  uint8_t data[] = { 0x0, 0x0, 0x0, 0x0 };
  display.setSegments(data);
  display.showNumberDec(1234, false);
  delay(TEST_DELAY);
  
  display.setSegments(data);
  display.showNumberDec(153, false, 3, 1);
  delay(TEST_DELAY);

  display.setSegments(data);
  for(int i=0; i<=1234; i++)
  {
    display.showNumberDec(i);
  }

}
