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

#ifndef YSS_MOD_WIZNET_SOCKET__H_
#define YSS_MOD_WIZNET_SOCKET__H_

#include <yss/Mutex.h>
#include <yss/error.h>

class iEthernet;

class WiznetSocket : public Mutex
{
	iEthernet *mPeri;
	uint8_t mSocketNumber, mInterruptFlag, mStatusFlag;
	int8_t *mRxBuffer;
	uint16_t mRxBufferSize, mHead, mTail;

  protected:

  public:
	enum
	{
		// Protocol
		TCP = 0x01,

		// Command
		OPEN = 0x01,
		CONNECT = 0x04,

		// Status
		TCP_SOCKET_OPEN_OK = 0x01,
		SOCKET_CONNECT_REQUEST_SENT = 0x02,
		SOCKET_ESTABLISHED = 0x03,
		SOCKET_DISCONNECTED = 0x04,

		// Interrupt
		SOCKET_INT_CON = 0x01,

		// Status Flag
		INITIALIZATION = 0x01,
		CONNECTION = 0x02,
		OVER_FLOW = 0x04,
	};

	struct Host
	{
		uint8_t ip[4];
		uint16_t port;
	};

	WiznetSocket(void);
	error init(iEthernet &obj, uint8_t socketNumber, uint16_t rxBufferSize);
	error connectToHost(const Host &host);
	error waitUntilConnect(uint32_t timeout = 20000);
	error sendData(void *src, uint32_t size);
	uint8_t getStatus(void);
	void isr(uint8_t interrupt);
	uint16_t getReceivedDataSize(void);
	uint8_t getReceivedByte(void);
	error getReceivedBytes(void *des, uint16_t size);
};

#endif

