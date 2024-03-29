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

#include <drv/mcu.h>

#if defined(EFM32PG22)

#include <drv/peripheral.h>
#include <yss/instance.h>
#include <config.h>
#include <targets/siliconlabs/efm32pg22_cmu.h>

static uint32_t  getApb0Frequency(void)
{
	return clock.getApb0Frequency();
}

static uint32_t  getApb1Frequency(void)
{
	return clock.getApb1Frequency();
}

#if defined(TIM0_ENABLE) && defined(TIMER0)
static void enableTimer0Clock(bool en)
{
	clock.lock();
    clock.enableApb0Clock(_CMU_CLKEN0_TIMER0_SHIFT, en);
	clock.unlock();
}

static void enableInterruptTim0(bool en)
{
	nvic.lock();
	nvic.enableInterrupt(TIMER0_IRQn);
	nvic.unlock();
}

static const Timer::Config gTimer0config = 
{
	TIMER0,
	Timer::BIT_32
};

static const Drv::Config gDrvTimer0Config = 
{
	enableTimer0Clock,		//void (*clockFunc)(bool en);
	enableInterruptTim0,	//void (*nvicFunc)(bool en);
	0,						//void (*resetFunc)(void);
	getApb0Frequency		//uint32_t (*getClockFunc)(void);
};

Timer timer0(gTimer0config, gDrvTimer0Config);

extern "C"
{
void TIMER0_IRQHandler(void)
{
	TIMER0->IF_CLR = _TIMER_IF_OF_MASK;
	timer0.isrUpdate();
}
}
#endif



#if defined(TIM1_ENABLE) && defined(TIMER1)
static void enableTimer1Clock(bool en)
{
	clock.lock();
    clock.enableApb0Clock(_CMU_CLKEN0_TIMER1_SHIFT, en);
	clock.unlock();
}

static void enableInterruptTim1(bool en)
{
	nvic.lock();
	nvic.enableInterrupt(TIMER1_IRQn);
	nvic.unlock();
}

static const Drv::Config gDrvTimer1Config
{
	enableTimer1Clock,		//void (*clockFunc)(bool en);
	enableInterruptTim1,	//void (*nvicFunc)(bool en);
	0,						//void (*resetFunc)(void);
	getApb0Frequency		//uint32_t (*getClockFunc)(void);
};

Timer timer1(TIMER1, gDrvTimer1Config);

extern "C"
{
void TIMER1_IRQHandler(void)
{
	TIMER1->IF_CLR = _TIMER_IF_OF_MASK;
	timer1.isrUpdate();
}
}
#endif



#if defined(TIM2_ENABLE) && defined(TIMER2)
static void enableTimer2Clock(bool en)
{
	clock.lock();
    clock.enableApb0Clock(_CMU_CLKEN0_TIMER2_SHIFT, en);
	clock.unlock();
}

static void enableInterruptTim2(bool en)
{
	nvic.lock();
	nvic.enableInterrupt(TIMER2_IRQn);
	nvic.unlock();
}

static const Drv::Config gDrvTimer2Config
{
	enableTimer2Clock,		//void (*clockFunc)(bool en);
	enableInterruptTim2,	//void (*nvicFunc)(bool en);
	0,						//void (*resetFunc)(void);
	getApb0Frequency		//uint32_t (*getClockFunc)(void);
};

Timer timer2(TIMER2, gDrvTimer2Config);

extern "C"
{
void TIMER2_IRQHandler(void)
{
	TIMER2->IF_CLR = _TIMER_IF_OF_MASK;
	timer2.isrUpdate();
}
}
#endif



#if defined(TIM3_ENABLE) && defined(TIMER3)
static void enableTimer3Clock(bool en)
{
	clock.lock();
    clock.enableApb0Clock(_CMU_CLKEN0_TIMER3_SHIFT, en);
	clock.unlock();
}

static void enableInterruptTim3(bool en)
{
	nvic.lock();
	nvic.enableInterrupt(TIMER3_IRQn);
	nvic.unlock();
}

static const Drv::Config gDrvTimer3Config
{
	enableTimer3Clock,		//void (*clockFunc)(bool en);
	enableInterruptTim3,	//void (*nvicFunc)(bool en);
	0,						//void (*resetFunc)(void);
	getApb0Frequency		//uint32_t (*getClockFunc)(void);
};

Timer timer3(TIMER3, gDrvTimer3Config);

extern "C"
{
void TIMER3_IRQHandler(void)
{
	TIMER3->IF_CLR = _TIMER_IF_OF_MASK;
	timer3.isrUpdate();
}
}
#endif



#if defined(TIM4_ENABLE) && defined(TIMER4)
static void enableTimer4Clock(bool en)
{
	clock.lock();
    clock.enableApb1Clock(_CMU_CLKEN1_TIMER4_SHIFT, en);
	clock.unlock();
}

static void enableInterruptTim4(bool en)
{
	nvic.lock();
	nvic.enableInterrupt(TIMER4_IRQn);
	nvic.unlock();
}

static const Drv::Config gDrvTimer4Config
{
	enableTimer4Clock,		//void (*clockFunc)(bool en);
	enableInterruptTim4,	//void (*nvicFunc)(bool en);
	0,						//void (*resetFunc)(void);
	getApb1Frequency		//uint32_t (*getClockFunc)(void);
};

Timer timer4(TIMER4, gDrvTimer4Config);

extern "C"
{
void TIMER4_IRQHandler(void)
{
	TIMER4->IF_CLR = _TIMER_IF_OF_MASK;
	timer4.isrUpdate();
}
}
#endif

#endif

