#ifndef MI0283QT2_h
#define MI0283QT2_h


#ifdef __cplusplus
extern "C" {
#endif
  #include <inttypes.h>
  #include <avr/pgmspace.h>
  #include "fonts.h"
#ifdef __cplusplus
}
#endif
#include "Print.h"


#define RGB(r,g,b)   (((r&0xF8)<<8)|((g&0xFC)<<3)|((b&0xF8)>>3)) //5 red | 6 green | 5 blue

#define COLOR_WHITE  RGB(255,255,255)
#define COLOR_BLACK  RGB(  0,  0,  0)
#define COLOR_RED    RGB(255,  0,  0)
#define COLOR_GREEN  RGB(  0,255,  0)
#define COLOR_BLUE   RGB(  0,  0,255)
#define COLOR_YELLOW RGB(255,255,  0)

// color definition in french (XH)
#define bleu RGB(0,0,255)
#define blanc RGB(255,255,255)
#define rouge RGB(255,0,0)
#define vert RGB(0,255,0)
#define noir RGB(0,0,0)
#define violet RGB(255,0,255)
#define jaune RGB(255,255,0)

// color definition in english (XH)
#define blue RGB(0,0,255)
#define white RGB(255,255,255)
#define red RGB(255,0,0)
#define green RGB(0,255,0)
#define black RGB(0,0,0)
#define purple RGB(255,0,255)
#define yellow RGB(255,255,0)

// base definition (XH)
#define DEC 10
#define HEX 16
#define OCT 8
#define BIN 2

#ifndef DEC
# define DEC (10)
#endif
#ifndef HEX
# define HEX (16)
#endif
#ifndef OCT
# define OCT (8)
#endif
#ifndef BIN
# define BIN (2)
#endif


class MI0283QT2 : public Print
{
  public:
    uint16_t lcd_orientation;
    uint16_t lcd_width, lcd_height;

    MI0283QT2();
    void init(uint8_t clock_div); //2 4 8 16 32
    void led(uint8_t power); //0-100

    void setOrientation(uint16_t o); //0 90 180 270
    uint16_t getWidth(void);
    uint16_t getHeight(void);
    void setArea(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
    void setCursor(uint16_t x, uint16_t y);

    void clear(uint16_t color);
    void drawStart(void);
    void draw(uint16_t color);
    void drawStop(void);
    void drawPixel(uint16_t x0, uint16_t y0, uint16_t color);
    void drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
    void drawRect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
    void fillRect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
    void drawCircle(uint16_t x0, uint16_t y0, uint16_t radius, uint16_t color);
    void fillCircle(uint16_t x0, uint16_t y0, uint16_t radius, uint16_t color);

// Add default Parameters by X. HINAULT - October 2010 : size = 1, color = black, bg_color=white
    uint16_t drawChar(uint16_t x, uint16_t y, char c, uint8_t size=1, uint16_t color= 0x0000, uint16_t bg_color=0xFFFF);
    uint16_t drawText(uint16_t x, uint16_t y, char *s, uint8_t size, uint16_t color, uint16_t bg_color);
    uint16_t drawText(uint16_t x, uint16_t y, int i, uint8_t size, uint16_t color, uint16_t bg_color);
    uint16_t drawText(uint16_t x, uint16_t y, unsigned int i, uint8_t size, uint16_t color, uint16_t bg_color);
    uint16_t drawText(uint16_t x, uint16_t y, long l, uint8_t size, uint16_t color, uint16_t bg_color);
    uint16_t drawText(uint16_t x, uint16_t y, unsigned long l, uint8_t size, uint16_t color, uint16_t bg_color);
    uint16_t drawText(uint16_t x, uint16_t y, String &s, uint8_t size, uint16_t color, uint16_t bg_color);
    uint16_t drawTextPGM(uint16_t x, uint16_t y, PGM_P s, uint8_t size, uint16_t color, uint16_t bg_color);

    uint16_t drawMLText(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, char *s, uint8_t size, uint16_t color, uint16_t bg_color);
    uint16_t drawMLText(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, String &s, uint8_t size, uint16_t color, uint16_t bg_color);
    uint16_t drawMLTextPGM(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, PGM_P s, uint8_t size, uint16_t color, uint16_t bg_color);

    // default parameters : base = 10, size = 1, color = black, bg_color=white (e.g. = DEC : default value = DEC if omitted )

//Next code added by X. HINAULT - October 2010
    void drawInteger(uint16_t x0, uint16_t y0, char n, uint8_t base= DEC, uint8_t  size= 1,  uint16_t  color= 0x0000, uint16_t  bg_color= 0xFFFF); 
    void drawInteger(uint16_t x0, uint16_t y0, unsigned char n, uint8_t  = DEC, uint8_t  = 1,  uint16_t = 0x0000, uint16_t  = 0xFFFF);
    void drawInteger(uint16_t x0, uint16_t y0, int n, uint8_t base = DEC, uint8_t  = 1,  uint16_t  = 0x0000, uint16_t  = 0xFFFF);
    void drawInteger(uint16_t x0, uint16_t y0, unsigned int n, uint8_t = DEC, uint8_t size = 1,  uint16_t color = 0x0000, uint16_t bg_color = 0xFFFF);
    void drawInteger(uint16_t x0, uint16_t y0, long n, uint8_t base = DEC, uint8_t size = 1,  uint16_t color = 0x0000, uint16_t bg_color = 0xFFFF);
    void drawInteger(uint16_t x0, uint16_t y0, unsigned long n, uint8_t base = DEC, uint8_t size=1,  uint16_t color = 0x0000, uint16_t bg_color = 0xFFFF); 
   

    void drawFloat(uint16_t x0, uint16_t y0, double number, uint8_t digits = 2, uint8_t size=1,  uint16_t color = 0x0000, uint16_t bg_color = 0xFFFF); 

/*
    uint16_t drawInteger(uint16_t x, uint16_t y, char val, uint8_t base=DEC, uint8_t size=1, uint16_t color= 0x0000, uint16_t bg_color= 0xFFFF); //base = DEC, HEX, OCT, BIN
    uint16_t drawInteger(uint16_t x, uint16_t y, unsigned char val, uint8_t base=DEC, uint8_t size=1, uint16_t color= 0x0000, uint16_t bg_color= 0xFFFF);
    uint16_t drawInteger(uint16_t x, uint16_t y, int val, uint8_t base=DEC, uint8_t size=1, uint16_t color= 0x0000, uint16_t bg_color= 0xFFFF);
    uint16_t drawInteger(uint16_t x, uint16_t y, long val, uint8_t base=DEC, uint8_t size=1, uint16_t color= 0x0000, uint16_t bg_color= 0xFFFF);
*/


    void printOptions(uint8_t size, uint16_t color, uint16_t bg_color);
    void printClear(void);
    void printXY(uint16_t x, uint16_t y);
    uint16_t printGetX(void);
    uint16_t printGetY(void);
    void printPGM(PGM_P s);
    virtual size_t write(uint8_t c);
    virtual size_t write(const char *s);
    virtual size_t write(const uint8_t *s, size_t size);

  private:
    uint8_t p_size;
    uint16_t p_fg, p_bg;
    uint16_t p_x, p_y;

    void reset(void);
    void wr_cmd(uint8_t reg, uint8_t param);
    void wr_data(uint16_t data);
    void wr_spi(uint8_t data);
    void delay_10ms(uint8_t ms);

//Next code added by X. HINAULT - October 2010
	void drawNumber(uint16_t x0, uint16_t y0, unsigned long n, uint8_t base, uint8_t size,  uint16_t color, uint16_t bg_color);

};


#endif //MI0283QT2_h
