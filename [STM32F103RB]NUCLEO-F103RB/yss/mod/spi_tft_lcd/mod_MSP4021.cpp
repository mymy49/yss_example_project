////////////////////////////////////////////////////////////////////////////////////////
//
// 저작권 표기 License_ver_3.0
// 본 소스 코드의 소유권은 홍윤기에게 있습니다.
// 어떠한 형태든 기여는 기증으로 받아들입니다.
// 본 소스 코드는 아래 사항에 동의할 경우에 사용 가능합니다.
// 아래 사항에 대해 동의하지 않거나 이해하지 못했을 경우 사용을 금합니다.
// 본 소스 코드를 사용하였다면 아래 사항을 모두 동의하는 것으로 자동 간주 합니다.
// 본 소스 코드의 상업적 또는 비 상업적 이용이 가능합니다.
// 본 소스 코드의 내용을 임의로 수정하여 재배포하는 행위를 금합니다.
// 본 소스 코드의 내용을 무단 전재하는 행위를 금합니다.
// 본 소스 코드의 사용으로 인해 발생하는 모든 사고에 대해서 어떠한 법적 책임을 지지 않습니다.
//
// Home Page : http://cafe.naver.com/yssoperatingsystem
// Copyright 2022. 홍윤기 all right reserved.
//
////////////////////////////////////////////////////////////////////////////////////////

#include <mod/spi_tft_lcd/MSP4021.h>
#include <yss/stdlib.h>

#if !defined(YSS_DRV_SPI_UNSUPPORTED) && !defined(YSS_DRV_GPIO_UNSUPPORTED)

MSP4021::MSP4021(void)
{
	Brush::setSize(Size{320, 480});
	mBmp888Brush = 0;
	mBmp888BufferSize = 0;
}

void MSP4021::setDirection(bool xMirror, bool yMirror, bool rotate)
{
	ST7796S_SPI::setDirection(xMirror, yMirror, rotate);

	if(rotate)
		Brush::setSize(Size{480, 320});
	else
		Brush::setSize(Size{320, 480});
}

void MSP4021::drawDot(int16_t x, int16_t y)
{
	if (y < mSize.height && x < mSize.width)
	{
		enable();
		setWindows(x, y);
		sendCmd(MEMORY_WRITE, &mBrushColor, 3);
		disable();
	}
}

void MSP4021::drawDot(int16_t x, int16_t y, uint16_t color)
{
}

void MSP4021::drawDot(int16_t x, int16_t y, uint32_t color)
{
	if (y < mSize.height && x < mSize.width)
	{
		enable();
		setWindows(x, y);
		sendCmd(MEMORY_WRITE, &color, 3);
		disable();
	}
}

void MSP4021::drawFontDot(int16_t x, int16_t y, uint8_t color)
{
}

void MSP4021::eraseDot(Position pos)
{
	if (pos.y < mSize.height && pos.x < mSize.width)
	{
		enable();
		setWindows(pos.x, pos.y);
		sendCmd(MEMORY_WRITE, mBgColor.byte, 3);
		disable();
	}
}

void MSP4021::setBrushColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
	mBrushColor.color.red = red;
	mBrushColor.color.green = green;
	mBrushColor.color.blue = blue;
}

void MSP4021::setFontColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
}

void MSP4021::setBackgroundColor(uint8_t red, uint8_t green, uint8_t blue)
{
	mBgColor.color.red = red;
	mBgColor.color.green = green;
	mBgColor.color.blue = blue;
}

void MSP4021::drawBmp(Position pos, const Bmp888 *image)
{
	// RGB888이 아니면 리턴
	if (image->type != 1)
		return;
	
	enable();
	setWindows(pos.x, pos.y, image->width, image->height);
	sendCmd(MEMORY_WRITE, image->data, image->width * image->height * 3);
	disable();
}

void MSP4021::setBmp888Brush(Bmp888Brush &obj)
{
	mBmp888Brush = &obj;
	mBmp888BufferSize = obj.getBufferSize();
}

void MSP4021::clear(void)
{
	if(!mBmp888Brush)
		return;
	uint32_t width, height, loop, lastPos = 0;

	if(mRotateFlag)
	{
		width = 480;
		height = (mBmp888BufferSize / 3) / width;
		loop = 320 / height;
		if(320 % height)
		{
			lastPos = 319 - height;
		}
	}
	else
	{
		width = 320;
		height = (mBmp888BufferSize / 3) / width;
		loop = 480 / height;
		if(480 % height)
		{
			lastPos = 479 - height;
		}
	}
	
	mBmp888Brush->setSize(width, height);
	mBmp888Brush->setBackgroundColor(mBgColor.color.red, mBgColor.color.green, mBgColor.color.blue);
	mBmp888Brush->clear();
	
	for(int32_t  i=0;i<loop;i++)
	{
		drawBmp(Position{0, (int16_t)(height * i)}, mBmp888Brush->getBmp888());
	}

	if(lastPos)
		drawBmp(Position{0, (int16_t)lastPos}, mBmp888Brush->getBmp888());
}

void MSP4021::fillRect(Position p1, Position p2)
{
	if(!mBmp888Brush)
		return;
	uint32_t width, height, loop, bufHeight, y;
	Position pos;

	if(p1.x < p2.x)
	{
		width = p2.x - p1.x;
		pos.x = p1.x;
	}
	else if(p1.x > p2.x)
	{
		width = p1.x - p2.x;
		pos.x = p2.x;
	}
	else
		return;

	if(p1.y < p2.y)
	{
		height = p2.y - p1.y;
		pos.y = p1.y;
		y = p1.y;
	}
	else if(p1.y > p2.y)
	{
		height = p1.y - p2.y;
		pos.y = p2.y;
		y = p2.y;
	}
	else
		return;

	bufHeight = (mBmp888BufferSize / 3) / width;
	loop = height / bufHeight;
	if(loop)
		mBmp888Brush->setSize(width, bufHeight);
	else
		mBmp888Brush->setSize(width, height);

	mBmp888Brush->setBackgroundColor(mBrushColor.color.red, mBrushColor.color.green, mBrushColor.color.blue);
	mBmp888Brush->clear();
	
	for(int32_t  i=0;i<loop;i++)
	{
		drawBmp(pos, mBmp888Brush->getBmp888());
		pos.y += bufHeight;
	}
	
	height -= loop * bufHeight;
	if(height)
	{
		mBmp888Brush->setSize(width, height);
		drawBmp(Position{pos.x, pos.y}, mBmp888Brush->getBmp888());
	}
}

void MSP4021::fillRect(Position pos, Size size)
{
	fillRect(pos, Position{(int16_t)(pos.x + size.width), (int16_t)(pos.y + size.height)});
}

#endif

