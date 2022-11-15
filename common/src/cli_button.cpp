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

#include "../inc/cli_led.h"
#include <yss/malloc.h>
#include <string.h>
#include <stdio.h>
#include <util/ElapsedTime.h>
#include <yss.h>

#if !(defined(YSS_DRV_UART_UNSUPPORTED) || defined(YSS_DRV_GPIO_UNSUPPORTED))

namespace Cli
{
namespace Button
{
	Gpio *gGpio;
	uint8_t gPinNumber;
	ElapsedTime gPressedTime;
	int32_t gTriggerId, gThreadId;
	uint32_t gTriggerEnteringTime, gButtonReleaseTime;
	bool gPressedFlag, gRisingFlag, gCompleteFlag;

	void trigger_button(void)
	{
		gTriggerEnteringTime = gPressedTime.getUsec();
	}

	void isr_button(void)
	{
		bool pin = gGpio->getInputData(gPinNumber);

		if(pin == gRisingFlag)
		{
			gPressedTime.reset();
			trigger::run(gTriggerId);
		}
		else
		{
			gButtonReleaseTime = gPressedTime.getUsec();
			gCompleteFlag = true;
			thread::signal(gThreadId);
		}
	}

	void setPin(Gpio &gpio, uint8_t pinNumber, bool rising)
	{
		gGpio = &gpio;
		gPinNumber = pinNumber;
		gRisingFlag = rising;
		
		using namespace define::exti::mode;
		exti.add(gpio, pinNumber, FALLING | RISING, isr_button);
		gTriggerId = trigger::add(trigger_button, 512);
	}

	error callback_button(Uart *peripheral, void *var)
	{
		char str[64];
		uint32_t len, enteringTime;
		int16_t input;

		sprintf(str, "\n\rWhen you Press 'x' key, It will terminates.\n\r");
		len = strlen(str);
		peripheral->lock();
		peripheral->send(str, len);
		peripheral->unlock();

		gThreadId = thread::getCurrentThreadId();
		
		while(1)
		{
			if(gCompleteFlag)
			{
				enteringTime = gPressedTime.getUsec();
				gCompleteFlag = false;
				
				peripheral->lock();
				sprintf(str, "Trigger Entering Time = %d us\n\r", gTriggerEnteringTime);
				len = strlen(str);
				peripheral->send(str, len);

				sprintf(str, "Button Release Time = %d us\n\r", gButtonReleaseTime);
				len = strlen(str);
				peripheral->send(str, len);

				sprintf(str, "Thread Entering Time = %d us\n\r", enteringTime);
				len = strlen(str);
				peripheral->send(str, len);
				peripheral->unlock();
			}

			input = peripheral->getReceivedByte();

			if(input == 'x')
				return Error::NONE;

			thread::yield();
		}
	}
	
	void registerCli(CommandLineInterface &cli)
	{
		static const uint8_t varType[1] = {CommandLineInterface::TERMINATE};
		cli.addCommand("button", varType, callback_button, "It measures Button pressed time. ex)button");
	}
}
}

#endif

