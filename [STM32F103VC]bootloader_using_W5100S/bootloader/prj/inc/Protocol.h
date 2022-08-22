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

#ifndef PROTOCOL__H_
#define PROTOCOL__H_

#include <yss/error.h>
#include <yss/Mutex.h>
#include <util/Timeout.h>

class WiznetSocket;

class Protocol
{
	struct ProtocolHeader
	{
		unsigned char stx = 0x02;
		unsigned char message;
		unsigned short size;
		unsigned short index;
		unsigned char echo = 0x0E;
	}__attribute__((packed));

	struct ProtocolTail
	{
		unsigned char etx = 0x03;
		unsigned char chksum;
	}__attribute__((packed));

	unsigned char mMessage, mSize[2], mData[512];
	unsigned short mIndex;
	bool mCompleteFlag;
	error mError;
	WiznetSocket *mSocket;
	Mutex mMutex;
	Timeout mTimeout;
	ProtocolHeader mSendHeader;
	ProtocolTail mSendTail;
	int mTriggerId, mThreadId;
	
	error waitUntilReceivedAndGetReceivedByte(unsigned char &data);

public:
	enum
	{
		MSG_HOW_ARE_YOU = 0,
		MSG_I_AM_FINE = 1,
		MSG_HAVE_YOU_NEW_FIRMWARE = 2,
		MSG_NO_I_HAVE_NOT = 3,
		MSG_YES_I_HAVE = 4,
		MSG_I_DON_T_KNOW = 5,
		MSG_GIVE_ME_TOTAL_PACKET = 6,
		MSG_IT_IS_TOTAL_PACKET = 7,
		MSG_GIVE_ME_FIRMWARE_PACKET = 8,
		MSG_IT_IS_FIRMWARE_PACKET = 9,
	};

	Protocol(WiznetSocket &socket);

	void interpret(void);
	void flush(void);
	error sendMessage(unsigned char message, unsigned char *data, unsigned short size);
	unsigned char getReceivedMessage(void);
	unsigned short getReceivedDataSize(void);
	unsigned char *getReceivedData(void);
};


#endif