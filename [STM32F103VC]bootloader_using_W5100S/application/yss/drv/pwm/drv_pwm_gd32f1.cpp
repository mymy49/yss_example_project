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

#include <drv/peripheral.h>

#if defined(GD32F1)

#include <drv/Pwm.h>
#include <yss/reg.h>

Pwm::Pwm(const Drv::Config &drvConfig, const Config &config) : Drv(drvConfig)
{
	mPeri = config.peri;
	mGetClockFreq = config.getClockFreq;
}

void Pwm::init(unsigned int psc, unsigned int arr, bool risingAtMatch)
{
	mPeri->PSC = (unsigned short)psc;
	mPeri->CARL = (unsigned short)arr;

	initChannel(risingAtMatch);
}

void Pwm::init(unsigned int freq, bool risingAtMatch)
{
	unsigned int psc, arr, clk = mGetClockFreq();

	arr = clk / freq;
	psc = arr / (0xffff + 1);
	arr /= psc + 1;

	mPeri->PSC = psc;
	mPeri->CARL = arr;

	initChannel(risingAtMatch);
}

unsigned int Pwm::getTop(void)
{
	return mPeri->CARL;
}

void Pwm::start(void)
{
	setBitData(mPeri->CTLR1, true, 0);	// Timer Enable
}

void Pwm::stop(void)
{
	setBitData(mPeri->CTLR1, false, 0);	// Timer Diable
}

void Pwm::setOnePulse(bool en)
{
	setBitData(mPeri->CTLR1, en, 3);
}

PwmCh1::PwmCh1(const Drv::Config &drvConfig, const Pwm::Config &config) : Pwm(drvConfig, config)
{
	
}

void PwmCh1::initChannel(bool risingAtMatch)
{
	setBitData(mPeri->BKDT, true, 15);				// Primary Output Enable
	setFieldData(mPeri->CHCTLR1, 0x3 << 0, 0, 0);	// 출력으로 설정
	setBitData(mPeri->CHCTLR1, true, 3);			// Shadow 활성화
	setBitData(mPeri->CHCTLR1, true, 2);			// Fast 활성화
	setBitData(mPeri->CHE, true, 0);				// Channel 활성화 

	if (risingAtMatch)
		setFieldData(mPeri->CHCTLR1, 0x7 << 4, 7, 4);
	else
		setFieldData(mPeri->CHCTLR1, 0x7 << 4, 6, 4);
}

unsigned int PwmCh1::getTop(void)
{
	return mPeri->CARL;
}

void PwmCh1::setRatio(float ratio)
{
	mPeri->CHCC1 = (unsigned short)((float)mPeri->CARL * ratio);
}

void PwmCh1::setCounter(int counter)
{
	mPeri->CHCC1 = counter;
}

PwmCh2::PwmCh2(const Drv::Config &drvConfig, const Pwm::Config &config) : Pwm(drvConfig, config)
{
	
}

void PwmCh2::initChannel(bool risingAtMatch)
{
	setBitData(mPeri->BKDT, true, 15);				// Primary Output Enable
	setFieldData(mPeri->CHCTLR1, 0x3 << 8, 0, 8);	// 출력으로 설정
	setBitData(mPeri->CHCTLR1, true, 11);			// Shadow 활성화
	setBitData(mPeri->CHCTLR1, true, 10);			// Fast 활성화
	setBitData(mPeri->CHE, true, 4);				// Channel 활성화 

	if (risingAtMatch)
		setFieldData(mPeri->CHCTLR1, 0x7 << 12, 7, 12);
	else
		setFieldData(mPeri->CHCTLR1, 0x7 << 12, 6, 12);
}

unsigned int PwmCh2::getTop(void)
{
	return mPeri->CARL;
}

void PwmCh2::setRatio(float ratio)
{
	mPeri->CHCC2 = (unsigned short)((float)mPeri->CARL * ratio);
}

void PwmCh2::setCounter(int counter)
{
	mPeri->CHCC2 = counter;
}

PwmCh3::PwmCh3(const Drv::Config &drvConfig, const Pwm::Config &config) : Pwm(drvConfig, config)
{
	
}

void PwmCh3::initChannel(bool risingAtMatch)
{
	setBitData(mPeri->BKDT, true, 15);				// Primary Output Enable
	setFieldData(mPeri->CHCTLR2, 0x3 << 0, 0, 0);	// 출력으로 설정
	setBitData(mPeri->CHCTLR2, true, 3);			// Shadow 활성화
	setBitData(mPeri->CHCTLR2, true, 2);			// Fast 활성화
	setBitData(mPeri->CHE, true, 8);				// Channel 활성화 

	if (risingAtMatch)
		setFieldData(mPeri->CHCTLR2, 0x7 << 4, 7, 4);
	else
		setFieldData(mPeri->CHCTLR2, 0x7 << 4, 6, 4);
}

unsigned int PwmCh3::getTop(void)
{
	return mPeri->CARL;
}

void PwmCh3::setRatio(float ratio)
{
	mPeri->CHCC3 = (unsigned short)((float)mPeri->CARL * ratio);
}

void PwmCh3::setCounter(int counter)
{
	mPeri->CHCC3 = counter;
}

PwmCh4::PwmCh4(const Drv::Config &drvConfig, const Pwm::Config &config) : Pwm(drvConfig, config)
{
	
}

void PwmCh4::initChannel(bool risingAtMatch)
{
	setBitData(mPeri->BKDT, true, 15);				// Primary Output Enable
	setFieldData(mPeri->CHCTLR2, 0x3 << 8, 0, 8);	// 출력으로 설정
	setBitData(mPeri->CHCTLR2, true, 11);			// Shadow 활성화
	setBitData(mPeri->CHCTLR2, true, 10);			// Fast 활성화
	setBitData(mPeri->CHE, true, 12);				// Channel 활성화 

	if (risingAtMatch)
		setFieldData(mPeri->CHCTLR2, 0x7 << 12, 7, 12);
	else
		setFieldData(mPeri->CHCTLR2, 0x7 << 12, 6, 12);
}

unsigned int PwmCh4::getTop(void)
{
	return mPeri->CARL;
}

void PwmCh4::setRatio(float ratio)
{
	mPeri->CHCC4 = (unsigned short)((float)mPeri->CARL * ratio);
}

void PwmCh4::setCounter(int counter)
{
	mPeri->CHCC4 = counter;
}

#endif

