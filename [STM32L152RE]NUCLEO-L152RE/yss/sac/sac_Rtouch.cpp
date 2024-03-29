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

#include <sac/Rtouch.h>
#include <yss/event.h>

#if USE_EVENT == true


namespace  sac
{
	Rtouch::Rtouch(void)
	{
		mP1X = mP1Y = mP2X = mP2Y = mWidth = mHeight = 0;
		mInitFlag = false;
	}

	void Rtouch::setCalibration(int32_t p1X, int32_t p1y, int32_t p2x, int32_t p2y)
	{
		mP1X = p1X;
		mP1Y = p1y;
		mP2X = p2x;
		mP2Y = p2y;
	}

	void Rtouch::getCalibration(int32_t *p1X, int32_t *p1y, int32_t *p2x, int32_t *p2y)
	{
	
	}

	void Rtouch::setSize(int32_t width, signed height)
	{
		mWidth = width - 40;
		mHeight = height - 40;
	}

	Position Rtouch::calculate(uint16_t x, uint16_t y)
	{
		int32_t tX = x, tY = y;
		Position pos;

		tX -= mP1X;
		tX *= mWidth;
		tX /= mP2X - mP1X;
		tX += 20;
		if(tX < 0)
			tX = 0;
		else if(tX > mWidth + 40)
			tX = mWidth + 40;
		pos.x = (uint16_t)tX;

		tY -= mP1Y;
		tY *= mHeight;
		tY /= mP2Y - mP1Y;
		tY += 20;
		if(tY < 0)
			tY = 0;
		else if(tY > mHeight + 40)
			tY = mHeight + 40;
		pos.y = (uint16_t)tY;

		return pos;
	}

	void Rtouch::set(uint16_t x, uint16_t y, uint8_t event)
	{
#if USE_GUI && YSS_L_HEAP_USE && USE_EVENT
		event::add(calculate(x, y), event);
#endif
	}

	void Rtouch::trigger(void)
	{
#if USE_GUI && YSS_L_HEAP_USE && USE_EVENT
		event::trigger();
#endif
	}
}

#else
namespace  sac
{
	Rtouch::Rtouch(void)
	{
	}
}
#endif
