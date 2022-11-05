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
#include <mod/sdram/MT48LC16M16A2_6A.h>

FunctionQueue functionQueue(16);
CommandLineInterface cli(uart2);

void initBoard(void)
{
	// USART2 초기화
	using namespace define::gpio;
	gpioA.setAsAltFunc(2, altfunc::PA2_USART2_TX);
	gpioA.setAsAltFunc(3, altfunc::PA3_USART2_RX);
	
	uart2.enableClock();
	uart2.init(9600, 256);
	uart2.enableInterrupt();

	// ADC 초기화
	//gpioA.setAsAnalog(0);

	//adc1.enableClock();
	//adc1.init();
	
	//using namespace define::adc;
	//adc1.add(0, lpfLv::LV10, bit::BIT16);
	//adc1.add(1, lpfLv::LV10, bit::BIT16);
	//adc1.add(4, lpfLv::LV10, bit::BIT16);

	//adc1.enableInterrupt();

	// LED 초기화
	led::init();
}

void initSdram(void)
{
	using namespace define::gpio::altfunc;

	Gpio::AltFunc sdramPort[39]{
		{(YSS_GPIO_Peri*)GPIOF, 0, PF0_FMC_A0},
		{(YSS_GPIO_Peri*)GPIOF, 1, PF1_FMC_A1},
		{(YSS_GPIO_Peri*)GPIOF, 2, PF2_FMC_A2},
		{(YSS_GPIO_Peri*)GPIOF, 3, PF3_FMC_A3},
		{(YSS_GPIO_Peri*)GPIOF, 4, PF4_FMC_A4},
		{(YSS_GPIO_Peri*)GPIOF, 5, PF5_FMC_A5},
		{(YSS_GPIO_Peri*)GPIOF, 12, PF12_FMC_A6},
		{(YSS_GPIO_Peri*)GPIOF, 13, PF13_FMC_A7},
		{(YSS_GPIO_Peri*)GPIOF, 14, PF14_FMC_A8},
		{(YSS_GPIO_Peri*)GPIOF, 15, PF15_FMC_A9},
		{(YSS_GPIO_Peri*)GPIOG, 0, PG0_FMC_A10},
		{(YSS_GPIO_Peri*)GPIOG, 1, PG1_FMC_A11},
		{(YSS_GPIO_Peri*)GPIOG, 2, PG2_FMC_A12},

		{(YSS_GPIO_Peri*)GPIOG, 4, PG4_FMC_BA0},
		{(YSS_GPIO_Peri*)GPIOG, 5, PG5_FMC_BA1},

		{(YSS_GPIO_Peri*)GPIOD, 14, PD14_FMC_D0},
		{(YSS_GPIO_Peri*)GPIOD, 15, PD15_FMC_D1},
		{(YSS_GPIO_Peri*)GPIOD, 0, PD0_FMC_D2},
		{(YSS_GPIO_Peri*)GPIOD, 1, PD1_FMC_D3},
		{(YSS_GPIO_Peri*)GPIOE, 7, PE7_FMC_D4},
		{(YSS_GPIO_Peri*)GPIOE, 8, PE8_FMC_D5},
		{(YSS_GPIO_Peri*)GPIOE, 9, PE9_FMC_D6},
		{(YSS_GPIO_Peri*)GPIOE, 10, PE10_FMC_D7},
		{(YSS_GPIO_Peri*)GPIOE, 11, PE11_FMC_D8},
		{(YSS_GPIO_Peri*)GPIOE, 12, PE12_FMC_D9},
		{(YSS_GPIO_Peri*)GPIOE, 13, PE13_FMC_D10},
		{(YSS_GPIO_Peri*)GPIOE, 14, PE14_FMC_D11},
		{(YSS_GPIO_Peri*)GPIOE, 15, PE15_FMC_D12},
		{(YSS_GPIO_Peri*)GPIOD, 8, PD8_FMC_D13},
		{(YSS_GPIO_Peri*)GPIOD, 9, PD9_FMC_D14},
		{(YSS_GPIO_Peri*)GPIOD, 10, PD10_FMC_D15},

		{(YSS_GPIO_Peri*)GPIOE, 0, PE0_FMC_NBL0},
		{(YSS_GPIO_Peri*)GPIOE, 1, PE1_FMC_NBL1},
		{(YSS_GPIO_Peri*)GPIOG, 8, PG8_FMC_SDCLK},
		{(YSS_GPIO_Peri*)GPIOC, 0, PC0_FMC_SDNWE},
		{(YSS_GPIO_Peri*)GPIOF, 11, PF11_FMC_SDNRAS},
		{(YSS_GPIO_Peri*)GPIOG, 15, PG15_FMC_SDNCAS},
		{(YSS_GPIO_Peri*)GPIOC, 3, PC3_FMC_SDCKE0},
		{(YSS_GPIO_Peri*)GPIOC, 2, PC2_FMC_SDNE0}
	};

	gpioA.setPackageAsAltFunc(sdramPort, 39, define::gpio::ospeed::FAST, define::gpio::otype::PUSH_PULL);
	
	clock.enableSdram();
	sdram.init(define::sdram::bank::BANK1, MT48LC16M16A2_6A);
}

