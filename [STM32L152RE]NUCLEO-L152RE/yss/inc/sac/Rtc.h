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

#ifndef YSS_SAC_RTC__H_
#define YSS_SAC_RTC__H_

#include <stdint.h>

namespace sac
{
class Rtc
{
  public:
	uint32_t getTimeLapse(void);
	int32_t compareTimeLapse(uint8_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec);
	uint32_t calculateTimeLapse(uint8_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec);

	virtual void refresh(void) = 0;

	virtual uint8_t getYear(void) = 0;
	virtual bool setYear(uint8_t year) = 0;
	virtual uint8_t getMonth(void) = 0;
	virtual bool setMonth(uint8_t month) = 0;
	virtual uint8_t getDay(void) = 0;
	virtual bool setDay(uint8_t day) = 0;
	virtual uint8_t getWeekDay(void) = 0;
	virtual bool setWeekDay(uint8_t weekDay) = 0;

	virtual uint8_t getHour(void) = 0;
	virtual bool setHour(uint8_t hour) = 0;
	virtual uint8_t getMin(void) = 0;
	virtual bool setMin(uint8_t min) = 0;
	virtual uint8_t getSec(void) = 0;
	virtual bool setSec(uint8_t min) = 0;
	virtual uint16_t getSubsec(void) = 0;
};
}

#endif