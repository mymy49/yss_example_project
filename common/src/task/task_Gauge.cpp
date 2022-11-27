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

#include <task.h>
#include <yss.h>
#include <gui/Gauge.h>
#include "../font/Abyssinica_SIL_25.h"
#include "../font/Abyssinica_SIL_10.h"

#if USE_GUI && YSS_L_HEAP_USE

namespace Task
{
	static float gValue1, gValue2, gValue3;
	static bool gTitleChangeStartFlag;

	Gauge *gGauge1;
	Gauge *gGauge2;
	Gauge *gGauge3;

	static void thread_displayGauge(void)
	{
		Size lcdSize = ltdc.getLcdSize();
		Size gaugeSize1, gaugeSize2, gaugeSize3;
		Position gaugePos1, gaugePos2, gaugePos3;
		float lastValue1 = 0, lastValue2 = 0, lastValue3 = 0;
		float buf;
		
		// 화면 구성 요소의 좌표와 크기 계산
		if(lcdSize.width > lcdSize.height)
		{
			gaugeSize1.height = lcdSize.height * 0.8f;
			gaugeSize1.width = lcdSize.height * 0.8f;
			buf = lcdSize.width - gaugeSize1.width;
			gaugeSize3.height = gaugeSize2.height = buf * 0.8f;
			gaugeSize3.width = gaugeSize2.width = buf * 0.8f;

		}
		else
		{
			gaugeSize1.height = lcdSize.width * 0.8f;
			gaugeSize1.width = lcdSize.width * 0.8f;
			buf = lcdSize.height - gaugeSize1.width;
			gaugeSize3.height = gaugeSize2.height = buf * 0.8f;
			gaugeSize3.width = gaugeSize2.width = buf * 0.8f;

			gaugePos1.x = lcdSize.width * 0.1f;
			gaugePos1.y = 0;

			gaugePos2.x = buf * 0.1f;
			gaugePos2.y = gaugeSize1.height;

			gaugePos3.x = buf * 0.1f + gaugePos2.x + gaugeSize2.width;
			gaugePos3.y = gaugeSize1.height;
		}
	
		// 화면 구성 instance를 생성하고 설정		
		thread::protect(); // frm이 새로 생성되고 setFrame() 함수에 전달 될 때까지 보호
		Frame *frm = new Frame;
		gGauge1 = new Gauge;
		gGauge2 = new Gauge;
		gGauge3 = new Gauge;
		
		frm->setBackgroundColor(0xA0, 0xA5, 0xA8);

		gGauge1->setPosition(gaugePos1);
		gGauge1->setBackgroundColor(0xA0, 0xA5, 0xA8);
		gGauge1->setSize(gaugeSize1);
		gGauge1->setTopValue(500);
		gGauge1->setFont(Font_Abyssinica_SIL_25);
		gGauge1->setTitle("Value1");
		gGauge1->makeBg();

		gGauge2->setBackgroundColor(0xA0, 0xA5, 0xA8);
		gGauge2->setSize(gaugeSize2);
		gGauge2->setTopValue(250);
		gGauge2->setFont(Font_Abyssinica_SIL_10);
		gGauge2->setTitle("Value2");
		gGauge2->makeBg();
		gGauge2->setPosition(gaugePos2);
		
		gGauge3->setBackgroundColor(0xA0, 0xA5, 0xA8);
		gGauge3->setSize(gaugeSize3);
		gGauge3->setTopValue(125);
		gGauge3->setFont(Font_Abyssinica_SIL_10);
		gGauge3->setTitle("Value3");
		gGauge3->makeBg();
		gGauge3->setPosition(gaugePos3);

		frm->add(gGauge1);
		frm->add(gGauge2);
		frm->add(gGauge3);

		setFrame(frm);
		gTitleChangeStartFlag = true;
		thread::unprotect();
		
		// 게이지 표시 값에 변경 사항이 있으면 업데이트하는 루프
		while(1)
		{
			if(gValue1 != lastValue1)
			{
				lastValue1 = gValue1;
				gGauge1->setValue(lastValue1);
			}

			if(gValue2 != lastValue2)
			{
				lastValue2 = gValue2;
				gGauge2->setValue(lastValue2);
			}

			if(gValue3 != lastValue3)
			{
				lastValue3 = gValue3;
				gGauge3->setValue(lastValue3);
			}

			thread::yield();
		}
	}
	
	// 화면에 표시된 게이지 이름을 1초마다 다른 외국어로 변경하는 쓰레드
	static void thread_changeTitle(void)
	{
		while(!gTitleChangeStartFlag)
			thread::yield();
		
		while(1)
		{
			thread::delay(5000);
			gGauge1->setTitle("値1");
			gGauge1->makeBg();
			gGauge2->setTitle("値2");
			gGauge2->makeBg();
			gGauge3->setTitle("値3");
			gGauge3->makeBg();
			
			thread::delay(5000);
			gGauge1->setTitle("значення1");
			gGauge1->makeBg();
			gGauge2->setTitle("значення2");
			gGauge2->makeBg();
			gGauge3->setTitle("значення3");
			gGauge3->makeBg();

			thread::delay(5000);
			gGauge1->setTitle("ค่า1");
			gGauge1->makeBg();
			gGauge2->setTitle("ค่า2");
			gGauge2->makeBg();
			gGauge3->setTitle("ค่า3");
			gGauge3->makeBg();

			thread::delay(5000);
			gGauge1->setTitle("Value1");
			gGauge1->makeBg();
			gGauge2->setTitle("Value2");
			gGauge2->makeBg();
			gGauge3->setTitle("Value3");
			gGauge3->makeBg();
		}
	}
	
	// 게이지에 표시되는 값을 증가시키는 쓰레드
	static void thread_upcountValue(void)
	{
		while(1)
		{
			gValue1 += 1;
			if(gValue1 >= 500)
				gValue1 = 0;

			gValue2 += 1;
			if(gValue2 >= 250)
				gValue2 = 0;

			gValue3 += 1;
			if(gValue3 >= 125)
				gValue3 = 0;

			thread::delay(10);
		}
	}

	error displayGauge(FunctionQueue *obj)
	{
		lock(); // unlock()을 만날 때까지 외부에서 이 함수를 강제 종료 시키지 못한다.
		gTitleChangeStartFlag = false;
		clearTask();
		addThread(thread_displayGauge, 2048);
		addThread(thread_upcountValue, 2048);
		addThread(thread_changeTitle, 2048);
		unlock();

		return Error::NONE;
	}
}

#endif

