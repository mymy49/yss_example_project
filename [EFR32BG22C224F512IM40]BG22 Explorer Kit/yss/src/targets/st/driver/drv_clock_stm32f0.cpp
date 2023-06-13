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

#include <drv/mcu.h>

#if defined(STM32F0_N)

#include <drv/peripheral.h>
#include <drv/Clock.h>
#include <yss/reg.h>
#include <targets/st/bitfield_stm32f030xx.h>

static uint32_t gHseFreq __attribute__((section(".non_init")));

static const int16_t gPpreDiv[8] = {1, 1, 1, 1, 2, 4, 8, 16};
static const int16_t gHpreDiv[16] = {1, 1, 1, 1, 1, 1, 1, 1, 2, 4, 8, 16, 64, 128, 256, 512};

#if defined(STM32F030xC)
// HSE
#define HSI_FREQ				8000000
#define HSE_MIN_FREQ			1000000
#define HSE_MAX_FREQ			32000000

#define AHB_MAX_FREQ			48000000
#define APB1_MAX_FREQ			48000000
#define APB2_MAX_FREQ			48000000

// Main PLL
#define PLL_IN_MIN_FREQ			1000000
#define PLL_IN_MAX_FREQ			24000000
#define PLL_OUT_MIN_FREQ		16000000
#define PLL_OUT_MAX_FREQ		48000000
#define PLL_SRC_MAX				1
#define PLL_MUL_MAX				14
#define PLL_XTPRE_MAX			1

#define I2SPLL_VCO_MIN_FREQ		100000000
#define I2SPLL_VCO_MAX_FREQ		432000000
#define I2SPLL_P_MAX_FREQ		216000000
#define I2SPLL_Q_MAX_FREQ		216000000
#define I2SPLL_R_MAX_FREQ		216000000
#define I2SPLL_N_MIN			50
#define I2SPLL_N_MAX			432
#define I2SPLL_M_MIN			2
#define I2SPLL_M_MAX			63
#define I2SPLL_P_MAX			3
#define I2SPLL_Q_MIN			2
#define I2SPLL_Q_MAX			15
#define I2SPLL_R_MIN			2
#define I2SPLL_R_MAX			7

#define SAIPLL_VCO_MIN_FREQ		100000000
#define SAIPLL_VCO_MAX_FREQ		432000000
#define SAIPLL_P_MAX_FREQ		216000000
#define SAIPLL_Q_MAX_FREQ		216000000
#define SAIPLL_N_MIN			50
#define SAIPLL_N_MAX			432
#define SAIPLL_P_MAX			3
#define SAIPLL_Q_MIN			2
#define SAIPLL_Q_MAX			15
#endif

bool Clock::enableHse(uint32_t hseHz, bool useOsc)
{
	volatile uint32_t* peri = (volatile uint32_t*)RCC;
	
	gHseFreq = hseHz;

	if (hseHz < HSE_MIN_FREQ && HSE_MAX_FREQ < hseHz)
		return false;

	if (useOsc)
		RCC->CR |= RCC_CR_HSEON_Msk | RCC_CR_HSEBYP_Msk;
	else
		RCC->CR |= RCC_CR_HSEON_Msk;

	for (uint32_t i = 0; i < 1000000; i++)
	{
		if (RCC->CR & RCC_CR_HSERDY_Msk)
			return true;
	}

	return false;
}

bool Clock::enableMainPll(uint8_t src, uint8_t xtpre, uint8_t mul)
{
	uint32_t pll;

	using namespace define::clock::sysclk;
	
	// 현재 SysClk 소스가 PLL인이 확인
	if (getFieldData(RCC->CFGR, RCC_CFGR_SWS_Msk, RCC_CFGR_SWS_Pos) == src::PLL)
		goto error;

	if (src > PLL_SRC_MAX)
		goto error;

	if (xtpre > PLL_XTPRE_MAX)
		goto error;

	if (mul > PLL_MUL_MAX)
		goto error;

	if (src == src::HSE)
	{
		// HSE 활성화 확인
		if (getBitData(RCC->CR, RCC_CR_HSERDY_Pos))
			pll = gHseFreq;
		else
			goto error;

		if (xtpre == define::clock::pll::xtpre::DIV2)
			pll >>= 1;
	}
	else
		pll = HSI_FREQ / 2;

	if (pll < PLL_IN_MIN_FREQ || PLL_IN_MAX_FREQ < pll)
		goto error;

	if (mul > PLL_MUL_MAX)
		mul = PLL_MUL_MAX;

	pll *= (mul + 2);
	if (pll < PLL_OUT_MIN_FREQ || PLL_OUT_MAX_FREQ < pll)
		goto error;
		
	// PLL Factor 설정
	setFieldData(RCC->CFGR, RCC_CFGR_PLLMUL_Msk, mul, RCC_CFGR_PLLMUL_Pos);
	setBitData(RCC->CFGR, xtpre, RCC_CFGR_PLLXTPRE_Pos);
	setBitData(RCC->CFGR, src, RCC_CFGR_PLLSRC_Pos);

	// PLL 활성화
	setBitData(RCC->CR, true, RCC_CR_PLLON_Pos);
	for (uint16_t i = 0; i < 10000; i++)
	{
		// PLL 활성화 확인
		if (getBitData(RCC->CR, RCC_CR_PLLRDY_Pos))
			return true;
	}

error:
	return false;
}

bool Clock::setSysclk(uint8_t sysclkSrc, uint8_t ahb, uint8_t apb1, uint8_t apb2, uint8_t vcc)
{
	volatile uint32_t* peri = (volatile uint32_t*)RCC;
	uint32_t clk, ahbClk, apb1Clk, apb2Clk, adcClk;
	uint8_t buf;

	using namespace define::clock::sysclk::src;
	switch (sysclkSrc)
	{
	case HSI:
		clk = HSI_FREQ;
		break;
	case HSE:
		// HSE 활성화 점검
		if (getBitData(RCC->CR, RCC_CR_HSERDY_Pos) == false)
			return false;
		clk = gHseFreq;
		break;
	case PLL:
		// PLL 활성화 점검
		if (getBitData(RCC->CR, RCC_CR_PLLRDY_Pos) == false)
			return false;
		clk = getMainPllFrequency();
		break;
	default:
		return false;
	}

	ahbClk = clk / gHpreDiv[ahb];
	if (ahbClk > AHB_MAX_FREQ)
		return false;

	apb1Clk = ahbClk / gPpreDiv[apb1];
	if (apb1Clk > APB1_MAX_FREQ)
		return false;

	//adcClk = apb1Clk / (ec::clock::adc::MAX_FREQ / 1000);
	//if (adcClk >= 1000)
	//{
	//	adcClk /= 1000;
	//	adcClk += 1;
	//	buf = adcClk / 2;
		
	//	// ADC 프리스케일러 설정
	//	setFieldData(RCC->CFGR, RCC_CFGR2_PREDIV_Msk, buf, RCC_CFGR2_PREDIV_Pos);
	//}
	
	// 버스 클럭 프리스케일러 설정
	setTwoFieldData(RCC->CFGR, RCC_CFGR_PPRE_Msk, apb1, RCC_CFGR_PPRE_Pos, RCC_CFGR_HPRE_Msk, ahb, RCC_CFGR_HPRE_Pos);
	
	// 클럭 소스 변경
	setFieldData(RCC->CFGR, RCC_CFGR_SW_Msk, sysclkSrc, RCC_CFGR_SW_Pos);

	return true;
}

uint32_t Clock::getMainPllFrequency(void)
{
	return gHseFreq / (((RCC->CFGR & RCC_CFGR_PLLXTPRE_Msk) >> RCC_CFGR_PLLXTPRE_Pos) + 1) * (((RCC->CFGR & RCC_CFGR_PLLMUL_Msk) >> RCC_CFGR_PLLMUL_Pos) + 2); 
}

uint32_t Clock::getSystemClockFrequency(void)
{
	uint32_t clk;

	using namespace define::clock::sysclk;
	switch((RCC->CFGR & RCC_CFGR_SWS_Msk) >> RCC_CFGR_SWS_Pos)
	{
	case src::HSI :
		return HSI_FREQ;
		break;
	
	case src::HSE :
		return gHseFreq;
		break;
	
	case src::PLL :
		return getMainPllFrequency();
		break;

	default :
		return 0;
	}
}

uint32_t Clock::getCoreClockFrequency(void)
{
	return getSystemClockFrequency() / gHpreDiv[((RCC->CFGR & RCC_CFGR_HPRE_Msk) >> RCC_CFGR_HPRE_Pos)];
}

uint32_t Clock::getAhbClockFrequency(void)
{
	return getCoreClockFrequency();
}

uint32_t Clock::getApb1ClockFrequency(void)
{
	return getSystemClockFrequency() / gPpreDiv[((RCC->CFGR & RCC_CFGR_PPRE_Msk) >> RCC_CFGR_PPRE_Pos)];
}

uint32_t Clock::getApb2ClockFrequency(void)
{
	return getSystemClockFrequency() / gPpreDiv[((RCC->CFGR & RCC_CFGR_PPRE_Msk) >> RCC_CFGR_PPRE_Pos)];
}

void Clock::enableAhb1Clock(uint32_t position, bool en)
{
	setBitData(RCC->AHBENR, en, position);
}

void Clock::enableApb1Clock(uint32_t position, bool en)
{
	volatile uint32_t* peri = (volatile uint32_t*)RCC;
	setBitData(RCC->APB1ENR, en, position);
}

void Clock::enableApb2Clock(uint32_t position, bool en)
{
	volatile uint32_t* peri = (volatile uint32_t*)RCC;
	setBitData(RCC->APB2ENR, en, position);
}

void Clock::resetAhb1(uint32_t position)
{
	setBitData(RCC->AHBRSTR, true, position);
	setBitData(RCC->AHBRSTR, false, position);
}

void Clock::resetApb1(uint32_t position)
{
	setBitData(RCC->APB1RSTR, true, position);
	setBitData(RCC->APB1RSTR, false, position);
}

void Clock::resetApb2(uint32_t position)
{
	setBitData(RCC->APB2RSTR, true, position);
	setBitData(RCC->APB2RSTR, false, position);
}

#endif

