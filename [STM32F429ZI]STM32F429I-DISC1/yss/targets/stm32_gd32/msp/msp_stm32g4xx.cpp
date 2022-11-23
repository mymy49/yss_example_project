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

#if defined(STM32G431xx) || defined(STM32G441xx) || \
	defined(STM32G471xx) || defined(STM32G473xx) || defined(STM32G474xx) || defined(STM32G483xx) || defined(STM32G484xx) || defined(STM32GBK1CB)

#include <config.h>

#if YSS_USE_DEFAULT_MSP == true

#include <yss/instance.h>

void __WEAK initSystem(void)
{
	clock.setVoltageScale(define::clock::voltageScale::RANGE1_NORMAL);
	clock.enableHse(HSE_CLOCK_FREQ);

	using namespace define::clock;

#if USE_LSI_CLOCK == true
	clock.enableLsi();
#endif

	clock.pll.enable(
		pll::src::HSE,    // uint8_t src
		288,              // uint32_t vcoMhz
		pll::pdiv::DIV31, // uint8_t pDiv
		pll::qdiv::DIV8,  // uint8_t qDiv
		pll::rdiv::DIV2   // uint8_t rDiv
	);

	clock.pll.setPEn(true);
	clock.pll.setQEn(true);
	clock.pll.setREn(true);
	clock.peripheral.setAdc12ClkSrc(adc::src::PLL);
	clock.peripheral.setCan1ClkSrc(can::src::PCLK1);
	clock.peripheral.setI2c1ClkSrc(i2c::src::HSI16);
	clock.peripheral.setI2c2ClkSrc(i2c::src::HSI16);
	clock.peripheral.setI2c3ClkSrc(i2c::src::HSI16);

	clock.setSysclk(
		sysclk::src::PLL,       // uint8_t sysclkSrc;
		divFactor::ahb::NO_DIV, // uint8_t ahb;
		divFactor::apb::DIV4,   // uint8_t apb1;
		divFactor::apb::DIV2,   // uint8_t apb2;
		33                      // uint8_t vcc
	);

	flash.setPrefetchEn(true);
	flash.setDCacheEn(true);
	flash.setICacheEn(true);

	clock.peripheral.setGpioAEn(true);
	clock.peripheral.setGpioBEn(true);
	clock.peripheral.setGpioCEn(true);
	clock.peripheral.setGpioDEn(true);
	clock.peripheral.setGpioEEn(true);
	clock.peripheral.setGpioFEn(true);
	clock.peripheral.setGpioGEn(true);
}

#endif

#endif