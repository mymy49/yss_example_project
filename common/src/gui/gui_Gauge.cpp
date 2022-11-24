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
#include <yss/gui.h>
#include "../inc/gui/Gauge.h"
#include <string.h>
#include <yss/instance.h>
#include <math.h>
#include <stdio.h>
#include <util/time.h>
#include <gui/painter.h>

#if USE_GUI && YSS_L_HEAP_USE

#define calculateToRadian(x)	(x / 180.f * 3.141592654f)

Gauge::Gauge(void)
{
	mBg = new Rgb565();
	mInitFlag = false;
	mTopValue = 100;
	mValue = 0;
	mTitleStr = 0;
}

Gauge::~Gauge(void)
{
	delete mBg;
}

void Gauge::destroy(void)
{
	delete mBg;
}

void Gauge::makeBg(void)
{
	unsigned int r;
	Position p1, p2, p3;
	mMutex.lock();
	mBg->setSize(mSize);

	if(mSize.width > mSize.height)
		r = mSize.height / 2 - 1;
	else
		r = mSize.width / 2 - 1;
	
	mCenter.x = r;
	mCenter.y = r;

	mBg->setBackgroundColor(mBgColor);
	mBg->clear();

	mBg->setBrushColor(0x00, 0x00, 0x00);
	mBg->fillCircle(mCenter, r);
	r -= 5;
	mBg->setBrushColor(0xFF, 0xFF, 0xFF);
	mBg->fillCircle(mCenter, r);
		
	mBg->setBrushColor(0x00, 0x00, 0x00);
	p1.x = mCenter.x + sin(5.5) * (r * 0.8f);
	p1.y = mCenter.y + cos(5.5) * (r * 0.8f);
	
	p2.x = mCenter.x + sin(5.5) * r + 2;
	p2.y = mCenter.y + cos(5.5) * r + 2;
	
	p3.x = mCenter.x + sin(5.5) * r - 2;
	p3.y = mCenter.y + cos(5.5) * r - 2;

	mBg->fillTriangle(p1, p2, p3);

	p1.x = mCenter.x + sin(0.79) * (r * 0.8f);
	p1.y = mCenter.y + cos(0.79) * (r * 0.8f);
	
	p2.x = mCenter.x + sin(0.79) * r - 2;
	p2.y = mCenter.y + cos(0.79) * r + 2;
	
	p3.x = mCenter.x + sin(0.79) * r + 2;
	p3.y = mCenter.y + cos(0.79) * r - 2;

	mBg->fillTriangle(p1, p2, p3);

	if(mTitleStr)
	{
		p1 = mCenter;
		p1.y /= 2;
		p1.x -= mFont.getStringWidth(mTitleStr) / 2;
		mBg->setFont(mFont);
		mBg->setBackgroundColor(0xFF, 0xFF, 0xFF);
		mBg->setFontColor(0x00, 0x00, 0x00);
		mBg->drawString(p1, mTitleStr);
	}

	mNeedleLen = r;
	mInitFlag = true;
	mMutex.unlock();
}

void Gauge::paint(void)
{
	Painter::draw(*this, *mBg, Position{0, 0});

	if(mInitFlag)
	{
		drawNeedle((1.f-mValue / mTopValue) * 270.f + 45.f);
	}
}

void Gauge::setTopValue(float value)
{
	mTopValue = value;
	paint();
	update();
}

void Gauge::setTitle(const char *str)
{
	mTitleStr = str;
}

void Gauge::setValue(float value)
{
	mMutex.lock();

	if(value > mTopValue)
		value = mTopValue;

	if(mValue != value)
	{
		mValue = value;
		paint();
		update();
	}

	mMutex.unlock();
}

void Gauge::drawNeedle(float angle)
{
	float radian = calculateToRadian(angle);
	float r1, r2 = mNeedleLen;
	r1 = r2 * 0.1f;
	Position p1, p2, p3;

	p1.x = sin(radian+calculateToRadian(90.f)) * r1 + mCenter.x;
	p1.y = cos(radian+calculateToRadian(90.f)) * r1 + mCenter.y;

	p2.x = sin(radian-calculateToRadian(90.f)) * r1 + mCenter.x;
	p2.y = cos(radian-calculateToRadian(90.f)) * r1 + mCenter.y;

	p3.x = sin(radian) * r2 + mCenter.x;
	p3.y = cos(radian) * r2 + mCenter.y;
	
	setBrushColor(0xFF, 0x00, 0x00);
	fillTriangle(p1, p2, p3);

	setBrushColor(0x80, 0x80, 0x80);
	drawLine(p3, mCenter);

	setBrushColor(0x00, 0x00, 0x00);
	drawLine(p3, p1);
	drawLine(p3, p2);
	fillCircle(mCenter, (unsigned short)(r1 * 1.5f));
}

void Gauge::setFont(Font font)
{
	mFont = font;
}

#endif