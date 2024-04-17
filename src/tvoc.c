#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

#include "common.h"
#include "tty_utils.h"

// https://item.taobao.com/item.htm?id=625081451633&skuId=5184707210910
void print_voc_data(unsigned char* frame, void *arg) {
  aq_struct *aq = (aq_struct *)arg;
  if (frame[0] != 0x2c || frame[1] != 0xe4) {
    printf("error frame\n");
    return;
  }
  unsigned char sum = frame[0] + frame[1]
    + frame[2] + frame[3]
    + frame[4] + frame[5]
    + frame[6] + frame[7];
  int voc = frame[2] * 0xff + frame[3];
  int co2 = frame[6] * 0xff + frame[7];
  if (sum == frame[8]) {
    aq->co2_ppm = co2;
    aq->tvoc_ug = voc;
  } else {
    printf("tvoc frame data error\n");
  }
}

void read_voc_data(int fd, aq_struct *aq) {
  unsigned char frame[8];
  loop_read_tty_data(fd, print_voc_data, 9, 0x2c, 2000, aq);
}

void measure_air_quality_tvoc(char* tty_name, aq_struct *aq) {
  int fd = open(tty_name, O_RDWR | O_NOCTTY);
  if (fd < 0) exit(1);

  tcflush(fd, TCIOFLUSH);
  if (set_tty_attrs(fd, B9600) < 0) exit(2);

  read_voc_data(fd, aq);

  close(fd);
}
