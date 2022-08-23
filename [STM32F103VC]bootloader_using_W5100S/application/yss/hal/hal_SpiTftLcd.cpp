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
#include <hal/SpiTftLcd.h>
#include <yss/malloc.h>
#include <yss/stdlib.h>

SpiTftLcd::SpiTftLcd(void)
{
}

/*
unsigned char SpiTftLcd::drawChar(Pos pos, unsigned int utf8)
{
	signed int buf;
	unsigned short *colorTable = mFontColor.getColorTable();

	if (mFont.setChar(utf8))
		return 0;

	YssFontInfo *fontInfo = mFont.getFontInfo();
	unsigned char *fontFb = mFont.getFrameBuffer(), color;
	int index = 0;
	unsigned short width = fontInfo->width, height = fontInfo->height, offset = 0;
	signed short xs = pos.x, ys = pos.y + (signed char)fontInfo->ypos;

	if (xs + width > mSize.width)
	{
		width = mSize.width - xs;
		offset = fontInfo->width - width;
	}
	if (ys + height > mSize.height)
		height = mSize.height - ys;

	width += xs;
	height += ys;

	for (int y = ys; y < height; y++)
	{
		for (int x = xs; x < width; x++, index++)
		{
			if (index % 2 == 0)
			{
				color = fontFb[index / 2] & 0x0f;
				drawDot(x, y, colorTable[color]);
			}
			else
			{
				color = (fontFb[index / 2] >> 4) & 0x0f;
				drawDot(x, y, colorTable[color]);
			}
		}
		index += offset;
	}

	return fontInfo->width;
}

void SpiTftLcd::fillRect(Pos p1, Pos p2)
{
	signed short buf;

	if (p1.x > p2.x)
	{
		buf = p1.x;
		p1.x = p2.x;
		p2.x = buf;
	}

	if (p1.y > p2.y)
	{
		buf = p1.y;
		p1.y = p2.y;
		p2.y = buf;
	}

	fillRect(p1, Size{(unsigned short)(p2.x - p1.x), (unsigned short)(p2.y - p1.y)});
}

void SpiTftLcd::fillRect(Pos pos, Size size)
{
	int loop = size.height, width = size.width;

	for (int i = 0; i < loop; i++)
	{
		drawDots(pos.x, pos.y + i, mBrushColor.halfword, width);
	}
}

void SpiTftLcd::clear(void)
{
	int loop = mSize.height, width = mSize.width;

	for (int i = 0; i < loop; i++)
	{
		drawDots(0, i, mBgColor.halfword, width);
	}
}

void SpiTftLcd::drawBmp(Pos pos, const Bmp565 *image)
{
	unsigned short *fb = (unsigned short *)image->data;
	unsigned short width = image->width;
	unsigned short height = image->height, offset = 0;
	signed short xs = pos.x, ys = pos.y;

	if (xs + width > mSize.width)
	{
		offset = (xs + width) - mSize.width;
		width = mSize.width - xs;
	}
	if (ys + height > mSize.height)
		height = mSize.height - ys;

	height += ys;

	for (signed short y = ys; y < height; y++)
	{
		drawDots(xs, y, fb, width);
		fb += width + offset;
	}
}

void SpiTftLcd::drawBmp(Pos pos, const Bmp565 &image)
{
	drawBmp(pos, &image);
}
*/