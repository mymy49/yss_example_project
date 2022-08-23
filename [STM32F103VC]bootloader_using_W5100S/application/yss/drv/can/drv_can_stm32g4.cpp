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

#include <drv/peripheral.h>

#if defined(STM32G4)

#include <__cross_studio_io.h>

#include <drv/Can.h>
#include <string.h>
#include <yss/malloc.h>
#include <yss/thread.h>

inline void setStdFilter(void *des, unsigned char type, unsigned char config, unsigned short id1, unsigned short id2)
{
	*(unsigned int *)des = (type & 0x3) << 30 | (config & 0x7) << 27 | (id1 & 0x3FF) << 16 | (id2 & 0x3FF);
}

Can::Can(YSS_CAN_Peri *peri, void (*clockFunc)(bool en), void (*nvicFunc)(bool en), void (*resetFunc)(void), unsigned int (*getClockFreq)(void)) : Drv(clockFunc, nvicFunc, resetFunc)
{
	unsigned int sa = (unsigned int)peri;

	mPeri = peri;
	mGetClockFreq = getClockFreq;
	mHead = 0;
	mTail = 0;
	mData = 0;
	mMaxDepth = 0;
	mTxFifoIndex = 0;
	mRxFifoIndex0 = 0;

	sa += 0x4000;
	mCanStdFilter = (unsigned int *)(sa + 0x000);
	mCanExtFilter = (unsigned int *)(sa + 0x070);
	mCanRxFifo0 = (unsigned int *)(sa + 0x0B0);
	mCanRxFifo1 = (unsigned int *)(sa + 0x188);
	mCanTxEventFifo = (unsigned int *)(sa + 0x260);
	mCanTxBuffer = (unsigned int *)(sa + 0x278);

	mRxFifo0[0] = (unsigned int *)((unsigned int)mPeri + 0x40B0 + 0x00);
	mRxFifo0[1] = (unsigned int *)((unsigned int)mPeri + 0x40B0 + 0x48);
	mRxFifo0[2] = (unsigned int *)((unsigned int)mPeri + 0x40B0 + 0x90);

	mTxbuf[0] = (unsigned int *)((unsigned int)mPeri + 0x4278 + 0x00);
	mTxbuf[1] = (unsigned int *)((unsigned int)mPeri + 0x4278 + 0x48);
	mTxbuf[2] = (unsigned int *)((unsigned int)mPeri + 0x4278 + 0x90);
}

bool Can::init(unsigned int baudRate, unsigned int bufDepth, float samplePoint)
{
	unsigned int clk = mGetClockFreq(), ts1, ts2, pres;

	clk /= baudRate;

	ts1 = (unsigned int)((float)clk * samplePoint);
	ts2 = clk - ts1;

	for (pres = ts2; pres > 0; pres--)
		if (((ts1 % pres) == 0) && ((ts2 % pres) == 0))
			break;

	ts1 -= pres;
	ts1 /= pres;
	ts2 /= pres;

	if (pres > 1 && pres <= 32)
		pres--;
	else
		goto retry1;

	if (ts1 > 0 && ts1 <= 32)
		ts1--;
	else
		goto retry1;

	if (ts2 > 0 && ts2 <= 16)
		ts2--;
	else
		goto retry1;

	goto next;

retry1:

	ts1 = (unsigned int)((float)clk * samplePoint);
	ts1++;

	ts2 = clk - ts1;
	for (pres = ts2; pres > 0; pres--)
		if (((ts1 % pres) == 0) && ((ts2 % pres) == 0))
			break;

	ts1 -= pres;
	ts1 /= pres;
	ts2 /= pres;

	if (pres > 1 && pres <= 32)
		pres--;
	else
		goto retry2;

	if (ts1 > 0 && ts1 <= 32)
		ts1--;
	else
		goto retry2;

	if (ts2 > 0 && ts2 <= 16)
		ts2--;
	else
		goto retry2;

	goto next;
retry2:
	ts1 = (unsigned int)((float)clk * samplePoint);
	ts1--;

	ts2 = clk - ts1;
	for (pres = ts2; pres > 0; pres--)
		if (((ts1 % pres) == 0) && ((ts2 % pres) == 0))
			break;

	ts1 -= pres;
	ts1 /= pres;
	ts2 /= pres;

	if (pres > 1 && pres <= 32)
		pres--;
	else
		goto error;

	if (ts1 > 0 && ts1 <= 32)
		ts1--;
	else
		goto error;

	if (ts2 > 0 && ts2 <= 16)
		ts2--;
	else
		goto error;

next:
	mPeri->CCCR = FDCAN_CCCR_CCE_Msk | FDCAN_CCCR_INIT_Msk;
	mPeri->CCCR = FDCAN_CCCR_PXHD_Msk | FDCAN_CCCR_DAR_Msk | FDCAN_CCCR_CCE_Msk | FDCAN_CCCR_INIT_Msk;
	mPeri->DBTP = pres << FDCAN_DBTP_DBRP_Pos | ts1 << FDCAN_DBTP_DTSEG1_Pos | ts2 << FDCAN_DBTP_DTSEG2_Pos | ts2 << FDCAN_DBTP_DSJW_Pos;
	mPeri->NBTP = pres << FDCAN_NBTP_NBRP_Pos | ts1 << FDCAN_NBTP_NTSEG1_Pos | ts2 << FDCAN_NBTP_NTSEG2_Pos | ts2 << FDCAN_NBTP_NSJW_Pos;
	if (mMaxDepth != bufDepth)
	{
		if (mData)
			delete mData;
		mData = new unsigned int[bufDepth * 16];
	}

	if (mData == 0)
		return false;

	mMaxDepth = bufDepth;
	mHead = 0;
	mTail = 0;

	memset(mCanStdFilter, 0x00, 1024);
	mPeri->RXGFC = 1 << FDCAN_RXGFC_LSS_Pos | 0 << FDCAN_RXGFC_ANFS_Pos | 0 << FDCAN_RXGFC_ANFE_Pos;

	mPeri->IE = FDCAN_IE_RF0NE_Msk | FDCAN_IE_RF1NE_Msk;
	mPeri->ILE = FDCAN_ILE_EINT0_Msk | FDCAN_ILE_EINT1_Msk;
	mPeri->CCCR &= ~FDCAN_CCCR_INIT_Msk;

	return true;
error:
	return false;
}

bool Can::isReceived(void)
{
	bool rt;
	if (mHead != mTail)
		rt = true;
	else
		rt = false;
	return rt;
}

bool Can::isStandard(void)
{
	unsigned int offset = mTail * 4;

	if (mData[offset] & 0x40000000)
		return false;
	else
		return true;
}

unsigned short Can::getStandardIdentifier(void)
{
	unsigned int offset = mTail * 4;
	return (mData[offset] >> 18) & 0x7ff;
}

unsigned char Can::getPriority(void)
{
	unsigned int offset = mTail * 4;
	unsigned int rt;
	rt = mData[offset] >> 26;
	return (unsigned char)rt & 0x07;
}

unsigned short Can::getPgn(void)
{
	unsigned int offset = mTail * 4;
	unsigned int rt;
	rt = mData[offset] >> 8;
	return (unsigned short)(rt & 0xffff);
}

unsigned char Can::getSrcAddr(void)
{
	unsigned int offset = mTail * 4;
	return (unsigned short)(mData[offset] & 0xff);
}

void Can::releaseFifo(void)
{
	mTail++;
	if (mTail >= mMaxDepth)
		mTail = 0;
}

char *Can::getData(void)
{
	unsigned int offset = mTail * 4 + 2;
	return (char *)&mData[offset];
}

unsigned char Can::getSize(void)
{
	unsigned int offset = mTail * 4 + 1;
	return (unsigned char)mData[offset] & 0x0f;
}

bool Can::sendJ1939(unsigned char priority, unsigned short pgn, unsigned char srcAddr, void *data, unsigned char size)
{
	unsigned int sendBuf[4];
	unsigned int *des, *src = sendBuf;

	if (size > 8)
		size = 8;

	sendBuf[0] = (unsigned int)(priority & 0x07) << 26 | (unsigned int)(pgn & 0xffff) << 8 | srcAddr | 1 << 30;
	sendBuf[1] = size << 16;
	sendBuf[2] = ((unsigned int *)data)[0];
	sendBuf[3] = ((unsigned int *)data)[1];

	des = (unsigned int *)mTxbuf[0];
	*des++ = *src++;
	*des++ = *src++;
	*des++ = *src++;
	*des++ = *src++;

	mPeri->IR = FDCAN_IR_TFE_Msk;
	mPeri->TXBAR = 1;

	while (!(mPeri->IR & FDCAN_IR_TFE_Msk))
		thread::yield();

	return true;
}

bool Can::send(unsigned short id, void *data, unsigned char size)
{
	unsigned int sendBuf[4];
	unsigned int *des, *src = sendBuf;

	if (size > 8)
		size = 8;

	sendBuf[0] = (unsigned int)(id & 0x7ff) << 18;
	sendBuf[1] = size << 16;
	sendBuf[2] = ((unsigned int *)data)[0];
	sendBuf[3] = ((unsigned int *)data)[1];

	des = (unsigned int *)mTxbuf[0];
	*des++ = *src++;
	*des++ = *src++;
	*des++ = *src++;
	*des++ = *src++;

	mPeri->IR = FDCAN_IR_TFE_Msk;
	mPeri->TXBAR = 1;

	while (!(mPeri->IR & FDCAN_IR_TFE_Msk))
		thread::yield();

	return true;
}

void Can::flush(void)
{
	mTail = mHead = 0;
}

void Can::isr(void)
{
	unsigned int *data;
	unsigned int offset = mHead++ * 4;

	if (mPeri->IE & FDCAN_IE_RF0NE_Msk && mPeri->IR & FDCAN_IR_RF0N_Msk)
	{
		data = (unsigned int *)mRxFifo0[mRxFifoIndex0];
		mData[offset++] = *data++;
		mData[offset++] = *data++;
		mData[offset++] = *data++;
		mData[offset++] = *data++;

		if (mHead >= mMaxDepth)
			mHead = 0;

		mPeri->RXF0A = mRxFifoIndex0++;
		if (mRxFifoIndex0 > 2)
			mRxFifoIndex0 = 0;
		FDCAN1->IR = FDCAN_IR_RF0N_Msk;
	}
}

#endif