#include "ili9486.h"

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

unsigned short prev_data = 0;
unsigned short line = 0;

unsigned short map(unsigned short x, unsigned short in_min, unsigned short in_max, unsigned short out_min, unsigned short out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void ILI9486_Fill(unsigned short xk1, unsigned short yk1, unsigned short xk2, unsigned short yk2, unsigned char r1, unsigned char g1, unsigned char b1)
{
  unsigned char r = map(r1, 0, 255, 0, 3);
  unsigned char g = map(g1, 0, 255, 0, 7);
  unsigned char b = map(b1, 0, 255, 0, 3);
  unsigned short v4 = (r<<0x06)|(g<<3)|b;
  ILI9486_SetWindow(xk1, yk1, xk2, yk2);
  ILI9486_WriteComD(0x2c);
  for (uint32_t v6 = 0; v6 < (yk2 - yk1 + 1) * (xk2 - xk1 + 1); v6++)
  {
    ILI9486_WriteData(v4);
  }
}

void ILI9486_SetWindow(unsigned short x, unsigned short y, unsigned short x1, unsigned short y1)
{
  ILI9486_WriteComD(0x2a);
  ILI9486_WriteData(x >> 8);
  ILI9486_WriteData(x);
  ILI9486_WriteData(x1 >> 8);
  ILI9486_WriteData(x1);
  ILI9486_WriteComD(0x2b);
  ILI9486_WriteData(y >> 8);
  ILI9486_WriteData(y);
  ILI9486_WriteData(y1 >> 8);
  ILI9486_WriteData(y1);
}

void WriteReg(unsigned short bite)
{
  MODIFY_REG(GPIOB->ODR, 0xFF, bite&0xFF);
  //MODIFY_REG(GPIOA->ODR, 0xFF, bite>>8);
}

void ILI9486_Init()
{
  WriteReg(0);
  RST_RESET;
  CS_RESET;
  WR_RESET;
  RS_RESET;
  HAL_Delay(50);
  RST_SET;
  ILI9486_WriteCom(0x11);    
  HAL_Delay(50);

  ILI9486_WriteCom(0xC0);   
  ILI9486_WriteData(0x0d);
  ILI9486_WriteData(0x0d);

  ILI9486_WriteCom(0xC1);    
  ILI9486_WriteData(0x43);
  ILI9486_WriteData(0x00);

  ILI9486_WriteCom(0xC2);    
  ILI9486_WriteData(0x00);

  ILI9486_WriteCom(0xC5);   
  ILI9486_WriteData(0x00);
  ILI9486_WriteData(0x48);

  ILI9486_WriteCom(0xB6);    
  ILI9486_WriteData(0x00);
  ILI9486_WriteData(0x22); //past 0x22  
  ILI9486_WriteData(0x3B);

  ILI9486_WriteCom(0xE0);    
  ILI9486_WriteData(0x0f);
  ILI9486_WriteData(0x24);
  ILI9486_WriteData(0x1c);
  ILI9486_WriteData(0x0a);
  ILI9486_WriteData(0x0f);
  ILI9486_WriteData(0x08);
  ILI9486_WriteData(0x43);
  ILI9486_WriteData(0x88);
  ILI9486_WriteData(0x32);
  ILI9486_WriteData(0x0f);
  ILI9486_WriteData(0x10);
  ILI9486_WriteData(0x06);
  ILI9486_WriteData(0x0f);
  ILI9486_WriteData(0x07);
  ILI9486_WriteData(0x00);

  ILI9486_WriteCom(0xE1);    
  ILI9486_WriteData(0x0F);
  ILI9486_WriteData(0x38);
  ILI9486_WriteData(0x30);
  ILI9486_WriteData(0x09);
  ILI9486_WriteData(0x0f);
  ILI9486_WriteData(0x0f);
  ILI9486_WriteData(0x4e);
  ILI9486_WriteData(0x77);
  ILI9486_WriteData(0x3c);
  ILI9486_WriteData(0x07);
  ILI9486_WriteData(0x10);
  ILI9486_WriteData(0x05);
  ILI9486_WriteData(0x23);
  ILI9486_WriteData(0x1b);
  ILI9486_WriteData(0x00); 

  ILI9486_WriteCom(0x20);   
  ILI9486_WriteData(0x00);  

  ILI9486_WriteCom(0x36);    
  ILI9486_WriteData(0x6A);

  ILI9486_WriteCom(0x3A);  
  ILI9486_WriteData(0x55); 

  ILI9486_WriteCom(0x2A);   
  ILI9486_WriteData(0x00);
  ILI9486_WriteData(0x00);
  ILI9486_WriteData(0x01);
  ILI9486_WriteData(0xDF);

  ILI9486_WriteCom(0x002B);
  ILI9486_WriteData(0x00);
  ILI9486_WriteData(0x00);
  ILI9486_WriteData(0x01);
  ILI9486_WriteData(0x3f);  
  HAL_Delay(50);
  ILI9486_WriteCom(0x0029);    
  ILI9486_WriteCom(0x002C);    



  ILI9486_WriteComD(0x2a);
  ILI9486_WriteData(0x00);
  ILI9486_WriteData(0x00);
  ILI9486_WriteData((unsigned short)HEIGHT>>8);
  ILI9486_WriteData((unsigned short)HEIGHT);
  ILI9486_WriteComD(0x2b); 
  ILI9486_WriteData(0x00);
  ILI9486_WriteData(0x00);
  ILI9486_WriteData((unsigned short)WIDTH>>8);
  ILI9486_WriteData((unsigned short)WIDTH);
  
  ILI9486_WriteCom(0x2c);  
}

void ILI9486_WriteCom(unsigned char command)
{
  WR_RESET;
  CS_RESET;
  RS_RESET;
  WriteReg(0);
  
  RST_SET;
  WriteReg(command);
  HAL_Delay(1);
  WR_SET;
}

void ILI9486_WriteComD(unsigned char comd)
{
   WR_RESET;
   CS_RESET;
   RS_RESET;
   WriteReg(0);
   
   RST_SET;
   CS_SET;
   CS_RESET;
   WriteReg(comd);
   WR_SET;
   CS_SET;
   WR_RESET;
}

void ILI9486_WriteData(unsigned short data)
{

   if (data != prev_data)
   {
     prev_data = data;
     WriteReg(0x0000);
     CS_SET;
     WR_RESET;
     RS_SET;
     CS_RESET;
     WriteReg(data);
     WR_SET;
     CS_SET;
     WR_RESET;
   }
   else if (prev_data == data)
   {
     prev_data = data;
     CS_RESET;
     WR_SET;
     CS_SET;
     WR_RESET;
   }
}

void ILI9486_WriteText(uint16_t ix, uint16_t iy, char* iString, unsigned char* font, uint8_t font_size, unsigned short font_color, unsigned short back_color)
{
    uint8_t iLength = strlen(iString);
    //iLength--;
    uint16_t xpos = ix;
    for(int sym = 0; sym<iLength; ++sym)
    {
        uint8_t sm = iString[sym];
        uint8_t snum = (sm<0xC0 ? sm-0x20 : sm-0xC0);
        const uint8_t* symbol = &font[font_size<=16 ? (font_size*2+1)*(snum) : (font_size*3+1)*(snum)];
        for(uint16_t x = 0; x<=symbol[0]; ++x)
        {
            for(uint16_t y = 0; y<font_size; ++y)
            {
                if(symbol[1+y/8+x*2] & (0x01<<(y%8)))
                {
                   ILI9486_SetWindow(xpos+x, iy+y, (xpos+x)+1, (iy+y)+1);
                   ILI9486_WriteComD(0x2c);
                   ILI9486_WriteData(font_color);
                }
                else
                {
                   ILI9486_SetWindow(xpos+x, iy+y, (xpos+x)+1, (iy+y)+1);
                   ILI9486_WriteComD(0x2c);
                   ILI9486_WriteData(back_color);
                }
            }
        }
        xpos += symbol[0];
    }
}

void ILI9486_DrawPixel(unsigned int x, unsigned int y, unsigned char R, unsigned char G, unsigned char B)
{
  unsigned char r = map(R, 0, 255, 0, 3);
  unsigned char g = map(G, 0, 255, 0, 6);
  unsigned char b = map(B, 0, 255, 0, 3);
  unsigned short color = (r<<0x06)|(g<<3)|b;
  ILI9486_SetWindow(x, y, x+1, y+1);
  ILI9486_WriteComD(0x2c);
  ILI9486_WriteData(color);
}

void drawLineOverlap(unsigned short aXStart, unsigned short aYStart, unsigned short aXEnd, unsigned short aYEnd, uint8_t aOverlap, uint8_t R, uint8_t G, uint8_t B)
{
    int16_t tDeltaX, tDeltaY, tDeltaXTimes2, tDeltaYTimes2, tError, tStepX, tStepY;
    if (aXStart >= WIDTH)
    {
        aXStart = WIDTH - 1;
    }
    if (aXEnd >= WIDTH)
    {
        aXEnd = WIDTH - 1;
    }
    if (aYStart >= HEIGHT)
    {
        aYStart = HEIGHT - 1;
    }
    if (aYEnd >= HEIGHT)
    {
        aYEnd = HEIGHT - 1;
    }

    if ((aXStart == aXEnd) || (aYStart == aYEnd))
    {
        ILI9486_Fill(aXStart, aYStart, aXEnd, aYEnd, R, G, B);
    } 
    else
    {
        tDeltaX = aXEnd - aXStart;
        tDeltaY = aYEnd - aYStart;
        if (tDeltaX < 0)
        {
            tDeltaX = -tDeltaX;
            tStepX = -1;
        } 
        else
        {
            tStepX = +1;
        }
        if (tDeltaY < 0)
        {
            tDeltaY = -tDeltaY;
            tStepY = -1;
        } 
        else
        {
            tStepY = +1;
        }
        tDeltaXTimes2 = tDeltaX << 1;
        tDeltaYTimes2 = tDeltaY << 1;
        ILI9486_DrawPixel(aXStart, aYStart, R, G, B);
        if (tDeltaX > tDeltaY)
        {
            tError = tDeltaYTimes2 - tDeltaX;
            while (aXStart != aXEnd)
            {
                aXStart += tStepX;
                if (tError >= 0)
                {
                    if (aOverlap & LINE_OVERLAP_MAJOR)
                    {
                        ILI9486_DrawPixel(aXStart, aYStart, R, G, B);
                    }
                    aYStart += tStepY;
                    if (aOverlap & LINE_OVERLAP_MINOR)
                    {
                        ILI9486_DrawPixel(aXStart - tStepX, aYStart, R, G, B);
                    }
                    tError -= tDeltaXTimes2;
                }
                tError += tDeltaYTimes2;
                ILI9486_DrawPixel(aXStart, aYStart, R, G, B);
            }
        } 
        else
        {
            tError = tDeltaXTimes2 - tDeltaY;
            while (aYStart != aYEnd)
            {
                aYStart += tStepY;
                if (tError >= 0)
                {
                    if (aOverlap & LINE_OVERLAP_MAJOR)
                    {
                        ILI9486_DrawPixel(aXStart, aYStart, R, G, B);
                    }
                    aXStart += tStepX;
                    if (aOverlap & LINE_OVERLAP_MINOR)
                    {
                        ILI9486_DrawPixel(aXStart, aYStart - tStepY, R, G, B);
                    }
                    tError -= tDeltaYTimes2;
                }
                tError += tDeltaXTimes2;
                ILI9486_DrawPixel(aXStart, aYStart, R, G, B);
            }
        }
    }
}

void ILI9486_DrawLine(unsigned short x, unsigned short y, unsigned short z, unsigned short j, unsigned char overlap, unsigned char R, unsigned char G, unsigned char B)
{
  drawLineOverlap(x, y, z, j, LINE_OVERLAP_MINOR, R, G, B);
}

//aThicknessMode can be one of LINE_THICKNESS_MIDDLE, LINE_THICKNESS_DRAW_CLOCKWISE, LINE_THICKNESS_DRAW_COUNTERCLOCKWISE
void ILI9486_DrawThickLine(unsigned short aXStart, unsigned short aYStart, unsigned short aXEnd, unsigned short aYEnd, unsigned char aThickness, unsigned char aThicknessMode, unsigned char R, unsigned char G, unsigned char B)
{
    int16_t i, tDeltaX, tDeltaY, tDeltaXTimes2, tDeltaYTimes2, tError, tStepX, tStepY;

    if (aThickness <= 1)
    {
        ILI9486_DrawLine(aXStart, aYStart, aXEnd, aYEnd, LINE_OVERLAP_NONE, R, G, B);
    }

    if (aXStart >= WIDTH)
    {
        aXStart = WIDTH - 1;
    }
    if (aXEnd >= WIDTH)
    {
        aXEnd = WIDTH - 1;
    }
    if (aYStart >= HEIGHT)
    {
        aYStart = HEIGHT - 1;
    }
    if (aYEnd >= HEIGHT)
    {
        aYEnd = HEIGHT - 1;
    }
    
    tDeltaY = aXEnd - aXStart;
    tDeltaX = aYEnd - aYStart;
    uint8_t tSwap = 1;
    if (tDeltaX < 0)
    {
        tDeltaX = -tDeltaX;
        tStepX = -1;
        tSwap = !tSwap;
    } else {
        tStepX = +1;
    }
    if (tDeltaY < 0)
    {
        tDeltaY = -tDeltaY;
        tStepY = -1;
        tSwap = !tSwap;
    } 
    else
    {
        tStepY = +1;
    }
    tDeltaXTimes2 = tDeltaX << 1;
    tDeltaYTimes2 = tDeltaY << 1;
    uint8_t tOverlap;
    int tDrawStartAdjustCount = aThickness / 2;
    if (aThicknessMode == LINE_THICKNESS_DRAW_COUNTERCLOCKWISE)
    {
        tDrawStartAdjustCount = aThickness - 1;
    } 
    else if (aThicknessMode == LINE_THICKNESS_DRAW_CLOCKWISE)
    {
        tDrawStartAdjustCount = 0;
    }
    if (tDeltaX >= tDeltaY)
    {
        if (tSwap)
        {
            tDrawStartAdjustCount = (aThickness - 1) - tDrawStartAdjustCount;
            tStepY = -tStepY;
        } 
        else
        {
            tStepX = -tStepX;
        }
        tError = tDeltaYTimes2 - tDeltaX;
        for (i = tDrawStartAdjustCount; i > 0; i--)
        {
            aXStart -= tStepX;
            aXEnd -= tStepX;
            if (tError >= 0)
            {
                aYStart -= tStepY;
                aYEnd -= tStepY;
                tError -= tDeltaXTimes2;
            }
            tError += tDeltaYTimes2;
        }

        ILI9486_DrawLine(aXStart, aYStart, aXEnd, aYEnd, 0, R, G, B);
        tError = tDeltaYTimes2 - tDeltaX;
        for (i = aThickness; i > 2; i--)
        {
            aXStart += tStepX;
            aXEnd += tStepX;
            tOverlap = LINE_OVERLAP_NONE;
            if (tError >= 0)
            {
                aYStart += tStepY;
                aYEnd += tStepY;
                tError -= tDeltaXTimes2;
                tOverlap = LINE_OVERLAP_MAJOR;
            }
            tError += tDeltaYTimes2;
            ILI9486_DrawLine(aXStart, aYStart, aXEnd, aYEnd, tOverlap, R, G, B);
        }
    }
    else
    {
        if (tSwap)
        {
            tStepX = -tStepX;
        } 
        else
        {
            tDrawStartAdjustCount = (aThickness - 1) - tDrawStartAdjustCount;
            tStepY = -tStepY;
        }
        tError = tDeltaXTimes2 - tDeltaY;
        for (i = tDrawStartAdjustCount; i > 0; i--)
        {
            aYStart -= tStepY;
            aYEnd -= tStepY;
            if (tError >= 0)
            {
                aXStart -= tStepX;
                aXEnd -= tStepX;
                tError -= tDeltaYTimes2;
            }
            tError += tDeltaXTimes2;
        }
        ILI9486_DrawLine(aXStart, aYStart, aXEnd, aYEnd, 0, R, G, B);
        tError = tDeltaXTimes2 - tDeltaY;
        for (i = aThickness; i > 2; i--)
        {
            aYStart += tStepY;
            aYEnd += tStepY;
            tOverlap = LINE_OVERLAP_NONE;
            if (tError >= 0)
            {
                aXStart += tStepX;
                aXEnd += tStepX;
                tError -= tDeltaYTimes2;
                tOverlap = LINE_OVERLAP_MAJOR;
            }
            tError += tDeltaXTimes2;
            ILI9486_DrawLine(aXStart, aYStart, aXEnd, aYEnd, tOverlap, R, G, B);
        }
    }
}

void ILI9486_DrawCircle(unsigned int xc, unsigned int yc, unsigned int r, unsigned char R, unsigned char G, unsigned char B)
{
  int x = -r, y = 0, err = 2-2*r, e2;
  do {
	ILI9486_DrawPixel(xc-x, yc+y, R, G, B);
	ILI9486_DrawPixel(xc+x, yc+y, R, G, B);
	ILI9486_DrawPixel(xc+x, yc-y, R, G, B);
	ILI9486_DrawPixel(xc-x, yc-y, R, G, B);
	e2 = err;
	if (e2 <= y) {
		err += ++y*2+1;
		if (-x == y && e2 <= x) e2 = 0;
	}
	if (e2 > x) err += ++x*2+1;
  } while (x <= 0);
}

void ILI9486_DrawRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned char R, unsigned char G, unsigned char B, unsigned short thickness)
{
  ILI9486_DrawThickLine(x, y, w, y, thickness, LINE_THICKNESS_MIDDLE, R, G, B);
  ILI9486_DrawThickLine(x, y, x, h, thickness, LINE_THICKNESS_MIDDLE, R, G, B);
  ILI9486_DrawThickLine(x, (y+x), w, (y+x), thickness, LINE_THICKNESS_MIDDLE, R, G, B);
  ILI9486_DrawThickLine((x+y), y, (x+y), h, thickness, LINE_THICKNESS_MIDDLE, R, G, B);
}