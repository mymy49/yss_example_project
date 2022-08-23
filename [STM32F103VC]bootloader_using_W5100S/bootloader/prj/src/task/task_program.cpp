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
#include <Protocol.h>


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
		gFq.add(task::program::programNewFirmware);
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
void thread_blinkLedConnectingToServer(void)
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
	clear();	// 기존에 등록된 쓰레드 제거
	gThreadId[0] = thread::add(thread_blinkLedConnectingToServer, 512);
	gMutex.unlock();

	const WiznetSocket::Host host =	// 호스트 주소 설정
	{
		{192, 168, 0, 17},	//unsigned char ip[4];
		8080				//unsigned short port;
	};

repeat :
	do
	{
		socket0.lock();
		result = socket0.connectToHost(host);	// 호스트에 연결 시도
		socket0.unlock();
		if(result != Error::NONE)
			thread::delay(3000);
	}while(result != Error::NONE);
	
	socket0.lock();
	result = socket0.waitUntilConnect();	// 소켓이 연결 될 때까지 대기
	socket0.unlock();
	
	// 연결 대기에서 연결에 실패하면 repeat로 돌아가 다시 시도
	if(result != Error::NONE)	
		goto repeat;

	return Error::NONE;
}

// 노란색 점멸
void thread_blinkLedCheckFirmwareFromServer(void)
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
	gThreadId[0] = thread::add(thread_blinkLedCheckFirmwareFromServer, 512);
	gMutex.unlock();

start:
	// 서버와 연결 상태 확인
	if(gProtocol.sendMessage(Protocol::MSG_HOW_ARE_YOU, 0, 0) != Error::NONE)
		goto start;
	
	if(gProtocol.getReceivedMessage() != Protocol::MSG_I_AM_FINE)
	{
		thread::delay(1000);
		goto start;
	}
	
	// 새 펌웨어가 있는지 확인 
	if(gProtocol.sendMessage(Protocol::MSG_HAVE_YOU_NEW_FIRMWARE, 0, 0) != Error::NONE)
		goto start;
	
	switch(gProtocol.getReceivedMessage())
	{
	// 새 펌웨어가 없으면 1초후 다시 문의
	case Protocol::MSG_NO_I_HAVE_NOT :
	default :
		thread::delay(1000);
		goto start;
		break;
	
	// 새 펌웨어가 있으면 리턴해서 다음 단계 진행
	case Protocol::MSG_YES_I_HAVE :
		break;
	}
	
	return Error::NONE;
}

// 초록색 점멸
void thread_blinkLedProgramNewFirmware(void)
{
	Period period(25000);

	while(1)
	{
		for(int i=0;i<256;i+=16)
		{
			period.wait();
			Led::setRgb(0, i, 0);
		}
		
		for(int i=0;i<256;i+=16)
		{
			period.wait();
			Led::setRgb(0, 255-i, 0);
		}
	}
}

error programNewFirmware(FunctionQueue *obj)
{
	error result;
	unsigned short size;
	unsigned char *data, *des = (unsigned char*)0x08010000;
	unsigned int packetCount, thisPacket;
	bool completeFlag;

	gMutex.lock();
	clear();
	gThreadId[0] = thread::add(thread_blinkLedProgramNewFirmware, 512);
	gMutex.unlock();

start:
	// 전체 펌웨어 패킷의 수를 요청 (1 패킷에 256 바이트)
	if(gProtocol.sendMessage(Protocol::MSG_GIVE_ME_TOTAL_PACKET, 0, 0) != Error::NONE)
		goto start;
	
	if(gProtocol.getReceivedMessage() != Protocol::MSG_IT_IS_TOTAL_PACKET)
	{
		thread::delay(1000);
		goto start;
	}

	size = gProtocol.getReceivedDataSize();
	data = gProtocol.getReceivedData();

	if(size == 4)
	{
		packetCount = *(unsigned int*)&data[0];
	}
	
	for(int i=0;i<packetCount;i++)
	{
		if(i % 8 == 0)
			flash.erase(32+i/8);

		while(1)
		{
			while(gProtocol.sendMessage(Protocol::MSG_GIVE_ME_FIRMWARE_PACKET, (unsigned char*)&i, 4) != Error::NONE)
				thread::delay(1000);
			
			size = gProtocol.getReceivedDataSize();
			data = gProtocol.getReceivedData();
			thisPacket = *(unsigned int*)&data[0];
			
			if(thisPacket != i)
				continue;
			
			flash.program(des, &data[4], size-4);
			des += size-4;
			break;
		}
	}

	thread::yield();

	return Error::NONE;
}

// 파란색 점멸
void thread_blinkLedFinish(void)
{
	Period period(25000);

	while(1)
	{
		for(int i=0;i<256;i+=16)
		{
			period.wait();
			Led::setRgb(0, 0, i);
		}
		
		for(int i=0;i<256;i+=16)
		{
			period.wait();
			Led::setRgb(0, 0, 255-i);
		}
	}
}

error finish(FunctionQueue *obj)
{
	error result;

	gMutex.lock();
	clear();
	gThreadId[0] = thread::add(thread_blinkLedFinish, 512);
	gMutex.unlock();
	
	return Error::NONE;
}
}
}
