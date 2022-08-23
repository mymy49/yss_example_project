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

#ifndef YSS_DRV_SDMMC__H_
#define YSS_DRV_SDMMC__H_

#include <drv/mcu.h>

#if defined(STM32F7)

typedef SDMMC_TypeDef	YSS_SDMMC_Peri;

#elif defined(GD32F1) || defined(STM32F4)

typedef SDIO_TypeDef	YSS_SDMMC_Peri;

#else

#define YSS_DRV_SDMMC_UNSUPPORTED

#endif

#ifndef YSS_DRV_SDMMC_UNSUPPORTED

#include <drv/Drv.h>
#include <drv/Gpio.h>
#include <sac/SdMemory.h>
#include <drv/Dma.h>
#include <yss/error.h>

class Sdmmc : public Drv, public sac::SdMemory
{
	YSS_SDMMC_Peri *mPeri;
	Dma *mTxDma, *mRxDma;
	Dma::DmaInfo mTxDmaInfo, mRxDmaInfo;
	bool mAcmdFlag;
	unsigned char mBlockSize;

  protected:
	error sendCmd(unsigned char cmd, unsigned int arg, unsigned char responseType);
	unsigned int getShortResponse(void);
	void getLongResponse(void *des);
	void setSdioClockBypass(bool en);
	void setSdioClockEn(bool en);
	void setClockFrequency(int frequency);
	void setPower(bool en);
	void readyRead(void *des, unsigned short length);
	void readyWrite(void *des, unsigned short length);
	void setDataBlockSize(unsigned char blockSize);
	error waitUntilReadComplete(void);
	error waitUntilWriteComplete(void);
	bool setBusWidth(unsigned char width);
	void unlockRead(void);
	void unlockWrite(void);

  public:
	struct Config
	{
		YSS_SDMMC_Peri *peri;
		Dma &txDma;
		Dma::DmaInfo txDmaInfo;
		Dma &rxDma;
		Dma::DmaInfo rxDmaInfo;
	};

	Sdmmc(const Drv::Config &drvConfig, const Config &config);

	bool init(void);

	void lock(void);
	void unlock(void);

};

#endif

#endif

