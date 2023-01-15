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

#include <yss/debug.h>
#include <yss.h>
#include <bsp.h>
#include <util/runtime.h>
#include <cli_led.h>
#include <cli_dump.h>
#include <cli_adc.h>
#include <cli_button.h>
#include <cli_logo.h>
#include <cli_gauge.h>
#include <task.h>

int main(void)
{
	initYss();
	initBoard();

	// CLI LED 설정
	Cli::Led::setNumOfLed(1);
	Cli::Led::setLedFunction(0, led::setOn);
	Cli::Led::registerCli(cli);

	// CLI DUMP 설정
	Cli::Dump::registerCli(cli);

	// CLI ANALOG 설정
	Cli::Analog::setNumOfAdc(3);
	Cli::Analog::setAdcChannel(0, 0, adc1);
	Cli::Analog::setAdcChannel(1, 8, adc1);
	Cli::Analog::setAdcChannel(2, 7, adc1);
	Cli::Analog::registerCli(cli);

	// CLI BUTTON 설정
	Cli::Button::setPin(gpioI, 11, true);
	Cli::Button::registerCli(cli);

	// CLI LOGO 설정
	Cli::Logo::setFunctionQueue(functionQueue);
	Cli::Logo::registerCli(cli);

	// CLI GAUGE 설정
	Cli::Guage::setFunctionQueue(functionQueue);
	Cli::Guage::registerCli(cli);

	cli.setGreetings("\r\n\nHello!!\n\rWelcome to yss operating system!!\n\rThis is an example for STM32F756G-DISCO board.\n\n\r");
	cli.start();

	functionQueue.add(Task::displayLogo);
	functionQueue.start();
	
	while(1)
	{
		thread::yield();
	}

	return 0;
}

