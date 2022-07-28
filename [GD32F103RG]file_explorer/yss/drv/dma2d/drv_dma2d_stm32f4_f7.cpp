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

#if (defined(STM32F4) || defined(STM32F7)) && defined(DMA2D)

#include <drv/Dma2d.h>
#include <drv/dma2d/register_dma2d_stm32f4_f7.h>

unsigned short gDma2dThreadNum;

Dma2d::Dma2d(DMA2D_TypeDef *peri, void (*clockFunc)(bool en), void (*nvicFunc)(bool en)) : Drv(clockFunc, nvicFunc)
{
	mFontInfo.size = 0;
	mFontInfo.yPos = 0;
	mFontInfo.pointer = 0;
	mFontInfo.base = 0;
}

void Dma2d::init(void)
{
}

void Dma2d::draw(Object &des, Object &src)
{
	draw(des, src, src.getPos());
}

void Dma2d::drawArea(Object &des, Pos areaPos, Size areaSize, Object &src)
{
	drawArea(des, areaPos, areaSize, src, src.getPos());
}

#endif