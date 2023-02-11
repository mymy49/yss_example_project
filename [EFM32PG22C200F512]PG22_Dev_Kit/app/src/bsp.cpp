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

#include <yss/debug.h>
#include <yss.h>
#include <bsp.h>

FunctionQueue functionQueue(16);
//CommandLineInterface cli(uart2);

void initBoard(void)
{
	using namespace define::gpio;

	// USART0 초기화
	gpioA.setAsAltFunc(5, altfunc::UART0_TX, alttype::PUSHPULL);
	gpioA.setAsAltFunc(6, altfunc::UART0_RX, alttype::INPUT);
	
	uart0.enableClock();
	uart0.initialize(115200, 256);
	uart0.enableInterrupt();

	// PDM 초기화
	gpioC.setAsOutput(7);
	gpioC.setOutput(7, true);
	gpioB.setAsAltFunc(0, altfunc::PDM_CLK, alttype::PUSHPULL);
	gpioB.setAsAltFunc(1, altfunc::PDM_DAT0, alttype::INPUT);
	
	using namespace define::pdm;
	Pdm::Configuration pdmConfig =
	{
		chClkPol::NORMAL,		//uint8_t ch0ClkPol;		// 채널0 클럭 엣지 설정
		chClkPol::NORMAL,		//uint8_t ch1ClkPol;		// 채널1 클럭 엣지 설정
		stereoMode::CH01ENABLE,	//uint8_t stereoMode;		// 스테레오 모드 설정
		fifoLevel::FOUR,		//uint8_t fifoLevel;		// FIFO 사용 길이 설정
		dataFormat::LEFT16,		//uint8_t dataFormat;		// 데이터 포멧 설정
		numOfCh::TWO,			//uint8_t numOfCh;			// 채널 개수 설정
		filterOrder::FIFTH,		//uint8_t filterOrder;		// Filter Order 설정
		delayMuxSel::ONE,		//uint8_t delayMuxSel;		// Data Delay Buffer Mux 설정
		1000000					//uint32_t clkFreq;			// Clock 주파수 설정
	};

	pdm0.enableClock();
	pdm0.initialize(pdmConfig, 128);
	pdm0.enableInterrupt();

	pdm0.start();

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
	Led::init();
}

