////////////////////////////////////////////////////////////////////////////////////////
//
// 저작권 표기 License_ver_3.1
// 본 소스 코드의 소유권은 홍윤기에게 있습니다.
// 어떠한 형태든 기여는 기증으로 받아들입니다.
// 본 소스 코드는 아래 사항에 동의할 경우에 사용 가능합니다.
// 아래 사항에 대해 동의하지 않거나 이해하지 못했을 경우 사용을 금합니다.
// 본 소스 코드를 사용하였다면 아래 사항을 모두 동의하는 것으로 자동 간주 합니다.
// 본 소스 코드의 상업적 또는 비 상업적 이용이 가능합니다.
// 본 소스 코드의 내용을 임의로 수정하여 재배포하는 행위를 금합니다.
// 본 소스 코드의 사용으로 인해 발생하는 모든 사고에 대해서 어떠한 법적 책임을 지지 않습니다.
//
// Home Page : http://cafe.naver.com/yssoperatingsystem
// Copyright 2022. 홍윤기 all right reserved.
//
////////////////////////////////////////////////////////////////////////////////////////


#ifndef YSS_DRV_CAN__H_
#define YSS_DRV_CAN__H_

#include "mcu.h"
#include <stdint.h>

#if defined(STM32F1) || defined(STM32F4) || defined(STM32F7)
struct CanFrame
{
	uint32_t reserved1 : 1;
	uint32_t remote : 1;
	uint32_t extension : 1;
	uint32_t id : 29;
	uint32_t dataLength : 4;
	uint32_t reserved2 : 28;
	uint8_t data[8];
};

struct J1939Frame
{
	uint32_t reserved1 : 1;
	uint32_t remote : 1;
	uint32_t extension : 1;
	uint32_t sa : 8;
	uint32_t pgn : 16;
	uint32_t dp : 1;
	uint32_t r : 1;
	uint32_t priority : 3;
	uint32_t dataLength : 4;
	uint32_t reserved2 : 28;
	uint8_t data[8];
};

typedef volatile uint32_t	YSS_CAN_Peri;

#elif defined(GD32F1)

struct CanFrame
{
	uint32_t reserved1 : 1;
	uint32_t remote : 1;
	uint32_t extension : 1;
	uint32_t id : 29;
	uint32_t dataLength : 4;
	uint32_t reserved2 : 28;
	uint8_t data[8];
};

struct J1939Frame
{
	uint32_t reserved1 : 1;
	uint32_t remote : 1;
	uint32_t extension : 1;
	uint32_t sa : 8;
	uint32_t pgn : 16;
	uint32_t dp : 1;
	uint32_t r : 1;
	uint32_t priority : 3;
	uint32_t dataLength : 4;
	uint32_t reserved2 : 28;
	uint8_t data[8];
};

typedef volatile uint32_t	YSS_CAN_Peri;

#else

#define YSS_DRV_CAN_UNSUPPORTED

typedef volatile uint32_t	YSS_CAN_Peri;
struct CanFrame{};
struct J1939Frame{};

#endif

#include "Drv.h"

class Can : public Drv
{
	CanFrame *mCanFrame;
	uint32_t mHead, mTail, mMaxDepth;
	uint32_t (*mGetClockFreq)(void);
	YSS_CAN_Peri *mPeri;

	void push(CanFrame *frame);

  public:
	Can(YSS_CAN_Peri *peri, void (*clockFunc)(bool en), void (*nvicFunc)(bool en), void (*resetFunc)(void), uint32_t (*getClockFreq)(void));
	bool init(uint32_t baudRate, uint32_t bufDepth, float samplePoint = 0.875);
	bool disableFilter(uint8_t index);
	bool setStandardMaskFilter(uint8_t index, uint16_t id, uint16_t mask);
	bool setExtendedMaskFilter(uint8_t index, uint32_t id, uint32_t mask);
	bool setStandardMatchFilter(uint8_t index, uint16_t id);
	bool setExtendedMatchFilter(uint8_t index, uint32_t id);
	bool isReceived(void);
	void flush(void);
	void releaseFifo(void);
	bool send(CanFrame packet);
	bool send(J1939Frame packet);
	void isr(void);
	uint8_t getSendErrorCount(void);
	uint8_t getReceiveErrorCount(void);
	CanFrame getPacket(void);
	J1939Frame generateJ1939FrameBuffer(uint8_t priority, uint16_t pgn, uint8_t sa, uint8_t count);
};

#endif

