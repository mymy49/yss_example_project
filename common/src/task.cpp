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

#include "../inc/task.h"
#include <yss.h>

#define MAX_TASK_THREAD		4

namespace Task
{
	uint32_t gThreadCnt;
	threadId gThreadId[MAX_TASK_THREAD];
	FunctionQueue *gFq;
	Mutex gMutex;

#if USE_GUI && YSS_L_HEAP_USE
	Frame *gFrame;
#endif

	void setFunctionQueue(FunctionQueue &obj)
	{
		gFq = &obj;
	}

	void lock(void)
	{
		gMutex.lock();
	}

	void unlock(void)
	{
		gMutex.unlock();
	}

	void addThread(void (*func)(void), uint32_t stackSize)
	{
		if(gThreadCnt < MAX_TASK_THREAD)
			gThreadId[gThreadCnt++] = thread::add(func, stackSize);
	}

	void clearTask(void)
	{
		for(uint32_t i=0;i<gThreadCnt;i++)
		{
			if(gThreadId[i])
			{
				thread::remove(gThreadId[i]);
				gThreadId[i] = 0;
			}
		}

#if USE_GUI && YSS_L_HEAP_USE
		if(gFrame)
		{
			delete gFrame;
			gFrame = 0;
		}
#endif
	}

#if USE_GUI && YSS_L_HEAP_USE
	void setFrame(Frame *obj)
	{
		setSystemFrame(obj);
		gFrame = obj;
	}
#endif
}

