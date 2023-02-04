////////////////////////////////////////////////////////////////////////////////////////
//
// 저작권 표기 License_ver_3.2
// 본 소스 코드의 소유권은 홍윤기에게 있습니다.
// 어떠한 형태든 기여는 기증으로 받아들입니다.
// 본 소스 코드는 아래 사항에 동의할 경우에 사용 가능합니다.
// 아래 사항에 대해 동의하지 않거나 이해하지 못했을 경우 사용을 금합니다.
// 본 소스 코드를 사용하였다면 아래 사항을 모두 동의하는 것으로 자동 간주 합니다.
// 본 소스 코드의 상업적 또는 비 상업적 이용이 가능합니다.
// 본 소스 코드의 내용을 임의로 수정하여 재배포하는 행위를 금합니다.
// 본 소스 코드의 사용으로 인해 발생하는 모든 사고에 대해서 어떠한 법적 책임을 지지 않습니다.
// 본 소스 코드의 어떤 형태의 기여든 기증으로 받아들입니다.
//
// Home Page : http://cafe.naver.com/yssoperatingsystem
// Copyright 2023. 홍윤기 all right reserved.
//
////////////////////////////////////////////////////////////////////////////////////////

#if defined(NRF52840_XXAA)

#include <drv/peripheral.h>
#include <drv/Clock.h>

bool Clock::enableHse(uint32_t, bool)
{
	NRF_CLOCK->TASKS_HFCLKSTART = 1;

	for (uint32_t i = 0; i < 1000000; i++)
	{
		if (NRF_CLOCK->EVENTS_HFCLKSTARTED)
			return true;
	}

	return false;
}

uint32_t Clock::getSystemClockFrequency(void)
{
	return 64000000;
}

uint32_t Clock::getCoreClockFrequency(void)
{
	return 64000000;
}

uint32_t Clock::getAhbClockFrequency(void)
{
	return 64000000;
}

uint32_t Clock::getApb1ClockFrequency(void)
{
	return 64000000;
}

uint32_t Clock::getApb2ClockFrequency(void)
{
	return 64000000;
}

/*
int32_t  Clock::getSysClkFreq(void)
{
	return 60000000;
}
*/
/*
uint32_t Clock::getApb1ClkFreq(void)
{
	return getSysClkFreq() / gPpreDiv[getFieldData(RCC->GCFGR, 0x7 << 8, 8)];
} 

uint32_t Clock::getApb2ClkFreq(void)
{
	return getSysClkFreq() / gPpreDiv[getFieldData(RCC->GCFGR, 0x7 << 11, 11)];
}

uint32_t Clock::getTimerApb1ClkFreq(void)
{
	uint8_t pre = getFieldData(RCC->GCFGR, 0x7 << 8, 8);
	uint32_t clk = getSysClkFreq() / gPpreDiv[pre];
	if (gPpreDiv[pre] > 1)
		clk <<= 1;
	return clk;
}

uint32_t Clock::getTimerApb2ClkFreq(void)
{
	uint8_t pre = getFieldData(RCC->GCFGR, 0x7 << 11, 11);
	uint32_t clk = getSysClkFreq() / gPpreDiv[pre];
	if (gPpreDiv[pre] > 1)
		clk <<= 1;
	return clk;
}

void Clock::setLatency(uint32_t freq, uint8_t vcc)
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

#endif