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

#include <mod/clcd/JLX1602A_4.h>
#include <string.h>

#if !(defined(YSS_DRV_I2C_UNSUPPORTED) || defined(YSS_DRV_GPIO_UNSUPPORTED))

#define ADDR 0x78
#define LINE_SIZE 2
#define COLUMN_SIZE 16

namespace mod
{
namespace clcd
{
static char gLine1[19] = {0x80, 0x80, 0x40, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20};
static char gLine2[19] = {0x80, 0xc0, 0x40, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20};

JLX1602A_4::JLX1602A_4(void)
{
	mDetectedFlag = false;
}

bool JLX1602A_4::init(I2c &peri, Gpio::Pin backLight)
{
	int8_t buf[5] = {0x00, 0x38, 0x0c, 0x01, 0x06};
	bool rt = true;

	mPeri = &peri;
	mBL.port = backLight.port;
	mBL.pin = backLight.pin;

	mPeri->lock();
	mDetectedFlag = mPeri->send(ADDR, buf, 5, 300);
	mPeri->stop();
	mPeri->unlock();

	return rt;
}

void JLX1602A_4::setBlackLight(bool en)
{
	mBL.port->setOutput(mBL.pin, en);
}

bool JLX1602A_4::isConnected(void)
{
	return mDetectedFlag;
}

bool JLX1602A_4::refresh(void)
{
	bool rt1, rt2;

	if (mDetectedFlag == false)
		return false;

	mPeri->lock();
	rt1 = mPeri->send(0x78, gLine1, 19, 300);
	mPeri->stop();
	mPeri->unlock();
	thread::delay(10);
	mPeri->lock();
	rt2 = mPeri->send(0x78, gLine2, 19, 300);
	mPeri->stop();
	mPeri->unlock();
	thread::delay(10);

	if (rt1 == false || rt2 == false)
		return false;
	else
		return true;
}

void JLX1602A_4::write(uint8_t line, uint8_t column, void *src)
{
	char *cSrc = (char *)src, *des;
	uint8_t len = strlen(cSrc);

	if (mDetectedFlag == false)
		return;

	if (line >= LINE_SIZE)
		return;

	if (column + len > COLUMN_SIZE)
		len = COLUMN_SIZE - column;

	if (line == 0)
	{
		des = &gLine1[3 + column];
	}
	else
	{
		des = &gLine2[3 + column];
	}

	for (uint8_t i = 0; i < len; i++)
	{
		*des++ = *cSrc++;
	}
}
}
}

#endif
