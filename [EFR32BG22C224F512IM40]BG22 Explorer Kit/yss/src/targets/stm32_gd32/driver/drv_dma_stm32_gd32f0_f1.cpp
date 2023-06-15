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

#if defined(STM32F1) || defined(STM32F0)

#include <drv/peripheral.h>
#include <targets/st_gigadevice/dma_stm32_gd32f1.h>
#include <drv/Dma.h>
#include <util/ElapsedTime.h>
#include <yss/reg.h>
#include <yss/thread.h>

#define checkError(sr) (sr & 0x08)
#define checkComplete(sr) (sr & 0x03)

static Mutex gMutex;

Dma::Dma(const Drv::Config drvConfig, const Config dmaConfig) : Drv(drvConfig)
{
	mDma = dmaConfig.dma;
	mPeri = dmaConfig.peri;
	mCompleteFlag = false;
	mErrorFlag = false;
	mAddr = 0;
	mRemainSize = 0;
}

void Dma::initialize(void)
{
}

error Dma::ready(DmaInfo &dmaInfo, void *buffer, int32_t  size)
{
	if(size == 0)
		return error::NO_DATA;

	mCompleteFlag = false;
	mErrorFlag = false;
	mThreadId = thread::getCurrentThreadId();

	if (size > 0xF000)
	{
		mPeri->CPAR = (uint32_t)dmaInfo.dataRegister;
		mPeri[DMA_REG::CNDTR] = 0xF000;
		mPeri[DMA_REG::CMAR] = (uint32_t)buffer;
		mAddr = (uint32_t)buffer;
		mRemainSize = size - 0xF000;
#if defined(STM32F0)
		mDma[DMA_REG::CSELR] &= ~dmaInfo.controlRegister2;
		mDma[DMA_REG::CSELR] |= dmaInfo.controlRegister3;
#endif
		mPeri[DMA_REG::CCR] = dmaInfo.controlRegister1;
	}
	else
	{
		mPeri->CPAR = (uint32_t)dmaInfo.dataRegister;
		mPeri[DMA_REG::CNDTR] = size;
		mPeri[DMA_REG::CMAR] = (uint32_t)buffer;
		mRemainSize = 0;
#if defined(STM32F0)
		gMutex.lock();
		mDma[DMA_REG::CSELR] &= ~dmaInfo.controlRegister2;
		mDma[DMA_REG::CSELR] |= dmaInfo.controlRegister3;
		gMutex.unlock();
#endif
		mPeri[DMA_REG::CCR] = dmaInfo.controlRegister1;
	}

	return error::ERROR_NONE;
}

error Dma::send(DmaInfo &dmaInfo, void *src, int32_t  size)
{
	uint32_t addr = (uint32_t)src;

	if(size == 0)
		return error::NO_DATA;

	mCompleteFlag = false;
	mErrorFlag = false;
	mThreadId = thread::getCurrentThreadId();
	
	if (size > 0xF000)
	{
		mPeri->CPAR = (uint32_t)dmaInfo.dataRegister;
		mPeri[DMA_REG::CNDTR] = 0xF000;
		mPeri[DMA_REG::CMAR] = addr;
		mAddr = addr;
		mRemainSize = size - 0xF000;
#if defined(STM32F0)
		gMutex.lock();
		mDma[DMA_REG::CSELR] &= ~dmaInfo.controlRegister2;
		mDma[DMA_REG::CSELR] |= dmaInfo.controlRegister3;
		gMutex.unlock();
#endif
		mPeri[DMA_REG::CCR] = dmaInfo.controlRegister1;
	}
	else
	{
		mPeri->CPAR = (uint32_t)dmaInfo.dataRegister;
		mPeri[DMA_REG::CNDTR] = size;
		mPeri[DMA_REG::CMAR] = addr;
		mRemainSize = 0;
#if defined(STM32F0)
		gMutex.lock();
		mDma[DMA_REG::CSELR] &= ~dmaInfo.controlRegister2;
		mDma[DMA_REG::CSELR] |= dmaInfo.controlRegister3;
		gMutex.unlock();
#endif
		mPeri[DMA_REG::CCR] = dmaInfo.controlRegister1;
	}

	while (!mCompleteFlag && !mErrorFlag && mPeri[DMA_REG::CNDTR])
	{
		thread::yield();
	}
	
	mPeri[DMA_REG::CCR] &= ~DMA_CCR_EN_Msk;

	if(mErrorFlag)
		return error::DMA;
	else
		return error::ERROR_NONE;
}

error Dma::receive(DmaInfo &dmaInfo, void *des, int32_t  size)
{
	if(size == 0)
		return error::NO_DATA;

	mCompleteFlag = false;
	mErrorFlag = false;
	mThreadId = thread::getCurrentThreadId();

	if (size > 0xF000)
	{
		mPeri->CPAR = (int32_t )dmaInfo.dataRegister;
		mPeri[DMA_REG::CNDTR] = 0xF000;
		mPeri[DMA_REG::CMAR] = (int32_t )des;
		mAddr = (int32_t )des;
		mRemainSize = size - 0xF000;
#if defined(STM32F0)
		gMutex.lock();
		mDma[DMA_REG::CSELR] &= ~dmaInfo.controlRegister2;
		mDma[DMA_REG::CSELR] |= dmaInfo.controlRegister3;
		gMutex.unlock();
#endif
		mPeri[DMA_REG::CCR] = dmaInfo.controlRegister1;
	}
	else
	{
		mPeri->CPAR = (int32_t )dmaInfo.dataRegister;
		mPeri[DMA_REG::CNDTR] = size;
		mPeri[DMA_REG::CMAR] = (int32_t )des;
		mRemainSize = 0;
		mPeri[DMA_REG::CCR] = dmaInfo.controlRegister1;
	}

	while (!mCompleteFlag && !mErrorFlag && mPeri[DMA_REG::CNDTR])
	{
		thread::yield();
	}

	mPeri[DMA_REG::CCR] &= ~DMA_CCR_EN_Msk;

	if (mErrorFlag)
		return error::DMA;
	else
		return error::ERROR_NONE;
}

void Dma::stop(void)
{
	mPeri[DMA_REG::CCR] &= ~DMA_CCR_EN_Msk;
}

bool Dma::isError(void)
{
	return mErrorFlag;
}

bool Dma::isComplete(void)
{
	return mCompleteFlag;
}

DmaChannel1::DmaChannel1(const Drv::Config drvConfig, const Dma::Config dmaConfig, const Config config) : Dma(drvConfig, dmaConfig)
{
	
}

void DmaChannel1::isr(void)
{
	register uint32_t sr = getFieldData(mDma[DMA_REG::ISR], 0xF << 0, 0);
	setFieldData(mDma[DMA_REG::IFCR], 0xF << 0, sr, 0);

	if (checkError(sr))
		mErrorFlag = true;

	if (mRemainSize)
	{
		mAddr += 0xF000;
		if (mRemainSize > 0xF000)
		{
			mPeri[DMA_REG::CNDTR] = 0xF000;
			mRemainSize -= 0xF000;
		}
		else
		{
			mPeri[DMA_REG::CNDTR] = mRemainSize;
			mRemainSize = 0;
		}

		mPeri[DMA_REG::CMAR] = mAddr;
		mPeri[DMA_REG::CCR] |= DMA_CCR_EN_Msk;
	}
	else if (checkComplete(sr))
		mCompleteFlag = true;
	
	thread::signal(mThreadId);
}

DmaChannel2::DmaChannel2(const Drv::Config drvConfig, const Dma::Config dmaConfig, const Config config) : Dma(drvConfig, dmaConfig)
{
	
}

void DmaChannel2::isr(void)
{
	register uint32_t sr = getFieldData(mDma[DMA_REG::ISR], 0xF << 4, 4);
	setFieldData(mDma[DMA_REG::IFCR], 0xF << 4, sr, 4);

	if (checkError(sr))
		mErrorFlag = true;
	if (mRemainSize)
	{
		mAddr += 0xF000;
		if (mRemainSize > 0xF000)
		{
			mPeri[DMA_REG::CNDTR] = 0xF000;
			mRemainSize -= 0xF000;
		}
		else
		{
			mPeri[DMA_REG::CNDTR] = mRemainSize;
			mRemainSize = 0;
		}

		mPeri[DMA_REG::CMAR] = mAddr;
		mPeri[DMA_REG::CCR] |= DMA_CCR_EN_Msk;
	}
	else if (checkComplete(sr))
		mCompleteFlag = true;

	thread::signal(mThreadId);
}



DmaChannel3::DmaChannel3(const Drv::Config drvConfig, const Dma::Config dmaConfig, const Config config) : Dma(drvConfig, dmaConfig)
{
	
}

void DmaChannel3::isr(void)
{
	register uint32_t sr = getFieldData(mDma[DMA_REG::ISR], 0xF << 8, 8);
	setFieldData(mDma[DMA_REG::IFCR], 0xF << 8, sr, 8);

	if (checkError(sr))
		mErrorFlag = true;

	if (mRemainSize)
	{
		mPeri[DMA_REG::CCR] &= ~DMA_CCR_EN_Msk;
		mAddr += 0xF000;
		if (mRemainSize > 0xF000)
		{
			mPeri[DMA_REG::CNDTR] = 0xF000;
			mRemainSize -= 0xF000;
		}
		else
		{
			mPeri[DMA_REG::CNDTR] = mRemainSize;
			mRemainSize = 0;
		}

		mPeri[DMA_REG::CMAR] = mAddr;
		mPeri[DMA_REG::CCR] |= DMA_CCR_EN_Msk;
	}
	else if (checkComplete(sr))
		mCompleteFlag = true;

	thread::signal(mThreadId);
}



DmaChannel4::DmaChannel4(const Drv::Config drvConfig, const Dma::Config dmaConfig, const Config config) : Dma(drvConfig, dmaConfig)
{
	
}

void DmaChannel4::isr(void)
{
	register uint32_t sr = getFieldData(mDma[DMA_REG::ISR], 0xF << 12, 12);
	mDma[DMA_REG::IFCR] = sr << 12;

	if (checkError(sr))
		mErrorFlag = true;

	if (mRemainSize)
	{
		mAddr += 0xF000;
		if (mRemainSize > 0xF000)
		{
			mPeri[DMA_REG::CNDTR] = 0xF000;
			mRemainSize -= 0xF000;
		}
		else
		{
			mPeri[DMA_REG::CNDTR] = mRemainSize;
			mRemainSize = 0;
		}

		mPeri[DMA_REG::CMAR] = mAddr;
		mPeri[DMA_REG::CCR] |= DMA_CCR_EN_Msk;
	}
	else if (checkComplete(sr))
		mCompleteFlag = true;

	thread::signal(mThreadId);
}



DmaChannel5::DmaChannel5(const Drv::Config drvConfig, const Dma::Config dmaConfig, const Config config) : Dma(drvConfig, dmaConfig)
{
	
}

void DmaChannel5::isr(void)
{
	register uint32_t sr = getFieldData(mDma[DMA_REG::ISR], 0xF << 16, 16);
	mDma[DMA_REG::IFCR] = sr << 16;

	if (checkError(sr))
		mErrorFlag = true;

	if (mRemainSize)
	{
		mAddr += 0xF000;
		if (mRemainSize > 0xF000)
		{
			mPeri[DMA_REG::CNDTR] = 0xF000;
			mRemainSize -= 0xF000;
		}
		else
		{
			mPeri[DMA_REG::CNDTR] = mRemainSize;
			mRemainSize = 0;
		}

		mPeri[DMA_REG::CMAR] = mAddr;
		mPeri[DMA_REG::CCR] |= DMA_CCR_EN_Msk;
	}
	else if (checkComplete(sr))
		mCompleteFlag = true;

	thread::signal(mThreadId);
}



DmaChannel6::DmaChannel6(const Drv::Config drvConfig, const Dma::Config dmaConfig, const Config config) : Dma(drvConfig, dmaConfig)
{
	
}

void DmaChannel6::isr(void)
{
	register uint32_t sr = getFieldData(mDma[DMA_REG::ISR], 0xF << 20, 20);
	setFieldData(mDma[DMA_REG::IFCR], 0xF << 20, sr, 20);

	if (checkError(sr))
		mErrorFlag = true;

	if (mRemainSize)
	{
		mAddr += 0xF000;
		if (mRemainSize > 0xF000)
		{
			mPeri[DMA_REG::CNDTR] = 0xF000;
			mRemainSize -= 0xF000;
		}
		else
		{
			mPeri[DMA_REG::CNDTR] = mRemainSize;
			mRemainSize = 0;
		}

		mPeri[DMA_REG::CMAR] = mAddr;
		mPeri[DMA_REG::CCR] |= DMA_CCR_EN_Msk;
	}
	else if (checkComplete(sr))
		mCompleteFlag = true;

	thread::signal(mThreadId);
}



DmaChannel7::DmaChannel7(const Drv::Config drvConfig, const Dma::Config dmaConfig, const Config config) : Dma(drvConfig, dmaConfig)
{
	
}

void DmaChannel7::isr(void)
{
	register uint32_t sr = getFieldData(mDma[DMA_REG::ISR], 0xF << 24, 24);
	setFieldData(mDma[DMA_REG::IFCR], 0xF << 24, sr, 24);

	if (checkError(sr))
		mErrorFlag = true;

	if (mRemainSize)
	{
		mAddr += 0xF000;
		if (mRemainSize > 0xF000)
		{
			mPeri[DMA_REG::CNDTR] = 0xF000;
			mRemainSize -= 0xF000;
		}
		else
		{
			mPeri[DMA_REG::CNDTR] = mRemainSize;
			mRemainSize = 0;
		}

		mPeri[DMA_REG::CMAR] = mAddr;
		mPeri[DMA_REG::CCR] |= DMA_CCR_EN_Msk;
	}
	else if (checkComplete(sr))
		mCompleteFlag = true;

	thread::signal(mThreadId);
}



DmaChannel8::DmaChannel8(const Drv::Config drvConfig, const Dma::Config dmaConfig, const Config config) : Dma(drvConfig, dmaConfig)
{
}

void DmaChannel8::isr(void)
{
	register uint32_t sr = getFieldData(mDma[DMA_REG::ISR], 0xF << 0, 0);
	setFieldData(mDma[DMA_REG::IFCR], 0xF << 0, sr, 0);

	if (checkError(sr))
		mErrorFlag = true;

	if (mRemainSize)
	{
		mAddr += 0xF000;
		if (mRemainSize > 0xF000)
		{
			mPeri[DMA_REG::CNDTR] = 0xF000;
			mRemainSize -= 0xF000;
		}
		else
		{
			mPeri[DMA_REG::CNDTR] = mRemainSize;
			mRemainSize = 0;
		}

		mPeri[DMA_REG::CMAR] = mAddr;
		mPeri[DMA_REG::CCR] |= DMA_CCR_EN_Msk;
	}
	else if (checkComplete(sr))
		mCompleteFlag = true;

	thread::signal(mThreadId);
}



DmaChannel9::DmaChannel9(const Drv::Config drvConfig, const Dma::Config dmaConfig, const Config config) : Dma(drvConfig, dmaConfig)
{
	
}

void DmaChannel9::isr(void)
{
	register uint32_t sr = getFieldData(mDma[DMA_REG::ISR], 0xF << 4, 4);
	setFieldData(mDma[DMA_REG::IFCR], 0xF << 4, sr, 4);

	if (checkError(sr))
		mErrorFlag = true;

	if (mRemainSize)
	{
		mAddr += 0xF000;
		if (mRemainSize > 0xF000)
		{
			mPeri[DMA_REG::CNDTR] = 0xF000;
			mRemainSize -= 0xF000;
		}
		else
		{
			mPeri[DMA_REG::CNDTR] = mRemainSize;
			mRemainSize = 0;
		}

		mPeri[DMA_REG::CMAR] = mAddr;
		mPeri[DMA_REG::CCR] |= DMA_CCR_EN_Msk;
	}
	else if (checkComplete(sr))
		mCompleteFlag = true;

	thread::signal(mThreadId);
}



DmaChannel10::DmaChannel10(const Drv::Config drvConfig, const Dma::Config dmaConfig, const Config config) : Dma(drvConfig, dmaConfig)
{
	
}

void DmaChannel10::isr(void)
{
	register uint32_t sr = getFieldData(mDma[DMA_REG::ISR], 0xF << 8, 8);
	setFieldData(mDma[DMA_REG::IFCR], 0xF << 8, sr, 8);

	if (checkError(sr))
		mErrorFlag = true;

	if (mRemainSize)
	{
		mAddr += 0xF000;
		if (mRemainSize > 0xF000)
		{
			mPeri[DMA_REG::CNDTR] = 0xF000;
			mRemainSize -= 0xF000;
		}
		else
		{
			mPeri[DMA_REG::CNDTR] = mRemainSize;
			mRemainSize = 0;
		}

		mPeri[DMA_REG::CMAR] = mAddr;
		mPeri[DMA_REG::CCR] |= DMA_CCR_EN_Msk;
	}
	else if (checkComplete(sr))
		mCompleteFlag = true;

	thread::signal(mThreadId);
}



DmaChannel11::DmaChannel11(const Drv::Config drvConfig, const Dma::Config dmaConfig, const Config config) : Dma(drvConfig, dmaConfig)
{
	
}

void DmaChannel11::isr(void)
{
	register uint32_t sr = getFieldData(mDma[DMA_REG::ISR], 0xF << 12, 12);
	setFieldData(mDma[DMA_REG::IFCR], 0xF << 12, sr, 12);

	if (checkError(sr))
		mErrorFlag = true;

	if (mRemainSize)
	{
		mAddr += 0xF000;
		if (mRemainSize > 0xF000)
		{
			mPeri[DMA_REG::CNDTR] = 0xF000;
			mRemainSize -= 0xF000;
		}
		else
		{
			mPeri[DMA_REG::CNDTR] = mRemainSize;
			mRemainSize = 0;
		}

		mPeri[DMA_REG::CMAR] = mAddr;
		mPeri[DMA_REG::CCR] |= DMA_CCR_EN_Msk;
	}
	else if (checkComplete(sr))
		mCompleteFlag = true;

	thread::signal(mThreadId);
}



DmaChannel12::DmaChannel12(const Drv::Config drvConfig, const Dma::Config dmaConfig, const Config config) : Dma(drvConfig, dmaConfig)
{
	
}

void DmaChannel12::isr(void)
{
	register uint32_t sr = getFieldData(mDma[DMA_REG::ISR], 0xF << 16, 16);
	setFieldData(mDma[DMA_REG::IFCR], 0xF << 16, sr, 16);

	if (checkError(sr))
		mErrorFlag = true;

	if (mRemainSize)
	{
		mAddr += 0xF000;
		if (mRemainSize > 0xF000)
		{
			mPeri[DMA_REG::CNDTR] = 0xF000;
			mRemainSize -= 0xF000;
		}
		else
		{
			mPeri[DMA_REG::CNDTR] = mRemainSize;
			mRemainSize = 0;
		}

		mPeri[DMA_REG::CMAR] = mAddr;
		mPeri[DMA_REG::CCR] |= DMA_CCR_EN_Msk;
	}
	else if (checkComplete(sr))
		mCompleteFlag = true;

	thread::signal(mThreadId);
}

#endif

