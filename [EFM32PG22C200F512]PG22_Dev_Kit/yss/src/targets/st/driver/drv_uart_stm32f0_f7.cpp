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

#if defined(STM32F0_N) || defined(STM32F7_N)

#include <drv/peripheral.h>
#include <drv/Uart.h>
#include <yss/reg.h>
#include <yss/thread.h>

#if defined(STM32F767xx)
#include <targets/st/bitfield_stm32f767xx.h>
#elif defined(STM32F030xC)
#include <targets/st/bitfield_stm32f030xx.h>
#elif defined(STM32F746xx)
#include <targets/st/bitfield_stm32f746xx.h>
#endif

Uart::Uart(const Drv::Config drvConfig, const Config config) : Drv(drvConfig)
{
	mTxDma = &config.txDma;
	mTxDmaInfo = config.txDmaInfo;
	mDev = config.dev;
	mRcvBuf = 0;
	mTail = 0;
	mHead = 0;
	mOneWireModeFlag = false;
	mIsrForFrameError = 0;
}

error Uart::initializeAsTransmitterOnly(int32_t baud)
{
	int32_t  man, fra, buf;
	int32_t  clk = Drv::getClockFrequency() >> 4;

	man = clk / baud;
	man &= 0xfff;
	fra = 16 * (clk % baud) / baud;
	fra &= 0xf;
	
	// 장치 비활성화
	setBitData(mDev->CR1, false, 13);
	
	// 보레이트 설정
	setTwoFieldData(mDev->BRR, 0xFFF << 4, man, 4, 0xF << 0, fra, 0);
	
	// TX En, 장치 En
	mDev->CR3 = USART_CR3_EIE_Msk;
	mDev->CR1 = USART_CR1_TE_Msk | USART_CR1_UE_Msk;

	return error::ERROR_NONE;
}

error Uart::initialize(int32_t  baud, void *receiveBuffer, int32_t  receiveBufferSize)
{
	int32_t  man, fra, buf;
	int32_t  clk = Drv::getClockFrequency() >> 4;

	mRcvBuf = (int8_t*)receiveBuffer;
	mRcvBufSize = receiveBufferSize;

	man = clk / baud;
	man &= 0xfff;
	fra = 16 * (clk % baud) / baud;
	fra &= 0xf;
	
	// 장치 비활성화
	setBitData(mDev->CR1, false, 13);
	
	// 보레이트 설정
	setTwoFieldData(mDev->BRR, 0xFFF << 4, man, 4, 0xF << 0, fra, 0);
	
	// TX En, RX En, Rxnei En, 장치 En
	mDev->CR3 = USART_CR3_EIE_Msk;
	mDev->CR1 = USART_CR1_TE_Msk | USART_CR1_RE_Msk | USART_CR1_RXNEIE_Msk | USART_CR1_UE_Msk;

	return error::ERROR_NONE;
}

error Uart::changeBaudrate(int32_t baud)
{
	int32_t  man, fra;
	int32_t  clk = Drv::getClockFrequency() >> 4;

	man = clk / baud;
	man &= 0xfff;
	fra = 16 * (clk % baud) / baud;
	fra &= 0xf;

	mDev->CR1 &= ~USART_CR1_UE_Msk;
	
	setTwoFieldData(mDev->BRR, 0xFFF << 4, man, 4, 0xF << 0, fra, 0);

	mDev->CR1 |= USART_CR1_UE_Msk;

	return error::ERROR_NONE;
}

error Uart::send(void *src, int32_t  size)
{
	error result;

	if(size == 0)
		return error::ERROR_NONE;

	if(mTxDma == 0)
		return error::DMA;

	mTxDma->lock();

	setBitData(mDev->CR3, true, USART_CR3_DMAT_Pos);	// TX DMA 활성화

	mDev->ICR = USART_ICR_TCCF_Msk;

	if(mOneWireModeFlag)
		setBitData(mDev->CR1, false, USART_CR1_RE_Pos);	// RX 비활성화
	
	result = mTxDma->send(mTxDmaInfo, src, size);

	if(result == error::ERROR_NONE)
		while (!(mDev->ISR & USART_ISR_TC_Msk))
			thread::yield();

	if(mOneWireModeFlag)
		setBitData(mDev->CR1, true, USART_CR1_RE_Pos);	// RX 활성화
	
	setBitData(mDev->CR3, false, USART_CR3_DMAT_Pos);		// TX DMA 비활성화

	mTxDma->unlock();

	return result;
}

void Uart::send(int8_t data)
{
	if(mOneWireModeFlag)
		setBitData(mDev->CR1, false, USART_CR1_RE_Pos);	// RX 비활성화

	mDev->ICR = USART_ICR_TCCF_Msk;
	mDev->TDR = data;
	while (~mDev->ISR & USART_ISR_TC)
		thread::yield();

	if(mOneWireModeFlag)
		setBitData(mDev->CR1, true, USART_CR1_RE_Pos);	// RX 활성화
}

void Uart::setIsrForFrameError(void (*isr)(void))
{
	mIsrForFrameError = isr;
}

void Uart::isr(void)
{
	uint32_t sr = mDev->ISR;

	if(sr & (USART_ISR_FE_Msk | USART_ISR_ORE_Msk | USART_ISR_NE_Msk))
	{
		if(sr & USART_ISR_FE_Msk && mIsrForFrameError)
			mIsrForFrameError();

		if(sr & USART_ISR_ORE_Msk)
			__NOP();

		mDev->RDR;
		mDev->ICR = USART_ICR_FECF_Msk | USART_ICR_ORECF_Msk | USART_ICR_NCF_Msk;
	}
	else
		push(mDev->RDR);
}

#endif

