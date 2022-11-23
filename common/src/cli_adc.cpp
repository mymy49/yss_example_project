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

#include "../inc/cli_adc.h"
#include <yss/malloc.h>
#include <string.h>
#include <stdio.h>

#if !(defined(YSS_DRV_UART_UNSUPPORTED) || defined(YSS_DRV_ADC_UNSUPPORTED))

namespace Cli
{
namespace Analog
{
	struct Config
	{
		Adc *adc;
		uint8_t channel;
	};

	uint8_t gNumOfAdc;
	Config *gConfig;

	void setNumOfAdc(uint8_t num)
	{
		gNumOfAdc = num;
		gConfig = new Config[num];
		memset(gConfig, 0, sizeof(Config)*num);
	}

	error setAdcChannel(uint8_t num, uint8_t channel, Adc &adc)
	{
		if(num >= gNumOfAdc)
			return Error::INDEX_OVER;
		
		gConfig[num].channel = channel;
		gConfig[num].adc = &adc;

		return Error::NONE;
	}

	error callback_adc(Uart *peripheral, void *var)
	{
		error result;

		char str[64];
		uint32_t num = *(uint32_t*)var, len;

		if(num >= gNumOfAdc)
		{
			result = Error::INDEX_OVER;
			goto error_handler;
		}
		
		sprintf(str, "\n\rADC[%d] : %5d", num, gConfig[num].adc->get(gConfig[num].channel));
		len = strlen(str);
		peripheral->lock();
		peripheral->send(str, len);
		peripheral->unlock();

		return Error::NONE;

error_handler :
		switch(result)
		{
		case Error::INDEX_OVER :
			sprintf(str, "\n\rError!! : The ADC channel %d is not valid.", num);
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
		static const uint8_t adcVarType[2] = {CommandLineInterface::INTEGER, CommandLineInterface::TERMINATE};
		cli.addCommand("adc", adcVarType, callback_adc, "It displays result of selected ADC. ex)adc 0 / adc 1");
	}
}
}

#endif

