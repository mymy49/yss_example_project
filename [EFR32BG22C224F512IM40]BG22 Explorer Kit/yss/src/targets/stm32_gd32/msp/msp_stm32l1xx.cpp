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

#include <drv/peripheral.h>

#if defined(STM32L1)

#include <config.h>
#include <targets/st_gigadevice/rcc_stm32l1.h>
#include <yss/instance.h>

extern "C"
{
void __WEAK SystemCoreClockUpdate(void)
{

}
}

void __WEAK initializeSystem(void)
{
	using namespace define::clock;

	// Power Control 장치 활성화
	clock.enableApb1Clock(RCC_APB1ENR_PWREN_Pos);

	// Voltage Scale 설정
	// 일부 클럭 관련 설정값은 Voltage Scale을 기준으로 하기 때문에 반드시 가장 먼저 설정이 되야 한다.
	// RANGE1 = 최대 클럭 32MHz
	// RANGE2 = 최대 클럭 16MHz
	// RANGE3 = 최대 클럭 8MHz
	clock.setVoltageScale(scale::RANGE1);
	
	// Flash 메모리 64비트 접근 활성화
	flash.set64bitAccess(true);

	// 외부 고속 클럭 활성화
	clock.enableHse(HSE_CLOCK_FREQ);

	// 주 PLL 활성화
	// pllClock = HSE_CLOCK_FREQ * (mul + 2) / (1 + xtpre);
#if HSE_CLOCK_FREQ == 8000000
	clock.enableMainPll(
		pll::src::HSE,	// uint8_t src;
		pll::div::DIV2,	// uint8_t xtpre;
		pll::mul::MUL8	// uint8_t mul;
	);
#define PLL_ENABLED
#elif HSE_CLOCK_FREQ == 4000000
	clock.enableMainPll(
		pll::src::HSE,	// uint8_t src;
		pll::div::DIV2,	// uint8_t xtpre;
		pll::mul::MUL8	// uint8_t mul;
	);
#define PLL_ENABLED
# elif HSE_CLOCK_FREQ == 12000000
	clock.enableMainPll(
		pll::src::HSE,	// uint8_t src;
		pll::div::DIV3,	// uint8_t xtpre;
		pll::mul::MUL8	// uint8_t mul;
	); 
#define PLL_ENABLED
#endif

#if defined(PLL_ENABLED)
	// 플래시 
#if HSE_CLOCK_FREQ == 4000000
	flash.setLatency(16000000);
#else
	flash.setLatency(32000000);
#endif
	// 시스템 클럭 설정
	clock.setSysclk(
		sysclk::src::PLL,		// uint8_t sysclkSrc;
		divFactor::ahb::NO_DIV, // uint8_t ahb;
		divFactor::apb::DIV2,	// uint8_t apb1;
		divFactor::apb::NO_DIV	// uint8_t apb2;
	);
#endif
	
	//// 플래시 메모리 prefetch 기능 활성화
	//flash.setPrefetchEn(true);
	//flash.setHalfCycleAccessEn(true);
	
	// GPIO 활성화
	clock.enableAhb1Clock(RCC_AHBENR_GPIOAEN_Pos);
	clock.enableAhb1Clock(RCC_AHBENR_GPIOBEN_Pos);
	clock.enableAhb1Clock(RCC_AHBENR_GPIOCEN_Pos);
	clock.enableAhb1Clock(RCC_AHBENR_GPIODEN_Pos);
	clock.enableAhb1Clock(RCC_AHBENR_GPIOEEN_Pos);
	clock.enableAhb1Clock(RCC_AHBENR_GPIOFEN_Pos);
	clock.enableAhb1Clock(RCC_AHBENR_GPIOGEN_Pos);
	clock.enableAhb1Clock(RCC_AHBENR_GPIOHEN_Pos);
	
	//// SWD 단자 외의 JTAG단자는 일반 포트로 전환
	//AFIO[GPIO_REG::MAPR] |= AFIO_MAPR_SWJ_CFG_NOJNTRST;
}

void __WEAK initDma(void)
{
//	// DMA1
//	dmaChannel1.enableClock();
//	dmaChannel1.init();
//	dmaChannel1.enableInterrupt();
//	dmaChannel2.init();
//	dmaChannel2.enableInterrupt();
//	dmaChannel3.init();
//	dmaChannel3.enableInterrupt();
//	dmaChannel4.init();
//	dmaChannel4.enableInterrupt();
//	dmaChannel5.init();
//	dmaChannel5.enableInterrupt();
//	dmaChannel6.init();
//	dmaChannel6.enableInterrupt();
//	dmaChannel7.init();
//	dmaChannel7.enableInterrupt();

//#if defined(DMA2)
//	// DMA2
//	dmaChannel8.enableClock();
//	dmaChannel8.init();
//	dmaChannel8.enableInterrupt();
//	dmaChannel9.init();
//	dmaChannel9.enableInterrupt();
//	dmaChannel10.init();
//	dmaChannel10.enableInterrupt();
//	dmaChannel11.init();
//	dmaChannel11.enableInterrupt();
//	dmaChannel12.init();
//	dmaChannel12.enableInterrupt();
//#endif
}

#endif
