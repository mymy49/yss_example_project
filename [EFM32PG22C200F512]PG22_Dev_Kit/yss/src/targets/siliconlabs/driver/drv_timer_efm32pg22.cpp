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

#include <drv/mcu.h>

#if defined(EFM32PG22)

#include <drv/peripheral.h>
#include <drv/Timer.h>
#include <yss/reg.h>

static uint32_t gDivider = 1;

Timer::Timer(YSS_TIMER_Peri *peri, const Drv::Config drvConfig) : Drv(drvConfig)
{
	mPeri = peri;
	mIsrUpdate = 0;
}

void Timer::initSystemTime(void)
{
	uint32_t clk = getClockFrequency();
	gDivider = clk / 1000000;

	mPeri->EN_SET = TIMER_EN_EN;
	mPeri->TOP = 0xFFFFFFFF;

	enableUpdateInterrupt();
}

void Timer::init(uint32_t psc, uint32_t arr)
{
	// 본 MCU에서는 타이머가 32비트이므로 PSC를 사용하지 않음
	mPeri->EN_SET = TIMER_EN_EN;
	mPeri->TOP = arr;
}

void Timer::init(uint32_t freq)
{
	uint32_t clk = getClockFrequency();

	mPeri->EN_SET = TIMER_EN_EN;
	mPeri->TOP = clk / freq - 1;
}

void Timer::enableUpdateInterrupt(bool en)
{
	setBitData(mPeri->IEN, en, _TIMER_IEN_OF_SHIFT);
}

void Timer::start(void)
{
	mPeri->CMD_SET = _TIMER_CMD_START_MASK;
}

void Timer::stop(void)
{
	mPeri->CMD_SET = _TIMER_CMD_STOP_MASK;
}

uint32_t Timer::getCounterValue(void)
{
	return mPeri->CNT / gDivider;
}

uint32_t Timer::getOverFlowCount(void)
{
	return 0xFFFFFFFF / gDivider;
}

void Timer::setUpdateIsr(void (*isr)(void))
{
	mIsrUpdate = isr;
}

void Timer::isrUpdate(void)
{
	if (mIsrUpdate)
		mIsrUpdate();
}

#endif

