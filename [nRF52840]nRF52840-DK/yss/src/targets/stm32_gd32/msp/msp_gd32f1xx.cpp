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

#if defined(GD32F1)

#include <config.h>
#include <yss/reg.h>
#include <targets/st_gigadevice/rcc_stm32_gd32f1.h>
#include <targets/st_gigadevice/gpio_stm32_gd32f1.h>

#include <yss/instance.h>

void __WEAK initSystem(void)
{
	// Power Control 장치 활성화
	clock.enableApb1Clock(RCC_APB1ENR_PWREN_Pos);

	// 외부 고속 클럭 활성화
	clock.enableHse(HSE_CLOCK_FREQ);

	using namespace define::clock;

	// 주 PLL 활성화
	// pllClock = HSE_CLOCK_FREQ * (mul + 2) / (1 + xtpre);
#if HSE_CLOCK_FREQ == 8000000
	clock.enableMainPll(
		pll::src::HSE,	// uint8_t src;
		0,				// uint8_t xtpre;
		11				// uint8_t mul;
	);
#define PLL_ENABLED
# elif HSE_CLOCK_FREQ == 12000000
	clock.enableMainPll(
		pll::src::HSE,	// uint8_t src;
		0,				// uint8_t xtpre;
		7				// uint8_t mul;
	); 
#define PLL_ENABLED
#endif

#if defined(PLL_ENABLED)
	// 시스템 클럭 설정
	clock.setSysclk(
		sysclk::src::PLL,		// uint8_t sysclkSrc;
		divFactor::ahb::NO_DIV, // uint8_t ahb;
		divFactor::apb::DIV2,	// uint8_t apb1;
		divFactor::apb::NO_DIV	// uint8_t apb2;
	);
#endif

	// GPIO 활성화
	clock.enableApb2Clock(RCC_APB2ENR_AFIOEN_Pos);
	clock.enableApb2Clock(RCC_APB2ENR_IOPAEN_Pos);
	clock.enableApb2Clock(RCC_APB2ENR_IOPBEN_Pos);
	clock.enableApb2Clock(RCC_APB2ENR_IOPCEN_Pos);
	clock.enableApb2Clock(RCC_APB2ENR_IOPDEN_Pos);
	clock.enableApb2Clock(RCC_APB2ENR_IOPEEN_Pos);
	clock.enableApb2Clock(RCC_APB2ENR_IOPFEN_Pos);
	clock.enableApb2Clock(RCC_APB2ENR_IOPGEN_Pos);

	// SWD 단자 외의 JTAG단자는 일반 포트로 전환
	AFIO[GPIO_REG::MAPR] |= AFIO_MAPR_SWJ_CFG_NOJNTRST;
}

void __WEAK initDma(void)
{
	// DMA1
	dmaChannel1.enableClock();
	dmaChannel1.init();
	dmaChannel1.enableInterrupt();
	dmaChannel2.init();
	dmaChannel2.enableInterrupt();
	dmaChannel3.init();
	dmaChannel3.enableInterrupt();
	dmaChannel4.init();
	dmaChannel4.enableInterrupt();
	dmaChannel5.init();
	dmaChannel5.enableInterrupt();
	dmaChannel6.init();
	dmaChannel6.enableInterrupt();
	dmaChannel7.init();
	dmaChannel7.enableInterrupt();

#if defined(DMA2)
	// DMA2
	dmaChannel8.enableClock();
	dmaChannel8.init();
	dmaChannel8.enableInterrupt();
	dmaChannel9.init();
	dmaChannel9.enableInterrupt();
	dmaChannel10.init();
	dmaChannel10.enableInterrupt();
	dmaChannel11.init();
	dmaChannel11.enableInterrupt();
	dmaChannel12.init();
	dmaChannel12.enableInterrupt();
#endif
}

extern "C"
{
void SystemCoreClockUpdate(void)
{

}
}

#endif