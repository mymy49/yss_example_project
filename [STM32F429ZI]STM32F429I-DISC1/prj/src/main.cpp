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
#include <util/time.h>
#include <cli_led.h>
#include <cli_dump.h>
#include <cli_adc.h>
#include <cli_button.h>
#include <cli_gauge.h>
#include <cli_logo.h>
#include <stdint.h>
#include <task.h>

float gTest;

void thread_dummy(void)
{
	while(1)
		thread::yield();
}

int32_t main(void)
{
	initYss();
	initBoard();

	uint32_t time;
	
	// CLI LED 설정
	Cli::Led::setNumOfLed(2);
	Cli::Led::setLedFunction(0, led::setOn0);
	Cli::Led::setLedFunction(1, led::setOn1);
	Cli::Led::registerCli(cli);

	// CLI DUMP 설정
	Cli::Dump::registerCli(cli);

	Cli::Analog::setNumOfAdc(1);
	Cli::Analog::setAdcChannel(0, 5, adc1);
	Cli::Analog::registerCli(cli);

	// CLI BUTTON 설정
	Cli::Button::setPin(gpioA, 0, true);
	Cli::Button::registerCli(cli);

	// CLI GAUGE 설정
	Cli::Guage::setFunctionQueue(functionQueue);
	Cli::Guage::registerCli(cli);

	// CLI LOGO 설정
	Cli::Logo::setFunctionQueue(functionQueue);
	Cli::Logo::registerCli(cli);

	cli.setGreetings("\r\n\nHello!!\n\rWelcome to yss operating system!!\n\rThis is an example for STM32F429I-DISC1 board.\n\n\r");
	cli.start();
	
	functionQueue.add(Task::displayLogo);
	functionQueue.add(Task::calibrateTouchScreen);
	functionQueue.add(Task::displayGauge);
	functionQueue.start();

	while(1)
	{
		time = time::getRunningMsec();
		gTest = (float)time / 1000.f;
		thread::yield();
	}

	return 0;
}

