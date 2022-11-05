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

#include <drv/mcu.h>

#if false

#include <stdint.h>
#include <drv/peripheral.h>
#include <drv/I2s.h>
#include <yss/thread.h>

enum
{
	CTLR1 = 0, CTLR2, STR, DTR,
	CPR, RCR, TCR, I2SCTLR,
	I2SPSC
};

I2s::I2s(const Drv::Config drvConfig, const Config config) : Drv(drvConfig)
{
	mPeri = config.peri;
	mTxDma = &config.txDma;
	mTxDmaInfo = &config.txDmaInfo;
	mRxDma = &config.rxDma;
	mRxDmaInfo = &config.rxDmaInfo;
	mLastSpec = 0;
}

error I2s::setSpecification(const Specification &spec)
{
	return Error::NOT_READY;
}

error I2s::initAsMain(void)
{
	uint32_t multiple = 384;
	uint32_t lrck = 128000;
	uint32_t mclk = 49152000;
	uint32_t clock = getClockFrequency();

	setBitData(mPeri[I2SCTLR], false, 10);	// SPI 비활성화
	
	mPeri[I2SPSC] = 1 << 9 | 0 << 8 | 1 << 0;
	mPeri[I2SCTLR] = 0 << 0 | 0 << 1 | 1 << 3 | 1 << 4 | 2 << 8 | 1 << 11;
	mPeri[CTLR2] = SPI_CTLR2_DMATE;
	setBitData(mPeri[I2SCTLR], true, 10);	// SPI 활성화

	return Error::NOT_INITIALIZED;
}

void I2s::transferAsCircularMode(void *src, uint16_t size)
{
	mTxDma->lock();
	return mTxDma->transferAsCircularMode(mTxDmaInfo, src, size);
}

void I2s::setThreadIdOfTransferCircularDataHandler(void)
{
	mTxDma->setThreadIdOfTransferCircularDataHandler();
}

uint16_t I2s::getCurrentTransferBufferCount(void)
{
	return mTxDma->getCurrentTransferBufferCount();
}

void I2s::stop(void)
{
	mTxDma->stop();
	mTxDma->unlock();
}

void I2s::isr(void)
{
	mPeri[CTLR2] = 0;
	thread::signal(mThreadId);
}



#endif
