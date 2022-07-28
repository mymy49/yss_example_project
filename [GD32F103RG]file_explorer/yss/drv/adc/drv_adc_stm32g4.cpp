////////////////////////////////////////////////////////////////////////////////////////
//
// 저작권 표기 License_ver_3.0
// 본 소스 코드의 소유권은 홍윤기에게 있습니다.
// 소스 코드 기여는 기증으로 받아들입니다.
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

#include <drv/peripheral.h>

#if defined(STM32G4) || defined(STM32L0) || defined(STM32F0)

#include <drv/Adc.h>
#include <yss/malloc.h>
#include <yss/thread.h>

Adc::Adc(ADC_TypeDef *peri, void (*clockFunc)(bool en), void (*nvicFunc)(bool en), void (*resetFunc)(void)) : Drv(clockFunc, nvicFunc, resetFunc)
{
	mPeri = peri;
	mIndex = 0;
	mNumOfCh = 0;

	for (int i = 0; i < 18; i++)
	{
		mChannel[i] = 0;
		mResult[i] = 0;
		mLpfLv[i] = define::adc::lpfLv::LV9;
		mBit[i] = define::adc::bit::BIT16;
	}
}

bool Adc::init(void)
{
	// ADC on
#if defined(STM32G4)
	mPeri->CR &= ~ADC_CR_DEEPPWD_Msk;
	mPeri->CR |= ADC_CR_ADVREGEN_Msk;
#endif
	mPeri->ISR = ADC_ISR_ADRDY_Msk;
	mPeri->CR |= ADC_CR_ADEN_Msk;
	while ((mPeri->ISR & ADC_ISR_ADRDY_Msk) == 0)
		thread::yield();
	mPeri->ISR = ADC_ISR_ADRDY_Msk;

	// 샘플 타임 설정
#if defined(STM32G4)
	mPeri->SMPR1 = ADC_SMPR1_SMP0_Msk | ADC_SMPR1_SMP1_Msk | ADC_SMPR1_SMP2_Msk |
				   ADC_SMPR1_SMP3_Msk | ADC_SMPR1_SMP4_Msk | ADC_SMPR1_SMP5_Msk |
				   ADC_SMPR1_SMP6_Msk | ADC_SMPR1_SMP7_Msk | ADC_SMPR1_SMP8_Msk |
				   ADC_SMPR1_SMP9_Msk;

	mPeri->SMPR2 = ADC_SMPR2_SMP10_Msk | ADC_SMPR2_SMP11_Msk | ADC_SMPR2_SMP12_Msk |
				   ADC_SMPR2_SMP13_Msk | ADC_SMPR2_SMP14_Msk | ADC_SMPR2_SMP15_Msk |
				   ADC_SMPR2_SMP16_Msk | ADC_SMPR2_SMP17_Msk | ADC_SMPR2_SMP18_Msk;
#elif defined(STM32L0)
	mPeri->SMPR = ADC_SMPR_SMP_Msk;
#endif

	mPeri->IER |= ADC_IER_EOCIE_Msk;
	mPeri->CR |= ADC_CR_ADSTART_Msk;
	return true;
}

void Adc::isr(void)
{
	signed int dr = mPeri->DR << 19, temp, abs;
	unsigned char index = mChannel[mIndex];

	temp = dr - mResult[index];
	temp >>= mLpfLv[mIndex];
	mResult[index] += temp;

	mIndex++;
	if (mIndex >= mNumOfCh)
		mIndex = 0;

#if defined(STM32G4)
	mPeri->SQR1 &= ~ADC_SQR1_SQ1_Msk;
	mPeri->SQR1 |= mChannel[mIndex] << ADC_SQR1_SQ1_Pos;
#elif defined(STM32L0) || defined(STM32F0)
	mPeri->CHSELR = 1 << mChannel[mIndex];
#endif

	mPeri->CR |= ADC_CR_ADSTART_Msk;
}

void Adc::add(unsigned char pin, unsigned char lpfLv, unsigned char bit)
{
	if (mNumOfCh >= 18)
		return;
	mChannel[mNumOfCh] = pin;
	mLpfLv[mNumOfCh] = lpfLv;
	mBit[pin] = bit;
	mNumOfCh++;
}

unsigned short Adc::get(unsigned char pin)
{
	return mResult[pin] >> mBit[pin];
}

#endif

