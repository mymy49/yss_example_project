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

#include <dev/led.h>
#include <task/program.h>
#include <task/task.h>
#include <util/Period.h>
#include <util/ElapsedTime.h>
#include <util/Timeout.h>
#include <bsp.h>
#include <__cross_studio_io.h>
#include <string.h>
#include <protocol.h>


namespace task
{
namespace program
{
void callback_linkup(bool linkup)
{
	gMutex.lock();
	clear();
	gMutex.unlock();
	
	Led::setRgb(0, 0, 0);

	if(linkup)
	{
		gFq.lock();
		gFq.stop();
		gFq.clear();
		gFq.add(task::program::connectingToServer);
		gFq.add(task::program::checkFirmwareFromServer);
		gFq.add(task::program::finish);
		gFq.start();
		gFq.unlock();
	}
	else
	{
		gFq.lock();
		gFq.stop();
		gFq.clear();
		gFq.unlock();
	}
}

// 빨간색 점멸
void thread_blinkConnectingToServerLed(void)
{
	Period period(25000);

	while(1)
	{
		for(int i=0;i<256;i+=16)
		{
			period.wait();
			Led::setRgb(i, 0, 0);
		}
		
		for(int i=0;i<256;i+=16)
		{
			period.wait();
			Led::setRgb(255-i, 0, 0);
		}
	}
}

error connectingToServer(FunctionQueue *obj)
{
	error result;

	gMutex.lock();
	clear();
	gThreadId[0] = thread::add(thread_blinkConnectingToServerLed, 512);
	gMutex.unlock();

	while(!w5100s.isLinkup())
		thread::yield();

	const WiznetSocket::Host host =
	{
		{192, 168, 0, 17},	//unsigned char ip[4];
		8080				//unsigned short port;
	};

repeat :
	do
	{
		socket0.lock();
		result = socket0.connectToHost(host);
		socket0.unlock();
		if(result != Error::NONE)
			thread::delay(3000);
	}while(result != Error::NONE);
	
	socket0.lock();
	result = socket0.waitUntilConnect();
	socket0.unlock();

	if(result != Error::NONE)
		goto repeat;

	return Error::NONE;
}

// 노란색 점멸
void thread_blinkCheckFirmwareFromServerLed(void)
{
	Period period(25000);

	while(1)
	{
		for(int i=0;i<256;i+=16)
		{
			period.wait();
			Led::setRgb(i, i, 0);
		}
		
		for(int i=0;i<256;i+=16)
		{
			period.wait();
			Led::setRgb(255-i, 255-i, 0);
		}
	}
}

error checkFirmwareFromServer(FunctionQueue *obj)
{
	error result;

	gMutex.lock();
	clear();
	gThreadId[0] = thread::add(thread_blinkCheckFirmwareFromServerLed, 512);
	gMutex.unlock();

start:

	gProtocol.sendMessage(Protocol::MSG_HOW_ARE_YOU, 0, 0);
	if(gProtocol.waitUntilComplete() != Error::NONE)
		goto start;
	
	debug_printf("Firmware Server handshake OK!!\n");
	return Error::NONE;
}

error finish(FunctionQueue *obj)
{
	error result;

	gMutex.lock();
	clear();
	Led::setRgb(0, 0, 0);
	gMutex.unlock();
	
	return Error::NONE;
}
}
}
