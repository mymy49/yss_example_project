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
#include <yss/Directory.h>
#include <yss/Fat32.h>
#include <yss/File.h>

#include "../font/Abyssinica_SIL_32_B.h"
#include "../font/Abyssinica_SIL_16.h"

namespace task
{
namespace display
{
	void drawBackground(void);
	void fadeinBackLight(void);
	void fadeoutBackLight(void);
	void drawAlarmBackground(void);
	void drawAlarmTitle(const char * title);
	void drawFileList(int &index, int &scroll, int dirCnt, int fileCnt);
	void clearListBox(void);

	const Pos gListPos = {25, 60};

	Fat32 gFat32ForDir(sdmmc);
	Directory gDirectory(gFat32ForDir);

	error displayLogo(FunctionQueue *obj)
	{
		gMutex.lock();
		clear();
		gMutex.unlock();

		lcd.lock();
		lcd.setBackgroundColor(0x00, 0xFF, 0x00);
		lcd.clear();

		gBrush.setSize(280, 35);
		gBrush.setFont(Font_Abyssinica_SIL_32_B);
		gBrush.setFontColor(0x00, 0x00, 0x00);
		gBrush.setBackgroundColor(0x00, 0xFF, 0x00);

		gBrush.clear();
		gBrush.drawString(Pos{0, 0}, "LOGO Output");
		lcd.drawBmp(Pos{100, 142}, gBrush.getBmp888());
		//lcd.drawBmp(Pos{40, 113}, &logo);
		lcd.unlock();

		fadeinBackLight();
		thread::delay(3000);
		fadeoutBackLight();

		return Error::NONE;
	}

	void thread_displayFileList(void)
	{
		int index = 0, scroll = 0;
		bool upKeyFlag = false, downKeyFlag = false, enterKeyFlag = false, cancelKeyFlag = false;

		gDirectory.init();

		int dirCnt = gDirectory.getDirectoryCount(), fileCnt = gDirectory.getFileCount();
		int totalCnt = dirCnt + fileCnt;

		lcd.lock();
		drawBackground();
		drawFileList(index, scroll, dirCnt, fileCnt);
		fadeinBackLight();
		lcd.unlock();
		
		key::clear();
		key::addPushHandler(Key::getUpKey, upKeyFlag);
		key::addPushHandler(Key::getDownKey, downKeyFlag);
		key::addPushHandler(Key::getEnterKey, enterKeyFlag);
		key::addPushHandler(Key::getCancelKey, cancelKeyFlag);

		while(1)
		{
			if(upKeyFlag)
			{
				upKeyFlag = false;
				index--;
				if(index < 0)
				{
					if(totalCnt > 0)
						index = totalCnt - 1;
					else
						index = 0;
				}

				drawFileList(index, scroll, dirCnt, fileCnt);
			}

			if(downKeyFlag)
			{
				downKeyFlag = false;
				index++;
				if(index >= totalCnt)
				{
					index = 0;
				}

				drawFileList(index, scroll, dirCnt, fileCnt);
			}

			if(enterKeyFlag)
			{
				enterKeyFlag = false;
				if(index <= dirCnt)
				{
					gDirectory.enterDirectory(index);
					dirCnt = gDirectory.getDirectoryCount();
					fileCnt = gDirectory.getFileCount();
					totalCnt = dirCnt + fileCnt;
					index = 0;
					scroll = 0;
					clearListBox();
					drawFileList(index, scroll, dirCnt, fileCnt);
				}
			}

			if(cancelKeyFlag)
			{
				cancelKeyFlag = false;
				gDirectory.returnDirectory();
				dirCnt = gDirectory.getDirectoryCount();
				fileCnt = gDirectory.getFileCount();
				totalCnt = dirCnt + fileCnt;
				index = 0;
				scroll = 0;
				clearListBox();
				drawFileList(index, scroll, dirCnt, fileCnt);
			}

			thread::yield();
		}
	}

	void thread_displayNoSdmmc(void)
	{
		lcd.lock();
		drawBackground();
		drawAlarmBackground();
		drawAlarmTitle("Notice");
		fadeinBackLight();
		lcd.unlock();

		while(1)
		{
			lcd.lock();
			gBrush.setSize(300, 30);
			gBrush.setFont(Font_Abyssinica_SIL_32_B);
			gBrush.setFontColor(0xFF, 0x00, 0x00);
			gBrush.setBackgroundColor(0xFF, 0xFF, 0xFF);

			gBrush.clear();
			gBrush.drawString(Pos{0, 0}, "삽입된 SD메모리가");
			lcd.drawBmp(Pos{70, 110}, gBrush.getBmp888());

			gBrush.clear();
			gBrush.drawString(Pos{0, 0}, "없습니다.");
			lcd.drawBmp(Pos{70, 150}, gBrush.getBmp888());
			lcd.unlock();

			thread::delay(500);

			lcd.lock();
			gBrush.setSize(300, 30);
			gBrush.setBackgroundColor(0xFF, 0xFF, 0xFF);
			gBrush.clear();
			lcd.drawBmp(Pos{70, 110}, gBrush.getBmp888());
			lcd.drawBmp(Pos{70, 150}, gBrush.getBmp888());
			lcd.unlock();

			thread::delay(500);
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
		lcd.lock();
		fadeoutBackLight();
		lcd.unlock();

		return Error::NONE;
	}

	void drawBackground(void)
	{
		lcd.setBackgroundColor(0x30, 0x30, 0x00);
		lcd.clear();
		lcd.setBrushColor(0x30, 0x30, 0x30);
		lcd.fillRect(Pos{15, 15}, Size{479-30, 319-30});
		clearListBox();

		gBrush.setSize(200, 35);
		gBrush.setFont(Font_Abyssinica_SIL_32_B);
		gBrush.setFontColor(0x00, 0x00, 0x00);
		gBrush.setBackgroundColor(0x30, 0x30, 0x30);
		gBrush.clear();
		gBrush.drawString(Pos{0, 0}, "File Explorer");
		lcd.drawBmp(Pos{20, 20}, gBrush.getBmp888());
	}

	void drawAlarmBackground(void)
	{
		lcd.setBrushColor(0x00, 0x00, 0xFF);
		lcd.fillRect(Pos{60, 60}, Size{479-120, 319-120});
		lcd.setBrushColor(0xFF, 0xFF, 0xFF);
		lcd.fillRect(Pos{65, 100}, Size{479-130, 319-165});
	}

	void clearListBox(void)
	{
		lcd.setBrushColor(0x00, 0x00, 0x00);
		lcd.fillRect(Pos{20, 55}, Size{479-40, 319-75});
	}

	void drawAlarmTitle(const char * title)
	{
		gBrush.setSize(300, 30);
		gBrush.setFont(Font_Abyssinica_SIL_32_B);
		gBrush.setFontColor(0xFF, 0xFF, 0xFF);
		gBrush.setBackgroundColor(0x00, 0x00, 0xFF);
		gBrush.clear();
		gBrush.drawString(Pos{0, 0}, title);
		lcd.drawBmp(Pos{65, 65}, gBrush.getBmp888());
	}

	void drawFileList(int &index, int &scroll, int dirCnt, int fileCnt)
	{
		int count = 0;
		Pos pos = gListPos;

		thread::protect(); // name의 malloc을 다시 반환 하기 전까지 쓰레드가 외부에 의해 강제로 종료 되는 것을 보호
		char *name = new char[256];
		
		gBrush.setSize(429, 19);
		gBrush.setFont(Font_Abyssinica_SIL_16);
		
		for(int i=0;i<dirCnt;i++)
		{
			gDirectory.getDirectoryName(i, name, 256);

			if(index == count)
			{
				gBrush.setFontColor(0x00, 0x00, 0x00);
				gBrush.setBackgroundColor(0xFF, 0xFF, 0x00);
			}
			else
			{
				gBrush.setFontColor(0xFF, 0xFF, 0x00);
				gBrush.setBackgroundColor(0x00, 0x00, 0x00);
			}

			gBrush.clear();
			gBrush.drawString(Pos{5, 1}, name);
			lcd.drawBmp(pos, gBrush.getBmp888());
			pos.y += 19;

			count++;
			if(count >= 12)
				goto complete;
		}
		

complete :
		delete name;
		thread::unprotect(); // name에 할당된 메모리가 정상적으로 반환된 이후로 쓰레드 보호 해제
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
