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
////////////////////////////////////////////////////////////////////////////////////////

#include <mod/sdram/IS42S16400J_7TL.h>

#ifndef YSS_DRV_SDRAM_UNSUPPORTED

#define	MODE_BURST_LENGTH_1				((uint16_t)0x0000)
#define	MODE_BURST_LENGTH_2				((uint16_t)0x0001)
#define	MODE_BURST_LENGTH_4				((uint16_t)0x0002)
#define	MODE_BURST_LENGTH_8				((uint16_t)0x0004)
#define	MODE_BURST_TYPE_SEQUENTIAL		((uint16_t)0x0000)
#define	MODE_BURST_TYPE_INTERLEAVED		((uint16_t)0x0008)
#define	MODE_CAS_LATENCY_2				((uint16_t)0x0020)
#define	MODE_CAS_LATENCY_3				((uint16_t)0x0030)
#define	MODE_OPERATING_MODE_STANDARD	((uint16_t)0x0000)
#define	MODE_WRITEBURST_MODE_PROGRAMMED	((uint16_t)0x0000)
#define	MODE_WRITEBURST_MODE_SINGLE		((uint16_t)0x0200)

Sdram::Specification IS42S16400J_7TL =
{
	define::sdram::columnAddress::BIT8,		//	uint8_t columnAddress;
	define::sdram::rowAddress::BIT12,		//	uint8_t rowAddress;
	define::sdram::dbusWidth::BIT16,		//	uint8_t dbusWidth;
	define::sdram::internalBank::FOUR,		//	uint8_t internalBank;
	define::sdram::casLatency::TWO,			//	uint8_t casLatency;
	133000000,								//	uint32_t maxFrequency;
	20000,									//	float tMrd;
	70000,									//	float tXsr;
	42000,									//	float tRas;
	63000,									//	float tRc;
	20000,									//	float tWr;
	15000,									//	float tRp;
	15000,									//	float tRcd;
	2700,									//	float tOh;
	5400,									//	uint32_t tAc;
	64000,									//	float tRefresh;
	4096,									//	uint16_t numOfRow;
	false,									//	bool writeProtection;
	false,									//	bool burstRead;
	MODE_BURST_LENGTH_1						//	uint16_t mode;
	| MODE_BURST_TYPE_SEQUENTIAL
	| MODE_CAS_LATENCY_2
	| MODE_WRITEBURST_MODE_PROGRAMMED
};

#endif
