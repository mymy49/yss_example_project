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

#include <config.h>

#if USE_GUI

#include <drv/peripheral.h>
#include <gui/Bmp888Brush.h>
#include <yss/string.h>

Bmp888Brush::Bmp888Brush(uint32_t pointSize)
{
	mBufferSize = pointSize * 3;
	mFrameBuffer = new uint8_t[mBufferSize];

	mBmp888.width = 0;
	mBmp888.height = 0;
	mBmp888.data = mFrameBuffer;
	mBmp888.type = 1;
}

Bmp888Brush::~Bmp888Brush(void)
{
	if (mFrameBuffer)
		delete mFrameBuffer;
}

void Bmp888Brush::setSize(uint16_t width, uint16_t height)
{
	if (mBufferSize < width * height * 3)
	{
		return;
	}

	mOkFlag = true;
	mBmp888.width = width;
	mBmp888.height = height;
	Brush::setSize(Size{width, height});
}

void Bmp888Brush::setSize(Size size)
{
	mBmp888.width = size.width;
	mBmp888.height = size.height;
	Brush::setSize(size);
}

uint32_t Bmp888Brush::getBufferSize(void)
{
	return mBufferSize;
}

void Bmp888Brush::drawDot(int16_t x, int16_t y, uint16_t color)
{
}

void Bmp888Brush::drawDot(int16_t x, int16_t y)
{
	uint8_t *des = &mFrameBuffer[y * mSize.width * 3 + x * 3];
	uint8_t *src = mBrushColor.byte;
	*des++ = *src++;
	*des++ = *src++;
	*des++ = *src++;
}

void Bmp888Brush::drawDot(int16_t x, int16_t y, uint32_t color)
{
	uint8_t *des = &mFrameBuffer[y * mSize.width * 3 + x * 3];
	uint8_t *src = (uint8_t*)&color;
	*des++ = *src++;
	*des++ = *src++;
	*des++ = *src++;
}

void Bmp888Brush::drawFontDot(int16_t x, int16_t y, uint8_t color)
{
	RGB888_union *colorTable = mFontColor.getColorTable();
	drawDot(x, y, *(uint32_t*)colorTable[color].byte);
}

void Bmp888Brush::eraseDot(Position pos)
{
	uint8_t *des = &mFrameBuffer[pos.y * mSize.width * 3 + pos.x * 3];
	uint8_t *src = mBgColor.byte;
	*des++ = *src++;
	*des++ = *src++;
	*des++ = *src++;
}

void Bmp888Brush::setBrushColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
	mBrushColor.color.red = red;
	mBrushColor.color.green = green;
	mBrushColor.color.blue = blue;
}

void Bmp888Brush::setFontColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
	mFontColor.setFontColor(red, green, blue);
	mFontColor.calculate();
}

void Bmp888Brush::setBackgroundColor(uint8_t red, uint8_t green, uint8_t blue)
{
	mBgColor.color.red = red;
	mBgColor.color.green = green;
	mBgColor.color.blue = blue;
	mFontColor.setBgColor(red, green, blue);
	mFontColor.calculate();
}

uint8_t Bmp888Brush::drawChar(Position pos, uint32_t utf8)
{
	int32_t buf;
	RGB888_union *colorTable = mFontColor.getColorTable();

	if (mFont.setChar(utf8))
		return 0;

	YssFontInfo *fontInfo = mFont.getFontInfo();
	uint8_t *fontFb = mFont.getFrameBuffer(), color;
	int32_t  index = 0;
	uint16_t width = fontInfo->width, height = fontInfo->height, offset = 0;
	int16_t xs = pos.x, ys = pos.y + (int8_t)fontInfo->ypos;

	if (xs + width > mSize.width)
	{
		width = mSize.width - xs;
		offset = fontInfo->width - width;
	}
	if (ys + height > mSize.height)
		height = mSize.height - ys;

	width += xs;
	height += ys;

	for (int32_t  y = ys; y < height; y++)
	{
		for (int32_t  x = xs; x < width; x++, index++)
		{
			if (index % 2 == 0)
			{
				color = fontFb[index / 2] & 0x0f;
				drawDot(x, y, *(uint32_t*)colorTable[color].byte);
			}
			else
			{
				color = (fontFb[index / 2] >> 4) & 0x0f;
				drawDot(x, y, *(uint32_t*)colorTable[color].byte);
			}
		}
		index += offset;
	}

	return fontInfo->width;
}

void Bmp888Brush::fillRect(Position pos, Size size)
{
	int16_t sx = pos.x, ex = pos.x + size.width, sy = pos.y, ey = pos.y + size.height;
	uint32_t *des = (uint32_t*)mFrameBuffer;

	if (ey > mSize.height - 1)
		ey = mSize.height - 1;
	if (ex > mSize.width - 1)
		ex = mSize.width - 1;

	des += sx * 3 + sy * mSize.width * 3;
	for (int16_t y = sy; y <= ey; y++)
	{
		memsethw(des, *(uint32_t*)mBrushColor.byte, mSize.width);
		des += mSize.width;
	}
}

void Bmp888Brush::fillRect(Position p1, Position p2)
{
	int16_t sx, ex, sy, ey;
	uint8_t *des = mFrameBuffer;

	if (p1.x < p2.x)
	{
		sx = p1.x;
		ex = p2.x;
	}
	else
	{
		sx = p2.x;
		ex = p1.x;
	}

	if (p1.y < p2.y)
	{
		sy = p1.y;
		ey = p2.y;
	}
	else
	{
		sy = p2.y;
		ey = p1.y;
	}

	if (ey > mSize.height - 1)
		ey = mSize.height - 1;
	if (ex > mSize.width - 1)
		ex = mSize.width - 1;

	des += sx * 3 + sy * mSize.width * 3;
	for (int16_t y = sy; y <= ey; y++)
	{
		memsethw(des, *(uint32_t*)mBrushColor.byte, mSize.width);
		des += mSize.width * 3;
	}
}

void Bmp888Brush::clear(void)
{
	copyRgb888DotPattern(mFrameBuffer, *(uint32_t*)mBgColor.byte, mSize.width * mSize.height);
}

Bmp888 *Bmp888Brush::getBmp888(void)
{
	return &mBmp888;
}

void Bmp888Brush::drawStringToCenterAligned(const char *str)
{
	Position pos;
	Size size = calculateStringSize(str);

	pos.x = (mSize.width - size.width) / 2;
	if (pos.x < 0)
		pos.x = 0;
	pos.y = (mSize.height - size.height) / 2;
	if (pos.y < 0)
		pos.y = 0;
	Brush::drawString(pos, str);
}

#endif

