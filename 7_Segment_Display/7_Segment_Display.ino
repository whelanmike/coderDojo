#include <TM1637.h>
// https://docs.google.com/document/d/1DJ__m0_GJ4j97u8TfvRoCejU2ZOkfG_-UwyFeOnnGKI/edit
// Configure pins on Arduino.
#define CLK 2 
#define DIO 3

TM1637 tm(CLK, DIO);

int brightness = 3;

void setup()
{
    tm.init();
    tm.setBrightness(brightness);
}

void loop()
{
    for (int i = -100 ; i < 10000; i++) {
        tm.dispNumber(i);
        delay(500);
        tm.setBrightness(brightness);
        brightness +=1;
        if (brightness >5){
          brightness =0;
    }
  }
}


//        tm.display(0, 8);
//        tm.display(1, 2);
//        tm.display(2, 5);
//        tm.display(3, 1);
