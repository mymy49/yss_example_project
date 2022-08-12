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
#include <bsp.h>
#include <dev/led.h>

void callback_linkup(bool linkup);

W5100S w5100s;
WiznetSocket socket0;

void initBoard(void)
{
	using namespace define::gpio;

	// RGB LED 초기화
	Led::init();

	// SPI2 초기화
	gpioB.setAsAltFunc(13, altfunc::PB13_SPI2_SCK, ospeed::FAST);
	gpioB.setAsAltFunc(14, altfunc::PB14_SPI2_MISO, ospeed::FAST);
	gpioB.setAsAltFunc(15, altfunc::PB15_SPI2_MOSI, ospeed::FAST);

	spi2.setClockEn(true);
	spi2.init();
	spi2.setInterruptEn(true);
	
	// W5100S 초기화
	gpioD.setAsOutput(7);
	gpioD.setAsOutput(8);
	gpioD.setAsInput(9);

	W5100S::Config w5100sConfig =
	{
		spi2, //drv::Spi &peri;
		{&gpioD, 8},				//config::gpio::Set RSTn;
		{&gpioD, 9},				//config::gpio::Set INTn;
		{&gpioD, 7},				//config::gpio::Set CSn;
		false,						//bool PPPoE;
		false,						//bool pingBlock;
		5000,						//unsigned short retransmissionTime;
		8,							//unsigned char retransmissionCount;
		{							//unsigned int txSocketBufferSize[4];
			W5100S::BUF_SIZE_2KB, 
			W5100S::BUF_SIZE_2KB, 
			W5100S::BUF_SIZE_2KB, 
			W5100S::BUF_SIZE_2KB
		},	
		{							//unsigned int rxSocketBufferSize[4];
			W5100S::BUF_SIZE_2KB, 
			W5100S::BUF_SIZE_2KB, 
			W5100S::BUF_SIZE_2KB, 
			W5100S::BUF_SIZE_2KB
		},	
	};
	
	w5100s.lock();
	w5100s.setCallbackLinkup(callback_linkup, 512);

	if(w5100s.init(w5100sConfig))
	{
		debug_printf("W5100S initialization Ok!!\n");
		
		const W5100S::IpConfig ipconfig = 
		{
			{0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC},	//unsigned char macAddress[6];
			{192, 168, 0, 1},						//unsigned char gatewayAddress[4];
			{255, 255, 255, 0},						//unsigned char subnetMask[4];
			{192, 168, 0, 100}						//unsigned char ipAddress[4];
		};

		w5100s.setIpConfig(ipconfig);
	}
	else
	{
		debug_printf("W5100S initialization Failed!!\n");
		while(1);
	}
	w5100s.unlock();
	
	// 소켓 초기화
	socket0.init(w5100s, 0);
}

void callback_linkup(bool linkup)
{
	if(linkup)
	{
	}
}

void initSystem(void)
{
	void (*application)(void) = (void (*)(void))(0x08010000);

	clock.peripheral.setGpioAEn(true);
	clock.peripheral.setGpioBEn(true);
	clock.peripheral.setGpioCEn(true);
	clock.peripheral.setGpioDEn(true);
	clock.peripheral.setAfioEn(true);

	// 부트로더가 완성될 때까지는 주석 처리
	//if(GPIOB->IDR & GPIO_IDR_IDR1_Msk)
	//{
	//	application();
	//}

	clock.peripheral.setPwrEn(true);
	clock.enableHse(HSE_CLOCK_FREQ);

	using namespace define::clock;

	clock.enableMainPll(
		pll::src::HSE,	// unsigned char src;
		0,				// unsigned char xtpre;
		4				// unsigned char mul;
	); 

	clock.setSysclk(
		sysclk::src::PLL,		// unsigned char sysclkSrc;
		divFactor::ahb::NO_DIV, // unsigned char ahb;
		divFactor::apb::DIV2,	// unsigned char apb1;
		divFactor::apb::NO_DIV	// unsigned char apb2;
	);

	flash.setPrefetchEn(true);

	AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_NOJNTRST_Msk;
}
