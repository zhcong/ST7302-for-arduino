#include "ST7302SPI.h"

#define _PIN_RST_ 1
#define _PIN_DC_ 0
#define _PIN_CS_ 7
#define _PIN_SCK_ 2
#define _PIN_MOSI_ 3
#define _PIN_MISO_ -1

ST7302SPI st7302spi(_PIN_SCK_, _PIN_MISO_, _PIN_MOSI_, _PIN_RST_, _PIN_DC_, _PIN_CS_, 250, 122);

void setup() {
  Serial.begin(115200);
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  digitalWrite(12, LOW);
  digitalWrite(13, HIGH);
  st7302spi.begin();
  char str1[] = "hello word1.";
  st7302spi. text(1, 20, str1, strlen(str1));
  st7302spi.flush_buffer();
  delay(2000);
  char str2[] = "hello word2.";
  st7302spi. text(1, 40, str2, strlen(str2));
  st7302spi.flush_buffer();
  delay(2000);
  st7302spi.inversion_on();
  char str3[] = "hello word3.";
  st7302spi. text(1, 80, str3, strlen(str3));
  st7302spi.flush_buffer();
}

void loop() {
  // delay(2000);
}
