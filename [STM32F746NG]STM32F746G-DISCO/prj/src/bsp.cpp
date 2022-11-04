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

#include <yss/debug.h>
#include <yss.h>
#include <bsp.h>
#include <mod/sdram/MT48LC4M32B2B5_6A.h>

FunctionQueue functionQueue(16);
CommandLineInterface cli(uart1);

void initBoard(void)
{
	// USART2 초기화
	using namespace define::gpio;
	gpioA.setAsAltFunc(9, altfunc::PA9_USART1_TX);
	gpioB.setAsAltFunc(7, altfunc::PB7_USART1_RX);
	
	uart1.enableClock();
	uart1.init(9600, 256);
	uart1.enableInterrupt();

	// ADC 초기화
	gpioA.setAsAnalog(0);
	gpioF.setAsAnalog(10);
	gpioF.setAsAnalog(9);

	adc1.enableClock();
	adc1.init();
	
	using namespace define::adc;
	adc1.add(0, lpfLv::LV10, bit::BIT16);
	adc1.add(8, lpfLv::LV10, bit::BIT16);
	adc1.add(7, lpfLv::LV10, bit::BIT16);

	adc1.enableInterrupt();

	// LED 초기화
	led::init();
}

void initSdram(void)
{
	using namespace define::gpio::altfunc;

	Gpio::AltFunc sdramPort[38]{
		{GPIOF, 0, PF0_FMC_A0},
		{GPIOF, 1, PF1_FMC_A1},
		{GPIOF, 2, PF2_FMC_A2},
		{GPIOF, 3, PF3_FMC_A3},
		{GPIOF, 4, PF4_FMC_A4},
		{GPIOF, 5, PF5_FMC_A5},
		{GPIOF, 12, PF12_FMC_A6},
		{GPIOF, 13, PF13_FMC_A7},
		{GPIOF, 14, PF14_FMC_A8},
		{GPIOF, 15, PF15_FMC_A9},
		{GPIOG, 0, PG0_FMC_A10},
		{GPIOG, 1, PG1_FMC_A11},
		{GPIOG, 4, PG4_FMC_BA0},
		{GPIOG, 5, PG5_FMC_BA1},
		{GPIOD, 14, PD14_FMC_D0},
		{GPIOD, 15, PD15_FMC_D1},
		{GPIOD, 0, PD0_FMC_D2},
		{GPIOD, 1, PD1_FMC_D3},
		{GPIOE, 7, PE7_FMC_D4},
		{GPIOE, 8, PE8_FMC_D5},
		{GPIOE, 9, PE9_FMC_D6},
		{GPIOE, 10, PE10_FMC_D7},
		{GPIOE, 11, PE11_FMC_D8},
		{GPIOE, 12, PE12_FMC_D9},
		{GPIOE, 13, PE13_FMC_D10},
		{GPIOE, 14, PE14_FMC_D11},
		{GPIOE, 15, PE15_FMC_D12},
		{GPIOD, 8, PD8_FMC_D13},
		{GPIOD, 9, PD9_FMC_D14},
		{GPIOD, 10, PD10_FMC_D15},
		{GPIOE, 0, PE0_FMC_NBL0},
		{GPIOE, 1, PE1_FMC_NBL1},
		{GPIOG, 8, PG8_FMC_SDCLK},
		{GPIOH, 5, PH5_FMC_SDNWE},
		{GPIOF, 11, PF11_FMC_SDNRAS},
		{GPIOG, 15, PG15_FMC_SDNCAS},
		{GPIOC, 3, PC3_FMC_SDCKE0},
		{GPIOH, 3, PH3_FMC_SDNE0}
	};

	gpioA.setPackageAsAltFunc(sdramPort, 38, define::gpio::ospeed::FAST, define::gpio::otype::PUSH_PULL);

	clock.enableSdram();
	sdram.init(define::sdram::bank::BANK1, MT48LC4M32B2B5_6A);
}

