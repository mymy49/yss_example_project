////////////////////////////////////////////////////////////////////////////////////////
//
// 저작권 표기 License_ver_3.1
// 본 소스 코드의 소유권은 홍윤기에게 있습니다.
// 어떠한 형태든 기여는 기증으로 받아들입니다.
// 본 소스 코드는 아래 사항에 동의할 경우에 사용 가능합니다.
// 아래 사항에 대해 동의하지 않거나 이해하지 못했을 경우 사용을 금합니다.
// 본 소스 코드를 사용하였다면 아래 사항을 모두 동의하는 것으로 자동 간주 합니다.
// 본 소스 코드의 상업적 또는 비 상업적 이용이 가능합니다.
// 본 소스 코드의 내용을 임의로 수정하여 재배포하는 행위를 금합니다.
// 본 소스 코드의 사용으로 인해 발생하는 모든 사고에 대해서 어떠한 법적 책임을 지지 않습니다.
//
// Home Page : http://cafe.naver.com/yssoperatingsystem
// Copyright 2022. 홍윤기 all right reserved.
//
////////////////////////////////////////////////////////////////////////////////////////

#ifndef YSS_MOD_TFT_LCD_DRIVER_ST7796__H_
#define YSS_MOD_TFT_LCD_DRIVER_ST7796__H_

#include <gui/BrushRgb888.h>
#include <gui/Bmp888Buffer.h>
#include <mod/tft_lcd_driver/ST7796S.h>

class ST7796S_with_Brush_RGB888 : public ST7796S, public BrushRgb888
{
  protected:
	Bmp888Buffer *mBmp888Buffer;
	uint32_t mBmp888BufferSize, mBrushColorCode;

  public:
	ST7796S_with_Brush_RGB888(void);
	void setBmp888Buffer(Bmp888Buffer &obj);

	// Brush
	virtual void drawDot(int16_t x, int16_t y); // virtual 0
	virtual void drawDot(int16_t x, int16_t y, uint32_t color); // virtual 0
	virtual void drawDot(int16_t x, int16_t y, Color color); // virtual 0

	virtual void eraseDot(Position pos); // virtual 0
	virtual void clear(void); // virtual
	virtual void fillRect(Position p1, Position p2);
	virtual void fillRect(Position pos, Size size);
	virtual void drawBmp(Position pos, const Bmp888 *image);
	virtual void setBrushColor(Color color);
	virtual void setBrushColor(uint8_t red, uint8_t green, uint8_t blue);
};

#endif