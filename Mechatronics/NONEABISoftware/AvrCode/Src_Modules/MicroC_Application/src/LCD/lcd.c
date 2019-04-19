/* Diese Datei ist Teil von lcd library for ssd1306/sh1106 oled-display.
 *
 * lcd library for ssd1306/sh1106 oled-display ist Freie Software: Sie können es unter den Bedingungen
 * der GNU General Public License, wie von der Free Software Foundation,
 * Version 3 der Lizenz oder jeder späteren
 * veröffentlichten Version, weiterverbreiten und/oder modifizieren.
 *
 * lcd library for ssd1306/sh1106 oled-display wird in der Hoffnung, dass es nützlich sein wird, aber
 * OHNE JEDE GEWÄHRLEISTUNG, bereitgestellt; sogar ohne die implizite
 * Gewährleistung der MARKTFÄHIGKEIT oder EIGNUNG FÜR EINEN BESTIMMTEN ZWECK.
 * Siehe die GNU General Public License für weitere Details.
 *
 * Sie sollten eine Kopie der GNU General Public License zusammen mit diesem
 * Programm erhalten haben. Wenn nicht, siehe <http://www.gnu.org/licenses/>.
 *
 *  lcd.h
 *
 *  Created by Michael Köhler on 22.12.16.
 *  Copyright 2016 Skie-Systems. All rights reserved.
 *
 *  lib for OLED-Display with ssd1306/sh1106-Controller
 *  first dev-version only for I2C-Connection
 *  at ATMega328P like Arduino Uno
 *
 *  at GRAPHICMODE lib needs static SRAM for display:
 *  DISPLAY-WIDTH * DISPLAY-HEIGHT + 2 bytes
 *  
 *  at TEXTMODE lib need static SRAM for display:
 *  2 bytes (cursorPosition)
 */

#include "lcd.h"
#include "font.h"
#include <string.h>

static struct {
	uint8_t x;
	uint8_t y;
} cursorPosition;
#if defined GRAPHICMODE
#include <stdlib.h>
static uint8_t displayBuffer[DISPLAY_HEIGHT/8][DISPLAY_WIDTH];
#elif defined TEXTMODE
#else
#error "No valid displaymode! Refer lcd.h"
#endif


const uint8_t init_sequence [] PROGMEM = {	// Initialization Sequence
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
    i2c_start((LCD_I2C_ADR << 1) | 0);
    i2c_byte(0x00);	// 0x00 for command, 0x40 for data
    for (uint8_t i=0; i<size; i++) {
        i2c_byte(cmd[i]);
    }
    i2c_stop();
}
void lcd_data(uint8_t data[], uint16_t size) {
    i2c_start((LCD_I2C_ADR << 1) | 0);
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
        commandSequence[i] = (pgm_read_byte(&init_sequence[i]));
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
#ifdef GRAPHICMODE
    for (uint8_t i = 0; i < DISPLAY_HEIGHT/8; i++){
	memset(displayBuffer[i], 0x00, sizeof(displayBuffer[i]));
        lcd_gotoxy(0,i);
        lcd_data(displayBuffer[i], sizeof(displayBuffer[i]));
    }
#elif defined TEXTMODE
    uint8_t displayBuffer[DISPLAY_WIDTH];
    memset(displayBuffer, 0x00, sizeof(displayBuffer));
    for (uint8_t i = 0; i < DISPLAY_HEIGHT/8; i++){
        lcd_gotoxy(0,i);
        lcd_data(displayBuffer, sizeof(displayBuffer));
    }
#endif
    lcd_home();
}
void lcd_home(void){
    lcd_gotoxy(0, 0);
}
void lcd_invert(uint8_t invert){
    i2c_start((LCD_I2C_ADR << 1) | 0);
    uint8_t commandSequence[1];
    if (invert != YES) {
        commandSequence[0] = 0xA6;
    } else {
        commandSequence[0] = 0xA7;
    }
    lcd_command(commandSequence, 1);
}
void lcd_sleep(uint8_t sleep){
    i2c_start((LCD_I2C_ADR << 1) | 0);
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
#ifdef GRAPHICMODE
            for (uint8_t i = 0; i < sizeof(FONT[0]); i++)
            {
                // load bit-pattern from flash
                displayBuffer[cursorPosition.y][cursorPosition.x+i] =pgm_read_byte(&(FONT[(uint8_t)c][i]));
            }
#elif defined TEXTMODE
            i2c_start(LCD_I2C_ADR << 1);
            i2c_byte(0x40);
            for (uint8_t i = 0; i <  sizeof(FONT[0]); i++)
            {
                // print font to ram, print 6 columns
                i2c_byte(pgm_read_byte(&(FONT[(uint8_t)c][i])));
            }
            i2c_stop();
#endif
            cursorPosition.x += sizeof(FONT[0]);
            break;
    }
    
}
void lcd_puts(const char* s){
    while (*s) {
        lcd_putc(*s++);
    }
}
void lcd_puts_p(const char* progmem_s){
    register uint8_t c;
    while ((c = pgm_read_byte(progmem_s++))) {
        lcd_putc(c);
    }
}
#ifdef GRAPHICMODE
#pragma mark -
#pragma mark GRAPHIC FUNCTIONS
void lcd_drawPixel(uint8_t x, uint8_t y, uint8_t color){
    if( x > DISPLAY_WIDTH-1 || y > (DISPLAY_HEIGHT-1)) return; // out of Display
    if( color == WHITE){
        displayBuffer[(y / (DISPLAY_HEIGHT/8))][x] |= (1 << (y % (DISPLAY_HEIGHT/8)));
    } else {
        displayBuffer[(y / (DISPLAY_HEIGHT/8))][x] &= ~(1 << (y % (DISPLAY_HEIGHT/8)));
    }
}
void lcd_drawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color){
    if( x1 > DISPLAY_WIDTH-1 ||
       x2 > DISPLAY_WIDTH-1 ||
       y1 > DISPLAY_HEIGHT-1 ||
       y2 > DISPLAY_HEIGHT-1) return;
    int dx =  abs(x2-x1), sx = x1<x2 ? 1 : -1;
    int dy = -abs(y2-y1), sy = y1<y2 ? 1 : -1;
    int err = dx+dy, e2; /* error value e_xy */
    
    while(1){
        lcd_drawPixel(x1, y1, color);
        if (x1==x2 && y1==y2) break;
        e2 = 2*err;
        if (e2 > dy) { err += dy; x1 += sx; } /* e_xy+e_x > 0 */
        if (e2 < dx) { err += dx; y1 += sy; } /* e_xy+e_y < 0 */
    }
}
void lcd_drawRect(uint8_t px1, uint8_t py1, uint8_t px2, uint8_t py2, uint8_t color){
    if( px1 > DISPLAY_WIDTH-1 ||
       px2 > DISPLAY_WIDTH-1 ||
       py1 > DISPLAY_HEIGHT-1 ||
       py2 > DISPLAY_HEIGHT-1) return;
    lcd_drawLine(px1, py1, px2, py1, color);
    lcd_drawLine(px2, py1, px2, py2, color);
    lcd_drawLine(px2, py2, px1, py2, color);
    lcd_drawLine(px1, py2, px1, py1, color);
}
void lcd_fillRect(uint8_t px1, uint8_t py1, uint8_t px2, uint8_t py2, uint8_t color){
    if( px1 > px2){
        uint8_t temp = px1;
        px1 = px2;
        px2 = temp;
        temp = py1;
        py1 = py2;
        py2 = temp;
    }
    for (uint8_t i=0; i<=(py2-py1); i++){
        lcd_drawLine(px1, py1+i, px2, py1+i, color);
    }
}
void lcd_drawCircle(uint8_t center_x, uint8_t center_y, uint8_t radius, uint8_t color){
    if( ((center_x + radius) > DISPLAY_WIDTH-1) ||
       ((center_y + radius) > DISPLAY_HEIGHT-1) ||
       center_x < radius ||
       center_y < radius) return;
    int16_t f = 1 - radius;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * radius;
    int16_t x = 0;
    int16_t y = radius;
    
    lcd_drawPixel(center_x  , center_y+radius, color);
    lcd_drawPixel(center_x  , center_y-radius, color);
    lcd_drawPixel(center_x+radius, center_y  , color);
    lcd_drawPixel(center_x-radius, center_y  , color);
    
    while (x<y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;
        
        lcd_drawPixel(center_x + x, center_y + y, color);
        lcd_drawPixel(center_x - x, center_y + y, color);
        lcd_drawPixel(center_x + x, center_y - y, color);
        lcd_drawPixel(center_x - x, center_y - y, color);
        lcd_drawPixel(center_x + y, center_y + x, color);
        lcd_drawPixel(center_x - y, center_y + x, color);
        lcd_drawPixel(center_x + y, center_y - x, color);
        lcd_drawPixel(center_x - y, center_y - x, color);
    }
}
void lcd_fillCircle(uint8_t center_x, uint8_t center_y, uint8_t radius, uint8_t color) {
    for(uint8_t i=0; i<= radius;i++){
        lcd_drawCircle(center_x, center_y, i, color);
    }
}
void lcd_drawBitmap(uint8_t x, uint8_t y, const uint8_t *picture, uint8_t width, uint8_t height, uint8_t color){
    uint8_t i,j, byteWidth = (width+7)/8;
    for (j = 0; j < height; j++) {
        for(i=0; i < width;i++){
            if(pgm_read_byte(picture + j * byteWidth + i / 8) & (128 >> (i & 7))){
                lcd_drawPixel(x+i, y+j, color);
            }
        }
    }
}
void lcd_display() {
#if defined SSD1306
    lcd_gotoxy(0,0);
    lcd_data(&displayBuffer[0][0], DISPLAY_WIDTH*DISPLAY_HEIGHT/8);
#elif defined SH1106
    for (uint8_t i = 0; i < DISPLAY_HEIGHT/8; i++){
        lcd_gotoxy(0,i);
        lcd_data(displayBuffer[i], sizeof(displayBuffer[i]));
    }
#endif
#endif
