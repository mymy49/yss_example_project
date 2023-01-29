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
#include <drv/Clock.h>
#include <yss/reg.h>

void Clock::initialize(void)
{
	mHfxoFrequency = 0;
}

error Clock::enableHfxo(HfxoConfig config, bool en)
{
	mHfxoFrequency = config.frequency;
	setThreeFieldData(HFXO0->XTALCTRL,	_HFXO_XTALCTRL_CTUNEXIANA_MASK, config.capacitorValue / 80, _HFXO_XTALCTRL_CTUNEXIANA_SHIFT,
										_HFXO_XTALCTRL_CTUNEXOANA_MASK,	config.capacitorValue / 80, _HFXO_XTALCTRL_CTUNEXOANA_SHIFT,
										_HFXO_XTALCTRL_COREBIASANA_MASK, config.biasCurrent / 10, _HFXO_XTALCTRL_COREBIASANA_SHIFT);

	setBitData(HFXO0->CTRL, en, _HFXO_CTRL_FORCEEN_SHIFT);
	
	for(uint32_t i=0;i<1000000;i++)
	{
		if(getBitData(HFXO0->STATUS, _HFXO_STATUS_RDY_SHIFT))
			return Error::NONE;
	}
	
	return Error::TIMEOUT;
}

void Clock::enableApb0Clock(uint32_t position, bool en)
{
	setBitData(CMU->CLKEN0, en, position);
}

void Clock::enableApb1Clock(uint32_t position, bool en)
{
	setBitData(CMU->CLKEN1, en, position);
}

error Clock::enableDpll(uint8_t dpllref, uint16_t n, uint16_t m)
{
	uint64_t clk;
	
	switch(dpllref)
	{
	case DPLLREF::DISABLED :
		return Error::WRONG_CONFIG;
	
	case DPLLREF::HFXO :
		if(!getBitData(HFXO0->STATUS, _HFXO_STATUS_RDY_SHIFT))
			return Error::HSE_NOT_READY;

		clk = mHfxoFrequency;
		break;
	
	case DPLLREF::LFXO_ :
		// 현재 이 설정은 지원 안됨
		return Error::WRONG_CONFIG;
	
	case DPLLREF::CLKIN0 :
		// 현재 이 설정은 지원 안됨
		return Error::WRONG_CONFIG;
	}

	if(n <= 300 || n >= 4096 || m >= 4096)
		return Error::WRONG_CONFIG;

	clk = clk * (n + 1) / (m + 1);
	switch(getFieldData(EMU->STATUS, _EMU_STATUS_VSCALE_MASK, _EMU_STATUS_VSCALE_SHIFT))
	{
	case 0 : // VSCALE0
		// 현재 이 설정은 지원 안됨
		return Error::WRONG_CONFIG;
	
	case 1 : // VSCALE1
		if(clk > 40000000)
			return Error::WRONG_CLOCK_FREQUENCY;
	
	case 2 : // VSCALE2		
		if(clk > 76800000)
			return Error::WRONG_CLOCK_FREQUENCY;
	}
	
	setFieldData(CMU->DPLLREFCLKCTRL, _CMU_DPLLREFCLKCTRL_MASK, dpllref, _CMU_DPLLREFCLKCTRL_CLKSEL_SHIFT);
//	DPLL0->CFG = 1;
	setTwoFieldData(DPLL0->CFG1, _DPLL_CFG1_N_MASK, n, _DPLL_CFG1_N_SHIFT, _DPLL_CFG1_M_MASK, m, _DPLL_CFG1_M_SHIFT);	
	DPLL0->EN_SET = _DPLL_EN_EN_MASK;

	return Error::NONE;
}

#endif