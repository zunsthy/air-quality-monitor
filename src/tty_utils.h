#ifndef TTY_UTILS
#define TTY_UTILS

int set_tty_attrs(int fd, int baudrate);

void loop_read_tty_data(int fd, void (*cb)(unsigned char *, void *), int frame_size, unsigned char start, int timeout, void *arg);

#endif /* TTY_UTILS */
