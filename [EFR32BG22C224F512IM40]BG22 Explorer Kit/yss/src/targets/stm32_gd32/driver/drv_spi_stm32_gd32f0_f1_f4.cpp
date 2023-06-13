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

#if defined(GD32F4) || defined(STM32F1) || defined(STM32F4) || defined(STM32F0)

#include <stdint.h>
#include <drv/peripheral.h>
#include <drv/Spi.h>
#include <yss/thread.h>
#include <yss/reg.h>
#include <targets/st_gigadevice/spi_stm32_gd32f0_f1_f4.h>

Spi::Spi(const Drv::Config drvConfig, const Config config) : Drv(drvConfig)
{
	mPeri = config.peri;
	mTxDma = &config.txDma;
	mTxDmaInfo = config.txDmaInfo;
	mRxDma = &config.rxDma;
	mRxDmaInfo = config.rxDmaInfo;
	mLastSpec = 0;
}

bool Spi::setSpecification(const Specification &spec)
{
	uint32_t reg, buf;

	if (mLastSpec == &spec)
		return true;
	mLastSpec = &spec;

	uint32_t mod;
	uint32_t div, clk = Drv::getClockFrequency();

	div = clk / spec.maxFreq;
	if (clk % spec.maxFreq)
		div++;

	if (div <= 2)
		div = 0;
	else if (div <= 4)
		div = 1;
	else if (div <= 8)
		div = 2;
	else if (div <= 16)
		div = 3;
	else if (div <= 32)
		div = 4;
	else if (div <= 64)
		div = 5;
	else if (div <= 128)
		div = 6;
	else if (div <= 256)
		div = 7;
	else
		return false;
	
	using namespace define::spi;

	switch(spec.bit)
	{
	case bit::BIT8 :
		buf = 0;
		break;
	case bit::BIT16 :
		buf = 1;
		break;
	default :
		buf = 0;
	}

	reg = mPeri[SPI_REG::CR1];
	reg &= ~(SPI_CR1_BR_Msk | SPI_CR1_CPHA_Msk | SPI_CR1_CPOL_Msk | SPI_CR1_DFF_Msk);
	reg |= spec.mode << SPI_CR1_CPHA_Pos | div << SPI_CR1_BR_Pos | buf << SPI_CR1_DFF_Pos;
	mPeri[SPI_REG::CR1] = reg;
	mPeri[SPI_REG::DR];

	return true;
}

error Spi::initialize(void)
{
	setBitData(mPeri[SPI_REG::CR1], false, 6);	// SPI 비활성화

	mPeri[SPI_REG::CR1] |= SPI_CR1_SSI_Msk | SPI_CR1_SSM_Msk | SPI_CR1_MSTR_Msk;

	return error::ERROR_NONE;
}

void Spi::enable(bool en)
{
	setBitData(mPeri[SPI_REG::CR1], en, 6);
}

error Spi::send(void *src, int32_t  size)
{
	error result;

	if(size == 1)
	{
		send(*(int8_t*)src);
		return error::ERROR_NONE;
	}

	mTxDma->lock();
#if defined(STM32F0)
	mPeri[SPI_REG::CR2] = SPI_CR2_TXDMAEN_Msk | 7 << 8 | 1 << 12;
#else
	mPeri[SPI_REG::CR2] = SPI_CR2_TXDMAEN_Msk;
#endif
	mThreadId = thread::getCurrentThreadId();

	result = mTxDma->send(mTxDmaInfo, src, size);
	mTxDma->unlock();
	
	if(mPeri[SPI_REG::SR] & SPI_SR_BSY_Msk)
	{
		mPeri[SPI_REG::DR];
#if defined(STM32F0)
		mPeri[SPI_REG::CR2] = SPI_CR2_RXNEIE_Msk | 7 << 8 | 1 << 12;
#else
		mPeri[SPI_REG::CR2] = SPI_CR2_RXNEIE_Msk;
#endif
		while(mPeri[SPI_REG::SR] & SPI_SR_BSY_Msk)
			thread::yield();
	}
	
	mPeri[SPI_REG::DR];
	
	return result;
}

error Spi::exchange(void *des, int32_t  size)
{
	bool rt = false;

	if(size == 1)
	{
		*(int8_t*)des = exchange(*(int8_t*)des);
		return error::ERROR_NONE;
	}

	mPeri[SPI_REG::DR];

	mRxDma->lock();
	mTxDma->lock();

#if defined(STM32F0)
	mPeri[SPI_REG::CR2] = SPI_CR2_TXDMAEN_Msk | SPI_CR2_RXDMAEN_Msk | 7 << 8 | 1 << 12;
#else
	mPeri[SPI_REG::CR2] = SPI_CR2_TXDMAEN_Msk | SPI_CR2_RXDMAEN_Msk;
#endif

	mRxDma->ready(mRxDmaInfo, des, size);
	rt = mTxDma->send(mTxDmaInfo, des, size);
	
	while(!mRxDma->isComplete())
		thread::yield();

#if defined(STM32F0)
	mPeri[SPI_REG::CR2] = 7 << 8;
#else
	mPeri[SPI_REG::CR2] = 0;
#endif

	mRxDma->stop();
	mRxDma->unlock();
	mTxDma->unlock();

	return rt;
}

int8_t Spi::exchange(int8_t data)
{
	mThreadId = thread::getCurrentThreadId();
#if defined(STM32F0)
	mPeri[SPI_REG::CR2] = SPI_CR2_RXNEIE_Msk | 7 << 8 | 1 << 12;
#else
	mPeri[SPI_REG::CR2] = SPI_CR2_RXNEIE_Msk;
#endif
	*(int8_t*)&mPeri[SPI_REG::DR] = data;
	while (~mPeri[SPI_REG::SR] & SPI_SR_RXNE_Msk)
		thread::yield();

	return mPeri[SPI_REG::DR];
}

void Spi::send(int8_t data)
{
	mPeri[SPI_REG::DR];
	mThreadId = thread::getCurrentThreadId();
#if defined(STM32F0)
	mPeri[SPI_REG::CR2] = SPI_CR2_RXNEIE_Msk | 7 << 8 | 1 << 12;
#else
	mPeri[SPI_REG::CR2] = SPI_CR2_RXNEIE_Msk;
#endif
	*(int8_t*)&mPeri[SPI_REG::DR] = data;
	while (~mPeri[SPI_REG::SR] & SPI_SR_RXNE_Msk)
		thread::yield();

	mPeri[SPI_REG::DR];
}

void Spi::isr(void)
{
#if defined(STM32F0)
	mPeri[SPI_REG::CR2] = 7 << 8 | 1 << 12;
#else
	mPeri[SPI_REG::CR2] = 0;
#endif
	thread::signal(mThreadId);
}

#endif
