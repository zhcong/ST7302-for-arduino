#include "ST7302SPI.h"

ST7302SPI::ST7302SPI(uint8_t sck_pin, uint8_t miso_pin, uint8_t mosi_pin, uint8_t reset_pin, uint8_t dc_pin, uint8_t cs_pin, uint8_t width, uint8_t height)
{
  _sck_pin = sck_pin;
  _miso_pin = miso_pin;
  _mosi_pin = mosi_pin;
  _reset_pin = reset_pin;
  _dc_pin = dc_pin;
  _cs_pin = cs_pin;
  _width = int(ceil(width / BYTE_BIT_SIZE) * BYTE_BIT_SIZE);
  _height = int(ceil(height / 12.0) * 12);
  _buffer_size = int(ceil(_height * _width / BYTE_BIT_SIZE));
  _buffer = new uint8_t[_buffer_size];
  pinMode(_reset_pin, OUTPUT);
  pinMode(_dc_pin, OUTPUT);
  pinMode(_cs_pin, OUTPUT);
}

void ST7302SPI::begin()
{
  SPI.begin(_sck_pin, _miso_pin, _mosi_pin, _cs_pin);
  SPI.beginTransaction(SPISettings(800000, MSBFIRST, SPI_MODE0));
  digitalWrite(_reset_pin, LOW);
  delay(100);
  digitalWrite(_reset_pin, HIGH);
  digitalWrite(_dc_pin, HIGH);
  digitalWrite(_cs_pin, LOW);
  delay(100);
  send_command(0x38);
  send_command(0xEB);//Enable OTP
  send_param(0x02);
  send_command(0xD7);//OTP Load Control
  send_param(0x68);
  send_command(0xD1);//Auto Power Control
  send_param(0x01);
  send_command(0xC0);//Gate Voltage Setting VGH=12V ; VGL=-5V
  send_param(0x80);
  send_command(0xC1);//VSH Setting
  send_param(0x28);//
  send_param(0x28);
  send_param(0x28);
  send_param(0x28);
  send_param(0x14);
  send_param(0x00);
  send_command(0xC2);//VSL Setting VSL=0
  send_param(0x00);
  send_param(0x00);
  send_param(0x00);
  send_param(0x00);
  send_command(0xCB);//VCOMH Setting
  send_param(0x14);//14  0C   7
  send_command(0xB4);//Gate EQ Setting HPM EQ LPM EQ
  send_param(0xE5);
  send_param(0x77);
  send_param(0xF1);
  send_param(0xFF);
  send_param(0xFF);
  send_param(0x4F);
  send_param(0xF1);
  send_param(0xFF);
  send_param(0xFF);
  send_param(0x4F);
  send_command(0x11);//Sleep out
  delay(100);  // delay_ms 100ms
  send_command(0xC7);//OSC Setting
  send_param(0xA6);
  send_param(0xE9);
  send_command(0xB0);   //Duty Setting
  send_param(0x64);  //250duty/4=63
  send_command(0x36);//Memory Data Access Control
  send_param(0x20);
  send_command(0x3A);//Data Format Select 3 write for 24 bit
  send_param(0x11);
  send_command(0xB9);//Source Setting
  send_param(0x23);
  send_command(0xB8);//Panel Setting Frame inversion
  send_param(0x09);
  send_command(0x2A);////Column Address Setting S61~S182
  send_param(0x05);
  send_param(0x36);
  send_command(0x2B);////Row Address Setting G1~G250
  send_param(0x00);
  send_param(0xC7);
  send_command(0xD0);
  send_param(0x1F);
  send_command(0x29);//Display on
  send_command(0xB9);//enable CLR RAM
  send_param(0xE3);
  delay(100);
  send_command(0xB9);//enable CLR RAM
  send_param(0x23);
  send_command(0x72);
  send_param(0x00);         //Destress OFF
  send_command(0x39);//LPM
  send_command(0x2A);   //Column Address Setting
  send_command(0x19);
  send_param(0x23);  //35
  send_command(0x2B);   //Row Address Setting
  send_param(0);
  send_param(0x7C);
  send_param(0x2C);   //write image data
  delay(100);
  clear();
}

void ST7302SPI::clear() {
  send_command(0xb9);  // enable CLR RAM
  send_param(0xe3);
  delay(200);
  send_command(0xb9);  // disable CLR RAM
  send_param(0x23);
  _clear_buffer();
}

void ST7302SPI::display() {
  send_command(0x29);
}

void ST7302SPI::no_display() {
  send_command(0x28);
}

void ST7302SPI::inversion_off()
{
  send_command(0x20);
}

void ST7302SPI::inversion_on()
{
  send_command(0x21);
}

void ST7302SPI::flush_buffer() {
  // address set
  send_command(0x2a);
  send_param(0x19);
  send_param(0x19 + _height / 12);
  send_command(0x2b);
  send_param(0x00);
  send_param(_width / 2 - 1);
  send_command(0x2c);
  // pin set
  digitalWrite(_cs_pin, LOW);
  digitalWrite(_dc_pin, HIGH);
  delayMicroseconds(10);
  // data send
  for (int i = 0; i != _height / 12; i++) {
    _send_12_row(i);
  }
  // pin reset
  delayMicroseconds(50);
  digitalWrite(_cs_pin, HIGH);
}

void ST7302SPI::_send_12_row(int _12_row) {
  int _12_row_start = int(_width / BYTE_BIT_SIZE) * _12_row * 12;
  int width_byte_size = int(_width / BYTE_BIT_SIZE);
  for (int i = 0; i != width_byte_size; i++) {
    _send_12_row_byte(_buffer[_12_row_start + width_byte_size * 0 + i],
                      _buffer[_12_row_start + width_byte_size * 1 + i],
                      _buffer[_12_row_start + width_byte_size * 2 + i],
                      _buffer[_12_row_start + width_byte_size * 3 + i],
                      _buffer[_12_row_start + width_byte_size * 4 + i],
                      _buffer[_12_row_start + width_byte_size * 5 + i],
                      _buffer[_12_row_start + width_byte_size * 6 + i],
                      _buffer[_12_row_start + width_byte_size * 7 + i],
                      _buffer[_12_row_start + width_byte_size * 8 + i],
                      _buffer[_12_row_start + width_byte_size * 9 + i],
                      _buffer[_12_row_start + width_byte_size * 10 + i],
                      _buffer[_12_row_start + width_byte_size * 11 + i]);
  }
}

void ST7302SPI::send_command(uint8_t command)
{
  digitalWrite(_cs_pin, LOW);
  digitalWrite(_dc_pin, LOW);
  delayMicroseconds(10);
  SPI.transfer(command);
  delayMicroseconds(50);
  digitalWrite(_dc_pin, HIGH);
  digitalWrite(_cs_pin, HIGH);
}

void ST7302SPI::send_param(uint8_t data)
{
  digitalWrite(_cs_pin, LOW);
  digitalWrite(_dc_pin, HIGH);
  delayMicroseconds(10);
  SPI.transfer(data);
  delayMicroseconds(50);
  digitalWrite(_cs_pin, HIGH);
}

void ST7302SPI::point(uint8_t x, uint8_t y, uint8_t color) {
  int bits_pos = int(ceil((y * _width + x) / BYTE_BIT_SIZE));
  bitWrite(_buffer[bits_pos], int(BYTE_BIT_SIZE) - (y * _width + x) % int(BYTE_BIT_SIZE), color == 1);
}

void ST7302SPI::_clear_buffer() {
  memset(_buffer, 0, _buffer_size);
}

void ST7302SPI::_send_12_row_byte(uint8_t byte0, uint8_t byte1, uint8_t byte2,
                                  uint8_t byte3, uint8_t byte4, uint8_t byte5,
                                  uint8_t byte6, uint8_t byte7, uint8_t byte8,
                                  uint8_t byte9, uint8_t byte10, uint8_t byte11) {
  _send_12_row_bit(0, byte0, byte1, byte2, byte3, byte4, byte5, byte6, byte7, byte8, byte9, byte10, byte11);
  _send_12_row_bit(2, byte0, byte1, byte2, byte3, byte4, byte5, byte6, byte7, byte8, byte9, byte10, byte11);
  _send_12_row_bit(4, byte0, byte1, byte2, byte3, byte4, byte5, byte6, byte7, byte8, byte9, byte10, byte11);
  _send_12_row_bit(6, byte0, byte1, byte2, byte3, byte4, byte5, byte6, byte7, byte8, byte9, byte10, byte11);
}

void ST7302SPI::_send_12_row_bit(int line_i, uint8_t byte0, uint8_t byte1, uint8_t byte2,
                                 uint8_t byte3, uint8_t byte4, uint8_t byte5,
                                 uint8_t byte6, uint8_t byte7, uint8_t byte8,
                                 uint8_t byte9, uint8_t byte10, uint8_t byte11) {
  uint8_t data = (byte0 << line_i >> 0 & 0xC0)
                 | (byte1 << line_i >> 2 & 0x30)
                 | (byte2 << line_i >> 4 & 0x0C)
                 | (byte3 << line_i >> 6 & 0x03);
  SPI.transfer(data);
  data = (byte4 << line_i>> 0 & 0xC0)
         | (byte5 << line_i>> 2 & 0x30)
         | (byte6 << line_i>> 4 & 0x0C)
         | (byte7 << line_i >> 6 & 0x03);
  SPI.transfer(data);
  data = (byte8 << line_i>> 0 & 0xC0)
         | (byte9 << line_i >> 2 & 0x30)
         | (byte10 << line_i>> 4 & 0x0C)
         | (byte11 << line_i >> 6 & 0x03);
  SPI.transfer(data);
}

void ST7302SPI::set_1() {
//      memset(_buffer, 0xaa, _buffer_size);
  for (int i = 0; i != _buffer_size; i++) {
    _buffer[i] = (i / (_width / 8) % 2) == 0 ? 0x55 : 0xaa;
  }
}
