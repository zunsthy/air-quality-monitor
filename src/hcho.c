#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

#include "common.h"
#include "tty_utils.h"

// https://www.dart-sensors.com/wp-content/uploads/2021/06/Operation-Manual-for-WZ-S-K-HCHO-Module-final.pdf
void print_hcho_data(unsigned char* frame, void *arg) {
  aq_struct *aq = (aq_struct *)arg;
  if (frame[0] != 0xff || frame[1] != 0x17) {
    printf("error frame\n");
    return;
  }
  int ppb = frame[4] * 0xff + frame[5];
  aq->hcho_ppb = ppb;
}

void read_hcho_data(int fd, aq_struct *aq) {
  unsigned char frame[8];
  loop_read_tty_data(fd, print_hcho_data, 8, 0xff, 2000, aq);
}

void measure_air_quality_hcho(char* tty_name, aq_struct *aq) {
  int fd = open(tty_name, O_RDWR | O_NOCTTY);
  if (fd < 0) exit(1);

  tcflush(fd, TCIOFLUSH);
  if (set_tty_attrs(fd, B9600) < 0) exit(2);

  read_hcho_data(fd, aq);

  close(fd);
}
