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

#include <drv/mcu.h>

#if defined(GD32F1) || defined(STM32F1)

#include <drv/peripheral.h>
#include <drv/Adc.h>
#include <yss/reg.h>
#include <targets/st_gigadevice/adc_stm32_gd32f1.h>

Adc::Adc(YSS_ADC_Peri *peri, void (*clockFunc)(bool en), void (*nvicFunc)(bool en), void (*resetFunc)(void)) : Drv(clockFunc, nvicFunc, resetFunc)
{
	mPeri = peri;
	mIndex = 0;
	mNumOfCh = 0;

	for (int32_t  i = 0; i < 18; i++)
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
	mPeri[ADC_REG::CR2] |= ADC_CR2_ADON_Msk | ADC_CR2_EXTSEL_Msk | ADC_CR2_EXTTRIG_Msk;

	//// 샘플 타임 기본 설정은 가장 느리게
	mPeri[ADC_REG::SMPR1] = ADC_SMPR1_SMP10_Msk | ADC_SMPR1_SMP11_Msk | ADC_SMPR1_SMP12_Msk | ADC_SMPR1_SMP13_Msk | ADC_SMPR1_SMP14_Msk | ADC_SMPR1_SMP15_Msk | ADC_SMPR1_SMP16_Msk | ADC_SMPR1_SMP17_Msk;
	mPeri[ADC_REG::SMPR2] = ADC_SMPR2_SMP0_Msk | ADC_SMPR2_SMP1_Msk | ADC_SMPR2_SMP2_Msk | ADC_SMPR2_SMP3_Msk | ADC_SMPR2_SMP4_Msk | ADC_SMPR2_SMP5_Msk | ADC_SMPR2_SMP6_Msk | ADC_SMPR2_SMP7_Msk | ADC_SMPR2_SMP8_Msk | ADC_SMPR2_SMP9_Msk;

	setBitData(mPeri[ADC_REG::CR1], true, 5);	// ADC 변환 완료 인터럽트 활성화
	setBitData(mPeri[ADC_REG::CR2], true, 22);	// ADC 변환 시작
	return true;
}

void Adc::isr(void)
{
	int32_t dr = mPeri[ADC_REG::DR] << 19, temp, abs;
	uint8_t index = mChannel[mIndex];

	temp = dr - mResult[index];
	temp >>= mLpfLv[mIndex];
	mResult[index] += temp;

	mIndex++;
	if (mIndex >= mNumOfCh)
		mIndex = 0;
	
	setFieldData(mPeri[ADC_REG::SQR3], 0x1F << 0, mChannel[mIndex], 0);	// ADC 채널 변경
	setBitData(mPeri[ADC_REG::CR2], true, 22);	// ADC 변환 시작
}

void Adc::add(uint8_t pin, uint8_t lpfLv, uint8_t bit)
{
	if (mNumOfCh >= 18)
		return;
	mChannel[mNumOfCh] = pin;
	mLpfLv[mNumOfCh] = lpfLv;
	mBit[pin] = bit;
	mNumOfCh++;
}

uint16_t Adc::get(uint8_t pin)
{
	return mResult[pin] >> mBit[pin];
}

void Adc::setSampleTime(uint8_t pin, uint8_t sampleTime)
{
	if (pin > 17)
		return;

	register uint8_t index = 1 - pin / 10;
	register uint32_t reg = ((uint32_t *)(&mPeri[ADC_REG::SMPR1]))[index];

	pin = pin % 10 * 3;
	reg &= ~(0x07 << pin);
	reg |= sampleTime << pin;
	((uint32_t *)(&mPeri[ADC_REG::SMPR1]))[index] = reg;
}

#endif