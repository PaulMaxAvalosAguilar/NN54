#ifndef LCD_H
#define LCD_H

#ifdef __cplusplus
extern "C" {
#endif
	
#include <inttypes.h>
#include "i2c.h"

/* TODO: define displaycontroller */
#define SSD1306                             	// or SH1106, check datasheet of your display

/* TODO: define font */
#define FONT			ssd1306oled_font// set font here, refer font-name at font.h/font.c

/* TODO: define I2C-adress for display */
// using 7-bit-adress for lcd-library
// if you use your own library for twi check I2C-adress-handle
#define LCD_I2C_ADR     	(0x78 >> 1)	// 7 bit slave-adress without r/w-bit
						// r/w-bit are set/unset by library
						// e.g. 8 bit slave-adress:
						// 0x78 = adress 0x3C with cleared r/w-bit (write-mode)
#ifndef YES
	#define YES		1
#endif

#define LCD_DISP_OFF		0xAE
#define LCD_DISP_ON		0xAF

#define WHITE			0x01
#define BLACK			0x00

#define DISPLAY_WIDTH		128
#define DISPLAY_HEIGHT		64



void lcd_command(uint8_t cmd[], uint8_t size);	// transmit command to display
void lcd_data(uint8_t data[], uint16_t size);	// transmit data to display
void lcd_init(uint8_t dispAttr);
void lcd_home(void);                        	// set cursor to 0,0
void lcd_invert(uint8_t invert);		// invert display
void lcd_sleep(uint8_t sleep);			// display goto sleep (power off)
void lcd_set_contrast(uint8_t contrast);	// set contrast for display
void lcd_puts(const char* s);			// print string, \n-terminated, from ram on screen (TEXTMODE)
void lcd_puts_p(const char* progmem_s);		// print string from flash on screen (TEXTMODE)
void lcd_clrscr(void);				// clear screen 
void lcd_gotoxy(uint8_t x, uint8_t y);		// set curser at pos x, y. x means character,
						// y means line (page, refer lcd manual)
void lcd_putc(char c);				// print character on screen at TEXTMODE

#ifdef __cplusplus
}
#endif
#endif /*  LCD_H  */
