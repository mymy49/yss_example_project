////////////////////////////////////////////////////////////////////////////////////////
//
// 저작권 표기 License_ver_3.1
// 본 소스 코드의 소유권은 홍윤기에게 있습니다.
// 어떠한 형태든 기여는 기증으로 받아들입니다.
// 본 소스 코드는 아래 사항에 동의할 경우에 사용 가능합니다.
// 아래 사항에 대해 동의하지 않거나 이해하지 못했을 경우 사용을 금합니다.
// 본 소스 코드를 사용하였다면 아래 사항을 모두 동의하는 것으로 자동 간주 합니다.
// 본 소스 코드의 상업적 또는 비 상업적 이용이 가능합니다.
// 본 소스 코드의 내용을 임의로 수정하여 재배포하는 행위를 금합니다.
// 본 소스 코드의 사용으로 인해 발생하는 모든 사고에 대해서 어떠한 법적 책임을 지지 않습니다.
//
// Home Page : http://cafe.naver.com/yssoperatingsystem
// Copyright 2022. 홍윤기 all right reserved.
//
////////////////////////////////////////////////////////////////////////////////////////

#ifndef YSS_DRV_I2C__H_
#define YSS_DRV_I2C__H_

#include "mcu.h"

#if false

typedef I2C_TypeDef		YSS_I2C_Peri;

#include "i2c/define_i2c_stm32f7.h"

#elif defined(GD32F1) || defined(STM32F1) || defined(STM32F4)

typedef volatile uint32_t	YSS_I2C_Peri;

#include <targets/st_gigadevice/define_i2c_stm32_gd32f1_f4.h>

#else

#define YSS_DRV_I2C_UNSUPPORTED
typedef volatile uint32_t	YSS_I2C_Peri;

#endif

#include "Drv.h"
#include "Dma.h"

class I2c : public Drv
{
	YSS_I2C_Peri *mPeri;

#if defined(GD32F1) || defined(STM32F1) || defined(STM32F4)
	uint32_t mDataCount;
	uint8_t *mDataBuf, mAddr;
	bool mDir;
#endif

  public:
	struct Config
	{
		YSS_I2C_Peri *peri;
		Dma &txDma;
		Dma::DmaInfo txDmaInfo;
		Dma &rxDma;
		Dma::DmaInfo rxDmaInfo;
	};

	I2c(const Drv::Config drvConfig, const Config config);
	bool init(uint8_t speed);
	bool send(uint8_t addr, void *src, uint32_t size, uint32_t timeout = 500);
	bool receive(uint8_t addr, void *des, uint32_t size, uint32_t timeout = 500);
	void stop(void);
	void isr(void);

#if defined(STM32F7)
	bool initAsSlave(void *rcvBuf, uint16_t rcvBufSize, uint8_t addr1, uint8_t addr2 = 0);
#endif
};

#endif

