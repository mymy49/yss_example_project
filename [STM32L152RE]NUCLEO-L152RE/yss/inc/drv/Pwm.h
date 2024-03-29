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

#ifndef YSS_DRV_PWM__H_
#define YSS_DRV_PWM__H_

#include "mcu.h"

#if defined(GD32F1) || defined(STM32F1) ||  defined(STM32F4) ||  defined(STM32F7)

typedef volatile uint32_t	YSS_PWM_Peri;

#else

#define YSS_DRV_PWM_UNSUPPORTED

#endif

#ifndef YSS_DRV_PWM_UNSUPPORTED

#include "Drv.h"

class Pwm : public Drv
{
  protected:
	YSS_PWM_Peri *mPeri;

  protected :
	virtual void initChannel(bool risingAtMatch = false) = 0;

  public:
	Pwm(YSS_PWM_Peri *peri, const Drv::Config drvConfig);

	void init(uint32_t freq, bool risingAtMatch = false);
	void init(uint32_t psc, uint32_t arr, bool risingAtMatch = false);
	void setOnePulse(bool en);

	void start(void);
	void stop(void);

	virtual uint32_t getTop(void) = 0;
	virtual void setRatio(float ratio) = 0;
	virtual void setCounter(int32_t  counter) = 0;
};

class PwmCh1 : public Pwm
{
  public:
	PwmCh1(YSS_PWM_Peri *peri, const Drv::Config drvConfig);

	void initChannel(bool risingAtMatch = false);
	uint32_t getTop(void);
	void setRatio(float ratio);
	void setCounter(int32_t  counter);
};

class PwmCh2 : public Pwm
{
  public:
	PwmCh2(YSS_PWM_Peri *peri, const Drv::Config drvConfig);

	void initChannel(bool risingAtMatch = false);
	uint32_t getTop(void);
	void setRatio(float ratio);
	void setCounter(int32_t  counter);
};

class PwmCh3 : public Pwm
{
  public:
	PwmCh3(YSS_PWM_Peri *peri, const Drv::Config drvConfig);

	void initChannel(bool risingAtMatch = false);
	uint32_t getTop(void);
	void setRatio(float ratio);
	void setCounter(int32_t  counter);
};

class PwmCh4 : public Pwm
{
  public:
	PwmCh4(YSS_PWM_Peri *peri, const Drv::Config drvConfig);

	void initChannel(bool risingAtMatch = false);
	uint32_t getTop(void);
	void setRatio(float ratio);
	void setCounter(int32_t  counter);
};

#endif

#endif

