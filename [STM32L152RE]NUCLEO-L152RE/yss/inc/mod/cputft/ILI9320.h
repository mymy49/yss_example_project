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

#ifndef YSS_MOD_CPUTFT_ILI9320__H_
#define YSS_MOD_CPUTFT_ILI9320__H_

#include <sac/CpuTft.h>
#include <yss/instance.h>

#if !(defined(YSS_DRV_SPI_UNSUPPORTED) || defined(YSS_DRV_GPIO_UNSUPPORTED))

class ILI9320 : public sac::CpuTft
{
  public:
	struct Config
	{
		Spi &peri;
		Size displayResolution;
		Gpio::Pin chipSelect;
		Gpio::Pin dataCommand;
		Gpio::Pin reset;
		uint8_t madctl;
	};

	enum
	{
		Y_MIRROR = 0x80,
		X_MIRROR = 0x40,
		V_MIRROR = 0x20
	};

	ILI9320(void);

	bool init(const Config config);

	void drawDots(uint16_t x, uint16_t y, uint16_t color, uint16_t size);
	void drawDots(uint16_t x, uint16_t y, uint16_t *src, uint16_t size);

	void drawDot(int16_t x, int16_t y);
	void drawDot(int16_t x, int16_t y, uint16_t color);
	void drawDot(int16_t x, int16_t y, uint32_t color);
	void drawFontDot(int16_t x, int16_t y, uint8_t color);
	void eraseDot(Position pos);
	void setColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255);
	void setFontColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255);
	void setBgColor(uint8_t red, uint8_t green, uint8_t blue);

	virtual void drawBmp(Position pos, const Bmp565 *image);
	virtual void drawBmp(Position pos, const Bmp565 &image);

  private:
	void sendCmd(uint8_t cmd);
	void sendCmd(uint8_t cmd, uint16_t data);
	void sendData(void *src, uint32_t size);

	Spi *mPeri;
	Gpio::Pin mCs, mDc, mRst;

	uint16_t *mLineBuffer;
	uint32_t mLineBufferSize;
};

#endif

#endif