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

#include <task/display.h>
#include <task/task.h>
#include <gui/Bmp565Brush.h>
#include <util/ElapsedTime.h>
#include <util/Period.h>
#include <util/key.h>
#include <dev/key.h>
#include "bsp.h"
#include "status.h"
#include "parm.h"
#include <__cross_studio_io.h>

namespace task
{
namespace display
{
	error displayLogo(FunctionQueue *obj)
	{
		gMutex.lock();
		clear();
		gMutex.unlock();

		lcd.lock();
		lcd.setBgColor(0x00, 0xFF, 0x00);
		lcd.clear();
		//lcd.drawBmp(Pos{40, 113}, &logo);
		lcd.unlock();

		for(int i=0;i<100;i++)
		{
			setBackLight((float)i / 99.f);
			thread::delay(12);
		}

		thread::delay(3000);

		for(int i=0;i<100;i++)
		{
			setBackLight(1.f - (float)i / 99.f);
			thread::delay(12);
		}

		return Error::NONE;
	}

}
}
