#include "lcd.h"
#include "font.h"
#include <string.h>

static struct {
  uint8_t x;
  uint8_t y;
} cursorPosition;

const uint8_t init_sequence []= {	// Initialization Sequence
				 LCD_DISP_OFF,	// Display OFF (sleep mode)
				 0x20, 0b00,		// Set Memory Addressing Mode
				 // 00=Horizontal Addressing Mode; 01=Vertical Addressing Mode;
				 // 10=Page Addressing Mode (RESET); 11=Invalid
				 0xB0,			// Set Page Start Address for Page Addressing Mode, 0-7
				 0xC8,			// Set COM Output Scan Direction
				 0x00,			// --set low column address
				 0x10,			// --set high column address
				 0x40,			// --set start line address
				 0x81, 0xFF,		// Set contrast control register (0 to 255)
				 0xA1,			// Set Segment Re-map. A0=address mapped; A1=address 127 mapped.
				 0xA6,			// Set display mode. A6=Normal; A7=Inverse
				 0xA8, 0x3F,		// Set multiplex ratio(1 to 64)
				 0xA4,			// Output RAM to Display
				 // 0xA4=Output follows RAM content; 0xA5,Output ignores RAM content
				 0xD3, 0x00,		// Set display offset. 00 = no offset
				 0xD5,			// --set display clock divide ratio/oscillator frequency
				 0xF0,			// --set divide ratio
				 0xD9, 0x22,		// Set pre-charge period
				 0xDA, 0x12,		// Set com pins hardware configuration
				 0xDB,			// --set vcomh
				 0x20,			// 0x20,0.77xVcc
				 0x8D, 0x14,		// Set DC-DC enable


};

void lcd_command(uint8_t cmd[], uint8_t size) {
  i2c_start((LCD_I2C_ADR), Write);
  i2c_byte(0x00);	// 0x00 for command, 0x40 for data
  for (uint8_t i=0; i<size; i++) {
    i2c_byte(cmd[i]);
  }
  i2c_stop();
}

void lcd_data(uint8_t data[], uint16_t size) {
  i2c_start((LCD_I2C_ADR), Write);
  i2c_byte(0x40);	// 0x00 for command, 0x40 for data
  for (uint16_t i = 0; i<size; i++) {
    i2c_byte(data[i]);
  }
  i2c_stop();
}

void lcd_init(uint8_t dispAttr){
  i2c_init();
  uint8_t commandSequence[sizeof(init_sequence)+1];
  for (uint8_t i = 0; i < sizeof (init_sequence); i++) {
    commandSequence[i] = init_sequence[i];
  }
  commandSequence[sizeof(init_sequence)]=(dispAttr);
  lcd_command(commandSequence, sizeof(commandSequence));
  lcd_clrscr();
}

void lcd_gotoxy(uint8_t x, uint8_t y){
  if( x > (DISPLAY_WIDTH/sizeof(FONT[0])) || y > (DISPLAY_HEIGHT/8-1)) return;// out of display
  x = x * sizeof(FONT[0]);
  cursorPosition.x=x;
  cursorPosition.y=y;
#if defined SSD1306
  uint8_t commandSequence[] = {0xb0+y, 0x21, x, 0x7f};
#elif defined SH1106
  uint8_t commandSequence[] = {0xb0+y, 0x21, 0x00+((2+x) & (0x0f)), 0x10+( ((2+x) & (0xf0)) >> 4 ), 0x7f};
#endif
  lcd_command(commandSequence, sizeof(commandSequence));
}

void lcd_clrscr(void){
  uint8_t displayBuffer[DISPLAY_WIDTH];
  memset(displayBuffer, 0x00, sizeof(displayBuffer));
  for (uint8_t i = 0; i < DISPLAY_HEIGHT/8; i++){
    lcd_gotoxy(0,i);
    lcd_data(displayBuffer, sizeof(displayBuffer));
  }
  lcd_home();
}

void lcd_home(void){
  lcd_gotoxy(0, 0);
}

void lcd_invert(uint8_t invert){
  i2c_start((LCD_I2C_ADR), Write);
  uint8_t commandSequence[1];
  if (invert != YES) {
    commandSequence[0] = 0xA6;
  } else {
    commandSequence[0] = 0xA7;
  }
  lcd_command(commandSequence, 1);
}

void lcd_sleep(uint8_t sleep){
  i2c_start((LCD_I2C_ADR), Write);
  uint8_t commandSequence[1];
  if (sleep != YES) {
    commandSequence[0] = 0xAF;
  } else {
    commandSequence[0] = 0xAE;
  }
  lcd_command(commandSequence, 1);
}

void lcd_set_contrast(uint8_t contrast){
  uint8_t commandSequence[2] = {0x81, contrast};
  lcd_command(commandSequence, sizeof(commandSequence));
}

void lcd_putc(char c){
  switch (c) {
  case '\b':
    // backspace
    lcd_gotoxy(cursorPosition.x-1, cursorPosition.y);
    lcd_putc(' ');
    lcd_gotoxy(cursorPosition.x-1, cursorPosition.y);
    break;
  case '\t':
    // tab
    if( (cursorPosition.x+4) < (DISPLAY_WIDTH/ sizeof(FONT[0])-4) ){
      lcd_gotoxy(cursorPosition.x+4, cursorPosition.y);
    }else{
      lcd_gotoxy(DISPLAY_WIDTH/ sizeof(FONT[0]), cursorPosition.y);
    }
    break;
  case '\n':
    // linefeed
    if(cursorPosition.y < (DISPLAY_HEIGHT/8-1)){
      lcd_gotoxy(cursorPosition.x, ++cursorPosition.y);
    }
    break;
  case '\r':
    // carrige return
    lcd_gotoxy(0, cursorPosition.y);
    break;
  default:
    // char doesn't fit in line
    if( (cursorPosition.x >= DISPLAY_WIDTH-sizeof(FONT[0])) || (c < ' ') ) break;
    // mapping char
    c -= ' ';

    // print char at display
    i2c_start((LCD_I2C_ADR), Write);
    i2c_byte(0x40);
    for (uint8_t i = 0; i <  sizeof(FONT[0]); i++)
      {
	// print font to ram, print 6 columns
	i2c_byte(FONT[(uint8_t)c][i]);
      }
    i2c_stop();
    cursorPosition.x += sizeof(FONT[0]);
    break;
  }
    
}

void lcd_puts(const char* s){
  while (*s) {
    lcd_putc(*s++);
  }
}

