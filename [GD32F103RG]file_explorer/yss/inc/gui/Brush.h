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

#ifndef YSS_GUI_BRUSH__H_
#define YSS_GUI_BRUSH__H_

#include "util.h"
#include <gui/Bmp1555.h>
#include <gui/Bmp565.h>
#include <gui/Bmp888.h>
#include <gui/Font.h>
#include <yss/Mutex.h>

class Brush
{
  protected:
	Size mSize;
	Mutex mMutex;
	Font mFont;

  public:
	Brush(void);
	~Brush(void);
	void setSize(Size size);

	void drawLine(Pos start, Pos end);
	void drawTriangle(Pos p1, Pos p2, Pos p3);
	void drawRect(Pos p1, Pos p2);
	void drawRect(Pos p1, Size size);
	void drawCircle(Pos p1, unsigned short r);
	unsigned char drawString(Pos pos, const char *str);
	unsigned char drawString(Pos pos, const char *str, unsigned char charWidth);

	void eraseRect(Pos p1, Pos p2);
	void eraseRect(Pos pos, Size size);
	void clear(void);

	Size calculateStringSize(const char *str);

	void setFont(Font font);


	virtual unsigned char drawChar(Pos pos, unsigned int utf8);
	virtual void drawBmp(Pos pos, const Bmp565 *image);
	virtual void drawBmp(Pos pos, const Bmp565 &image);

	virtual void drawBmp(Pos pos, const Bmp888 *image);
//	virtual void drawBmp(Pos pos, const Bmp888 &image);

	virtual void drawBmp(Pos pos, const Bmp1555 *image);
	virtual void drawBmp(Pos pos, const Bmp1555 &image);

	virtual void fill(void);
	virtual void fillCircle(Pos p1, unsigned short r);
	virtual void fillRect(Pos p1, Pos p2);
	virtual void fillRect(Pos pos, Size size);
	virtual void fillTriangle(Pos p1, Pos p2, Pos p3);

	virtual void drawDot(signed short x, signed short y) = 0;
	virtual void drawDot(signed short x, signed short y, unsigned short color) = 0;
	virtual void drawDot(signed short x, signed short y, unsigned int color) = 0;
	virtual void drawFontDot(signed short x, signed short y, unsigned char color) = 0;
	virtual void eraseDot(Pos pos) = 0;
	virtual void setBrushColor(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255) = 0;
	virtual void setFontColor(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255) = 0;
	virtual void setBackgroundColor(unsigned char red, unsigned char green, unsigned char blue) = 0;
};

#endif