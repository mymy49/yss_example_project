////////////////////////////////////////////////////////////////////////////////////////
//
// 저작권 표기 License_ver_3.1
// 본 소스 코드의 소유권은 홍윤기에게 있습니다.
// 어떠한 형태든 기여는 기증으로 받아들입니다.
// 본 소스 코드는 아래 사항에 동의할 경우에 사용 가능합니다.
// 아래 사항에 대해 동의하지 않거나 이해하지 못했을 경우 사용을 금합니다.
// 본 소스 코드를 사용하였다면 아래 사항을 모두 동의하는 것으로 자동 간주 합니다.
// 본 소스 코드의 상업적 또는 비 상업적 이용이 가능합니다.
// 본 소스 코드의 내용을 임의로 수정하여 재배포하는 행위를 금합니다.
// 본 소스 코드의 사용으로 인해 발생하는 모든 사고에 대해서 어떠한 법적 책임을 지지 않습니다.
//
// Home Page : http://cafe.naver.com/yssoperatingsystem
// Copyright 2022. 홍윤기 all right reserved.
//
////////////////////////////////////////////////////////////////////////////////////////
#ifndef COMMON_BUTTON__H_
#define COMMON_BUTTON__H_

#include <util/CommandLineInterface.h>
#include <drv/Gpio.h>
#include <yss/error.h>

#if !(defined(YSS_DRV_UART_UNSUPPORTED) || defined(YSS_DRV_GPIO_UNSUPPORTED))

// BUTTON이 눌린 시간을 측정하는 CLI를 명령을 생성한다.
// 사용 순서는 아래와 같다.
// 1. setPin() 함수를 사용해 버튼이 연결된 핀을 설정한다.
// 2. registerCli() 함수를 사용해 CommandLineInterface class의 instance에 등록한다.
namespace Cli
{
namespace Button
{
	// 버튼에 연결된 핀을 설정하는 함수이다.
	// 
	// Gpio &gpio
	//		핀이 연결된 GPIO 그룹을 설정한다.
	// uint8_t pinNumber
	//		핀이 연결된 번호를 설정한다.
	// bool falling
	//		버튼 회로의 구성이 버튼을 누를때, 핀의 전압이 상승하면 true,
	//		하강하면 false를 설정한다.
	void setPin(Gpio &gpio, uint8_t pinNumber, bool rising);

	// CommandLineInterface class의 instance에 명령어와 관련 함수등을 등록한다.
	// 
	// CommandLineInterface &cli
	//		등록할 CLI를 설정한다.
	void registerCli(CommandLineInterface &cli);
}
}

#endif

#endif

