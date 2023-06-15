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

#if defined(STM32F1) || defined(STM32F4) || defined(STM32F0)

#include <yss/instance.h>
#include <config.h>
#include <yss.h>

#if defined(GD32F1) || defined(STM32F1)
#include <targets/st_gigadevice/dma_stm32_gd32f1.h>
#include <targets/st_gigadevice/rcc_stm32_gd32f1.h>
#elif defined(STM32F4)
#include <targets/st_gigadevice/dma_stm32_gd32f4_f7.h>
#include <targets/st_gigadevice/rcc_stm32_gd32f4_f7.h>
#elif defined(STM32F0)
#include <targets/st_gigadevice/dma_stm32_gd32f1.h>
#include <targets/st_gigadevice/rcc_stm32f0.h>
#endif
#include <targets/st_gigadevice/spi_stm32_gd32f0_f1_f4.h>

#if defined(GD32F1)
#if defined(__SEGGER_LINKER)
#define YSS_SPI1_IRQHandler			SPI0_IRQHandler
#define YSS_SPI2_IRQHandler			SPI1_IRQHandler
#define YSS_SPI3_IRQHandler			SPI2_IRQHandler
#else
#define YSS_SPI1_IRQHandler			SPI1_IRQHandler
#define YSS_SPI2_IRQHandler			SPI2_IRQHandler
#define YSS_SPI3_IRQHandler			SPI3_IRQHandler
#endif
#elif defined(STM32F1) || defined(STM32F4) || defined(STM32F0)
#define YSS_SPI1_IRQHandler			SPI1_IRQHandler
#define YSS_SPI2_IRQHandler			SPI2_IRQHandler
#define YSS_SPI3_IRQHandler			SPI3_IRQHandler
#define YSS_SPI4_IRQHandler			SPI4_IRQHandler
#define YSS_SPI5_IRQHandler			SPI5_IRQHandler
#endif

static uint32_t getApb1ClockFrequency(void)
{
	return clock.getApb1ClockFrequency();
}

static uint32_t getApb2ClockFrequency(void)
{
	return clock.getApb2ClockFrequency();
}

#if defined(SPI1_ENABLE) && defined(SPI1)
static void enableSpi1Clock(bool en)
{
	clock.lock();
    clock.enableApb2Clock(RCC_APB2ENR_SPI1EN_Pos, en);
	clock.unlock();
}

static void enableSpi1Interrupt(bool en)
{
	nvic.lock();
	nvic.enableInterrupt(SPI1_IRQn, en);
	nvic.unlock();
}

static void resetSpi1(void)
{
	clock.lock();
    clock.resetApb2(RCC_APB2RSTR_SPI1RST_Pos);
	clock.unlock();
}

static const Drv::Config gDrvSpi1Config = 
{
	enableSpi1Clock,		//void (*clockFunc)(bool en);
	enableSpi1Interrupt,	//void (*nvicFunc)(bool en);
	resetSpi1,				//void (*resetFunc)(void);
	getApb2ClockFrequency,	//uint32_t (*getClockFreq)(void);
};

static const Dma::DmaInfo gSpi1TxDmaInfo = 
{
#if defined(STM32F1) || defined(GD32F1)
	(define::dma::priorityLevel::LOW << DMA_CCR_PL_Pos) |	 // uint32_t controlRegister1
	(define::dma::size::BYTE << DMA_CCR_MSIZE_Pos) |
	(define::dma::size::BYTE << DMA_CCR_PSIZE_Pos) |
	DMA_CCR_MINC_Msk | 
	(define::dma::dir::MEM_TO_PERI << DMA_CCR_DIR_Pos) | 
	DMA_CCR_TCIE_Msk | 
	DMA_CCR_TEIE_Msk | 
	DMA_CCR_EN_Msk,
	0,															// uint32_t controlRegister2
	0,															// uint32_t controlRegister3
	(void*)&SPI1[SPI_REG::DR],									//void *dataRegister;
#elif defined(STM32F4)
	(define::dma2::stream3::SPI1_TX << DMA_SxCR_CHSEL_Pos) |	// uint32_t controlRegister1
	(define::dma::burst::SINGLE << DMA_SxCR_MBURST_Pos) | 
	(define::dma::burst::SINGLE << DMA_SxCR_PBURST_Pos) | 
	(define::dma::priorityLevel::LOW << DMA_SxCR_PL_Pos) |
	(define::dma::size::BYTE << DMA_SxCR_MSIZE_Pos) |
	(define::dma::size::BYTE << DMA_SxCR_PSIZE_Pos) |
	DMA_SxCR_MINC_Msk | 
	(define::dma::dir::MEM_TO_PERI << DMA_SxCR_DIR_Pos) | 
	DMA_SxCR_TCIE_Msk | 
	DMA_SxCR_TEIE_Msk | 
	DMA_SxCR_EN_Msk,
	0,															// uint32_t controlRegister2
	0,															// uint32_t controlRegister3
	(void*)&SPI1[SPI_REG::DR],									//void *dataRegister;
#endif
};

static const Dma::DmaInfo gSpi1RxDmaInfo = 
{
#if defined(STM32F1) || defined(GD32F1)
	(define::dma::priorityLevel::LOW << DMA_CCR_PL_Pos) |		// uint32_t controlRegister1
	(define::dma::size::BYTE << DMA_CCR_MSIZE_Pos) |
	(define::dma::size::BYTE << DMA_CCR_PSIZE_Pos) |
	DMA_CCR_MINC_Msk | 
	(define::dma::dir::PERI_TO_MEM << DMA_CCR_DIR_Pos) | 
	DMA_CCR_TCIE_Msk | 
	DMA_CCR_TEIE_Msk | 
	DMA_CCR_EN_Msk,
	0,															// uint32_t controlRegister2
	0,															// uint32_t controlRegister3
	(void*)&SPI1[SPI_REG::DR],									//void *dataRegister;
#elif defined(STM32F4)
	(define::dma2::stream0::SPI1_RX << DMA_SxCR_CHSEL_Pos) |	// uint32_t controlRegister1
	(define::dma::burst::SINGLE << DMA_SxCR_MBURST_Pos) | 
	(define::dma::burst::SINGLE << DMA_SxCR_PBURST_Pos) | 
	(define::dma::priorityLevel::LOW << DMA_SxCR_PL_Pos) |
	(define::dma::size::BYTE << DMA_SxCR_MSIZE_Pos) |
	(define::dma::size::BYTE << DMA_SxCR_PSIZE_Pos) |
	DMA_SxCR_MINC_Msk | 
	(define::dma::dir::PERI_TO_MEM << DMA_SxCR_DIR_Pos) | 
	DMA_SxCR_TCIE_Msk | 
	DMA_SxCR_TEIE_Msk | 
	DMA_SxCR_EN_Msk,
	0,															// uint32_t controlRegister2
	0,															// uint32_t controlRegister3
	(void*)&SPI1[SPI_REG::DR],									//void *dataRegister;
#endif
};

static const Spi::Config gSpi1Config = 
{
	(volatile uint32_t*)SPI1,	//YSS_SPI_Peri *peri;
	dmaChannel3,	//Dma &txDma;
	gSpi1TxDmaInfo,	//Dma::DmaInfo txDmaInfo;
	dmaChannel2,	//Dma &rxDma;
	gSpi1RxDmaInfo,	//Dma::DmaInfo rxDmaInfo;
	
};

Spi spi1(gDrvSpi1Config, gSpi1Config);

extern "C"
{
	void YSS_SPI1_IRQHandler(void)
	{
		spi1.isr();
	}
}
#endif

#if defined(SPI2_ENABLE) && defined(SPI2)
static void enableSpi2Clock(bool en)
{
	clock.lock();
    clock.enableApb1Clock(RCC_APB1ENR_SPI2EN_Pos, en);
	clock.unlock();
}

static void enableSpi2Interrupt(bool en)
{
	nvic.lock();
	nvic.enableInterrupt(SPI2_IRQn, en);
	nvic.unlock();
}

static void resetSpi2(void)
{
	clock.lock();
    clock.resetApb1(RCC_APB1RSTR_SPI2RST_Pos);
	clock.unlock();
}

static const Drv::Config gDrvSpi2Config = 
{
	enableSpi2Clock,		//void (*clockFunc)(bool en);
	enableSpi2Interrupt,	//void (*nvicFunc)(bool en);
	resetSpi2,				//void (*resetFunc)(void);
	getApb1ClockFrequency	//uint32_t (*getClockFreq)(void);
};

static const Dma::DmaInfo gSpi2TxDmaInfo = 
{
#if defined(STM32F1) || defined(STM32F0) || defined(GD32F1)
	(define::dma::priorityLevel::LOW << DMA_CCR_PL_Pos) |		 // uint32_t controlRegister1
	(define::dma::size::BYTE << DMA_CCR_MSIZE_Pos) |
	(define::dma::size::BYTE << DMA_CCR_PSIZE_Pos) |
	DMA_CCR_MINC_Msk | 
	(define::dma::dir::MEM_TO_PERI << DMA_CCR_DIR_Pos) | 
	DMA_CCR_TCIE_Msk | 
	DMA_CCR_TEIE_Msk | 
	DMA_CCR_EN_Msk ,
	0x0F << (4 * 4),									// uint32_t controlRegister2
	0x03 << (4 * 4),											// uint32_t controlRegister3
	(void*)&SPI2[SPI_REG::DR],									//void *dataRegister;
#elif defined(STM32F4)
	(define::dma1::stream4::SPI2_TX << DMA_SxCR_CHSEL_Pos) |	// uint32_t controlRegister1
	(define::dma::burst::SINGLE << DMA_SxCR_MBURST_Pos) | 
	(define::dma::burst::SINGLE << DMA_SxCR_PBURST_Pos) | 
	(define::dma::priorityLevel::LOW << DMA_SxCR_PL_Pos) |
	(define::dma::size::BYTE << DMA_SxCR_MSIZE_Pos) |
	(define::dma::size::BYTE << DMA_SxCR_PSIZE_Pos) |
	DMA_SxCR_MINC_Msk | 
	(define::dma::dir::MEM_TO_PERI << DMA_SxCR_DIR_Pos) | 
	DMA_SxCR_TCIE_Msk | 
	DMA_SxCR_TEIE_Msk | 
	DMA_SxCR_EN_Msk,
	0,															// uint32_t controlRegister2
	0,															// uint32_t controlRegister3
	(void*)&SPI2[SPI_REG::DR],									//void *dataRegister;
#endif
};

static const Dma::DmaInfo gSpi2RxDmaInfo = 
{
#if defined(STM32F1) || defined(STM32F0) || defined(GD32F1)
	(define::dma::priorityLevel::LOW << DMA_CCR_PL_Pos) |		// uint32_t controlRegister1
	(define::dma::size::BYTE << DMA_CCR_MSIZE_Pos) |
	(define::dma::size::BYTE << DMA_CCR_PSIZE_Pos) |
	DMA_CCR_MINC_Msk | 
	(define::dma::dir::PERI_TO_MEM << DMA_CCR_DIR_Pos) | 
	DMA_CCR_TCIE_Msk | 
	DMA_CCR_TEIE_Msk | 
	DMA_CCR_EN_Msk ,
	0x0F << (4 * 3),											// uint32_t controlRegister2
	0x03 << (4 * 3),											// uint32_t controlRegister3
	(void*)&SPI2[SPI_REG::DR],									//void *dataRegister;
#elif defined(STM32F4)
	(define::dma1::stream3::SPI2_RX << DMA_SxCR_CHSEL_Pos) |	// uint32_t controlRegister1
	(define::dma::burst::SINGLE << DMA_SxCR_MBURST_Pos) | 
	(define::dma::burst::SINGLE << DMA_SxCR_PBURST_Pos) | 
	(define::dma::priorityLevel::LOW << DMA_SxCR_PL_Pos) |
	(define::dma::size::BYTE << DMA_SxCR_MSIZE_Pos) |
	(define::dma::size::BYTE << DMA_SxCR_PSIZE_Pos) |
	DMA_SxCR_MINC_Msk | 
	(define::dma::dir::PERI_TO_MEM << DMA_SxCR_DIR_Pos) | 
	DMA_SxCR_TCIE_Msk | 
	DMA_SxCR_TEIE_Msk | 
	DMA_SxCR_EN_Msk,
	0,															// uint32_t controlRegister2
	0,															// uint32_t controlRegister3
	(void*)&SPI2[SPI_REG::DR],									//void *dataRegister;
#endif
};

static const Spi::Config gSpi2Config = 
{
	(volatile uint32_t*)SPI2,	//YSS_SPI_Peri *peri;
	dmaChannel5,	//Dma &txDma;
	gSpi2TxDmaInfo,	//Dma::DmaInfo txDmaInfo;
	dmaChannel4,	//Dma &rxDma;
	gSpi2RxDmaInfo	//Dma::DmaInfo rxDmaInfo;
};

Spi spi2(gDrvSpi2Config, gSpi2Config);

extern "C"
{
	void YSS_SPI2_IRQHandler(void)
	{
		spi2.isr();
	}
}
#endif

#if defined(SPI3_ENABLE) && defined(SPI3)
static void enableSpi3Clock(bool en)
{
	clock.lock();
    clock.enableApb1Clock(RCC_APB1ENR_SPI3EN_Pos, en);
	clock.unlock();
}

static void enableSpi3Interrupt(bool en)
{
	nvic.lock();
	nvic.enableInterrupt(SPI3_IRQn, en);
	nvic.unlock();
}

static void resetSpi3(void)
{
	clock.lock();
    clock.resetApb1(RCC_APB1RSTR_SPI3RST_Pos);
	clock.unlock();
}

static const Drv::Config gDrvSpi3Config
{
	enableSpi3Clock,		//void (*clockFunc)(bool en);
	enableSpi3Interrupt,	//void (*nvicFunc)(bool en);
	resetSpi3,				//void (*resetFunc)(void);
	getApb1ClockFrequency	//uint32_t (*getClockFreq)(void);
};

static const Dma::DmaInfo gSpi3TxDmaInfo = 
{
#if defined(STM32F1) || defined(GD32F1)
	(define::dma::priorityLevel::LOW << DMA_CCR_PL_Pos) |		// uint32_t controlRegister1
	(define::dma::size::BYTE << DMA_CCR_MSIZE_Pos) |
	(define::dma::size::BYTE << DMA_CCR_PSIZE_Pos) |
	DMA_CCR_MINC_Msk | 
	(define::dma::dir::MEM_TO_PERI << DMA_CCR_DIR_Pos) | 
	DMA_CCR_TCIE_Msk | 
	DMA_CCR_TEIE_Msk | 
	DMA_CCR_EN_Msk ,
	0,															// uint32_t controlRegister2
	0,															// uint32_t controlRegister3
	(void*)&SPI3[SPI_REG::DR],									//void *dataRegister;
#elif defined(STM32F4)
	(define::dma1::stream5::SPI3_TX << DMA_SxCR_CHSEL_Pos) |	// uint32_t controlRegister1
	(define::dma::burst::SINGLE << DMA_SxCR_MBURST_Pos) | 
	(define::dma::burst::SINGLE << DMA_SxCR_PBURST_Pos) | 
	(define::dma::priorityLevel::LOW << DMA_SxCR_PL_Pos) |
	(define::dma::size::BYTE << DMA_SxCR_MSIZE_Pos) |
	(define::dma::size::BYTE << DMA_SxCR_PSIZE_Pos) |
	DMA_SxCR_MINC_Msk | 
	(define::dma::dir::MEM_TO_PERI << DMA_SxCR_DIR_Pos) | 
	DMA_SxCR_TCIE_Msk | 
	DMA_SxCR_TEIE_Msk | 
	DMA_SxCR_EN_Msk,
	0,															// uint32_t controlRegister2
	0,															// uint32_t controlRegister3
	(void*)&SPI3[SPI_REG::DR],									//void *dataRegister;
#endif
};

static const Dma::DmaInfo gSpi3RxDmaInfo = 
{
#if defined(STM32F1) || defined(GD32F1)
	(define::dma::priorityLevel::LOW << DMA_CCR_PL_Pos) |		// uint32_t controlRegister1
	(define::dma::size::BYTE << DMA_CCR_MSIZE_Pos) |
	(define::dma::size::BYTE << DMA_CCR_PSIZE_Pos) |
	DMA_CCR_MINC_Msk | 
	(define::dma::dir::PERI_TO_MEM << DMA_CCR_DIR_Pos) | 
	DMA_CCR_TCIE_Msk | 
	DMA_CCR_TEIE_Msk | 
	DMA_CCR_EN_Msk ,
	0,															// uint32_t controlRegister2
	0,															// uint32_t controlRegister3
	(void*)&SPI3[SPI_REG::DR],									//void *dataRegister;
#elif defined(STM32F4)
	(define::dma1::stream0::SPI3_RX << DMA_SxCR_CHSEL_Pos) |	// uint32_t controlRegister1
	(define::dma::burst::SINGLE << DMA_SxCR_MBURST_Pos) | 
	(define::dma::burst::SINGLE << DMA_SxCR_PBURST_Pos) | 
	(define::dma::priorityLevel::LOW << DMA_SxCR_PL_Pos) |
	(define::dma::size::BYTE << DMA_SxCR_MSIZE_Pos) |
	(define::dma::size::BYTE << DMA_SxCR_PSIZE_Pos) |
	DMA_SxCR_MINC_Msk | 
	(define::dma::dir::PERI_TO_MEM << DMA_SxCR_DIR_Pos) | 
	DMA_SxCR_TCIE_Msk | 
	DMA_SxCR_TEIE_Msk | 
	DMA_SxCR_EN_Msk,
	0,															// uint32_t controlRegister2
	0,															// uint32_t controlRegister3
	(void*)&SPI3[SPI_REG::DR],									//void *dataRegister;
#endif
};

static const Spi::Config gSpi3Config
{
	(volatile uint32_t*)SPI3,	//YSS_SPI_Peri *peri;
	dmaChannel6,	//Dma &txDma;
	gSpi3TxDmaInfo,	//Dma::DmaInfo txDmaInfo;
	dmaChannel1,	//Dma &rxDma;
	gSpi3RxDmaInfo	//Dma::DmaInfo rxDmaInfo;
};

Spi spi3(gDrvSpi3Config, gSpi3Config);

extern "C"
{
	void YSS_SPI3_IRQHandler(void)
	{
		spi3.isr();
	}
}
#endif



#if defined(SPI4_ENABLE) && defined(SPI4)
static void enableSpi4Clock(bool en)
{
	clock.lock();
    clock.enableApb1Clock(RCC_APB2ENR_SPI4EN_Pos, en);
	clock.unlock();
}

static void enableSpi4Interrupt(bool en)
{
	nvic.lock();
	nvic.enableInterrupt(SPI4_IRQn, en);
	nvic.unlock();
}

static void resetSpi4(void)
{
	clock.lock();
    clock.resetApb2(RCC_APB2RSTR_SPI4RST_Pos);
	clock.unlock();
}

static const Drv::Config gDrvSpi4Config
{
	enableSpi4Clock,		//void (*clockFunc)(bool en);
	enableSpi4Interrupt,	//void (*nvicFunc)(bool en);
	resetSpi4,				//void (*resetFunc)(void);
	getApb2ClockFrequency	//uint32_t (*getClockFreq)(void);
};

static const Dma::DmaInfo gSpi4TxDmaInfo = 
{
	(define::dma2::stream1::SPI4_TX << DMA_SxCR_CHSEL_Pos) |	// uint32_t controlRegister1
	(define::dma::burst::SINGLE << DMA_SxCR_MBURST_Pos) | 
	(define::dma::burst::SINGLE << DMA_SxCR_PBURST_Pos) | 
	(define::dma::priorityLevel::LOW << DMA_SxCR_PL_Pos) |
	(define::dma::size::BYTE << DMA_SxCR_MSIZE_Pos) |
	(define::dma::size::BYTE << DMA_SxCR_PSIZE_Pos) |
	DMA_SxCR_MINC_Msk | 
	(define::dma::dir::MEM_TO_PERI << DMA_SxCR_DIR_Pos) | 
	DMA_SxCR_TCIE_Msk | 
	DMA_SxCR_TEIE_Msk | 
	DMA_SxCR_EN_Msk,
	0,															// uint32_t controlRegister2
	0,															// uint32_t controlRegister3
	(void*)&SPI4[SPI_REG::DR],									//void *dataRegister;
};

static const Dma::DmaInfo gSpi4RxDmaInfo = 
{
	(define::dma2::stream0::SPI4_RX << DMA_SxCR_CHSEL_Pos) |	// uint32_t controlRegister1
	(define::dma::burst::SINGLE << DMA_SxCR_MBURST_Pos) | 
	(define::dma::burst::SINGLE << DMA_SxCR_PBURST_Pos) | 
	(define::dma::priorityLevel::LOW << DMA_SxCR_PL_Pos) |
	(define::dma::size::BYTE << DMA_SxCR_MSIZE_Pos) |
	(define::dma::size::BYTE << DMA_SxCR_PSIZE_Pos) |
	DMA_SxCR_MINC_Msk | 
	(define::dma::dir::PERI_TO_MEM << DMA_SxCR_DIR_Pos) | 
	DMA_SxCR_TCIE_Msk | 
	DMA_SxCR_TEIE_Msk | 
	DMA_SxCR_EN_Msk,
	DMA_SxFCR_DMDIS,											// uint32_t controlRegister2
	0,															// uint32_t controlRegister3
	(void*)&SPI4[SPI_REG::DR],									//void *dataRegister;
};

static const Spi::Config gSpi4Config
{
	(volatile uint32_t*)SPI4,	//YSS_SPI_Peri *peri;
	dmaChannel10,	//Dma &txDma;
	gSpi4TxDmaInfo,	//Dma::DmaInfo txDmaInfo;
	dmaChannel9,	//Dma &rxDma;
	gSpi4RxDmaInfo	//Dma::DmaInfo rxDmaInfo;
};

Spi spi4(gDrvSpi4Config, gSpi4Config);

extern "C"
{
	void YSS_SPI4_IRQHandler(void)
	{
		spi4.isr();
	}
}
#endif



#if defined(SPI5_ENABLE) && defined(SPI5)
static void enableSpi5Clock(bool en)
{
	clock.lock();
    clock.enableApb2Clock(RCC_APB2ENR_SPI5EN_Pos, en);
	clock.unlock();
}

static void enableSpi5Interrupt(bool en)
{
	nvic.lock();
	nvic.enableInterrupt(SPI5_IRQn, en);
	nvic.unlock();
}

static void resetSpi5(void)
{
	clock.lock();
    clock.resetApb2(RCC_APB2RSTR_SPI5RST_Pos);
	clock.unlock();
}

static const Drv::Config gDrvSpi5Config
{
	enableSpi5Clock,		//void (*clockFunc)(bool en);
	enableSpi5Interrupt,	//void (*nvicFunc)(bool en);
	resetSpi5,				//void (*resetFunc)(void);
	getApb2ClockFrequency	//uint32_t (*getClockFreq)(void);
};

static const Dma::DmaInfo gSpi5TxDmaInfo = 
{
	(define::dma2::stream4::SPI5_TX << DMA_SxCR_CHSEL_Pos) |	// uint32_t controlRegister1
	(define::dma::burst::SINGLE << DMA_SxCR_MBURST_Pos) | 
	(define::dma::burst::SINGLE << DMA_SxCR_PBURST_Pos) | 
	(define::dma::priorityLevel::LOW << DMA_SxCR_PL_Pos) |
	(define::dma::size::BYTE << DMA_SxCR_MSIZE_Pos) |
	(define::dma::size::BYTE << DMA_SxCR_PSIZE_Pos) |
	DMA_SxCR_MINC_Msk | 
	(define::dma::dir::MEM_TO_PERI << DMA_SxCR_DIR_Pos) | 
	DMA_SxCR_TCIE_Msk | 
	DMA_SxCR_TEIE_Msk | 
	DMA_SxCR_EN_Msk,
	0,															// uint32_t controlRegister2
	0,															// uint32_t controlRegister3
	(void*)&SPI5[SPI_REG::DR],									//void *dataRegister;
};

static const Dma::DmaInfo gSpi5RxDmaInfo = 
{
	(define::dma2::stream3::SPI5_RX << DMA_SxCR_CHSEL_Pos) |	// uint32_t controlRegister1
	(define::dma::burst::SINGLE << DMA_SxCR_MBURST_Pos) | 
	(define::dma::burst::SINGLE << DMA_SxCR_PBURST_Pos) | 
	(define::dma::priorityLevel::LOW << DMA_SxCR_PL_Pos) |
	(define::dma::size::BYTE << DMA_SxCR_MSIZE_Pos) |
	(define::dma::size::BYTE << DMA_SxCR_PSIZE_Pos) |
	DMA_SxCR_MINC_Msk | 
	(define::dma::dir::PERI_TO_MEM << DMA_SxCR_DIR_Pos) | 
	DMA_SxCR_TCIE_Msk | 
	DMA_SxCR_TEIE_Msk | 
	DMA_SxCR_EN_Msk,
	0,															// uint32_t controlRegister2
	0,															// uint32_t controlRegister3
	(void*)&SPI5[SPI_REG::DR],									//void *dataRegister;
};

static const Spi::Config gSpi5Config
{
	(volatile uint32_t*)SPI5,	//YSS_SPI_Peri *peri;
	dmaChannel13,	//Dma &txDma;
	gSpi5TxDmaInfo,	//Dma::DmaInfo txDmaInfo;
	dmaChannel12,	//Dma &rxDma;
	gSpi5RxDmaInfo	//Dma::DmaInfo rxDmaInfo;
};

Spi spi5(gDrvSpi5Config, gSpi5Config);

extern "C"
{
	void YSS_SPI5_IRQHandler(void)
	{
		spi5.isr();
	}
}
#endif

#endif

