#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>

#define MAXEVENTS 16

int set_tty_attrs(int fd, int baudrate)
{
  struct termios opt;
  if (tcgetattr(fd, &opt) != 0)
  {
    return -1;
  }

  cfsetispeed(&opt, baudrate);
  cfsetospeed(&opt, baudrate);

  opt.c_cc[VMIN] = 0;
  opt.c_lflag = 0;
  opt.c_cflag &= ~CSIZE;
  opt.c_cflag |= CS8;

  return tcsetattr(fd, TCSANOW, &opt);
}

void loop_read_tty_data(int fd, void (*cb)(unsigned char *, void *), int frame_size, unsigned char start, int timeout, void* arg) {
  struct epoll_event event;
  struct epoll_event *events;

  int efd = epoll_create1(0);
  event.data.fd = fd;
  event.events = EPOLLIN | EPOLLET;
  
  epoll_ctl(efd, EPOLL_CTL_ADD, fd, &event);
  events = calloc(MAXEVENTS, sizeof event);

  unsigned char frame[frame_size];
  // least 2 frames and 8n bytes
  unsigned char buf[(frame_size * 2 / 8 + 1) * 8];

  for (;;) {
    int n = epoll_wait(efd, events, MAXEVENTS, timeout);
    if (n > 0) {
      int len = read(events[0].data.fd, buf, sizeof buf);
      if (len > 0) {
        int i;
        for (i = 0; i < len; i++) {
          if (buf[i] == start && i <= len - frame_size) {
            memcpy(frame, buf, frame_size);
            cb(frame, arg);
            i += frame_size - 1;
          }
        }
      }
    }
  }

  free(events);
}
