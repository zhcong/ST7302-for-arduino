#include <Print.h>
#include <SPI.h>
#include <Arduino.h>

#define BYTE_BIT_SIZE 8.0

class ST7302SPI
{
  public:
    ST7302SPI(uint8_t sck_pin, uint8_t miso_pin, uint8_t mosi_pin, uint8_t reset_pin, uint8_t dc_pin, uint8_t cs_pin, uint8_t width, uint8_t height);

    void begin(); // Initially the LCD with the defaults settings
    void inversion_off();
    void inversion_on();
    void clear();
    void display();
    void point(uint8_t x, uint8_t y, uint8_t color);
    void no_display();

    void flush_buffer();
    void send_command(uint8_t command);
    void send_param(uint8_t param);
    void set_1();

  private:
    uint8_t _sck_pin, _miso_pin, _mosi_pin, _reset_pin, _dc_pin, _cs_pin;
    int _width, _height;
    uint8_t *_buffer;
    int _buffer_size;

    uint8_t _mix_bit(uint8_t bit1, uint8_t bit2);
    void _clear_buffer();
    void _send_12_row(int _12_row);
    void _send_12_row_byte(uint8_t byte0, uint8_t byte1, uint8_t byte2,
                           uint8_t byte3, uint8_t byte4, uint8_t byte5,
                           uint8_t byte6, uint8_t byte7, uint8_t byte8,
                           uint8_t byte9, uint8_t byte10, uint8_t byte11);
    void _send_12_row_bit(int line_i, uint8_t byte0, uint8_t byte1, uint8_t byte2,
                          uint8_t byte3, uint8_t byte4, uint8_t byte5,
                          uint8_t byte6, uint8_t byte7, uint8_t byte8,
                          uint8_t byte9, uint8_t byte10, uint8_t byte11);
};
