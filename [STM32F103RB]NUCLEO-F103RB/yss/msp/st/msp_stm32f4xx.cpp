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

#if defined(STM32F4)

#include <config.h>

#include <yss/instance.h>

void __WEAK initSystem(void)
{
	// 외부 고속 클럭 활성화
	clock.enableHse(HSE_CLOCK_FREQ);

	using namespace define::clock;
	
	// Main PLL 설정
	clock.enableMainPll(
		pll::src::HSE,				// uint8_t src
		HSE_CLOCK_FREQ / 1000000,	// uint8_t m
		360,						// uint16_t n
		pll::pdiv::DIV2,			// uint8_t pDiv
		pll::qdiv::DIV8,			// uint8_t qDiv
		0							// uint8_t rDiv
	);
	
	// SAI PLL 설정
#if defined(SAI1) || defined(SAI)
	clock.enableSaiPll(
		192,                                // uint16_t n
		0,                                  // uint8_t pDiv
		saipll::qdiv::DIV15, // uint8_t qDiv
		saipll::rdiv::DIV7   // uint8_t rDiv
	);
#endif

	clock.setSysclk(
		sysclk::src::PLL,		// uint8_t sysclkSrc;
		divFactor::ahb::NO_DIV, // uint8_t ahb;
		divFactor::apb::DIV4,	// uint8_t apb1;
		divFactor::apb::DIV2,	// uint8_t apb2;
		33						// uint8_t vcc
	);

	// Flash Prefetch, D/I 캐시 활성화
	FLASH->ACR |= FLASH_ACR_DCEN | FLASH_ACR_ICEN | FLASH_ACR_PRFTEN;

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

	// Power Controller 활성화
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
}

#endif