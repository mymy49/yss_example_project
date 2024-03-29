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

#if USE_GUI && YSS_L_HEAP_USE

#include <yss/instance.h>
#include <yss/gui.h>
#include <std_ext/malloc.h>

FrameBuffer::FrameBuffer(void)
{
	mDotSize = 0;
	mSize.height = 0;
	mSize.width = 0;
	mFrameBuffer = 0;
	mAlpha = 0xff;
}

FrameBuffer::~FrameBuffer(void)
{
	if (mFrameBuffer)
		lfree(mFrameBuffer);
}

void FrameBuffer::setSize(Size size)
{
	setSize(size.width, size.height);
}

void FrameBuffer::setSize(int16_t width, int16_t height)
{
	mSize = Size{width, height};

	if (mFrameBuffer)
		lfree(mFrameBuffer);

	if (height == 0 || width == 0)
		mFrameBuffer = 0;
	else
		mFrameBuffer = lmalloc(width * height * mDotSize);

	Brush::setSize(mSize);
}

void *FrameBuffer::getFrameBuffer(void)
{
	return mFrameBuffer;
}

Size FrameBuffer::getSize(void)
{
	return mSize;
}

uint8_t FrameBuffer::getDotSize(void)
{
	return mDotSize;
}

uint8_t FrameBuffer::getColorMode(void)
{
	return mColorMode;
}

uint8_t FrameBuffer::getAlpha(void)
{
	return mAlpha;
}

void FrameBuffer::setAlpha(uint8_t alpha)
{
	mAlpha = alpha;
}

Font* FrameBuffer::getFont(void)
{
	return &mFont;
}

#endif