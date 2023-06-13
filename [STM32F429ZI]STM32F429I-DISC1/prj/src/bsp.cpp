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
#include <mod/sdram/IS42S16400J_7TL.h>
#include <mod/rgb_tft_lcd/SF_TC240T_9370_T.h>
#include <mod/rtouch/STMPE811.h>
#include <targets/st_gigadevice/rcc_stm32_gd32f4_f7.h>
#include <yss/event.h>

void initLcd(void);

FunctionQueue functionQueue(16);
CommandLineInterface cli(uart1);
SF_TC240T_9370_T lcd;
//STMPE811 touch;

void initBoard(void)
{
	// USART1 초기화
	using namespace define::gpio;
	gpioA.setAsAltFunc(9, altfunc::PA9_USART1_TX);
	gpioA.setAsAltFunc(10, altfunc::PA10_USART1_RX);
	
	uart1.enableClock();
	uart1.initialize(9600, 256);
	uart1.enableInterrupt();

	// ADC 초기화
	gpioA.setAsAnalog(5);

	adc1.enableClock();
	adc1.initialize();
	
	using namespace define::adc;
	adc1.add(5, lpfLv::LV10, bit::BIT16);
	adc1.enableInterrupt();

	// SPI5 초기화
	gpioF.setAsAltFunc(7, altfunc::PF7_SPI5_SCK, ospeed::FAST, otype::PUSH_PULL); // SCK
	gpioF.setAsAltFunc(9, altfunc::PF9_SPI5_MOSI, ospeed::FAST, otype::PUSH_PULL); // MOSI

	spi5.enableClock();
	spi5.initializeAsMain();
	spi5.enableInterrupt();
	
	// TFT LCD 초기화
	initLcd();

	// LED 초기화
	led::init();

	// I2C 초기화
	gpioA.setAsAltFunc(8, altfunc::PA8_I2C3_SCL, ospeed::MID, otype::OPEN_DRAIN);
	gpioC.setAsAltFunc(9, altfunc::PC9_I2C3_SDA, ospeed::MID, otype::OPEN_DRAIN);

	i2c3.enableClock();
	i2c3.initializeAsMain(define::i2c::speed::STANDARD);
	i2c3.enableInterrupt();

	const STMPE811::Config touchConfig =
	{
		i2c3,			// I2c &peri
		{&gpioA, 15}	// Gpio::Pin isrPin
	};

	//touch.init(touchConfig);
	//event::setPointerDevice(touch);
//	touch.setCalibration(3440, 690, 500, 3650);
//	touch.setSize(240, 320);
}

void initLcd(void)
{
	using namespace define::gpio;
	using namespace define::gpio::altfunc;

	Gpio::AltFunc lcdPort[20]{
		{GPIOB, 0, PB0_LCD_R3},
		{GPIOA, 11, PA11_LCD_R4},
		{GPIOA, 12, PA12_LCD_R5},
		{GPIOB, 1, PB1_LCD_R6},
		{GPIOG, 6, PG6_LCD_R7},

		{GPIOA, 6, PA6_LCD_G2},
		{GPIOG, 10, PG10_LCD_G3},
		{GPIOB, 10, PB10_LCD_G4},
		{GPIOB, 11, PB11_LCD_G5},
		{GPIOC, 7, PC7_LCD_G6},
		{GPIOD, 3, PD3_LCD_G7},

		{GPIOG, 11, PG11_LCD_B3},
		{GPIOG, 12, PG12_LCD_B4},
		{GPIOA, 3, PA3_LCD_B5},
		{GPIOB, 8, PB8_LCD_B6},
		{GPIOB, 9, PB9_LCD_B7},

		{GPIOA, 4, PA4_LCD_VSYNC},
		{GPIOC, 6, PC6_LCD_HSYNC},
		{GPIOF, 10, PF10_LCD_DE},
		{GPIOG, 7, PG7_LCD_CLK}
	};

	gpioA.setPackageAsAltFunc(lcdPort, 20, define::gpio::ospeed::FAST, define::gpio::otype::PUSH_PULL);

	gpioC.setAsOutput(2, ospeed::FAST, otype::PUSH_PULL);  // CS
	gpioD.setAsOutput(13, ospeed::FAST, otype::PUSH_PULL); // DCX

	Gpio::Pin lcdCs = {&gpioC, 2};
	Gpio::Pin lcdDcx = {&gpioD, 13};
	
	SF_TC240T_9370_T::Config config = 
	{
		spi5,			//Spi &peri;
		{&gpioC, 2},	//Gpio::Pin chipSelect;
		{&gpioD, 13},	//Gpio::Pin dataCommand;
		{0, 0}			//Gpio::Pin reset;
	};

	lcd.setConfig(config);
	lcd.initialize();
	setSystemTftLcd(lcd);

	ltdc.enableClock();
	ltdc.initialize(lcd.getSpec());
	ltdc.enableInterrupt();

	// DMA2D 초기화
	dma2d.enableClock();
	dma2d.initialize();
	dma2d.enableInterrupt();
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
		{GPIOC, 0, PC0_FMC_SDNWE},
		{GPIOF, 11, PF11_FMC_SDNRAS},
		{GPIOG, 15, PG15_FMC_SDNCAS},
		{GPIOB, 5, PB5_FMC_SDCKE1},
		{GPIOB, 6, PB6_FMC_SDNE1}
	};

	gpioA.setPackageAsAltFunc(sdramPort, 38, define::gpio::ospeed::FAST, define::gpio::otype::PUSH_PULL);

	clock.enableSdram();
	sdram.initialize(define::sdram::bank::BANK2, IS42S16400J_7TL, 180000000);
}

void initSystem(void)
{
	// Power Controller 클럭 활성화
	clock.enableApb1Clock(RCC_APB1ENR_PWREN_Pos);

	// SYSCFG 클럭 활성화
	clock.enableApb2Clock(RCC_APB2ENR_SYSCFGEN_Pos);

	// 외부 고속 클럭 활성화
	clock.enableHse(HSE_CLOCK_FREQ);

	using namespace define::clock;
	
	// Main PLL 설정
	clock.enableMainPll(
		pll::src::HSE,				// uint8_t src
		HSE_CLOCK_FREQ / 1000000,	// uint8_t m
#if defined(STM32F411xE)
		192,						// uint16_t n Sysclk
		pll::pdiv::DIV2,			// uint8_t pDiv
		pll::qdiv::DIV4,			// uint8_t qDiv
#elif defined(STM32F429xx)
		360,						// uint16_t n Sysclk
		pll::pdiv::DIV2,			// uint8_t pDiv
		pll::qdiv::DIV6,			// uint8_t qDiv
#endif
		0
	);
	
	clock.setLtdcDivisionFactor(divisionFactor::ltdc::DIV4);

	// SAI PLL 설정
#if defined(STM32F429xx)
	clock.enableSaiPll(
		192,				// uint16_t n
		0,					// uint8_t pDiv <- 사용되지 않음
		pll::qdiv::DIV15,	// uint8_t qDiv SAI Clock
		pll::rdiv::DIV7		// uint8_t rDiv TFT-LCD Clock
	);
#endif

#if defined(STM32F411xE)
	flash.setLatency(96000000, 33);
#elif defined(STM32F429xx)
	flash.setLatency(180000000, 33);
#endif
	clock.setSysclk(
		sysclk::src::PLL,				// uint8_t sysclkSrc;
		divisionFactor::ahb::NO_DIV,	// uint8_t ahb;
		divisionFactor::apb::DIV4,		// uint8_t apb1;
		divisionFactor::apb::DIV2,		// uint8_t apb2;
		33								// uint8_t vcc
	);
	
	// Flash Prefetch, D/I 캐시 활성화
	flash.enableDataCache();
	flash.enableInstructionCache();
	flash.enablePrefetch();
	
	// GPIO 클럭 활성화
	clock.enableAhb1Clock(RCC_AHB1ENR_GPIOAEN_Pos);
	clock.enableAhb1Clock(RCC_AHB1ENR_GPIOBEN_Pos);
	clock.enableAhb1Clock(RCC_AHB1ENR_GPIOCEN_Pos);
	clock.enableAhb1Clock(RCC_AHB1ENR_GPIODEN_Pos);
	clock.enableAhb1Clock(RCC_AHB1ENR_GPIOEEN_Pos);
	clock.enableAhb1Clock(RCC_AHB1ENR_GPIOFEN_Pos);
	clock.enableAhb1Clock(RCC_AHB1ENR_GPIOGEN_Pos);
	clock.enableAhb1Clock(RCC_AHB1ENR_GPIOHEN_Pos);
	clock.enableAhb1Clock(RCC_AHB1ENR_GPIOIEN_Pos);
	clock.enableAhb1Clock(RCC_AHB1ENR_GPIOJEN_Pos);
	clock.enableAhb1Clock(RCC_AHB1ENR_GPIOKEN_Pos);
}

