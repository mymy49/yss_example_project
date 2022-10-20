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

namespace Cli
{
namespace Led
{
	uint8_t gNumOfLed;
	void (**setLedOn)(bool en);

	void setNumOfLed(uint8_t num)
	{
		uint32_t size = sizeof(void*)*num;
		gNumOfLed = num;
		lockHmalloc();
		setLedOn = (void (**)(bool))hmalloc(size);
		unlockHmalloc();
		memset(setLedOn, 0, size);
	}

	error setLedFunction(uint8_t num, void (*func)(bool))
	{
		if(num >= gNumOfLed)
			return Error::INDEX_OVER;
		
		setLedOn[num] = func;

		return Error::NONE;
	}

	error callback_setLed(Uart *peripheral, void *var)
	{
		error result;
		char str[64];
		char *cvar = (char*)var;
		uint32_t num = *(uint32_t*)cvar, len;
		cvar += sizeof(uint32_t);

		if(num >= gNumOfLed)
		{
			result = Error::INDEX_OVER;
			goto error_handler;
		}
		
		if(!strcmp(cvar, "ON"))
			setLedOn[num](true);
		else if(!strcmp(cvar, "on"))
			setLedOn[num](true);
		else if(!strcmp(cvar, "OFF"))
			setLedOn[num](false);
		else if(!strcmp(cvar, "off"))
			setLedOn[num](false);
		else
		{
			result = Error::UNKNOWN;
			goto error_handler;
		}

		return Error::NONE;

error_handler :
		switch(result)
		{
		case Error::INDEX_OVER :
			sprintf(str, "\n\rError!! : LED number %d is not valid.\n\r", num);
			break;

		case Error::UNKNOWN :
			sprintf(str, "\n\rError!! : \"%s\" is not valid string.\n\r", cvar);
			break;
		}

		len = strlen(str);
		peripheral->lock();
		peripheral->send(str, len);
		peripheral->unlock();
		return result;
	}

	void registerCli(CommandLineInterface &cli)
	{
		static const uint8_t ledVarType[3] = {CommandLineInterface::INT, CommandLineInterface::STRING, CommandLineInterface::TERMINATE};
		cli.addCommand("led", ledVarType, callback_setLed, "This function turns the LED on/off. ex)led 0 ON / led 1 OFF");
	}
}
}
