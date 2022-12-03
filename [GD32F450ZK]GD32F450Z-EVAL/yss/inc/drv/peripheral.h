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

#include <targets/st_gigadevice/stm32f1xx.h>

#elif defined(STM32F4)

#include <targets/st_gigadevice/stm32f4xx.h>

#elif defined(STM32F7)

#include <targets/st_gigadevice/stm32f7xx.h>

#elif defined(STM32G4)

#include <stm32g4xx.h>

#elif defined(GD32F1)

#include <targets/st_gigadevice/gd32f10x.h>

#elif defined(GD32F4)

#include <targets/st_gigadevice/gd32f4xx.h>

#elif defined(NRF52840_XXAA)

#include <targets/nordic/nrf52840.h>

#elif defined(STM32L1)

#include <targets/st_gigadevice/stm32l1xx.h>

#elif defined(STM32F0)

#include <targets/st_gigadevice/stm32f0xx.h>

#else

inline void __disable_irq(void) {}
inline void __enable_irq(void) {}
//inline void NVIC_SetPriority(uint8_t val1, uint8_t val2) {}

typedef volatile int IRQn_Type;

#define PendSV_IRQn 0
#define SysTick_CTRL_CLKSOURCE_Pos 0
#define SysTick_CTRL_TICKINT_Pos 0
#define SysTick_CTRL_ENABLE_Pos 0

#define SysTick ((SysTick_Type *)0) // !< SysTick configuration struct

#define NVIC_DisableIRQ
#define NVIC_EnableIRQ

#endif

#endif