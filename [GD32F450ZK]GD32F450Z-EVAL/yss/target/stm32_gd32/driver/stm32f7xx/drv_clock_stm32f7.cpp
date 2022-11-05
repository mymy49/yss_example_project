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

#include <drv/peripheral.h>

#if false

#include <drv/Clock.h>
#include <cmsis/mcu/st_gigadevice/rcc_stm32_gd32f4.h>

int32_t  Clock::mHseFreq __attribute__((section(".non_init")));

static const int16_t gPpreDiv[8] = {1, 1, 1, 1, 2, 4, 8, 16};
static const int16_t gHpreDiv[16] = {1, 1, 1, 1, 1, 1, 1, 1, 2, 4, 8, 16, 64, 128, 256, 512};

bool Clock::enableHse(uint32_t hseHz, bool useOsc)
{
	mHseFreq = hseHz;
	
	using namespace ec::clock::hse;
	if (hseHz < HSE_MIN_FREQ || HSE_MAX_FREQ < hseHz)
		return false;

	if (useOsc)
		RCC[RCC_REG::CR] |= RCC_CR_HSEON_Msk | RCC_CR_HSEBYP_Msk;
	else
		RCC[RCC_REG::CR] |= RCC_CR_HSEON_Msk;

	for (uint32_t i = 0; i < 100000; i++)
	{
		if (RCC[RCC_REG::CR] & RCC_CR_HSERDY_Msk)
			return true;
	}

	return false;
}

/*
bool Clock::enableLsi(bool)
{
	setRccLsiEn(true);
	for (uint16_t i = 0; i < 10000; i++)
	{
		if (getRccLsiReady())
			return true;
	}

	return false;
}

bool Clock::setUsbClockSource(uint8_t src)
{
	if (src < 0 || src > 1)
		return false;

	setRccUsbClkSel(src);
	return true;
}

bool Clock::enableLse(bool en)
{
	setRccLseEn(en);
	if (en == true)
	{
		for (uint16_t i = 0; i < 10000; i++)
		{
			if (getRccLseReady())
				return true;
		}
	}
	else
	{
		return true;
	}

	return false;
}

bool Clock::enableMainPll(uint8_t src, uint8_t m, uint16_t n, uint8_t pDiv, uint8_t qDiv, uint8_t rDiv)
{
	uint32_t vco, pll, pll48, buf;

	if (ec::clock::pll::M_MIN > m || m > ec::clock::pll::M_MAX)
		goto error;

	if (ec::clock::pll::N_MIN > n || n > ec::clock::pll::N_MAX)
		goto error;

	if (pDiv > ec::clock::pll::P_MAX)
		goto error;

	if (ec::clock::pll::Q_MIN > qDiv || qDiv > ec::clock::pll::Q_MAX)
		goto error;

	switch (src)
	{
	case define::clock::pll::src::HSI:
		buf = ec::clock::hsi::FREQ;
		break;
	case define::clock::pll::src::HSE:
		if (getRccHseReady() == false)
			goto error;
		buf = (uint32_t)mHseFreq;
		break;
	default:
		goto error;
	}

	if (getRccMainPllReady() == true && getRccSysclkSw() == define::clock::sysclk::src::PLL)
		goto error;

	vco = buf / m * n;
	if (vco < ec::clock::pll::VCO_MIN_FREQ || ec::clock::pll::VCO_MAX_FREQ < vco)
		goto error;

	pll = vco / (2 << pDiv);

	if (pll > ec::clock::sysclk::MAX_FREQ)
		goto error;

	pll48 = vco / qDiv;
	if (pll48 > ec::clock::pll::USB48_MAX_FREQ)
		goto error;

	setRccMainPllSrc(src);
	setRccMainPllm(m);
	setRccMainPlln(n);
	setRccMainPllp(pDiv);
	setRccMainPllq(qDiv);

	setRccMainPllOn(true);

	for (uint16_t i = 0; i < 10000; i++)
	{
		if (getRccMainPllReady())
		{
			mPllFreq = pll;
			mMainPllUsbFreq = pll48;
			return true;
		}
	}

error:
	mPllFreq = 0;
	return false;
}

int32_t  Clock::getTimerApb1ClkFreq(void)
{
	int8_t pre = getRccPpre1();
	int32_t  clk = getSysClkFreq() / gPpreDiv[pre];
	if (gPpreDiv[pre] > 1)
		clk <<= 1;
	return clk;
}

int32_t  Clock::getTimerApb2ClkFreq(void)
{
	uint8_t pre = getRccPpre2();
	uint32_t clk = getSysClkFreq() / gPpreDiv[pre];
	if (gPpreDiv[pre] > 1)
		clk <<= 1;
	return clk;
}

int32_t  Clock::getApb1ClkFreq(void)
{
	return getSysClkFreq() / gPpreDiv[getRccPpre1()];
}

int32_t  Clock::getApb2ClkFreq(void)
{
	return (uint32_t)(getSysClkFreq() / gPpreDiv[getRccPpre2()]);
}

void Clock::setLatency(uint32_t freq, uint8_t vcc)
{
	uint32_t div, wait;

	if (vcc > 27)
	{
		div = 30;
	}
	else if (vcc > 24)
	{
		div = 24;
	}
	else if (vcc > 21)
	{
		div = 22;
	}
	else
	{
		div = 20;
	}

	freq /= 1000000;
	wait = freq / div;
	if (!(freq % div))
		wait--;
	
	FLASH->ACR = (FLASH->ACR & ~FLASH_ACR_LATENCY_Msk) | ((wait << FLASH_ACR_LATENCY_Pos) & FLASH_ACR_LATENCY_Msk);
}

int32_t  Clock::getSysClkFreq(void)
{
	uint32_t clk;

	switch (getRccSysclkSw())
	{
	case define::clock::sysclk::src::HSI:
		clk = ec::clock::hsi::FREQ;
		break;
	case define::clock::sysclk::src::HSE:
		clk = mHseFreq;
		break;
	case define::clock::sysclk::src::PLL:
		clk = mPllFreq;
		break;
	}

	clk /= gHpreDiv[getRccHpre()];

	return clk;
}

bool Clock::enableSaiPll(uint16_t n, uint8_t pDiv, uint8_t qDiv, uint8_t rDiv)
{
	uint32_t vco, q, r, sai, pll48, lcd, buf, m;
	bool able = getRccMainPllReady();

	using namespace ec::clock;

	if (!able)
		goto error;

	if (saipll::N_MIN > n || n > saipll::N_MAX)
		goto error;

	if (pDiv > saipll::P_MAX)
		goto error;

	if (saipll::Q_MIN > qDiv || qDiv > saipll::Q_MAX)
		goto error;

	if (saipll::R_MIN > rDiv || rDiv > saipll::R_MAX)
		goto error;

	switch (getBitData(RCC->PLLCFGR, RCC_PLLCFGR_PLLSRC_Pos))
	{
	case define::clock::pll::src::HSI:
		buf = ec::clock::hsi::FREQ;
		break;
	case define::clock::pll::src::HSE:
		if (getRccHseReady() == false)
			goto error;
		buf = (uint32_t)mHseFreq;
		break;
	default:
		goto error;
	}

	vco = buf / getFieldData(RCC->PLLCFGR, RCC_PLLCFGR_PLLM_Msk, RCC_PLLCFGR_PLLM_Pos) * n ;
	if (vco < saipll::VCO_MIN_FREQ || saipll::VCO_MAX_FREQ < vco)
		goto error;

	pll48 = vco / (2 << pDiv);

	sai = vco / qDiv;
	if (saipll::SAI_MAX_FREQ < sai)
		goto error;

	lcd = vco / rDiv;
	if (saipll::LCD_MAX_FREQ < lcd)
		goto error;

	setRccSaiPlln(n);
	setRccSaiPllp(pDiv);
	setRccSaiPllq(qDiv);
	setRccSaiPllr(rDiv);

	setRccSaiPllOn(true);

	for (uint16_t i = 0; i < 10000; i++)
	{
		if (getRccSaiPllReady())
		{
			mLcdPllFreq = lcd;
			mSaiPllFreq = sai;
			return true;
		}
	}

error:
	mLcdPllFreq = 0;
	mSaiPllFreq = 0;
	return false;
}

bool Clock::setSysclk(uint8_t sysclkSrc, uint8_t ahb, uint8_t apb1, uint8_t apb2, uint8_t vcc)
{
	uint32_t clk, ahbClk, apb1Clk, apb2Clk, adcClk;

	using namespace define::clock::sysclk::src;
	switch (sysclkSrc)
	{
	case HSI:
		clk = ec::clock::hsi::FREQ;
		break;
	case HSE:
		if (getRccHseReady() == false)
			return false;

		clk = mHseFreq;
		break;
	case PLL:
		if (getRccMainPllReady() == false)
			return false;

		clk = mPllFreq;
		break;
	default:
		return false;
	}

	ahbClk = clk / gHpreDiv[ahb];
	if (ahbClk > ec::clock::sysclk::MAX_FREQ)
		return false;

	apb1Clk = ahbClk / gPpreDiv[apb1];
	if (apb1Clk > ec::clock::apb1::MAX_FREQ)
		return false;

	apb2Clk = ahbClk / gPpreDiv[apb2];
	if (apb2Clk > ec::clock::apb2::MAX_FREQ)
		return false;

	if (ahbClk > ec::clock::sysclk::OVER_DRIVE_FREQ)
	{
		setRegBit(RCC->APB1ENR, 1, RCC_APB1ENR_PWREN_Pos);
		setRegBit(PWR->CR1, 1, PWR_CR1_ODEN_Pos);
		while (!getRegBit(PWR->CR1, PWR_CR1_ODEN_Pos))
			;
		
		setRegBit(PWR->CR1, 1, PWR_CR1_ODSWEN_Pos);
		while (!getRegBit(PWR->CR1, PWR_CR1_ODSWEN_Pos))
			;
	}

	setRccHpre(ahb);
	setRccPpre1(apb1);
	setRccPpre2(apb2);

	setLatency(ahbClk, vcc);
	setRccSysclkSw(sysclkSrc);

	return true;
}

int32_t  Clock::getSdmmcClockFrequency(void)
{
	if(RCC->DCKCFGR2 & RCC_DCKCFGR2_SDMMC1SEL_Msk)
		return getSysClkFreq();
	else
		return mMainPllUsbFreq;
}
*/
#endif