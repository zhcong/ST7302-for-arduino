# ST7302-for-arduino
- micropython version see [https://github.com/zhcong/ST7302-for-micropython](https://github.com/zhcong/ST7302-for-micropython)   
- datasheet pdf file see [this link](doc/ST7302_V0.0.pdf)
## screen direction
![direction](doc/screen-arrow.png)

## usage
``` c++
#include <ST7302SPI.h>

#define _PIN_RST_ 1
#define _PIN_DC_ 0
#define _PIN_CS_ 7
#define _PIN_SCK_ 2
#define _PIN_MOSI_ 3
#define _PIN_MISO_ -1

ST7302SPI st7302spi(_PIN_SCK_, _PIN_MISO_, _PIN_MOSI_, _PIN_RST_, _PIN_DC_, _PIN_CS_, 250, 122);

void setup() {
  st7302spi.begin();
  // inversion screen
  st7302spi.inversion_on();
  //draw point
  st7302spi.point(10,10,1);
  // draw ascii
  char str1[] = "hello word1.";
  st7302spi. text(1, 20, str1, strlen(str1));
  st7302spi.flush_buffer();
}

void loop() {
}
```