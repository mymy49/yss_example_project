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
#include <yss/instance.h>
#include <config.h>

#if defined(GD32F1)

static unsigned int getTimerApb2ClkFreq(void)
{
	return clock.getTimerApb2ClkFreq();
}

static unsigned int getTimerApb1ClkFreq(void)
{
	return clock.getTimerApb1ClkFreq();
}

#if defined(CAPTURE1_ENABLE) && defined(TIMER1)
static void setTim1ClockEn(bool en)
{
	clock.peripheral.setTimer1En(en);
}

static void setTim1IntEn(bool en)
{
	nvic.setTimer1En(en);
}

static void resetTim1(void)
{
	clock.peripheral.resetTimer1();
}

static const Drv::Config gDrvTim1Config = 
{
	setTim1ClockEn,			//void (*clockFunc)(bool en);
	setTim1IntEn,			//void (*nvicFunc)(bool en);
	resetTim1,				//void (*resetFunc)(void);
	getTimerApb2ClkFreq		//unsigned int (*getClockFunc)(void);
};
static unsigned long long gCapture1UpdateCnt;
static const drv::Capture::Config gTim11Config = 
{
	TIMER1,				//YSS_PWM_Peri *peri;
	&gCapture1UpdateCnt	//unsigned long long *updateCnt;
};

drv::CaptureCh1 capture1Ch1(gDrvTim1Config, gTim11Config);
drv::CaptureCh2 capture1Ch2(gDrvTim1Config, gTim11Config);

extern "C"
{
void TIMER1_UP_TIMER10_IRQHandler(void)
{
	bool event1 = false;

	if (TIMER1->DIE & TIMER_DIE_UPIE && TIMER1->STR & TIMER_STR_UPIF)
	{
		TIMER1->STR = ~TIMER_STR_UPIF;
		capture1Ch1.isrUpdate();
		event1 = true;
	}

	if (TIMER1->DIE & TIMER_DIE_CH1IE && TIMER1->STR & TIMER_STR_CH1IF)
	{
		TIMER1->STR = ~TIMER_STR_CH1IF;
		capture1Ch1.isrCapture(event1);
	}

	if (TIMER1->DIE & TIMER_DIE_CH2IE && TIMER1->STR & TIMER_STR_CH2IF)
	{
		TIMER1->STR = ~TIMER_STR_CH2IF;
		capture1Ch2.isrCapture(event1);
	}
}

void TIMER1_CC_IRQHandler(void)
{
	bool event1 = false;

	if (TIMER1->DIE & TIMER_DIE_UPIE && TIMER1->STR & TIMER_STR_UPIF)
	{
		TIMER1->STR = ~TIMER_STR_UPIF;
		capture1Ch1.isrUpdate();
		event1 = true;
	}

	if (TIMER1->DIE & TIMER_DIE_CH1IE && TIMER1->STR & TIMER_STR_CH1IF)
	{
		TIMER1->STR = ~TIMER_STR_CH1IF;
		capture1Ch1.isrCapture(event1);
	}

	if (TIMER1->DIE & TIMER_DIE_CH2IE && TIMER1->STR & TIMER_STR_CH2IF)
	{
		TIMER1->STR = ~TIMER_STR_CH2IF;
		capture1Ch2.isrCapture(event1);
	}
}
}

#endif

#endif

