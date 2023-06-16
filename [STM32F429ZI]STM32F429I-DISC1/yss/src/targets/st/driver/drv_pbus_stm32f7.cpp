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

#include <drv/peripheral.h>

#if defined(STM32F7_N)

#include <drv/Pbus.h>
#include <yss.h>

#if defined(STM32F767xx)
#include <targets/st/bitfield_stm32f767xx.h>
#elif defined(STM32F746xx)
#include <targets/st/bitfield_stm32f746xx.h>
#endif

#if defined(FMC_Bank1)

Pbus::Pbus(const Drv::Setup drvSetup) : Drv(drvSetup)
{
	
}

error Pbus::initialize(void)
{
	//FMC_Bank1->BTCR[2] = FMC_Bank1->BTCR[0];
	//FMC_Bank1->BTCR[4] = FMC_Bank1->BTCR[0];
	//FMC_Bank1->BTCR[6] = FMC_Bank1->BTCR[0];

	FMC_Bank1->BTCR[2] |= FMC_BCR2_MBKEN_Msk;
	FMC_Bank1->BTCR[4] |= FMC_BCR3_MBKEN_Msk;
	FMC_Bank1->BTCR[6] |= FMC_BCR4_MBKEN_Msk | FMC_BCR4_WAITEN_Msk | FMC_BCR4_CBURSTRW_Msk | FMC_BCR4_BURSTEN_Msk;
	FMC_Bank1->BTCR[7] = 0x00002000;
//	FMC_Bank1->BTCR[7] = 0x02224022;

	return error::ERROR_NONE;
}

#endif

#endif

