#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

// https://www.kernel.org/doc/Documentation/i2c/dev-interface
// https://www.ti.com/lit/ds/symlink/pcf8574.pdf
// https://en.wikipedia.org/wiki/Hitachi_HD44780_LCD_controller

#define PCF8574_ADDR 0b00100111

#define BACKLIGHT_ON 0b00001000
#define BACKLIGHT_OFF 0b00000000

typedef unsigned char BYTE;

BYTE BL = BACKLIGHT_OFF;

int fd = 0;

void lcd_send_4bit_code(BYTE code) {
  BYTE b = 0;
  b = (code << 4) | 0x04;
  write(fd, &b, 1);
  b = 0;
  write(fd, &b, 1);
  usleep(40); // > 37us
}

void lcd_send_8bit_code(BYTE code) {
  usleep(5); // align clock
  BYTE b = 0;
  b = code & 0xf0 | 0x04 | BL;
  write(fd, &b, 1);
  b = 0 | BL;
  write(fd, &b, 1);
  b = (code & 0x0f) << 4 | 0x04 | BL;
  write(fd, &b, 1);
  b = 0 | BL;
  write(fd, &b, 1);
  usleep(40); // > 37us
}

void lcd_send_code(BYTE code) {
  lcd_send_8bit_code(code);
}

void lcd_send_byte(BYTE data) {
  BYTE buf[4] = {
    data & 0xf0 | 0x05 | BL,
    0 | BL,
    ((data & 0x0f) << 4) | 0x05 | BL,
    0 | BL
  };
  write(fd, buf, 4);
}

void lcd_send_data(BYTE *data, size_t len) {
  size_t i;
  for (i = 0; i < len; i++)
    lcd_send_byte(data[i]);
}

void lcd_init(int lcd_fd) {
  fd = lcd_fd;
  if (fd < 0) exit(1);

  if (ioctl(fd, I2C_SLAVE, PCF8574_ADDR) < 0) exit(2);

  usleep(15000); // > 15ms

  lcd_send_4bit_code(0x3); // function set DL=1
  usleep(4100); // > 4.1ms
  lcd_send_4bit_code(0x3); // function set DL=1
  usleep(100); // > 100us
  lcd_send_4bit_code(0x3); // function set DL=1
  usleep(4100);
  // set 4-bit mode
  lcd_send_4bit_code(0x2); // function set DL=0

  // 2 line display
  lcd_send_8bit_code(0x28); // function set DL=0 N=1 F=0
  // display off
  lcd_send_8bit_code(0x08); // display control D=0 C=0 B=0
  // clear display
  lcd_send_8bit_code(0x01); // clear display
  // entry mode: inc, no shift
  lcd_send_8bit_code(0x06); // entry mode set I/D=1 S=0
  // display on
  lcd_send_8bit_code(0x0c); // display control D=1 C=0 B=0
  
  BL = BACKLIGHT_ON;
}
