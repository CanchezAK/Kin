#include "main.h"
#include "string.h"

#define WIDTH (uint16_t)319
#define HEIGHT (uint16_t)479

//Chip Select pin control
#define CS_SET HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET)
#define CS_RESET HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET)
//Write Read pin control
#define WR_SET HAL_GPIO_WritePin(WR_GPIO_Port, WR_Pin, GPIO_PIN_SET)
#define WR_RESET HAL_GPIO_WritePin(WR_GPIO_Port, WR_Pin, GPIO_PIN_RESET)
//RS pin control
#define RS_SET HAL_GPIO_WritePin(RS_GPIO_Port, RS_Pin, GPIO_PIN_SET)
#define RS_RESET HAL_GPIO_WritePin(RS_GPIO_Port, RS_Pin, GPIO_PIN_RESET)
//Reset pin control
#define RST_SET HAL_GPIO_WritePin(RST_GPIO_Port, RST_Pin, GPIO_PIN_SET)
#define RST_RESET HAL_GPIO_WritePin(RST_GPIO_Port, RST_Pin, GPIO_PIN_RESET)
#define LINE_OVERLAP_NONE 0
#define LINE_OVERLAP_MAJOR 0x01
#define LINE_OVERLAP_MINOR 0x02
#define LINE_OVERLAP_BOTH 0x03
#define LINE_THICKNESS_MIDDLE 0
#define LINE_THICKNESS_DRAW_CLOCKWISE 1
#define LINE_THICKNESS_DRAW_COUNTERCLOCKWISE 2

void ILI9486_Init();
void ILI9486_WriteCom(unsigned char command);
void ILI9486_WriteComD(unsigned char comd);
void ILI9486_WriteData(unsigned short data);
void ILI9486_DrawLine(unsigned short x, unsigned short y, unsigned short z, unsigned short j, unsigned char overlap, unsigned char R, unsigned char G, unsigned char B);
void ILI9486_DrawThickLine(unsigned short aXStart, unsigned short aYStart, unsigned short aXEnd, unsigned short aYEnd, unsigned char aThickness, unsigned char aThicknessMode, unsigned char R, unsigned char G, unsigned char B);
void ILI9486_DrawRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned char R, unsigned char G, unsigned char B, unsigned short thickness);
void ILI9486_DrawCircle(unsigned int xc, unsigned int yc, unsigned int r, unsigned char R, unsigned char G, unsigned char B);
void ILI9486_DrawPixel(unsigned int x, unsigned int y, unsigned char R, unsigned char G, unsigned char B);
void ILI9486_SetWindow(unsigned short x, unsigned short y, unsigned short x1, unsigned short y1);
void ILI9486_WriteText(uint16_t ix, uint16_t iy, char* iString, unsigned char* font, uint8_t font_size, unsigned short font_color, unsigned short back_color);
void ILI9486_Fill(unsigned short xk1, unsigned short yk1, unsigned short xk2, unsigned short yk2, unsigned char r1, unsigned char g1, unsigned char b1);

void ILI9486_DrawTestChar(uint16_t x, uint16_t y, char c, unsigned char* Font, uint16_t TextColor, uint16_t BGColor);
void ILI9486_WriteTestText(uint16_t x, uint16_t y, char *str, unsigned char* Font, uint16_t TextColor, uint16_t BGColor);