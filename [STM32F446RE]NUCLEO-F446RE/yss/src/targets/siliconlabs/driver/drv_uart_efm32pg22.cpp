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

#include <drv/mcu.h>

#if defined(EFM32PG22)

#include <drv/peripheral.h>
#include <drv/Uart.h>
#include <yss/reg.h>
#include <yss/thread.h>
#include <util/Timeout.h>
#include <targets/siliconlabs/efm32pg22_usart.h>

Uart::Uart(const Drv::Config drvConfig, const Config config) : Drv(drvConfig)
{
	mDev = config.dev;
	mRcvBuf = 0;
	mTail = 0;
	mHead = 0;
	mOneWireModeFlag = false;
}

error Uart::initialize(int32_t  baud, void *receiveBuffer, int32_t  receiveBufferSize)
{
	uint32_t clk = getClockFrequency(), div;

	mRcvBuf = (int8_t*)receiveBuffer;
	mRcvBufSize = receiveBufferSize;
		
	// 장치 비활성화
	mDev->EN_SET = _USART_EN_EN_MASK;

	// 보레이트 설정
	div = 256 * (clk / (baud * 4) - 1) / 8;
	if(div < 0xFFFFF)
	{
		setFieldData(mDev->CTRL, _USART_CTRL_OVS_MASK, 3, _USART_CTRL_OVS_SHIFT);
		goto next;
	}

	div = 256 * (clk / (baud * 6) - 1) / 8;
	if(div < 0xFFFFF)
	{
		setFieldData(mDev->CTRL, _USART_CTRL_OVS_MASK, 2, _USART_CTRL_OVS_SHIFT);
		goto next;
	}

	div = 256 * (clk / (baud * 8) - 1) / 8;
	if(div < 0xFFFFF)
	{
		setFieldData(mDev->CTRL, _USART_CTRL_OVS_MASK, 1, _USART_CTRL_OVS_SHIFT);
		goto next;
	}

	div = 256 * (clk / (baud * 16) - 1) / 8;
	if(div < 0xFFFFF)
	{
		setFieldData(mDev->CTRL, _USART_CTRL_OVS_MASK, 1, _USART_CTRL_OVS_SHIFT);
		goto next;
	}

	return Error::WRONG_CONFIG;

next:
	setFieldData(mDev->CLKDIV, _USART_CLKDIV_DIV_MASK, div, _USART_CLKDIV_DIV_SHIFT);
	
	// TX En, RX En, Rxnei En, 장치 En
	mDev->CMD = _USART_CMD_RXEN_MASK | _USART_CMD_TXEN_MASK;

	return Error::NONE;
}

error Uart::send(void *src, int32_t  size)
{
	error result;
	int8_t *data = (int8_t*)src;
	
	for(uint32_t i=0;i<size;i++)
	{
		mDev->IF_CLR = 0xFFFF;
		mDev->TXDATA = *data++;

		while(~mDev->IF & _USART_IF_TXC_MASK)
			thread::yield();
	}

	return Error::NONE;
}

void Uart::send(int8_t data)
{
	mDev->IF_CLR = 0xFFFF;
	mDev->TXDATA = data;

	while(~mDev->IF & _USART_IF_TXC_MASK)
		thread::yield();
}

void Uart::isr(void)
{
	
}

#endif

