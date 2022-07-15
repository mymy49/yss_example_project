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

#include <__cross_studio_io.h>
#include <yss/yss.h>
#include <yss/Fat32.h>
#include <bsp.h>
#include <yss/reg.h>
#include <gui/Bmp888Brush.h>

CAT24C256 eeprom;
MSP4021 lcd;
Bmp888Brush gBrush(10000);

void initBoard(void)
{
	// PWM3 설정
	using namespace define::gpio;
	gpioB.setAsAltFunc(4, altfunc::PB4_TIM3_CH1);
	pwm3Ch1.setClockEn(true);
	pwm3Ch1.init(10000);
	pwm3Ch1.start();
	pwm3Ch1.setRatio(0.f);

	// ADC1 설정
	using namespace define::adc;
	gpioA.setAsAnalog(0);

	adc1.setClockEn(true);
	adc1.init();
	adc1.add(0, lpfLv::LV9, bit::BIT16);
	adc1.setInterruptEn(true);
	
	// SPI1 설정
	using namespace define::gpio;
	gpioA.setAsAltFunc(5, altfunc::PA5_SPI1_SCK, ospeed::MID);
	gpioA.setAsAltFunc(6, altfunc::PA6_SPI1_MISO, ospeed::MID);
	gpioA.setAsAltFunc(7, altfunc::PA7_SPI1_MOSI, ospeed::MID);
	
	spi1.setClockEn(true);
	spi1.init();
	spi1.setInterruptEn(true);
	
	// I2C1 설정
	using namespace define::gpio;

	gpioB.setAsAltFunc(6, altfunc::PB6_I2C1_SCL, ospeed::MID, otype::OPEN_DRAIN);
	gpioB.setAsAltFunc(7, altfunc::PB7_I2C1_SDA, ospeed::MID, otype::OPEN_DRAIN);

	i2c1.setClockEn(true);
	i2c1.init(define::i2c::speed::STANDARD);
	i2c1.setInterruptEn(true);

	// SPI2 설정
	using namespace define::gpio;
	gpioB.setAsAltFunc(13, altfunc::PB13_SPI2_SCK, ospeed::MID);
	gpioB.setAsAltFunc(14, altfunc::PB14_SPI2_MISO, ospeed::MID);
	gpioB.setAsAltFunc(15, altfunc::PB15_SPI2_MOSI, ospeed::MID);
	
	spi2.setClockEn(true);
	spi2.init();
	spi2.setInterruptEn(true);
	
	// SDIO 설정
	using namespace define::gpio;
	gpioC.setAsAltFunc(8, altfunc::PC8_SDIO_D0, ospeed::FAST);
	gpioC.setAsAltFunc(9, altfunc::PC9_SDIO_D1, ospeed::FAST);
	gpioC.setAsAltFunc(10, altfunc::PC10_SDIO_D2, ospeed::FAST);
	gpioC.setAsAltFunc(11, altfunc::PC11_SDIO_D3, ospeed::FAST);
	gpioC.setAsAltFunc(12, altfunc::PC12_SDIO_CK, ospeed::FAST);
	gpioD.setAsAltFunc(2, altfunc::PD2_SDIO_CMD, ospeed::FAST);
	
	gpioA.setAsInput(8);

	sdmmc.setClockEn(true);
	sdmmc.init();
	sdmmc.setInterruptEn(true);

	// LCD 설정
	using namespace define::gpio;
	gpioA.setAsOutput(1, ospeed::MID);
	gpioA.setAsOutput(2, ospeed::MID);
	gpioA.setAsOutput(3, ospeed::MID);

	MSP4021::Config lcdConfig = 
	{
		spi1,			//drv::Spi &peri;
		{&gpioA, 1},	//config::gpio::Set chipSelect;
		{&gpioA, 3},	//config::gpio::Set dataCommand;
		{&gpioA, 2}		//config::gpio::Set reset;
	};

	lcd.setConfig(lcdConfig);
	lcd.MSP4021::ST7796S_SPI::init();
	lcd.setDirection(true, true, true);
	lcd.setBmp888Brush(gBrush);

	// EEPROM 설정
	CAT24C256::Config eepromConfig = 
	{
		i2c1,	//drv::I2c &peri;
		{0, 0},	//config::gpio::Set writeProtectPin;
		0		//unsigned char addr;
	};

	eeprom.init(eepromConfig);

}

void setBackLight(float ratio)
{
	pwm3Ch1.setRatio(ratio);
}

void initSystem(void)
{
	clock.peripheral.setPwrEn(true);
	clock.enableHse(HSE_CLOCK_FREQ, true);

	using namespace define::clock;

	clock.enableMainPll(
		pll::src::HSE,	// unsigned char src;
		0,					// unsigned char xtpre;
		7					// unsigned char mul;
	); 

	clock.setSysclk(
		sysclk::src::PLL,		// unsigned char sysclkSrc;
		divFactor::ahb::NO_DIV, // unsigned char ahb;
		divFactor::apb::DIV2,	// unsigned char apb1;
		divFactor::apb::NO_DIV	// unsigned char apb2;
	);

	clock.peripheral.setGpioAEn(true);
	clock.peripheral.setGpioBEn(true);
	clock.peripheral.setGpioCEn(true);
	clock.peripheral.setGpioDEn(true);
	clock.peripheral.setGpioEEn(true);
	clock.peripheral.setGpioFEn(true);
	clock.peripheral.setGpioGEn(true);
	clock.peripheral.setAfioEn(true);

	setFieldData(AFIO->AFIO_PCFR1, 0x7 << 24, 2, 24);	// JTAG-DP Disabled and SW-DP Enabled
	setFieldData(RCC->GCCR, 0x1F << 3, 0x15, 3);
}
