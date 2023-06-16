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

#include <drv/mcu.h>

#if defined(EFM32PG22)

#include <drv/peripheral.h>
#include <yss/instance.h>
#include <config.h>
#include <targets/siliconlabs/efm32pg22_cmu.h>
#include <targets/siliconlabs/efm32pg22_ldma.h>
#include <targets/siliconlabs/efm32pg22_ldmaxbar_defines.h>
#include <targets/siliconlabs/define_efm32pg22_efr32bg22.h>

static uint32_t getClockFreqeuncy(void)
{
	return clock.getApb0Frequency();
}

using namespace define::dma;

#if USART_COUNT >= 1 && defined(SPI0_ENABLE)
#if defined(SPI0_ENABLE) && defined(UART0_ENABLE)
#error "SPI0과 UART0은 동시에 활성화 될 수 없습니다."
#endif
static void enableClockUart0(bool en)
{
	clock.lock();
	clock.enableApb0Clock(_CMU_CLKEN0_USART0_SHIFT, en);
	clock.unlock();
}

static void enableInterruptUart0(bool en)
{
	//nvic.lock();
	//nvic.enableInterrupt(USART0_RX_IRQn, en);
	//nvic.unlock();
}

static const Drv::Setup gDrvUart0Config
{
	enableClockUart0,		//void (*clockFunc)(bool en);
	enableInterruptUart0,	//void (*nvicFunc)(bool en);
	0,						//void (*resetFunc)(void);
	getClockFreqeuncy		//uint32_t (*getClockFunc)(void);
};

static const Dma::DmaInfo gUart0TxDmaInfo = 
{
	(dstinc::NONE << _LDMA_CH_CTRL_DSTINC_SHIFT) |	// uint32_t controlRegister1
	(size::BYTE << _LDMA_CH_CTRL_SIZE_SHIFT) |
	(srcinc::ONE << _LDMA_CH_CTRL_SRCINC_SHIFT) |	
	(reqmode::BLOCK << _LDMA_CH_CTRL_REQMODE_SHIFT) |	
	(blocksize::UNIT2 << _LDMA_CH_CTRL_BLOCKSIZE_SHIFT) |	
	(structtype::TRANSFER << _LDMA_CH_CTRL_STRUCTTYPE_SHIFT) |
	_LDMA_CH_CTRL_DONEIEN_MASK,
	0,								// uint32_t controlRegister2
	0x80000000 |					// uint32_t controlRegister3
	LDMAXBAR_CH_REQSEL_SOURCESEL_USART0 |
	LDMAXBAR_CH_REQSEL_SIGSEL_USART0TXEMPTY,
	(void*)&USART0_S->TXDATA			//void *dataRegister;
};

static const Spi::Setup gUart0Config
{
	USART0_S,			//YSS_SPI_Peri *peri;
	dmaChannelList,		//Dma *dmaChannelList;
	&gUart0TxDmaInfo	//Dma::DmaInfo txDmaInfo;
};

Spi spi0(gDrvUart0Config, gUart0Config);
#endif



#if USART_COUNT >= 2 && defined(SPI1_ENABLE)
#if defined(SPI1_ENABLE) && defined(UART1_ENABLE)
#error "SPI1과 UART1은 동시에 활성화 될 수 없습니다."
#endif
#error "현재 SPI1은 동작하지 않습니다. 확인중입니다."
static void enableClockUart1(bool en)
{
	clock.lock();
	clock.enableApb0Clock(_CMU_CLKEN0_USART1_SHIFT, en);
	clock.unlock();
}

static void enableInterruptUart1(bool en)
{
	//nvic.lock();
	//nvic.enableInterrupt(USART1_RX_IRQn, en);
	//nvic.unlock();
}

static const Drv::Setup gDrvUart1Config
{
	enableClockUart1,		//void (*clockFunc)(bool en);
	enableInterruptUart1,	//void (*nvicFunc)(bool en);
	0,						//void (*resetFunc)(void);
	getClockFreqeuncy		//uint32_t (*getClockFunc)(void);
};

static const Dma::DmaInfo gUart1TxDmaInfo = 
{
	(dstinc::NONE << _LDMA_CH_CTRL_DSTINC_SHIFT) |	// uint32_t controlRegister1
	(size::BYTE << _LDMA_CH_CTRL_SIZE_SHIFT) |
	(srcinc::ONE << _LDMA_CH_CTRL_SRCINC_SHIFT) |	
	(reqmode::BLOCK << _LDMA_CH_CTRL_REQMODE_SHIFT) |	
	(blocksize::UNIT2 << _LDMA_CH_CTRL_BLOCKSIZE_SHIFT) |	
	(structtype::TRANSFER << _LDMA_CH_CTRL_STRUCTTYPE_SHIFT) |
	_LDMA_CH_CTRL_DONEIEN_MASK,
	0,								// uint32_t controlRegister2
	0x80000000 |					// uint32_t controlRegister3
	LDMAXBAR_CH_REQSEL_SOURCESEL_USART1 |
	LDMAXBAR_CH_REQSEL_SIGSEL_USART1TXEMPTY,
	(void*)&USART1_S->TXDATA			//void *dataRegister;
};

static const Spi::Setup gUart1Config
{
	USART1_S,				//YSS_SPI_Peri *peri;
	dmaChannelList,		//Dma *dmaChannelList;
	&gUart1TxDmaInfo	//Dma::DmaInfo txDmaInfo;
};

Spi spi1(gDrvUart1Config, gUart1Config);
#endif

#endif

