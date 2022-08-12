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

#include <drv/mcu.h>

#if defined(STM32F1)

#include <drv/peripheral.h>
#include <drv/Flash.h>
#include <drv/flash/register_flash_stm32f1.h>
#include <yss/thread.h>

Flash::Flash(void) : Drv(0, 0)
{
}

void Flash::setLatency(unsigned int freq)
{
	if (freq < 24000000)
		setFlashLatency(0);
	else if (freq < 48000000)
		setFlashLatency(1);
	else
		setFlashLatency(2);
}

void Flash::setPrefetchEn(bool en)
{
	setFlashPrefetchEn(en);
}

void Flash::setHalfCycleAccessEn(bool en)
{
	setFlashHlfcyaEn(en);
}

unsigned int Flash::getAddress(unsigned short sector)
{
	unsigned int max, size;
#if defined(STM32F10X_MD) || defined(STM32F10X_LD)
	max = *(unsigned short *)FLASHSIZE_BASE;
	size = 1024;
#elif defined(STM32F10X_HD)
	max = *(unsigned short *)FLASHSIZE_BASE;
	size = 2048;
#else
	max = *(unsigned short *)FLASHSIZE_BASE;
	size = 2048;
#endif

	if (sector > max)
		sector = max;

	return 0x08000000 + (unsigned int)sector * size;
}

void Flash::erase(unsigned short sector)
{
	unsigned int addr;
	addr = sector;
#if defined(STM32F10X_XL) || defined(STM32F10X_HD)
	addr *= 2048;
#else
	addr *= 1024;
#endif
	addr += 0x08000000;

#if defined(STM32F10X_XL)
	while (getFlashBusy() || getFlashBusy2())
		thread::yield();
#else
	while (getFlashBusy())
		thread::yield();
#endif

	if (sector < 256)
	{
		if (getFlashLock())
		{
			setFlashKey(0x45670123);
			setFlashKey(0xcdef89ab);
		}

		while (getFlashLock())
			thread::yield();

		setFlashSectorErase(true);
		setFlashSectorNumber(addr);
		setFlashEraseStart();

		__NOP();
		__NOP();
		while (getFlashBusy())
			;

		__NOP();
		__NOP();
		setFlashSectorErase(false);
		setFlashLock();
	}
#if defined(STM32F10X_XL)
	else
	{
		if (getFlashLock2())
		{
			setFlashKey2(0x45670123);
			setFlashKey2(0xcdef89ab);
		}

		while (getFlashLock2())
			thread::yield();

		setFlashSectorErase2(true);
		setFlashSectorNumber2(addr);
		setFlashEraseStart2();

		__NOP();
		__NOP();
		while (getFlashBusy2())
			;

		__NOP();
		__NOP();
		setFlashSectorErase2(false);
		setFlashLock2();
	}
#endif
}

void *Flash::program(unsigned int sector, void *src, unsigned int size)
{
	unsigned short *addr;
	unsigned int temp;

	temp = sector;
#if defined(STM32F10X_XL) || defined(STM32F10X_HD)
	temp *= 2048;
#else
	temp *= 1024;
#endif
	temp += 0x08000000;

	addr = (unsigned short *)temp;

	size += 1;
	size >>= 1;

#if defined(STM32F10X_XL)
	while (getFlashBusy() || getFlashBusy2())
		thread::yield();
#else
	while (getFlashBusy())
		thread::yield();
#endif

	if (sector < 256)
	{
		if (getFlashLock())
		{
			setFlashKey(0x45670123);
			setFlashKey(0xcdef89ab);
		}

		while (getFlashLock())
			thread::yield();

		setFlashProgramming(true);

		__NOP();
		__NOP();
		for (unsigned long i = 0; i < size; i++)
		{
			addr[i] = ((unsigned short *)src)[i];
			__NOP();
			__NOP();
			while (getFlashBusy())
				;
		}

		__NOP();
		__NOP();
		setFlashProgramming(false);
		setFlashLock();
	}
#if defined(STM32F10X_XL)
	else
	{
		if (getFlashLock2())
		{
			setFlashKey2(0x45670123);
			setFlashKey2(0xcdef89ab);
		}

		while (getFlashLock2())
			thread::yield();

		setFlashProgramming2(true);

		__NOP();
		__NOP();
		for (unsigned long i = 0; i < size; i++)
		{
			addr[i] = ((unsigned short *)src)[i];
			__NOP();
			__NOP();
			while (getFlashBusy2())
				;
		}

		__NOP();
		__NOP();
		setFlashProgramming2(false);
		setFlashLock2();
	}
#endif
	return &addr[size];
}

void *Flash::program(void *des, void *src, unsigned int size)
{
	unsigned short *addr = (unsigned short *)des;

	size += 1;
	size >>= 1;

#if defined(STM32F10X_XL)
	while (getFlashBusy() || getFlashBusy2())
		thread::yield();
#else
	while (getFlashBusy())
		thread::yield();
#endif

	if ((int)des < 0x08080000)
	{
		if (getFlashLock())
		{
			setFlashKey(0x45670123);
			setFlashKey(0xcdef89ab);
		}

		while (getFlashLock())
			thread::yield();

		setFlashProgramming(true);

		__NOP();
		__NOP();
		for (unsigned long i = 0; i < size; i++)
		{
			addr[i] = ((unsigned short *)src)[i];
			__NOP();
			__NOP();
			while (getFlashBusy())
				;
		}

		__NOP();
		__NOP();
		setFlashProgramming(false);
		setFlashLock();
	}
#if defined(STM32F10X_XL)
	else
	{
		if (getFlashLock2())
		{
			setFlashKey2(0x45670123);
			setFlashKey2(0xcdef89ab);
		}

		while (getFlashLock2())
			thread::yield();

		setFlashProgramming2(true);

		__NOP();
		__NOP();
		for (unsigned long i = 0; i < size; i++)
		{
			addr[i] = ((unsigned short *)src)[i];
			__NOP();
			__NOP();
			while (getFlashBusy2())
				;
		}

		__NOP();
		__NOP();
		setFlashProgramming2(false);
		setFlashLock2();
	}
#endif
	return &addr[size];
}
#endif

