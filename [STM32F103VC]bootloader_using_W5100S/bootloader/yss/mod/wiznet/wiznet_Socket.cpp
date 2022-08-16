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

#include <yss/instance.h>
#include <mod/wiznet/WiznetSocket.h>
#include <mod/wiznet/iEhternet.h>
#include <util/Timeout.h>
#include <__cross_studio_io.h>

#ifndef YSS_DRV_SPI_UNSUPPORTED

WiznetSocket::WiznetSocket(void)
{
	mStatusFlag = 0;
}

error WiznetSocket::init(iEthernet &obj, unsigned char socketNumber)
{
	if(socketNumber >= obj.getSocketLength())
		return Error::OUT_OF_RANGE;

	mPeri = &obj;
	mPeri->lock();
	if(mPeri->isWorking())
		mStatusFlag |= INITIALIZATION;
	mSocketNumber = socketNumber;
	mPeri->setSocket(socketNumber, *this);
	mPeri->unlock();

	if(mStatusFlag & INITIALIZATION)
	{
		mPeri->lock();
		mPeri->setSocketInterruptEnable(socketNumber, true);
		mPeri->unlock();
		return Error::NONE;
	}
	else
		return Error::NOT_INITIALIZED;
}

error WiznetSocket::connectToHost(const Host &host)
{
	if(~mStatusFlag & INITIALIZATION)
		return Error::NOT_INITIALIZED;

	unsigned char status;

	thread::yield();

	mPeri->lock();
	if(mPeri->setSocketMode(mSocketNumber, TCP, 0) == false)
	{
		mPeri->unlock();
		return Error::OUT_OF_RANGE;
	}

	mPeri->setSocketPort(mSocketNumber, host.port);
	mPeri->setSocketDestinationPort(mSocketNumber, host.port);
	mPeri->setSocketDestinationIpAddress(mSocketNumber, (unsigned char*)host.ip);
	mPeri->unlock();
	
	for(int i=0;i<3;i++)
	{
		mPeri->lock();
		mPeri->command(mSocketNumber, OPEN);
		if(mPeri->getSocketStatus(mSocketNumber) != TCP_SOCKET_OPEN_OK)
		{
			mPeri->unlock();
			thread::delay(500);
			continue;
		}
		mPeri->unlock();
	
		mPeri->lock();
		mPeri->command(mSocketNumber, CONNECT);
		mPeri->unlock();
		
		// 여기서 딜레이를 안주면 status가 제대로 업데이트 안됨.
		thread::delay(5);

		mPeri->lock();
		status = mPeri->getSocketStatus(mSocketNumber);
		mPeri->unlock();

		if(status == SOCKET_ESTABLISHED)
			return Error::NONE;
		else if(status == SOCKET_CONNECT_REQUEST_SENT)
			return Error::NONE;

		thread::delay(500);
	}
	
	return Error::TIMEOUT;
}

error WiznetSocket::waitUntilConnect(unsigned int timeout)
{
	if(~mStatusFlag & INITIALIZATION)
		return Error::NOT_INITIALIZED;

	Timeout tout(timeout);
	unsigned char status;

	while(!tout.isTimeout())
	{
		mPeri->lock();
		status = mPeri->getSocketStatus(mSocketNumber);
		mPeri->unlock();

		if(status == SOCKET_ESTABLISHED)
			return Error::NONE;
		else if(status == 0)
			return Error::UNKNOWN;
		
		thread::delay(100);
	}

	return Error::TIMEOUT;
}

error WiznetSocket::sendData(void *src, unsigned int count)
{
	if(~mStatusFlag & INITIALIZATION)
		return Error::NOT_INITIALIZED;
	else if(~mStatusFlag & CONNECTION)
		return Error::NOT_CONNECTED;

	unsigned int freeBufferSize;
	char *csrc = (char*)src;

	while(count)
	{
		mPeri->lock();
		freeBufferSize = mPeri->getTxFreeBufferSize(mSocketNumber);
		if(freeBufferSize > count)
			freeBufferSize = count;
		if(freeBufferSize > 0)
			mPeri->sendSocketData(mSocketNumber, csrc, freeBufferSize);
		mPeri->unlock();

		csrc += freeBufferSize;
		count -= freeBufferSize;
		if(freeBufferSize == 0)
			thread::yield();
	}

	return Error::NONE;
}

unsigned char WiznetSocket::getStatus(void)
{
	unsigned char status;
	
	mPeri->lock();
	status = mPeri->getSocketStatus(mSocketNumber);
	mPeri->unlock();

	return status;
}

void WiznetSocket::isr(unsigned char interrupt)
{
	if(interrupt & 0x01)
		mStatusFlag |= CONNECTION;
	
	if(interrupt & 0x02)
		mStatusFlag &= ~CONNECTION;
}

#endif

