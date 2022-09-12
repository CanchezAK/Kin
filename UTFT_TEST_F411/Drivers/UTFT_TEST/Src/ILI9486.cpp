/*
  ILI9486.cpp - Multi-Platform library support for Color TFT LCD Boards
  Copyright (C)2015 Rinky-Dink Electronics, Henning Karlsen. All right reserved
  
  This library is the continuation of my ITDB02_Graph, ITDB02_Graph16
  and RGB_GLCD libraries for Arduino and chipKit. As the number of 
  supported display modules and controllers started to increase I felt 
  it was time to make a single, universal library as it will be much 
  easier to maintain in the future.

  Basic functionality of this library was origianlly based on the 
  demo-code provided by ITead studio (for the ITDB02 modules) and 
  NKC Electronics (for the RGB GLCD module/shield).

  This library supports a number of 8bit, 16bit and serial graphic 
  displays, and will work with both Arduino, chipKit boards and select 
  TI LaunchPads. For a full list of tested display modules and controllers,
  see the document ILI9486_Supported_display_modules_&_controllers.pdf.

  When using 8bit and 16bit display modules there are some 
  requirements you must adhere to. These requirements can be found 
  in the document ILI9486_Requirements.pdf.
  There are no special requirements when using serial displays.

  You can find the latest version of the library at 
  http://www.RinkyDinkElectronics.com/

  This library is free software; you can redistribute it and/or
  modify it under the terms of the CC BY-NC-SA 3.0 license.
  Please see the included documents for further information.

  Commercial use of this library requires you to buy a license that
  will allow commercial use. This includes using the library,
  modified or not, as a tool to sell products.

  The license applies to all part of the library including the 
  examples and tools supplied with the library.
*/

#include "itoa.h"
#include "dtostrf.h"
#include <pgmspace.h>
#include <ILI9486.h>

// Include hardware-specific functions

#include "main.h"
#include "stm32f4xx_hal.h"

#include "WString.h"
#include "wiring_constants.h"
#include "memorysaver.h"

ILI9486::ILI9486()
{
}

ILI9486::ILI9486(int RS, int WR, int CS, int RST, int RD)
{ 

	disp_x_size =			479;
	disp_y_size =			319;

}

void ILI9486::LCD_Write_COM(char VL)  
{   
		HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_RESET); //clear(P_RS, B_RS);
		LCD_Writ_Bus(0x00,VL);

}

void ILI9486::LCD_Write_DATA(char VH,char VL)
{
		HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET); //set(P_RS, B_RS);
		LCD_Writ_Bus(VH,VL);

}

void ILI9486::LCD_Write_DATA(char VL)
{
		HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET); //set(P_RS, B_RS);
		LCD_Writ_Bus(0x00,VL);
}

void ILI9486::LCD_Write_COM_DATA(char com1,int dat1)
{
     LCD_Write_COM(com1);
     LCD_Write_DATA(dat1>>8,dat1);
}

void ILI9486::InitLCD(byte orientation)
{
	orient=orientation;

	HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET); //set(P_RST, B_RST);
	HAL_Delay(5);
	HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET); //clear(P_RST, B_RST);
	HAL_Delay(15);
	HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET); //set(P_RST, B_RST);
	HAL_Delay(15);

	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET); // clear(P_CS, B_CS);
	//HAL_GPIO_WritePin(LCD_RD_GPIO_Port, LCD_RD_Pin, GPIO_PIN_SET); // set(P_RD, B_RD);

//***** display settings ****************************************************************

    /*LCD_Write_COM(0x11);	//Software Reset
    HAL_Delay(200);

    LCD_Write_COM(0x28);    // Display off

    LCD_Write_COM(0xC0);    // Pixel Format
    LCD_Write_DATA(0x00,0x0d);   // 565, 16-bit
    LCD_Write_DATA(0x00,0x0d);

    LCD_Write_COM(0xC1);    // VCOMS
    LCD_Write_DATA(0x00,0x43);   //
    LCD_Write_DATA(0x00,0x00);

    LCD_Write_COM(0xC2);    // VRHS
    LCD_Write_DATA(0x00,0x00);   //

    LCD_Write_COM(0xC5);    // PWCTRL
    LCD_Write_DATA(0x00,0x48);   //

    LCD_Write_COM(0xB6);    //Exit Sleep
    LCD_Write_DATA(0x00,0x00);
    LCD_Write_DATA(0x00,0x4A);
    LCD_Write_DATA(0x00,0x3b);
    HAL_Delay(120);
    LCD_Write_COM(0xE0);
    LCD_Write_DATA(0x00,0x0F);
    LCD_Write_DATA(0x00,0x24);
    LCD_Write_DATA(0x00,0x1c);
    LCD_Write_DATA(0x00,0x0a);
    LCD_Write_DATA(0x00,0x0f);
    LCD_Write_DATA(0x00,0x08);
    LCD_Write_DATA(0x00,0x43);
    LCD_Write_DATA(0x00,0x88);
    LCD_Write_DATA(0x00,0x32);
    LCD_Write_DATA(0x00,0x0F);
    LCD_Write_DATA(0x00,0x10);
    LCD_Write_DATA(0x00,0x06);
    LCD_Write_DATA(0x00,0x0F);
    LCD_Write_DATA(0x00,0x07);
    LCD_Write_DATA(0x00,0x00);
    
    LCD_Write_COM(0xE1);
    LCD_Write_DATA(0x00,0x0F);
    LCD_Write_DATA(0x00,0x38);
    LCD_Write_DATA(0x00,0x30);
    LCD_Write_DATA(0x00,0x09);
    LCD_Write_DATA(0x00,0x0f);
    LCD_Write_DATA(0x00,0x0f);
    LCD_Write_DATA(0x00,0x4e);
    LCD_Write_DATA(0x00,0x77);
    LCD_Write_DATA(0x00,0x3c);
    LCD_Write_DATA(0x00,0x07);
    LCD_Write_DATA(0x00,0x10);
    LCD_Write_DATA(0x00,0x05);
    LCD_Write_DATA(0x00,0x23);
    LCD_Write_DATA(0x00,0x1b);
    LCD_Write_DATA(0x00,0x00);
    
    LCD_Write_COM(0x20);
    LCD_Write_DATA(0x00,0x00);
    
    LCD_Write_COM(0x36);
    LCD_Write_DATA(0x00,0x6a);
    
    LCD_Write_COM(0x3a);
    LCD_Write_DATA(0x00,0x55);
    
    LCD_Write_COM(0x2A);
    LCD_Write_DATA(0x00,0x00);
    LCD_Write_DATA(0x00,0x00);
    LCD_Write_DATA(0x00,0x01);
    LCD_Write_DATA(0x00,0xDF);
    
    LCD_Write_COM(0x2B);
    LCD_Write_DATA(0x00,0x00);
    LCD_Write_DATA(0x00,0x00);
    LCD_Write_DATA(0x00,0x01);
    LCD_Write_DATA(0x00,0x3F);
    
    LCD_Write_COM(0x29);
    LCD_Write_COM(0x2C);

    LCD_Write_COM(0x2A);    //Display on
    LCD_Write_DATA(0x00,0x00);
    LCD_Write_DATA(0x00,0x00);
    LCD_Write_DATA(0x00,319 << 8);
    LCD_Write_DATA(0x00,319);
    LCD_Write_COM(0x2B);    //Display on
    LCD_Write_DATA(0x00,0x00);
    LCD_Write_DATA(0x00,0x00);
    LCD_Write_DATA(0x00,479 << 8);
    LCD_Write_DATA(0x00,479);
    
    LCD_Write_COM(0x2c); 	//Memory Write

// *************************************************************************************************

	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET); // set (P_CS, B_CS);

	setColor(255, 255, 255);
	setBackColor(0, 0, 0);
	cfont.font=0;
	_transparent = false;*/
        LCD_Write_COM(0x11);	//Software Reset
    HAL_Delay(200);

    LCD_Write_COM(0x28);    // Display off

    LCD_Write_COM(0xC0);    // Pixel Format
    LCD_Write_DATA(0x0d,0x0d);   // 565, 16-bit

    LCD_Write_COM(0xC1);    // VCOMS
    LCD_Write_DATA(0x43,0x00);   //
    LCD_Write_DATA(0x00,0x00);   //

    LCD_Write_COM(0xC2);    // VRHS
    LCD_Write_DATA(0x00,0x00);   //

    LCD_Write_COM(0xC5);    // PWCTRL
    LCD_Write_DATA(0x48,0x00);   //

    LCD_Write_COM(0xB6);    //Exit Sleep
    LCD_Write_DATA(0x00,0x4A);
    LCD_Write_DATA(0x3b,0x00);
    HAL_Delay(120);
    LCD_Write_COM(0xE0);
    LCD_Write_DATA(0x0F, 0x24);
    LCD_Write_DATA(0x1c, 0x0a);
    LCD_Write_DATA(0x0f, 0x08);
    LCD_Write_DATA(0x43, 0x88);
    LCD_Write_DATA(0x32, 0x0F);
    LCD_Write_DATA(0x10, 0x06);
    LCD_Write_DATA(0x0F, 0x07);
    LCD_Write_DATA(0x00,0x00);
    
    LCD_Write_COM(0xE1);
    LCD_Write_DATA(0x0F, 0x38);
    LCD_Write_DATA(0x30, 0x09);
    LCD_Write_DATA(0x0f, 0x0f);
    LCD_Write_DATA(0x4e, 0x77);
    LCD_Write_DATA(0x3c, 0x07);
    LCD_Write_DATA(0x10, 0x05);
    LCD_Write_DATA(0x23, 0x1b);
    LCD_Write_DATA(0x00,0x00);
    
    LCD_Write_COM(0x20);
    LCD_Write_DATA(0x00,0x00);
    
    LCD_Write_COM(0x36);
    LCD_Write_DATA(0x6a, 0x00);
    
    LCD_Write_COM(0x3a);
    LCD_Write_DATA(0x55, 0x00);
    
    LCD_Write_COM(0x2A);
    LCD_Write_DATA(0x00,0x00);
    LCD_Write_DATA(0x01,0xDF);
    
    LCD_Write_COM(0x2B);
    LCD_Write_DATA(0x00,0x00);
    LCD_Write_DATA(0x01,0x3F);
    
    LCD_Write_COM(0x29);
    LCD_Write_COM(0x2C);

    LCD_Write_COM(0x2A);    //Display on
    LCD_Write_DATA(0x00,0x00);
    LCD_Write_DATA(319 << 8, 319);
    
    LCD_Write_COM(0x2B);    //Display on
    LCD_Write_DATA(0x00,0x00);
    LCD_Write_DATA(479 << 8, 479);
    
    LCD_Write_COM(0x2c); 	//Memory Write

// *************************************************************************************************

	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET); // set (P_CS, B_CS);

	setColor(255, 255, 255);
	setBackColor(0, 0, 0);
	cfont.font=0;
	_transparent = false;
}

void ILI9486::setXY(word x1, word y1, word x2, word y2)
{
	if (orient==LANDSCAPE)
	{
		swap(word, x1, y1);
		swap(word, x2, y2)
		y1=disp_y_size-y1;
		y2=disp_y_size-y2;
		swap(word, y1, y2)
	}

//***** display settings *******************************************************

	LCD_Write_COM(0x2a);
	LCD_Write_DATA(x1>>8,x1);
	LCD_Write_DATA(x2>>8,x2);
	LCD_Write_COM(0x2b);
	LCD_Write_DATA(y1>>8,y1);
	LCD_Write_DATA(y2>>8,y2);
	LCD_Write_COM(0x2c);

//*************************************************************************************
}

void ILI9486::clrXY()
{
	if (orient==PORTRAIT)
		setXY(0,0,disp_x_size,disp_y_size);
	else
		setXY(0,0,disp_y_size,disp_x_size);
}

void ILI9486::drawRect(int x1, int y1, int x2, int y2)
{
	if (x1>x2)
	{
		swap(int, x1, x2);
	}
	if (y1>y2)
	{
		swap(int, y1, y2);
	}

	drawHLine(x1, y1, x2-x1);
	drawHLine(x1, y2, x2-x1);
	drawVLine(x1, y1, y2-y1);
	drawVLine(x2, y1, y2-y1);
}

void ILI9486::drawRoundRect(int x1, int y1, int x2, int y2)
{
	if (x1>x2)
	{
		swap(int, x1, x2);
	}
	if (y1>y2)
	{
		swap(int, y1, y2);
	}
	if ((x2-x1)>4 && (y2-y1)>4)
	{
		drawPixel(x1+1,y1+1);
		drawPixel(x2-1,y1+1);
		drawPixel(x1+1,y2-1);
		drawPixel(x2-1,y2-1);
		drawHLine(x1+2, y1, x2-x1-4);
		drawHLine(x1+2, y2, x2-x1-4);
		drawVLine(x1, y1+2, y2-y1-4);
		drawVLine(x2, y1+2, y2-y1-4);
	}
}

void ILI9486::fillRect(int x1, int y1, int x2, int y2)
{
	if (x1>x2)
	{
		swap(int, x1, x2);
	}
	if (y1>y2)
	{
		swap(int, y1, y2);
	}
	if(fch==fcl)
		{
		HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET); // clear(P_CS, B_CS);
		setXY(x1, y1, x2, y2);
		HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET); // 	set(P_RS, B_RS);
		_fast_fill_8(fch,((long(x2-x1)+1)*(long(y2-y1)+1)));
		HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET); // 	set(P_CS, B_CS)
		}
	else
		{
			if (orient==PORTRAIT)
			{
				for (int i=0; i<((y2-y1)/2)+1; i++)
				{
					drawHLine(x1, y1+i, x2-x1);
					drawHLine(x1, y2-i, x2-x1);
				}
			}
			else
			{
				for (int i=0; i<((x2-x1)/2)+1; i++)
				{
					drawVLine(x1+i, y1, y2-y1);
					drawVLine(x2-i, y1, y2-y1);
				}
			}
		};

}

void ILI9486::fillRoundRect(int x1, int y1, int x2, int y2)
{
	if (x1>x2)
	{
		swap(int, x1, x2);
	}
	if (y1>y2)
	{
		swap(int, y1, y2);
	}

	if ((x2-x1)>4 && (y2-y1)>4)
	{
		for (int i=0; i<((y2-y1)/2)+1; i++)
		{
			switch(i)
			{
			case 0:
				drawHLine(x1+2, y1+i, x2-x1-4);
				drawHLine(x1+2, y2-i, x2-x1-4);
				break;
			case 1:
				drawHLine(x1+1, y1+i, x2-x1-2);
				drawHLine(x1+1, y2-i, x2-x1-2);
				break;
			default:
				drawHLine(x1, y1+i, x2-x1);
				drawHLine(x1, y2-i, x2-x1);
			}
		}
	}
}

void ILI9486::drawCircle(int x, int y, int radius)
{
	int f = 1 - radius;
	int ddF_x = 1;
	int ddF_y = -2 * radius;
	int x1 = 0;
	int y1 = radius;

	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET); // clear(P_CS, B_CS);
	setXY(x, y + radius, x, y + radius);
	LCD_Write_DATA(fch,fcl);
	setXY(x, y - radius, x, y - radius);
	LCD_Write_DATA(fch,fcl);
	setXY(x + radius, y, x + radius, y);
	LCD_Write_DATA(fch,fcl);
	setXY(x - radius, y, x - radius, y);
	LCD_Write_DATA(fch,fcl);
 
	while(x1 < y1)
	{
		if(f >= 0) 
		{
			y1--;
			ddF_y += 2;
			f += ddF_y;
		}
		x1++;
		ddF_x += 2;
		f += ddF_x;    
		setXY(x + x1, y + y1, x + x1, y + y1);
		LCD_Write_DATA(fch,fcl);
		setXY(x - x1, y + y1, x - x1, y + y1);
		LCD_Write_DATA(fch,fcl);
		setXY(x + x1, y - y1, x + x1, y - y1);
		LCD_Write_DATA(fch,fcl);
		setXY(x - x1, y - y1, x - x1, y - y1);
		LCD_Write_DATA(fch,fcl);
		setXY(x + y1, y + x1, x + y1, y + x1);
		LCD_Write_DATA(fch,fcl);
		setXY(x - y1, y + x1, x - y1, y + x1);
		LCD_Write_DATA(fch,fcl);
		setXY(x + y1, y - x1, x + y1, y - x1);
		LCD_Write_DATA(fch,fcl);
		setXY(x - y1, y - x1, x - y1, y - x1);
		LCD_Write_DATA(fch,fcl);
	}
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET); // set(P_CS, B_CS);
	clrXY();
}

void ILI9486::fillCircle(int x, int y, int radius)
{
	for(int y1=-radius; y1<=0; y1++) 
		for(int x1=-radius; x1<=0; x1++)
			if(x1*x1+y1*y1 <= radius*radius) 
			{
				drawHLine(x+x1, y+y1, 2*(-x1));
				drawHLine(x+x1, y-y1, 2*(-x1));
				break;
			}
}

void ILI9486::clrScr()
{
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET); // clear(P_CS, B_CS);
	clrXY();

	HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET); // set(P_RS, B_RS);

	_fast_fill_8(0x0,((disp_x_size+1)*(disp_y_size+1)));

	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET); // set(P_CS, B_CS);
}

void ILI9486::fillScr(byte r, byte g, byte b)
{
	word color = ((r&248)<<8 | (g&252)<<3 | (b&248)>>3);
	fillScr(color);
}

void ILI9486::fillScr(word color)
{
	long i;
	char ch, cl;
	
	ch=byte(color>>8);
	cl=byte(color & 0xFF);

	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET); // clear(P_CS, B_CS);
	clrXY();

	HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET); // set(P_RS, B_RS);

	if(ch==cl)
		{
		_fast_fill_8(ch,((disp_x_size+1)*(disp_y_size+1)));
		}
	else
		{
			for (i=0; i<((disp_x_size+1)*(disp_y_size+1)); i++)
				LCD_Writ_Bus(ch,cl);
		}
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET); // set(P_CS, B_CS);
}

void ILI9486::setColor(byte r, byte g, byte b)
{
	fch=((r&248)|g>>5);
	fcl=((g&28)<<3|b>>3);
}

void ILI9486::setColor(word color)
{
	fch=byte(color>>8);
	fcl=byte(color & 0xFF);
}

word ILI9486::getColor()
{
	return (fch<<8) | fcl;
}

void ILI9486::setBackColor(byte r, byte g, byte b)
{
	bch=((r&248)|g>>5);
	bcl=((g&28)<<3|b>>3);
	_transparent=false;
}

void ILI9486::setBackColor(uint32_t color)
{
	if (color==VGA_TRANSPARENT)
		_transparent=true;
	else
	{
		bch=byte(color>>8);
		bcl=byte(color & 0xFF);
		_transparent=false;
	}
}

word ILI9486::getBackColor()
{
	return (bch<<8) | bcl;
}

void ILI9486::setPixel(word color)
{
	LCD_Write_DATA((color>>8),(color&0xFF));	// rrrrrggggggbbbbb
}

void ILI9486::drawPixel(int x, int y)
{
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET); // clear(P_CS, B_CS);
	setXY(x, y, x, y);
	setPixel((fch<<8)|fcl);
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET); // set(P_CS, B_CS);
	clrXY();
}

void ILI9486::drawLine(int x1, int y1, int x2, int y2)
{
	if (y1==y2)
		drawHLine(x1, y1, x2-x1);
	else if (x1==x2)
		drawVLine(x1, y1, y2-y1);
	else
	{
		unsigned int	dx = (x2 > x1 ? x2 - x1 : x1 - x2);
		short			xstep =  x2 > x1 ? 1 : -1;
		unsigned int	dy = (y2 > y1 ? y2 - y1 : y1 - y2);
		short			ystep =  y2 > y1 ? 1 : -1;
		int				col = x1, row = y1;

		HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET); // clear(P_CS, B_CS);
		if (dx < dy)
		{
			int t = - (dy >> 1);
			while (true)
			{
				setXY (col, row, col, row);
				LCD_Write_DATA (fch, fcl);
				if (row == y2)
					return;
				row += ystep;
				t += dx;
				if (t >= 0)
				{
					col += xstep;
					t   -= dy;
				}
			} 
		}
		else
		{
			int t = - (dx >> 1);
			while (true)
			{
				setXY (col, row, col, row);
				LCD_Write_DATA (fch, fcl);
				if (col == x2)
					return;
				col += xstep;
				t += dy;
				if (t >= 0)
				{
					row += ystep;
					t   -= dx;
				}
			} 
		}
		HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET); // set(P_CS, B_CS);
	}
	clrXY();
}

void ILI9486::drawHLine(int x, int y, int l)
{
	if (l<0)
	{
		l = -l;
		x -= l;
	}
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET); // clear(P_CS, B_CS);
	setXY(x, y, x+l, y);
	if (fch==fcl)
		{
		HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET); // set(P_RS, B_RS);
		_fast_fill_8(fch,l);
		}
	else
		{
		for (int i=0; i<l+1; i++)
				{
					LCD_Write_DATA(fch, fcl);
				}
		}
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET); // set(P_CS, B_CS);
	clrXY();
}

void ILI9486::drawVLine(int x, int y, int l)
{
	if (l<0)
	{
		l = -l;
		y -= l;
	}
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET); // clear(P_CS, B_CS);
	setXY(x, y, x, y+l);
	if (fch==fcl)
		{
		HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET); // set(P_RS, B_RS);
		_fast_fill_8(fch,l);
		}
	else
		{
		for (int i=0; i<l+1; i++)
				{
					LCD_Write_DATA(fch, fcl);
				}
		}

	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET); // set(P_CS, B_CS);
	clrXY();
}

void ILI9486::printChar(byte c, int x, int y)
{
	byte i,ch;
	word j;
	word temp; 

	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET); // clear(P_CS, B_CS);
  
	if (!_transparent)
	{
		if (orient==PORTRAIT)
		{
			setXY(x,y,x+cfont.x_size-1,y+cfont.y_size-1);
	  
			temp=((c-cfont.offset)*((cfont.x_size/8)*cfont.y_size))+4;
			for(j=0;j<((cfont.x_size/8)*cfont.y_size);j++)
			{
				ch=pgm_read_byte(&cfont.font[temp]);
				for(i=0;i<8;i++)
				{   
					if((ch&(1<<(7-i)))!=0)   
					{
						setPixel((fch<<8)|fcl);
					} 
					else
					{
						setPixel((bch<<8)|bcl);
					}   
				}
				temp++;
			}
		}
		else
		{
			temp=((c-cfont.offset)*((cfont.x_size/8)*cfont.y_size))+4;

			for(j=0;j<((cfont.x_size/8)*cfont.y_size);j+=(cfont.x_size/8))
			{
				setXY(x,y+(j/(cfont.x_size/8)),x+cfont.x_size-1,y+(j/(cfont.x_size/8)));
				for (int zz=(cfont.x_size/8)-1; zz>=0; zz--)
				{
					ch=pgm_read_byte(&cfont.font[temp+zz]);
					for(i=0;i<8;i++)
					{   
						if((ch&(1<<i))!=0)   
						{
							setPixel((fch<<8)|fcl);
						} 
						else
						{
							setPixel((bch<<8)|bcl);
						}   
					}
				}
				temp+=(cfont.x_size/8);
			}
		}
	}
	else
	{
		temp=((c-cfont.offset)*((cfont.x_size/8)*cfont.y_size))+4;
		for(j=0;j<cfont.y_size;j++) 
		{
			for (int zz=0; zz<(cfont.x_size/8); zz++)
			{
				ch=pgm_read_byte(&cfont.font[temp+zz]); 
				for(i=0;i<8;i++)
				{   
				
					if((ch&(1<<(7-i)))!=0)   
					{
						setXY(x+i+(zz*8),y+j,x+i+(zz*8)+1,y+j+1);
						setPixel((fch<<8)|fcl);
					} 
				}
			}
			temp+=(cfont.x_size/8);
		}
	}

	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET); // set(P_CS, B_CS);
	clrXY();
}

void ILI9486::rotateChar(byte c, int x, int y, int pos, int deg)
{
	byte i,j,ch;
	word temp; 
	int newx,newy;
	double radian;
	radian=deg*0.0175;  

	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET); // clear(P_CS, B_CS);

	temp=((c-cfont.offset)*((cfont.x_size/8)*cfont.y_size))+4;
	for(j=0;j<cfont.y_size;j++) 
	{
		for (int zz=0; zz<(cfont.x_size/8); zz++)
		{
			ch=pgm_read_byte(&cfont.font[temp+zz]); 
			for(i=0;i<8;i++)
			{   
				newx=x+(((i+(zz*8)+(pos*cfont.x_size))*cos(radian))-((j)*sin(radian)));
				newy=y+(((j)*cos(radian))+((i+(zz*8)+(pos*cfont.x_size))*sin(radian)));

				setXY(newx,newy,newx+1,newy+1);
				
				if((ch&(1<<(7-i)))!=0)   
				{
					setPixel((fch<<8)|fcl);
				} 
				else  
				{
					if (!_transparent)
						setPixel((bch<<8)|bcl);
				}   
			}
		}
		temp+=(cfont.x_size/8);
	}
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET); // set(P_CS, B_CS);
	clrXY();
}

void ILI9486::print(char *st, int x, int y, int deg)
{
	int stl, i;

	stl = strlen(st);

	if (orient==PORTRAIT)
	{
	if (x==RIGHT)
		x=(disp_x_size+1)-(stl*cfont.x_size);
	if (x==CENTER)
		x=((disp_x_size+1)-(stl*cfont.x_size))/2;
	}
	else
	{
	if (x==RIGHT)
		x=(disp_y_size+1)-(stl*cfont.x_size);
	if (x==CENTER)
		x=((disp_y_size+1)-(stl*cfont.x_size))/2;
	}

	for (i=0; i<stl; i++)
		if (deg==0)
			printChar(*st++, x + (i*(cfont.x_size)), y);
		else
			rotateChar(*st++, x, y, i, deg);
}

void ILI9486::print(String st, int x, int y, int deg)
{
	//char buf[st.length()+1;];
        char buf[] = {0};

	st.toCharArray(buf, st.length()+1);
	print(buf, x, y, deg);
}

void ILI9486::printNumI(long num, int x, int y, int length, char filler)
{
	char buf[25];
	char st[27];
	boolean neg=false;
	int c=0, f=0;
  
	if (num==0)
	{
		if (length!=0)
		{
			for (c=0; c<(length-1); c++)
				st[c]=filler;
			st[c]=48;
			st[c+1]=0;
		}
		else
		{
			st[0]=48;
			st[1]=0;
		}
	}
	else
	{
		if (num<0)
		{
			neg=true;
			num=-num;
		}
	  
		while (num>0)
		{
			buf[c]=48+(num % 10);
			c++;
			num=(num-(num % 10))/10;
		}
		buf[c]=0;
	  
		if (neg)
		{
			st[0]=45;
		}
	  
		if (length>(c+neg))
		{
			for (int i=0; i<(length-c-neg); i++)
			{
				st[i+neg]=filler;
				f++;
			}
		}

		for (int i=0; i<c; i++)
		{
			st[i+neg+f]=buf[c-i-1];
		}
		st[c+neg+f]=0;

	}

	print(st,x,y);
}

void ILI9486::printNumF(double num, byte dec, int x, int y, char divider, int length, char filler)
{
	char st[27];
	boolean neg=false;

	if (dec<1)
		dec=1;
	else if (dec>5)
		dec=5;

	if (num<0)
		neg = true;

	_convert_float(st, num, length, dec);

	if (divider != '.')
	{
		for (uint32_t i=0; i<sizeof(st); i++)
			if (st[i]=='.')
				st[i]=divider;
	}

	if (filler != ' ')
	{
		if (neg)
		{
			st[0]='-';
			for (uint32_t i=1; i<sizeof(st); i++)
				if ((st[i]==' ') || (st[i]=='-'))
					st[i]=filler;
		}
		else
		{
			for (uint32_t i=0; i<sizeof(st); i++)
				if (st[i]==' ')
					st[i]=filler;
		}
	}

	print(st,x,y);
}

void ILI9486::setFont(uint8_t* font)
{
	cfont.font=font;
	cfont.x_size=fontbyte(0);
	cfont.y_size=fontbyte(1);
	cfont.offset=fontbyte(2);
	cfont.numchars=fontbyte(3);
}

uint8_t* ILI9486::getFont()
{
	return cfont.font;
}

uint8_t ILI9486::getFontXsize()
{
	return cfont.x_size;
}

uint8_t ILI9486::getFontYsize()
{
	return cfont.y_size;
}

void ILI9486::drawBitmap(int x, int y, int sx, int sy, bitmapdatatype data, int scale)
{
	unsigned int col;
	int tx, ty, tc, tsx, tsy;

	if (scale==1)
	{
		if (orient==PORTRAIT)
		{
			HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET); // clear(P_CS, B_CS);
			setXY(x, y, x+sx-1, y+sy-1);
			for (tc=0; tc<(sx*sy); tc++)
			{
				col=pgm_read_word(&data[tc]);
				LCD_Write_DATA(col>>8,col & 0xff);
			}
			HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET); // set(P_CS, B_CS);
		}
		else
		{
			HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET); // clear(P_CS, B_CS);
			for (ty=0; ty<sy; ty++)
			{
				setXY(x, y+ty, x+sx-1, y+ty);
				for (tx=sx-1; tx>=0; tx--)
				{
					col=pgm_read_word(&data[(ty*sx)+tx]);
					LCD_Write_DATA(col>>8,col & 0xff);
				}
			}
			HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET); // set(P_CS, B_CS);
		}
	}
	else
	{
		if (orient==PORTRAIT)
		{
			HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET); // clear(P_CS, B_CS);
			for (ty=0; ty<sy; ty++)
			{
				setXY(x, y+(ty*scale), x+((sx*scale)-1), y+(ty*scale)+scale);
				for (tsy=0; tsy<scale; tsy++)
					for (tx=0; tx<sx; tx++)
					{
						col=pgm_read_word(&data[(ty*sx)+tx]);
						for (tsx=0; tsx<scale; tsx++)
							LCD_Write_DATA(col>>8,col & 0xff);
					}
			}
			HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET); // set(P_CS, B_CS);
		}
		else
		{
			HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET); // clear(P_CS, B_CS);
			for (ty=0; ty<sy; ty++)
			{
				for (tsy=0; tsy<scale; tsy++)
				{
					setXY(x, y+(ty*scale)+tsy, x+((sx*scale)-1), y+(ty*scale)+tsy);
					for (tx=sx-1; tx>=0; tx--)
					{
						col=pgm_read_word(&data[(ty*sx)+tx]);
						for (tsx=0; tsx<scale; tsx++)
							LCD_Write_DATA(col>>8,col & 0xff);
					}
				}
			}
			HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET); // set(P_CS, B_CS);
		}
	}
	clrXY();
}

void ILI9486::drawBitmap(int x, int y, int sx, int sy, bitmapdatatype data, int deg, int rox, int roy)
{
	unsigned int col;
	int tx, ty, newx, newy;
	double radian;
	radian=deg*0.0175;  

	if (deg==0)
		drawBitmap(x, y, sx, sy, data);
	else
	{
		HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET); // clear(P_CS, B_CS);
		for (ty=0; ty<sy; ty++)
			for (tx=0; tx<sx; tx++)
			{
				col=pgm_read_word(&data[(ty*sx)+tx]);

				newx=x+rox+(((tx-rox)*cos(radian))-((ty-roy)*sin(radian)));
				newy=y+roy+(((ty-roy)*cos(radian))+((tx-rox)*sin(radian)));

				setXY(newx, newy, newx, newy);
				LCD_Write_DATA(col>>8,col & 0xff);
			}
		HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET); // set(P_CS, B_CS);
	}
	clrXY();
}

void ILI9486::lcdOff()
{
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET); // clear(P_CS, B_CS);

	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET); // set(P_CS, B_CS);
}

void ILI9486::lcdOn()
{
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET); // clear(P_CS, B_CS);

	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET); // set(P_CS, B_CS);
}

void ILI9486::setContrast(char c)
{
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET); // clear(P_CS, B_CS);

	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET); // set(P_CS, B_CS);
}

int ILI9486::getDisplayXSize()
{
	if (orient==PORTRAIT)
		return disp_x_size+1;
	else
		return disp_y_size+1;
}

int ILI9486::getDisplayYSize()
{
	if (orient==PORTRAIT)
		return disp_y_size+1;
	else
		return disp_x_size+1;
}

void ILI9486::setBrightness(byte br)
{
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET); // clear(P_CS, B_CS);

	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET); // set(P_CS, B_CS);
}

void ILI9486::setDisplayPage(byte page)
{
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET); // clear(P_CS, B_CS);

	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET); // set(P_CS, B_CS);
}

void ILI9486::setWritePage(byte page)
{
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET); // clear(P_CS, B_CS);

	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET); // set(P_CS, B_CS);
}

/***** Hardware specific bus defines ***************************************************/

void ILI9486::LCD_Writ_Bus(char VH,char VL)
{

// Pins depend on your display or wiring
//	printf("bus: %X, %X\n", VH, VL);
	HAL_GPIO_WritePin(LCD_D0_GPIO_Port, LCD_D0_Pin, (VH & 1) == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_D1_GPIO_Port, LCD_D1_Pin, (VH & 2) == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_D2_GPIO_Port, LCD_D2_Pin, (VH & 4) == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_D3_GPIO_Port, LCD_D3_Pin, (VH & 8) == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_D4_GPIO_Port, LCD_D4_Pin, (VH & 16) == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_D5_GPIO_Port, LCD_D5_Pin, (VH & 32) == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_D6_GPIO_Port, LCD_D6_Pin, (VH & 64) == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_D7_GPIO_Port, LCD_D7_Pin, (VH & 128) == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET);

	HAL_GPIO_WritePin(LCD_WR_GPIO_Port, LCD_WR_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_WR_GPIO_Port, LCD_WR_Pin, GPIO_PIN_SET);

	HAL_GPIO_WritePin(LCD_D0_GPIO_Port, LCD_D0_Pin, (VL & 1) == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_D1_GPIO_Port, LCD_D1_Pin, (VL & 2) == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_D2_GPIO_Port, LCD_D2_Pin, (VL & 4) == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_D3_GPIO_Port, LCD_D3_Pin, (VL & 8) == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_D4_GPIO_Port, LCD_D4_Pin, (VL & 16) == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_D5_GPIO_Port, LCD_D5_Pin, (VL & 32) == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_D6_GPIO_Port, LCD_D6_Pin, (VL & 64) == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_D7_GPIO_Port, LCD_D7_Pin, (VL & 128) == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_WR_GPIO_Port, LCD_WR_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_WR_GPIO_Port, LCD_WR_Pin, GPIO_PIN_SET);

}

void ILI9486::_fast_fill_8(int ch, long pix)
{
	long blocks;

	HAL_GPIO_WritePin(LCD_D0_GPIO_Port, LCD_D0_Pin, (ch & 1) == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_D1_GPIO_Port, LCD_D1_Pin, (ch & 2) == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_D2_GPIO_Port, LCD_D2_Pin, (ch & 4) == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_D3_GPIO_Port, LCD_D3_Pin, (ch & 8) == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_D4_GPIO_Port, LCD_D4_Pin, (ch & 16) == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_D5_GPIO_Port, LCD_D5_Pin, (ch & 32) == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_D6_GPIO_Port, LCD_D6_Pin, (ch & 64) == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_D7_GPIO_Port, LCD_D7_Pin, (ch & 128) == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET);
//printf("fastbus: %X\n",ch);

	blocks = pix/16;
	for (int i=0; i<blocks; i++)
	{
		pulse_low(LCD_WR_GPIO_Port, LCD_WR_Pin); pulse_low(LCD_WR_GPIO_Port, LCD_WR_Pin);
		pulse_low(LCD_WR_GPIO_Port, LCD_WR_Pin); pulse_low(LCD_WR_GPIO_Port, LCD_WR_Pin);
		pulse_low(LCD_WR_GPIO_Port, LCD_WR_Pin); pulse_low(LCD_WR_GPIO_Port, LCD_WR_Pin);
		pulse_low(LCD_WR_GPIO_Port, LCD_WR_Pin); pulse_low(LCD_WR_GPIO_Port, LCD_WR_Pin);
		pulse_low(LCD_WR_GPIO_Port, LCD_WR_Pin); pulse_low(LCD_WR_GPIO_Port, LCD_WR_Pin);
		pulse_low(LCD_WR_GPIO_Port, LCD_WR_Pin); pulse_low(LCD_WR_GPIO_Port, LCD_WR_Pin);
		pulse_low(LCD_WR_GPIO_Port, LCD_WR_Pin); pulse_low(LCD_WR_GPIO_Port, LCD_WR_Pin);
		pulse_low(LCD_WR_GPIO_Port, LCD_WR_Pin); pulse_low(LCD_WR_GPIO_Port, LCD_WR_Pin);
		pulse_low(LCD_WR_GPIO_Port, LCD_WR_Pin); pulse_low(LCD_WR_GPIO_Port, LCD_WR_Pin);
		pulse_low(LCD_WR_GPIO_Port, LCD_WR_Pin); pulse_low(LCD_WR_GPIO_Port, LCD_WR_Pin);
		pulse_low(LCD_WR_GPIO_Port, LCD_WR_Pin); pulse_low(LCD_WR_GPIO_Port, LCD_WR_Pin);
		pulse_low(LCD_WR_GPIO_Port, LCD_WR_Pin); pulse_low(LCD_WR_GPIO_Port, LCD_WR_Pin);
		pulse_low(LCD_WR_GPIO_Port, LCD_WR_Pin); pulse_low(LCD_WR_GPIO_Port, LCD_WR_Pin);
		pulse_low(LCD_WR_GPIO_Port, LCD_WR_Pin); pulse_low(LCD_WR_GPIO_Port, LCD_WR_Pin);
		pulse_low(LCD_WR_GPIO_Port, LCD_WR_Pin); pulse_low(LCD_WR_GPIO_Port, LCD_WR_Pin);
		pulse_low(LCD_WR_GPIO_Port, LCD_WR_Pin); pulse_low(LCD_WR_GPIO_Port, LCD_WR_Pin);

	}
	if ((pix % 16) != 0)
		for (int i=0; i<(pix % 16)+1; i++)
		{
			pulse_low(LCD_WR_GPIO_Port, LCD_WR_Pin); pulse_low(LCD_WR_GPIO_Port, LCD_WR_Pin);
		}

}
void ILI9486::_convert_float(char *buf, double num, int width, byte prec)
{
	dtostrf(num, width, prec, buf);
}
