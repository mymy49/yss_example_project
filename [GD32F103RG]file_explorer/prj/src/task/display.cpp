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

#include "../font/Abyssinica_SIL_32_B.h"

namespace task
{
namespace display
{
	void drawBackground(void);
	void fadeinBackLight(void);
	void fadeoutBackLight(void);
	void drawAlarmBackground(void);

	error displayLogo(FunctionQueue *obj)
	{
		gMutex.lock();
		clear();
		gMutex.unlock();

		lcd.lock();
		lcd.setBackgroundColor(0x00, 0xFF, 0x00);
		lcd.clear();
		//lcd.drawBmp(Pos{40, 113}, &logo);
		lcd.unlock();

		fadeinBackLight();
		thread::delay(3000);
		fadeoutBackLight();

		return Error::NONE;
	}

	void thread_displayFileList(void)
	{
		drawBackground();
		fadeinBackLight();

		while(1)
		{

		}
	}

	void thread_displayNoSdmmc(void)
	{
		drawBackground();
		drawAlarmBackground();
		fadeinBackLight();

		while(1)
		{

		}
	}

	void thread_checkSdmmc(void)
	{
		bool lastSdCardFlag = sdmmc.isConnected();

		while(1)
		{
			if(lastSdCardFlag != sdmmc.isConnected())
			{
				gFq.lock();
				gFq.add(fadeoutBackLight);
				gFq.add(displayFileList);
				gFq.unlock();
				while(1)
					thread::yield();
			}
		}
	}

	error displayFileList(FunctionQueue *obj)
	{
		gMutex.lock();
		clear();
		gMutex.unlock();
		
		if(sdmmc.isConnected())
			gThreadId[0] = thread::add(thread_displayFileList, 2048);
		else
			gThreadId[0] = thread::add(thread_displayNoSdmmc, 2048);
		
		gThreadId[1] = thread::add(thread_checkSdmmc, 512);

		return Error::NONE;
	}

	error fadeoutBackLight(FunctionQueue *obj)
	{
		fadeoutBackLight();

		return Error::NONE;
	}

	void drawBackground(void)
	{
		lcd.lock();
		lcd.setBackgroundColor(0x30, 0x30, 0x00);
		lcd.clear();
		lcd.setBrushColor(0x30, 0x30, 0x30);
		lcd.fillRect(Pos{15, 15}, Size{479-30, 319-30});
		lcd.unlock();
	}

	void drawAlarmBackground(void)
	{
		lcd.lock();
		lcd.setBrushColor(0x00, 0x00, 0x00);
		lcd.fillRect(Pos{60, 60}, Size{479-120, 319-120});
		lcd.unlock();
	}

	void fadeinBackLight(void)
	{
		for(int i=0;i<100;i++)
		{
			setBackLight((float)i / 99.f);
			thread::delay(6);
		}
	}

	void fadeoutBackLight(void)
	{
		for(int i=0;i<100;i++)
		{
			setBackLight(1.f - (float)i / 99.f);
			thread::delay(6);
		}
	}
}
}
