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

#include <Interpreter.h>
#include <yss/thread.h>
#include <mod/wiznet/WiznetSocket.h>
#include <util/Timeout.h>

#define STX		0x02
#define ECHO	0x0E
#define ETX		0x03

static void trigger_interpret(void *obj)
{
	((Protocol*)obj)->interpret();
}

Protocol::Protocol(WiznetSocket &socket)
{
	mSocket = &socket;
	mTimeoutFlag = false;
	mIndex = 0;
	mTriggerId = trigger::add(trigger_interpret, this, 512);
}

error Protocol::waitUntilReceivedAndGetReceivedByte(unsigned char &data)
{
	unsigned short size;
	
	data = 0;
	while(1)
	{
		mMutex.lock();
		size = mSocket->getReceivedDataSize();
		if(size)
		{
			data = mSocket->getReceivedByte();
			mMutex.unlock();

			return Error::NONE;
		}
		else if(mTimeout.isTimeout())
		{
			mMutex.unlock();
			return Error::TIMEOUT;
		}
		mMutex.unlock();

		thread::yield();
	}
}

void Protocol::interpret(void)
{
	unsigned char data, checksum, index[2], retry = 3;
	error result;
	mTimeout.reset(5000);

	while(1)
	{
start:
		// STX 체크
		if(waitUntilReceivedAndGetReceivedByte(data) != Error::NONE)
			goto timeout_handler;
		if(data != STX)
			goto start;
		checksum = STX;
		
		// 명령 수신
		if(waitUntilReceivedAndGetReceivedByte(mCommand) != Error::NONE)
			goto timeout_handler;
		checksum ^= mCommand;
		
		// 사이즈 수신
		if(waitUntilReceivedAndGetReceivedByte(mSize[0]) != Error::NONE)
			goto timeout_handler;
		checksum ^= mSize[0];

		if(waitUntilReceivedAndGetReceivedByte(mSize[1]) != Error::NONE)
			goto timeout_handler;
		checksum ^= mSize[1];

		// 인덱스 수신
		if(waitUntilReceivedAndGetReceivedByte(index[0]) != Error::NONE)
			goto timeout_handler;
		checksum ^= index[0];

		if(waitUntilReceivedAndGetReceivedByte(index[1]) != Error::NONE)
			goto timeout_handler;
		checksum ^= index[1];
		
		// 0x0E 체크
		if(waitUntilReceivedAndGetReceivedByte(data) != Error::NONE)
			goto timeout_handler;
		if(data != ECHO)
			goto start;
		checksum ^= data;

		// 데이터 수신
		for(int i=0;i<*(unsigned short*)mSize;i++)
		{
			if(waitUntilReceivedAndGetReceivedByte(mData[i]) != Error::NONE)
				goto timeout_handler;
		}

		// ETX 체크
		if(waitUntilReceivedAndGetReceivedByte(data) != Error::NONE)
			goto timeout_handler;
		if(data != ETX)
			goto start;
		checksum ^= data;
		
		// 체크썸 확인
		if(waitUntilReceivedAndGetReceivedByte(data) != Error::NONE)
			goto timeout_handler;
		if(data != checksum)
			goto start;
		
		if(*(unsigned short*)index != mSendHeader.index)
			mError = Error::WRONG_INDEX;
		else
			mError = Error::NONE;

		mCompleteFlag = true;
		thread::signal(mThreadId);

		return;
		
timeout_handler :
		mError = Error::TIMEOUT;
		mCompleteFlag = true;
		return;
	}
}

void Protocol::sendMessage(unsigned char message, unsigned char *data, unsigned short size)
{
	unsigned char chksum = 0, *buf;
	mSendHeader.message = message;
	mSendHeader.index = mIndex++;
	mSendHeader.size = size;
	
	buf = (unsigned char*)&mSendHeader;
	for(int i=0;i<sizeof(mSendHeader);i++)
		chksum ^= buf[i];

	for(int i=0;i<size;i++)
		chksum ^= data[i];
	
	chksum ^= ETX;
	mSendTail.chksum = chksum;

	mCompleteFlag = false;
	mTimeoutFlag = false;

	mSocket->lock();
	mSocket->sendData(&mSendHeader, sizeof(mSendHeader));
	if(size)
		mSocket->sendData(data, size);
	mSocket->sendData(&mSendTail, sizeof(mSendTail));
	mSocket->unlock();

	trigger::run(mTriggerId);
}

void Protocol::flush(void)
{
	unsigned short size;

	mMutex.lock();
	size = mSocket->getReceivedDataSize();
	for(int i=0;i<size;i++)
		mSocket->getReceivedByte();
	mMutex.unlock();
}

error Protocol::waitUntilComplete(void)
{
	mThreadId = thread::getCurrentThreadNum();

	while(!mCompleteFlag && !mTimeoutFlag)
		thread::yield();
	
	if(mTimeoutFlag)
		return Error::TIMEOUT;
	else
		return Error::NONE;
}
