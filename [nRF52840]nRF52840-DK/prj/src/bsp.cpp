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

FunctionQueue functionQueue(16);
CommandLineInterface cli(uart0);

void initBoard(void)
{
	// USART2 초기화
	using namespace define::gpio;
	gpio0.setAsAltFunc(8, altfunc::UART0_RXD);
	gpio0.setAsAltFunc(6, altfunc::UART0_TXD);
	gpio0.setAsAltFunc(5, altfunc::UART0_RTS);
	gpio0.setAsAltFunc(7, altfunc::UART0_CTS);

	uart0.enableClock();
	uart0.initialize(9600, 256);
	uart0.enableInterrupt();

	// Radio 초기화
	radio.enableClock();
	radio.init();

	// LED 초기화
	led::init();
}

