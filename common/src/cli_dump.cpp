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

#if !defined(YSS_DRV_UART_UNSUPPORTED)

namespace Cli
{
namespace Dump
{
	error callback_displayMemoryData(Uart *peripheral, void *var)
	{
		error result;
		char str[64];
		char *cvar = (char*)var;
		uint32_t addr = *(uint32_t*)cvar, len;
		cvar += sizeof(uint32_t);
		uint32_t size = *(uint32_t*)cvar;
		cvar += sizeof(uint32_t);
		uint32_t count = *(uint32_t*)cvar;
		cvar += sizeof(uint32_t);

		peripheral->lock();
		switch(size)
		{
		case 1 :
			if(!strcasecmp(cvar, "HEX"))
			{
				for(uint32_t i=0;i<count;i++)
				{
					sprintf(str, "\n\r0x%08X : 0x%02X", addr, *(uint8_t*)addr);
					len = strlen(str);
					peripheral->send(str, len);
					addr++;
				}
			}
			else if(!strcasecmp(cvar, "DEC"))
			{
				for(uint32_t i=0;i<count;i++)
				{
					sprintf(str, "\n\r0x%08X : %d", addr, *(uint8_t*)addr);
					len = strlen(str);
					peripheral->send(str, len);
					addr++;
				}
			}
			else if(!strcasecmp(cvar, "FLOAT"))
			{
				result = Error::WRONG_FORMAT;
				goto error_handler;
			}
			else
			{
				result = Error::UNKNOWN;
				goto error_handler;
			}
			break;
		case 2 :
			if(!strcasecmp(cvar, "HEX"))
			{
				for(uint32_t i=0;i<count;i++)
				{
					sprintf(str, "\n\r0x%08X : 0x%04X", addr, *(uint16_t*)addr);
					len = strlen(str);
					peripheral->send(str, len);
					addr += 2;
				}
			}
			else if(!strcasecmp(cvar, "DEC"))
			{
				for(uint32_t i=0;i<count;i++)
				{
					sprintf(str, "\n\r0x%08X : %d", addr, *(uint16_t*)addr);
					len = strlen(str);
					peripheral->send(str, len);
					addr += 2;
				}
			}
			else if(!strcasecmp(cvar, "FLOAT"))
			{
				result = Error::WRONG_FORMAT;
				goto error_handler;
			}
			else
			{
				result = Error::UNKNOWN;
				goto error_handler;
			}
			break;

		case 3 :
			if(!strcasecmp(cvar, "HEX"))
			{
				for(uint32_t i=0;i<count;i++)
				{
					sprintf(str, "\n\r0x%08X : 0x%06X", addr, *(uint32_t*)addr & 0x00FFFFFF);
					len = strlen(str);
					peripheral->send(str, len);
					addr += 3;
				}
			}
			else if(!strcasecmp(cvar, "DEC"))
			{
				for(uint32_t i=0;i<count;i++)
				{
					sprintf(str, "\n\r0x%08X : %d", addr, *(uint32_t*)addr & 0x00FFFFFF);
					len = strlen(str);
					peripheral->send(str, len);
					addr += 3;
				}
			}
			else if(!strcasecmp(cvar, "FLOAT"))
			{
				result = Error::WRONG_FORMAT;
				goto error_handler;
			}
			else
			{
				result = Error::UNKNOWN;
				goto error_handler;
			}
			break;

		case 4 :
			if(!strcasecmp(cvar, "HEX"))
			{
				for(uint32_t i=0;i<count;i++)
				{
					sprintf(str, "\n\r0x%08X : 0x%08X", addr, *(uint32_t*)addr);
					len = strlen(str);
					peripheral->send(str, len);
					addr += 4;
				}
			}
			else if(!strcasecmp(cvar, "DEC"))
			{
				for(uint32_t i=0;i<count;i++)
				{
					sprintf(str, "\n\r0x%08X : %d", addr, *(uint32_t*)addr);
					len = strlen(str);
					peripheral->send(str, len);
					addr += 3;
				}
			}
			else if(!strcasecmp(cvar, "FLOAT"))
			{
				for(uint32_t i=0;i<count;i++)
				{
					sprintf(str, "\n\r0x%08X : %f", addr, *(float*)addr);
					len = strlen(str);
					peripheral->send(str, len);
					addr += 4;
				}
			}
			else
			{
				result = Error::UNKNOWN;
				goto error_handler;
			}
			break;
			
		default :
			result = Error::WRONG_SIZE;
			goto error_handler;
			break;
		}
		peripheral->unlock();

		return Error::NONE;
error_handler :
		switch(result)
		{
		case Error::WRONG_SIZE :
			sprintf(str, "\n\rError!! : The data size(%d) is not valid.\n\r", size);
			break;

		case Error::UNKNOWN :
			sprintf(str, "\n\rError!! : The data type \"%s\" is not valid string.\n\r", cvar);
			break;

		case Error::WRONG_FORMAT :
			sprintf(str, "\n\rError!! : When The data size is %d, It is not support float data type.\n\r", size);
			break;
		}

		len = strlen(str);
		peripheral->send(str, len);
		peripheral->unlock();

		return result;
	}

	void registerCli(CommandLineInterface &cli)
	{
		static const uint8_t varType[5] = {CommandLineInterface::HEXADECIMAL, CommandLineInterface::INTEGER, CommandLineInterface::INTEGER, CommandLineInterface::STRING, CommandLineInterface::TERMINATE};
		cli.addCommand("dump", varType, callback_displayMemoryData, "It displays values stored in memory. ex)dump 0x20000000 4 1 float / dump 20000000 1 4 hex");
	}
}
}

#endif

