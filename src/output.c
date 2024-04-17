#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include "common.h"
#include "lcd.h"

void set_lcd_screen(int fd) {
  lcd_init(fd);
  printf("LCD init complete.\n");

  lcd_send_code(0x80 | 0x00); // set ddram 0x00
  usleep(40);
  lcd_send_data("HCHO     ppb", 12);
  lcd_send_code(0x80 | 0x40); // set ddram 0x40
  lcd_send_data("TVOC     ug/m3", 14);
  usleep(40);
}

void set_lcd_data(aq_struct* aq) {
  char hcho[5] = { 0 };
  char tvoc[5] = { 0 };
  snprintf(hcho, 5, "%4d", aq->hcho_ppb);
  lcd_send_code((0x80 | 0x00) + 5);
  lcd_send_data(hcho, 4);
  snprintf(tvoc, 5, "%4d", aq->tvoc_ug);
  lcd_send_code((0x80 | 0x40) + 5);
  lcd_send_data(tvoc, 4);
}

void print_air_quality_data(char *i2c_dev, aq_struct* aq) {
  int fd = open(i2c_dev, O_RDWR);
  set_lcd_screen(fd);

  for (;;) {
    sleep(1);
    set_lcd_data(aq);
    printf("HCHO: %3dppb, TVOC: %3dug/m3, CO2: %3dppm\n",
        aq->hcho_ppb,
        aq->tvoc_ug,
        aq->co2_ppm);
  }

  close(fd);
}
