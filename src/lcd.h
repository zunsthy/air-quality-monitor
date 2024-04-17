#ifndef AIR_QUALITY_LCD
#define AIR_QUALITY_LCD

typedef unsigned char BYTE;

void lcd_init(int fd);

void lcd_send_code(BYTE code);
void lcd_send_byte(BYTE code);
void lcd_send_data(BYTE *data, size_t len);

#endif /* AIR_QUALITY_LCD */
