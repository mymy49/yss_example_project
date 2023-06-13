////////////////////////////////////////////////////////////////////////////////////////
//
// 저작권 표기 License_ver_3.2
// 본 소스 코드의 소유권은 홍윤기에게 있습니다.
// 어떠한 형태든 기여는 기증으로 받아들입니다.
// 본 소스 코드는 아래 사항에 동의할 경우에 사용 가능합니다.
// 아래 사항에 대해 동의하지 않거나 이해하지 못했을 경우 사용을 금합니다.
// 본 소스 코드를 사용하였다면 아래 사항을 모두 동의하는 것으로 자동 간주 합니다.
// 본 소스 코드의 상업적 또는 비 상업적 이용이 가능합니다.
// 본 소스 코드의 내용을 임의로 수정하여 재배포하는 행위를 금합니다.
// 본 소스 코드의 사용으로 인해 발생하는 모든 사고에 대해서 어떠한 법적 책임을 지지 않습니다.
// 본 소스 코드의 어떤 형태의 기여든 기증으로 받아들입니다.
//
// Home Page : http://cafe.naver.com/yssoperatingsystem
// Copyright 2023. 홍윤기 all right reserved.
//
////////////////////////////////////////////////////////////////////////////////////////

#include <yss.h>
#include <yss/debug.h>
#include <bsp.h>
#include <dev/led.h>

triggerId gTriggerId;

void trigger_handleMic(void)
{
	uint32_t count = pdm0.getCount();
	uint32_t *data = pdm0.getCurrentBuffer();

	for(uint32_t i=0;i<count;i++)
	{
		debug_printf("%d\n", (int32_t)*data++ >> 16);
	}

	pdm0.releaseBuffer(count);
}

void isr_timer1(void)
{
	trigger::run(gTriggerId);
}

void thread_sendHelloWorld(void)
{
	while(1)
	{
		uart1.send("hello world!!\n\r", sizeof("hello world!!\n\r")); 
	}
}

static const Spi::Specification gLcdSpec =
{
	define::spi::mode::MODE0,	//uint8_t mode;
	10000000,					//uint32_t maxFreq;
	define::spi::bit::BIT8		//uint8_t bit;
};

int main(int argc, char *argv[])
{
	initializeYss();
	initBoard();
	
	gTriggerId = trigger::add(trigger_handleMic, 1024);

	timer1.enableClock();
	timer1.initialize(1000);
	timer1.enableUpdateInterrupt();
	timer1.setUpdateIsr(isr_timer1);
	timer1.enableInterrupt();
	timer1.start();

	thread::add(thread_sendHelloWorld, 1024);

	spi0.setSpecification(gLcdSpec);

	while(1)
	{
		spi0.send('A');
	}

	while(1)
	{
		Led::setOn(true);
		thread::delay(500);

		Led::setOn(false);
		thread::delay(500);
	}

	return 0;
}
