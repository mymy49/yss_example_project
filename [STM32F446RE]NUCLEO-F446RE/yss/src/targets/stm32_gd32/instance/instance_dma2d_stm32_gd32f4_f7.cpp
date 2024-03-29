////////////////////////////////////////////////////////////////////////////////////////
//
// 저작권 표기 License_ver_3.2
// 본 소스 코드의 소유권은 홍윤기에게 있습니다.
// 어떠한 형태든 기여는 기증으로 받아들입니다.
// 본 소스 코드는 아래 사항에 동의할 경우에 사용 가능합니다.
// 아래 사항에 대해 동의하지 않거나 이해하지 못했을 경우 사용을 금합니다.
// 본 소스 코드를 사용하였다면 아래 사항을 모두 동의하는 것으로 자동 간주 합니다.
// 본 소스 코드의 상업적 또는 비 상업적 이용이 가능합니다.
// 본 소스 코드의 내용을 임의로 수정하여 재배포하는 행위를 금합니다.
// 본 소스 코드의 사용으로 인해 발생하는 모든 사고에 대해서 어떠한 법적 책임을 지지 않습니다.
// 본 소스 코드의 어떤 형태의 기여든 기증으로 받아들입니다.
//
// Home Page : http://cafe.naver.com/yssoperatingsystem
// Copyright 2023. 홍윤기 all right reserved.
//
////////////////////////////////////////////////////////////////////////////////////////

#include <drv/Dma2d.h>

#ifndef YSS_DRV_DMA2D_UNSUPPORTED

#include <yss/instance.h>

#if defined(DMA2D) && USE_GUI

#include <targets/st_gigadevice/rcc_stm32_gd32f4_f7.h>

#if defined(GD32F4)
#define DMA2D_IRQHandler	IPA_IRQHandler
#endif

static void enableClock(bool en)
{
	clock.lock();
    clock.enableAhb1Clock(RCC_AHB1ENR_DMA2DEN_Pos, en);
	clock.unlock();
}

static void enableInterrup(bool en)
{
	nvic.lock();
	nvic.enableInterrupt(DMA2D_IRQn, en);
	nvic.unlock();
}

static void reset(void)
{
	clock.lock();
    clock.resetAhb1(RCC_AHB1RSTR_DMA2DRST_Pos);
	clock.unlock();
}

static const Drv::Config gDrvConfig
{
	enableClock,	//void (*clockFunc)(bool en);
	enableInterrup,	//void (*nvicFunc)(bool en);
	reset			//void (*resetFunc)(void);
};

static const Dma2d::Config gConfig
{
	(YSS_DMA2D_Peri*)DMA2D	//YSS_DMA2D_Peri *peri;
};

Dma2d dma2d(gDrvConfig, gConfig);

extern "C"
{
	void DMA2D_IRQHandler(void)
	{
		dma2d.isr();
	}
}

#endif

#endif
