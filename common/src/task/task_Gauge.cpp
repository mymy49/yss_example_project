
#include <task.h>
#include <yss/gui.h>
#include "../font/Abyssinica_SIL_25.h"
#include <yss.h>
#include <stdio.h>
#include <util/key.h>
#include <gui/Gauge.h>

namespace Task
{
	static float gValue1, gValue2, gValue3;
	static bool gTitleChangeStartFlag;

	Gauge *gGauge1;
	Gauge *gGauge2;
	Gauge *gGauge3;

	static void thread_displayGauge(void)
	{
		float lastValue1 = 0, lastValue2 = 0, lastValue3 = 0;

		thread::protect(); // frm이 새로 생성되고 gFrame에 무사히 대입 될 때까지 보호
		Frame *frm = new Frame;
		gGauge1 = new Gauge;
		gGauge2 = new Gauge;
		gGauge3 = new Gauge;
		
		thread::unprotect();

		frm->setBackgroundColor(0xA0, 0xA5, 0xA8);

		gGauge1->setPosition(Position{60, 10});
		gGauge1->setBackgroundColor(0xA0, 0xA5, 0xA8);
		gGauge1->setSize(200, 200);
		gGauge1->setTopValue(500);
		gGauge1->setFont(Font_Abyssinica_SIL_25);
		gGauge1->setTitle("Value1");
		gGauge1->makeBg();

		gGauge2->setBackgroundColor(0xA0, 0xA5, 0xA8);
		gGauge2->setSize(150, 150);
		gGauge2->setTopValue(250);
		gGauge2->setFont(Font_Abyssinica_SIL_25);
		gGauge2->setTitle("Value2");
		gGauge2->makeBg();
		gGauge2->setPosition(5, 220);
		
		gGauge3->setBackgroundColor(0xA0, 0xA5, 0xA8);
		gGauge3->setSize(150, 150);
		gGauge3->setTopValue(125);
		gGauge3->setFont(Font_Abyssinica_SIL_25);
		gGauge3->setTitle("Value3");
		gGauge3->makeBg();
		gGauge3->setPosition(165, 220);

		frm->add(gGauge1);
		frm->add(gGauge2);
		frm->add(gGauge3);

		setFrame(frm);
		gTitleChangeStartFlag = true;

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

	static void thread_changeTitle(void)
	{
		while(!gTitleChangeStartFlag)
			thread::yield();
		
		while(1)
		{
			thread::delay(1000);
			gGauge1->setTitle("値1");
			gGauge1->makeBg();
			gGauge2->setTitle("値2");
			gGauge2->makeBg();
			gGauge3->setTitle("値3");
			gGauge3->makeBg();
			
			thread::delay(1000);
			gGauge1->setTitle("значення1");
			gGauge1->makeBg();
			gGauge2->setTitle("значення2");
			gGauge2->makeBg();
			gGauge3->setTitle("значення3");
			gGauge3->makeBg();

			thread::delay(1000);
			gGauge1->setTitle("ค่า1");
			gGauge1->makeBg();
			gGauge2->setTitle("ค่า2");
			gGauge2->makeBg();
			gGauge3->setTitle("ค่า3");
			gGauge3->makeBg();

			thread::delay(1000);
			gGauge1->setTitle("Value1");
			gGauge1->makeBg();
			gGauge2->setTitle("Value2");
			gGauge2->makeBg();
			gGauge3->setTitle("Value3");
			gGauge3->makeBg();
		}
	}

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
		lock();
		gTitleChangeStartFlag = false;
		clearTask();
		addThread(thread_displayGauge, 2048);
		addThread(thread_upcountValue, 2048);
		addThread(thread_changeTitle, 2048);
		unlock();

		return Error::NONE;
	}
}