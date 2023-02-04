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

#if defined (STM32F746xx) || defined(STM32F745xx) || \
	defined (STM32F765xx) || defined(STM32F767xx) || defined(STM32F768xx) || defined(STM32F769xx) || \
	defined (STM32F405xx) ||	defined(STM32F415xx) ||	\
	defined (STM32F407xx) ||	defined(STM32F417xx) ||	\
	defined (STM32F427xx) ||	defined(STM32F437xx) ||	\
	defined (STM32F429xx) ||	defined(STM32F439xx) || \
	defined (STM32F100xB) || defined(STM32F100xE) || \
	defined (STM32F101x6) || defined(STM32F101xB) || defined(STM32F101xE) || defined(STM32F101xG) || \
	defined (STM32F102x6) || defined(STM32F102xB) || \
	defined (STM32F103x6) || defined(STM32F103xB) || defined(STM32F103xE) || defined(STM32F103xG) || \
	defined (STM32F105xC) || \
	defined (STM32F107xC) || \
	defined (STM32L151xB) || \
	defined (STM32G431xx) || defined (STM32G441xx) || \
	defined (STM32G471xx) || defined (STM32G473xx) || defined (STM32G474xx) || defined (STM32G483xx) || defined (STM32G484xx) || defined (STM32GBK1CB) || \
	defined (GD32F10X_XD) || \
	defined(GD32F10X_XD) || defined(GD32F10X_HD)

#define __CORE_CM3_CM4_CM7_H_GENERIC
#else
#define __CORE_CM0PLUS_H_GENERIC
#endif

#if defined(__CORE_CM3_CM4_CM7_H_GENERIC)
	.thumb_func
	.syntax unified
	.func copyRgb888DotPattern
	.type copyRgb888DotPattern, %function
	.global copyRgb888DotPattern
	.section .text, "ax"
copyRgb888DotPattern:
	push {r3-r7}
	bic r1, #0xff000000
	mov r5, r1
	mov r6, #0
	mov r7, #0
	lsl r3, r1, #24
	orr r5, r3
	lsr r6, r1, #8
	lsl r3, r1, #16
	orr r6, r3
	lsr r7, r1, #16
	lsl r3, r1, #8
	orr r7, r3
repeat:
	subs r3, r2, #64
	bge greater
	mov r3, #64
	subs r4, r3, r2
	mov r3, #4
	udiv r4, r4, r3
	lsl r4, #1
	bic r2, #0xFFFFFFFC
	add pc, pc, r4
greater:
	subs r2, #64
	stmia   R0!,{R5-R7}
	stmia   R0!,{R5-R7}
	stmia   R0!,{R5-R7}
	stmia   R0!,{R5-R7}
	stmia   R0!,{R5-R7}
	stmia   R0!,{R5-R7}
	stmia   R0!,{R5-R7}
	stmia   R0!,{R5-R7}
	stmia   R0!,{R5-R7}
	stmia   R0!,{R5-R7}
	stmia   R0!,{R5-R7}
	stmia   R0!,{R5-R7}
	stmia   R0!,{R5-R7}
	stmia   R0!,{R5-R7}
	stmia   R0!,{R5-R7}
	stmia   R0!,{R5-R7}
	subs r3, r2, #4
	bhi repeat
	movs r2, r2
	beq finish
	strb r5, [r0], #1
	strb r6, [r0], #1
	strb r7, [r0], #1
	sub r2, #1
	movs r2, r2
	beq finish
	strb r5, [r0], #1
	strb r6, [r0], #1
	strb r7, [r0], #1
	sub r2, #1
	movs r2, r2
	beq finish
	strb r5, [r0], #1
	strb r6, [r0], #1
	strb r7, [r0], #1
finish:
	pop {r3-r7}
	bx lr
#endif
