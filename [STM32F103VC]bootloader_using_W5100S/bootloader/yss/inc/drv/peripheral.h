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

#ifndef YSS_PERIPHERAL__H_
#define YSS_PERIPHERAL__H_

#include "mcu.h"

#if defined(STM32F1)

#include <stm32f1xx.h>

#elif defined(STM32F4)

#include <stm32f4xx.h>

#elif defined(STM32F7)

#include <stm32f7xx.h>

#elif defined(STM32G4)

#include <stm32g4xx.h>

#elif defined(GD32F1)

#include <gd32f10x.h>

__STATIC_INLINE uint32_t SysTick_Config(uint32_t ticks)
{
  if ((ticks - 1UL) > SysTick_LOAD_RELOAD_Msk)
  {
    return (1UL);                                                   /* Reload value impossible */
  }

  SysTick->LOAD  = (uint32_t)(ticks - 1UL);                         /* set reload register */
  NVIC_SetPriority (SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL); /* set Priority for Systick Interrupt */
  SysTick->VAL   = 0UL;                                             /* Load the SysTick Counter Value */
  SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
                   SysTick_CTRL_TICKINT_Msk   |
                   SysTick_CTRL_ENABLE_Msk;                         /* Enable SysTick IRQ and SysTick Timer */
  return (0UL);                                                     /* Function successful */
}

#elif defined(GD32F4)

#include <gd32f4xx.h>

#elif defined(NRF52840_XXAA)

#include <nrf52840.h>

#else

inline void __disable_irq(void) {}
inline void __enable_irq(void) {}
inline void NVIC_SetPriority(unsigned char val1, unsigned char val2) {}

#define PendSV_IRQn 0
#define SysTick_CTRL_CLKSOURCE_Pos 0
#define SysTick_CTRL_TICKINT_Pos 0
#define SysTick_CTRL_ENABLE_Pos 0

#define SysTick ((SysTick_Type *)0) // !< SysTick configuration struct

#endif

#endif