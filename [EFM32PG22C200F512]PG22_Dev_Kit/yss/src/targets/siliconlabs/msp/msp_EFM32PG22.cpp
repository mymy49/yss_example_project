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

#include <drv/peripheral.h>

#if defined(EFM32PG22)

#include <config.h>
#include <yss/reg.h>
#include <drv/peripheral.h>
#include <yss/instance.h>

void __WEAK initSystem(void)
{
	clock.initialize();
	
	// DCDC 컨버터를 사용하도록 회로가 구성된 경우 사용
	clock.enableApb0Clock(_CMU_CLKEN0_DCDC_SHIFT, true);
	DCDC->EN_SET = _DCDC_EN_EN_MASK;
	DCDC->CTRL_SET = _DCDC_CTRL_MODE_DCDCREGULATION;

	// 외부 크리스탈 활성화
	Clock::HfxoConfig hfxoConfig = 
	{
		11000,			//uint16_t capacitorValue;	// fF(팸토패럿) 단위
		160,			//uint16_t biasCurrent;		// uA 단위
		HSE_CLOCK_FREQ	//uint32_t frequency;		// Hz 단위
	};
	clock.enableApb0Clock(_CMU_CLKEN0_HFXO0_SHIFT, true);
	clock.enableHfxo(hfxoConfig);

	// DPLL 활성화
	clock.enableDpll(Clock::DPLLREF::HFXO, 399, 199); 
}

void initDma(void)
{
}

extern "C"
{
void __WEAK SystemCoreClockUpdate(void)
{

}
}

#endif