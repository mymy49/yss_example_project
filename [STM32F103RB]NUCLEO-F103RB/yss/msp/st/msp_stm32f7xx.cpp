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

#include <drv/mcu.h>

#if defined(STM32F7)

#include <config.h>
#include <yss/instance.h>
#include <yss/reg.h>

void __WEAK initSystem(void)
{
	// 외부 클럭 활성화
	clock.enableHse(HSE_CLOCK_FREQ);

	// 내부 32kHz 소스 활성화
	clock.enableLsi();

	// USB 클럭 소스 선택
	clock.setUsbClockSource(define::clock::usbclk::src::MAIN_PLL);

	// Main PLL 클럭 설정
	// inputVCO = inputClock / m;	1~2 MHz를 만들어야 함
	// VCO = inputVCO * n;			192~432 MHz를 만들어야 함
	// P(PLLCLK) = VCO / pDiv;		180 MHz를 넘어선 안됨
	// Q(PLL48CK) = VCO / qDiv;		적정 클럭은 48 MHz, 75MHz를 넘어선 안됨
	// R은 사용 안함

	// SAI PLL 클럭 설정
	// inputVCO = inputClock / m;	Main PLL에서 설정된 값을 그대로 적용 받음
	// VCO = inputVCO * n;			192~432 MHz를 만들어야 함
	// P는 사용 안함
	// Q(PLLSAICLK) = VCO / qDiv;	45 MHz를 넘어선 안됨
	// R(PLLLCDCLK) = VCO / rDiv;	42 MHz를 넘어선 안됨

	using namespace define::clock;

	// Main PLL 설정
	clock.enableMainPll(
		pll::src::HSE,				// uint8_t src
		HSE_CLOCK_FREQ / 1000000,	// uint8_t m
		432,						// uint16_t n
		pll::pdiv::DIV2,			// uint8_t pDiv
		pll::qdiv::DIV9,			// uint8_t qDiv
		0							// uint8_t rDiv
	);
	
	// SAI PLL 설정
	clock.enableSaiPll(
		192,                 // uint32_t n
		saipll::pdiv::DIV4,  // uint8_t pDiv
		saipll::qdiv::DIV15, // uint8_t qDiv
		saipll::rdiv::DIV7   // uint8_t rDiv
	);
	
	// 시스템 클럭 설정
	clock.setSysclk(
		define::clock::sysclk::src::PLL,       // uint8_t sysclkSrc;
		define::clock::divFactor::ahb::NO_DIV, // uint8_t ahb;
		define::clock::divFactor::apb::DIV4,   // uint8_t apb1;
		define::clock::divFactor::apb::DIV2,   // uint8_t apb2;
		33                                     // uint8_t vcc
	);
	
	// 명령어 캐쉬 활성화
	SCB_EnableICache();
	
	// Flash Prefetch, ART Accelerator 활성화
	FLASH->ACR |= FLASH_ACR_PRFTEN_Msk | 
					FLASH_ACR_ARTEN_Msk;
	
	// GPIO 클럭 활성화
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN_Msk | 
					RCC_AHB1ENR_GPIOBEN_Msk | 
					RCC_AHB1ENR_GPIOCEN_Msk |
					RCC_AHB1ENR_GPIODEN_Msk | 
					RCC_AHB1ENR_GPIOEEN_Msk | 
					RCC_AHB1ENR_GPIOFEN_Msk | 
					RCC_AHB1ENR_GPIOGEN_Msk | 
					RCC_AHB1ENR_GPIOHEN_Msk | 
					RCC_AHB1ENR_GPIOIEN_Msk | 
					RCC_AHB1ENR_GPIOJEN_Msk | 
					RCC_AHB1ENR_GPIOKEN_Msk;
	
	// SDRAM 주소 Remap
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN_Msk;
	setFieldData(SYSCFG->MEMRMP, 0x3UL << 10, 1, 10);

	// Power Controller 활성화
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
}

extern "C"
{
	void __WEAK SystemCoreClockUpdate(void)
	{

	}
}

#endif
