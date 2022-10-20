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

#ifndef COMMON_LED__H_
#define COMMON_LED__H_

#include <util/CommandLineInterface.h>
#include <drv/Gpio.h>
#include <yss/error.h>

// LED를 ON/OFF 하는 CLI를 명령을 생성한다.
// 사용 순서는 아래와 같다.
// 1. setNumOfLed() 함수를 사용해 전체 LED 수를 설정한다.
// 2. setLedFunction() 함수를 사용해 각 LED를 ON/OFF 하는 함수를 LED마다 등록한다.
// 3. registerCli() 함수를 사용해 CommandLineInterface class의 instance에 등록한다.

namespace Cli
{
namespace Led
{
	// 제어할 LED의 전체 개수를 설정하는 함수이다.
	// 
	// uint8_t num
	//		LED의 전체 개수를 설정한다.
	void setNumOfLed(uint8_t num);

	// LED를 ON/OFF 하는 함수를 설정하는 함수이다.
	//
	// uint8_t num
	//		설정할 LED의 인덱스를 설정한다.
	// void (*func)(bool)
	//		num에서 설정된 LED를 켜고 끄는 함수의 포인터를 설정한다.
	// 반환
	//		발생한 error를 반환한다.
	error setLedFunction(uint8_t num, void (*func)(bool));

	// CommandLineInterface class의 instance에 명령어와 관련 함수등을 등록한다.
	// 
	// CommandLineInterface &cli
	//		등록할 CLI를 설정한다.
	void registerCli(CommandLineInterface &cli);
}
}

#endif