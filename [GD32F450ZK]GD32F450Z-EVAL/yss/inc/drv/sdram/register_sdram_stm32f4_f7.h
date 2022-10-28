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

#ifndef	YSS_DRV_SDRAM_ST_TYPE_A_REG__H_
#define	YSS_DRV_SDRAM_ST_TYPE_A_REG__H_

#if defined(STM32F746xx) || defined(STM32F745xx) || \
	defined(STM32F765xx) || defined(STM32F767xx) || defined(STM32F768xx) || defined(STM32F769xx) || \
	defined(STM32F405xx) ||	defined(STM32F415xx) ||	\
	defined(STM32F407xx) ||	defined(STM32F417xx) ||	\
	defined(STM32F427xx) ||	defined(STM32F437xx) ||	\
	defined(STM32F429xx) ||	defined(STM32F439xx)

#include <yss/reg.h>

#define	setSdramSdtrTrcd(bank, val)		setRegField(FMC_Bank5_6->SDTR[bank], 0xFUL,	val, 24)
#define	setSdramSdtrTrp(bank, val)		setRegField(FMC_Bank5_6->SDTR[bank], 0xFUL,	val, 20)
#define	setSdramSdtrTwr(bank, val)		setRegField(FMC_Bank5_6->SDTR[bank], 0xFUL,	val, 16)
#define	setSdramSdtrTrc(bank, val)		setRegField(FMC_Bank5_6->SDTR[bank], 0xFUL,	val, 12)
#define	setSdramSdtrTras(bank, val)		setRegField(FMC_Bank5_6->SDTR[bank], 0xFUL,	val, 8)
#define	setSdramSdtrTxsr(bank, val)		setRegField(FMC_Bank5_6->SDTR[bank], 0xFUL,	val, 4)
#define	setSdramSdtrTmrd(bank, val)		setRegField(FMC_Bank5_6->SDTR[bank], 0xFUL,	val, 0)

#define	setSdramSdrtrRtr(val)			setRegField(FMC_Bank5_6->SDRTR,	0xFFFUL, val, 1)

#endif

#endif
