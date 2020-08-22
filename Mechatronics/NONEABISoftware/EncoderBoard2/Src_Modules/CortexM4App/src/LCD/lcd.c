#include "lcd.h"
#include "font.h"
#include <string.h>

static struct {
  uint8_t x;
  uint8_t y;
} cursorPosition;


void lcd_command(uint8_t cmd[], uint8_t size) {
  i2c_start(LCD_I2C_ADR,size);
  cmd[0] = 0x00;// 0x00 for command, 0x40 for data
  for (uint8_t i=0; i<size; i++) {
    i2c_write(cmd[i]);
  }
}

void lcd_data(uint8_t data[], uint16_t size) {
  i2c_start(LCD_I2C_ADR,size);
  data[0] = 0x40; // 0x00 for command, 0x40 for data
  for (uint16_t i = 0; i<size; i++) {
    i2c_write(data[i]);
  }
}


void lcd_init(){
  uint8_t commandSequence[]= {	// Initialization Sequence
			      0x00, //Space for command
			      0xAE,	// Display OFF (sleep mode)
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
			      0xAF, //turn on SSD1306 panel
  };
  
  lcd_command(commandSequence, sizeof(commandSequence));
  lcd_clrscr();
}

void lcd_home(void){
  lcd_gotoxy(0, 0);
}

void lcd_invert(uint8_t invert){
  uint8_t commandSequence[2] = {0x00, 0xA6};
  if (invert == YES) {
    commandSequence[1] = 0xA7;
  }
  
  lcd_command(commandSequence, sizeof(commandSequence));
}

void lcd_sleep(uint8_t sleep){
  uint8_t commandSequence[2] = {0x00, 0xAF};
  if (sleep == YES) {
    commandSequence[1] = 0xAE;
  } 
  lcd_command(commandSequence, sizeof(commandSequence));
}

void lcd_set_contrast(uint8_t contrast){
  uint8_t commandSequence[3] = {0x00,0x81, contrast};
  lcd_command(commandSequence, sizeof(commandSequence));
}

void lcd_clrscr(void){
  uint8_t displayBuffer[DISPLAY_WIDTH+1];
  memset(displayBuffer, 0x00, sizeof(displayBuffer));
  for (uint8_t i = 0; i < DISPLAY_HEIGHT/8; i++){
    lcd_gotoxy(0,i);
    lcd_data(displayBuffer, sizeof(displayBuffer));
  }
  lcd_home();
}

void lcd_gotoxy(uint8_t x, uint8_t y){
  if( x > (DISPLAY_WIDTH/sizeof(FONT[0])) || y > (DISPLAY_HEIGHT/8-1)) return;// out of display
  x = x * sizeof(FONT[0]);
  cursorPosition.x=x;
  cursorPosition.y=y;
  uint8_t commandSequence[] = {0x00,0xb0+y, 0x21, x, 0x7f};
  lcd_command(commandSequence, sizeof(commandSequence));
}

void lcd_puts(const char* s){
  uint8_t bufferLen = (strlen(s) * FONT_COLUMNS)+ 1; 
  uint8_t buffer[bufferLen];
  int count = 1;

  while(*s){
    char c = *s;
    if( (cursorPosition.x >= DISPLAY_WIDTH-sizeof(FONT[0])) || (c < ' ') ) continue;
    // mapping char
    c -= ' ';

    for (uint8_t i = 0; i <  sizeof(FONT[0]); i++)
      {      
	buffer[count++] = FONT[(uint8_t)c][i];
      }
    s++;
  }

  lcd_data(buffer, bufferLen);//print char at display
}

