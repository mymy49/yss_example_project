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

#include <util/Period.h>
#include <util/time.h>
#include <yss/thread.h>

#if !(defined(__CORE_CM0PLUS_H_GENERIC) || defined(__CORE_CM0_H_GENERIC))

Period::Period(void)
{
	mPeriod = 1000000;
	mLastTime = 0;
}

Period::Period(uint32_t time)
{
	mPeriod = time;
	mLastTime = 0;
}

void Period::reset(void)
{
	mLastTime = time::getRunningUsec();
}

uint32_t Period::wait(void)
{
	uint64_t thisTime;

	mLastTime += mPeriod;
	do
	{
		thread::yield();
		thisTime = time::getRunningUsec();
	} while (mLastTime >= thisTime);

	return (uint32_t)(mLastTime - thisTime + mPeriod);
}

void Period::setPeriod(uint32_t time)
{
	mPeriod = time;
	reset();
}

#endif
