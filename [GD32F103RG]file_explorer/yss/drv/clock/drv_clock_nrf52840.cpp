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

#if defined(NRF52840_XXAA)

#include <drv/peripheral.h>
#include <drv/clock/register_clock_gd32f1.h>
#include <drv/Clock.h>

namespace drv
{
bool Clock::enableHse(unsigned int, bool)
{
	NRF_CLOCK->TASKS_HFCLKSTART = 1;

	for (unsigned int i = 0; i < 1000000; i++)
	{
		if (NRF_CLOCK->EVENTS_HFCLKSTARTED)
			return true;
	}

	return false;
}

unsigned int Clock::getSysClkFreq(void)
{
	return 60000000;
}

/*
unsigned int Clock::getApb1ClkFreq(void)
{
	return getSysClkFreq() / gPpreDiv[getFieldData(RCC->GCFGR, 0x7 << 8, 8)];
} 

unsigned int Clock::getApb2ClkFreq(void)
{
	return getSysClkFreq() / gPpreDiv[getFieldData(RCC->GCFGR, 0x7 << 11, 11)];
}

unsigned int Clock::getTimerApb1ClkFreq(void)
{
	unsigned char pre = getFieldData(RCC->GCFGR, 0x7 << 8, 8);
	unsigned int clk = getSysClkFreq() / gPpreDiv[pre];
	if (gPpreDiv[pre] > 1)
		clk <<= 1;
	return clk;
}

unsigned int Clock::getTimerApb2ClkFreq(void)
{
	unsigned char pre = getFieldData(RCC->GCFGR, 0x7 << 11, 11);
	unsigned int clk = getSysClkFreq() / gPpreDiv[pre];
	if (gPpreDiv[pre] > 1)
		clk <<= 1;
	return clk;
}

void Clock::setLatency(unsigned int freq, unsigned char vcc)
{
	//if (freq < 24000000)
	//	FLASH->ACR &= ~FLASH_ACR_LATENCY_Msk;
	//else if (freq < 48000000)
	//{
	//	reg = FLASH->ACR;
	//	reg = (reg & ~FLASH_ACR_LATENCY_Msk) | (1 << FLASH_ACR_LATENCY_Pos);
	//	FLASH->ACR = reg;
	//}
	//else
	//{
	//	reg = FLASH->ACR;
	//	reg = (reg & ~FLASH_ACR_LATENCY_Msk) | (2 << FLASH_ACR_LATENCY_Pos);
	//	FLASH->ACR = reg;
	//}
}
*/
}

#endif