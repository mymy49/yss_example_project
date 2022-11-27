////////////////////////////////////////////////////////////////////////////////////////
//
// 저작권 표기 License_ver_2.0
// 본 소스코드의 소유권은 yss Embedded Operating System 네이버 카페 관리자와 운영진에게 있습니다.
// 운영진이 임의로 코드의 권한을 타인에게 양도할 수 없습니다.
// 본 소스코드는 아래 사항에 동의할 경우에 사용 가능합니다.
// 아래 사항에 대해 동의하지 않거나 이해하지 못했을 경우 사용을 금합니다.
// 본 소스코드를 사용하였다면 아래 사항을 모두 동의하는 것으로 자동 간주 합니다.
// 본 소스코드의 상업적 또는 비상업적 이용이 가능합니다.
// 본 소스코드의 내용을 임의로 수정하여 재배포하는 행위를 금합니다.
// 본 소스코드의 내용을 무단 전재하는 행위를 금합니다.
// 본 소스코드의 사용으로 인해 발생하는 모든 사고에 대해서 어떤한 법적 책임을 지지 않습니다.
//
//	Home Page : http://cafe.naver.com/yssoperatingsystem
//	Copyright 2020.	yss Embedded Operating System all right reserved.
//  
//  주담당자 : 아이구 (mymy49@nate.com) 2016.04.30 ~ 현재
//  부담당자 : -
//
////////////////////////////////////////////////////////////////////////////////////////

#include <yss/thread.h>
#include <config.h>
#include <internal/malloc.h>
#include <drv/peripheral.h>

#if defined(CCMSRAM_BASE)
#define CCMDATARAM_BASE	CCMSRAM_BASE
#endif

#if YSS_C_HEAP_USE && defined(CCMDATARAM_BASE)

static uint32_t gWaitNum, gCurrentNum;

static Malloc::MallocSet gMallocSet = 
{
	(void*)YSS_C_HEAP_BASE_ADDR, 
	(Malloc::MallocTable*)YSS_C_HEAP_TABLE_BASE_ADDR, 
	(uint32_t *)YSS_C_HEAP_CLUSTER_BASE_ADDR, 
	YSS_C_HEAP_TOTAL_CLUSTER_SIZE, YSS_C_HEAP_CLUSTER_SIZE,
	YSS_C_MAX_NUM_OF_MALLOC,
	CCMDATARAM_BASE + YSS_C_HEAP_SIZE
};

void* cmalloc(uint32_t size)
{
	void *addr;
	uint32_t myNum;
	
	thread::protect();
	__disable_irq();
	myNum = gWaitNum;
	gWaitNum++;
	__enable_irq();

	while(myNum != gCurrentNum)
	{
		thread::yield();
	}

	addr = Malloc::malloc(gMallocSet, size);

	__disable_irq();
	gCurrentNum++;
	__enable_irq();
	thread::unprotect();

	return addr;
}

void cfree(void *addr)
{
	uint32_t myNum;
	
	thread::protect();
	__disable_irq();
	myNum = gWaitNum;
	gWaitNum++;
	__enable_irq();

	while(myNum != gCurrentNum)
	{
		thread::yield();
	}

	Malloc::free(gMallocSet, addr);

	__disable_irq();
	gCurrentNum++;
	__enable_irq();
	thread::unprotect();
}

#endif

