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

/*
#include <yss/instance.h>
#include <config.h>

#if defined(STM32F4) || defined(STM32F7)

#if defined(SAI1_ENABLE) & defined(SAI1)
static void setClock1En(bool en)
{
	clock.peripheral.setSai1En(en);
}

static void setInterrupt1En(bool en)
{
	nvic.setSai1En(en);
}

static uint32_t getSai1Clock(void)
{
#if defined(STM32F4) | defined(STM32F7)
	return clock.getApb2ClkFreq();
#else
	return 0;
#endif
}

static const Drv::Config gSai1DrvConfig
{
	setClock1En,		//void (*clockFunc)(bool en) = 0;
	setInterrupt1En,	//void (*nvicFunc)(bool en) = 0;
	0					//void (*resetFunc)(void) = 0;
};

static const drv::Sai::Config gSai1Config
{
	SAI1,								//YSS_SAI_Peri *peri;
	SAI1_Block_A,						//SAI_Block_TypeDef *blockA;
	SAI1_Block_B,							//SAI_Block_TypeDef *blockB;
	YSS_DMA_MAP_SAI1_A_STREAM,			//Stream *streamA;
	YSS_DMA_MAP_SAI1_A_CHANNEL,			//uint8_t channelA;
	YSS_DMA_MAP_SAI1_B_STREAM,			//Stream *streamB;
	YSS_DMA_MAP_SAI1_B_CHANNEL,			//uint8_t channelB;
	define::dma::priorityLevel::LOW,	//uint16_t priority;
	getSai1Clock,						//uint32_t (*getClockFreq)(void);
};

drv::Sai sai1(gSai1DrvConfig, gSai1Config);
#endif

#if defined(SAI2_ENABLE) & defined(SAI2)
static void setClock2En(bool en)
{
	clock.peripheral.setSai2En(en);
}

static void setInterrupt2En(bool en)
{
	nvic.setSai2En(en);
}

static uint32_t getSai2Clock(void)
{
#if defined(STM32F7)
	return clock.getApb2ClkFreq();
#else
	return 0;
#endif
}

static const Drv::Config gSai2DrvConfig
{
	setClock2En,		//void (*clockFunc)(bool en) = 0;
	setInterrupt2En,	//void (*nvicFunc)(bool en) = 0;
	0					//void (*resetFunc)(void) = 0;
};

static const drv::Sai::Config gSai2Config
{
	SAI2,								//YSS_SAI_Peri *peri;
	SAI2_Block_A,						//SAI_Block_TypeDef *blockA;
	SAI2_Block_B,							//SAI_Block_TypeDef *blockB;
	YSS_DMA_MAP_SAI2_A_STREAM,			//Stream *streamA;
	YSS_DMA_MAP_SAI2_A_CHANNEL,			//uint8_t channelA;
	YSS_DMA_MAP_SAI2_B_STREAM,			//Stream *streamB;
	YSS_DMA_MAP_SAI2_B_CHANNEL,			//uint8_t channelB;
	define::dma::priorityLevel::LOW,	//uint16_t priority;
	getSai2Clock,						//uint32_t (*getClockFreq)(void);
};

drv::Sai sai2(gSai2DrvConfig, gSai2Config);
#endif

#endif
*/


