#include "lcd.h"
#include "font.h"
#include <string.h>

static struct {
  uint8_t x;
  uint8_t y;
} cursorPosition;

uint8_t lcdSendBuffer[256];

void lcd_command(uint8_t cmd[], uint8_t size) {
  cmd[0] = 0x00;// 0x00 for command, 0x40 for data
  i2c_transmit_dma(LCD_I2C_ADR, cmd,size);
  //i2c_transmit_polling(LCD_I2C_ADR, cmd,size);
}

void lcd_data(uint8_t data[], uint16_t size) {
  data[0] = 0x40; // 0x00 for command, 0x40 for data
  i2c_transmit_dma(LCD_I2C_ADR, data,size);
    //i2c_transmit_polling(LCD_I2C_ADR, data,size);
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

  int count = 0;

  while(*s){
    char c = *s;
    if( (cursorPosition.x >= DISPLAY_WIDTH-sizeof(FONT[0])) || (c < ' ') ) continue;
    // mapping char

    c -= ' ';

    for (uint8_t i = 0; i <  sizeof(FONT[0]); i++)
      {      
	lcdSendBuffer[++count] = FONT[(uint8_t)c][i];
      }
    s++;
  }

  lcd_data(lcdSendBuffer, (count) + 1);//print char at display
}

void lcdPutsBlinkFree(const char *g, int ypos){
  int i = 0;
  char text[22];//max used characters used plus null terminator
  int blankchars;

  while(*(g + i)){
    if(i >20) break;
    text[i] = *(g +i);
    i++;
  }

  blankchars = 21 -i;
  while(blankchars){
    text[i] = ' ';
    blankchars--;
    i++;
  }
  text[i] = '\0';
  lcd_gotoxy(0, ypos);
  lcd_puts(text);
  
}

