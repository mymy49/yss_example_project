////////////////////////////////////////////////////////////////////////////////////////
//
// 저작권 표기 License_ver_3.1
// 본 소스 코드의 소유권은 홍윤기에게 있습니다.
// 어떠한 형태든 기여는 기증으로 받아들입니다.
// 본 소스 코드는 아래 사항에 동의할 경우에 사용 가능합니다.
// 아래 사항에 대해 동의하지 않거나 이해하지 못했을 경우 사용을 금합니다.
// 본 소스 코드를 사용하였다면 아래 사항을 모두 동의하는 것으로 자동 간주 합니다.
// 본 소스 코드의 상업적 또는 비 상업적 이용이 가능합니다.
// 본 소스 코드의 내용을 임의로 수정하여 재배포하는 행위를 금합니다.
// 본 소스 코드의 사용으로 인해 발생하는 모든 사고에 대해서 어떠한 법적 책임을 지지 않습니다.
//
// Home Page : http://cafe.naver.com/yssoperatingsystem
// Copyright 2022. 홍윤기 all right reserved.
//
////////////////////////////////////////////////////////////////////////////////////////

#include <config.h>

#if USE_GUI == true

#include <gui/BrushRgb565.h>
#include <gui/Bmp565.h>

#define PI (float)3.14159265358979323846

BrushRgb565::BrushRgb565(void)
{
}

BrushRgb565::~BrushRgb565(void)
{
}

void BrushRgb565::drawBmp(Position pos, const Bmp565 *image)
{
	uint16_t *fb = (uint16_t *)image->data, *src;
	uint16_t width = image->width;
	uint16_t height = image->height;
	int16_t xs = pos.x, ys = pos.y;

	if (xs + width > mSize.width)
		width = mSize.width - xs;
	if (ys + height > mSize.height)
		height = mSize.height - ys;

	width += xs;
	height += ys;

	for (int16_t y = ys; y < height; y++)
	{
		src = fb;
		fb += image->width;

		for (int16_t x = xs; x < width; x++)
		{
			drawDot(x, y, *src++);
		}
	}
}

void BrushRgb565::drawBmp(Position pos, const Bmp565 &image)
{
	drawBmp(pos, &image);
}


void BrushRgb565::updateFontColor(void)
{
	for(uint8_t i=0;i<16;i++)
		mFontColorTable[i] = mFontColor.calculateFontColorLevel(mBgColor, i).getRgb565Code();
}

#endif