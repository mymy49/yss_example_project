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

#ifndef YSS_MOD_GLCD_TM0027__H_
#define YSS_MOD_GLCD_TM0027__H_

#include <gui/Font.h>
#include <gui/util.h>
#include <sac/MonoLcd.h>
#include <yss/instance.h>

#ifndef YSS_DRV_SPI_UNSUPPORTED

namespace mod
{
namespace glcd
{
class TM0027 : public sac::MonoLcd
{
	Spi *mPeri;
	Gpio::Pin mCs, mA0, mRst;

	void sendCmd(unsigned char cmd);
	void sendData(void *data, unsigned int size);

  public:
	TM0027(void);
	bool init(Spi &spi, Gpio::Pin &cs, Gpio::Pin &A0, Gpio::Pin &rst);
	void clear(void);
	void refresh(void);
	void fill(void);
	void drawDot(unsigned short x, unsigned short y, bool data = true);
};
}
}

#endif

#endif