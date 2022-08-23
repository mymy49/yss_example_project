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

#ifndef YSS_DRV_CLOCK_ST_TYPE_C_EC__H_
#define YSS_DRV_CLOCK_ST_TYPE_C_EC__H_

#if defined(STM32F405xx) || defined(STM32F415xx) || \
	defined(STM32F407xx) || defined(STM32F417xx) || \
	defined(STM32F427xx) || defined(STM32F437xx) || \
	defined(STM32F429xx) || defined(STM32F439xx)

#if defined(STM32F427xx) || defined(STM32F437xx) || \
	defined(STM32F429xx) || defined(STM32F439xx)

#define STM32F42_F43

#endif

namespace ec
{
namespace clock
{
namespace sysclk
{
enum
{
#if defined(STM32F42_F43)
	MAX_FREQ = 180000000,
	OVER_DRIVE_FREQ = 168000000,
#else
	MAX_FREQ = 168000000
#endif
};
}

namespace apb1
{
enum
{
	MAX_FREQ = 45000000,
};
}

namespace apb2
{
enum
{
	MAX_FREQ = 90000000,
};
}

namespace hsi
{
enum
{
	FREQ = 16000000,
};
}

namespace hse
{
enum
{
	HSE_MIN_FREQ = 1000000,
	HSE_MAX_FREQ = 50000000,
};
}

namespace pll
{
enum
{
	VCO_MIN_FREQ = 100000000,
	VCO_MAX_FREQ = 432000000,
	USB48_MAX_FREQ = 75000000,
	M_MIN = 2,
	M_MAX = 63,
	N_MIN = 2,
	N_MAX = 432,
	P_MAX = 3,
	Q_MIN = 2,
	Q_MAX = 15
};
}

namespace saipll
{
enum
{
	VCO_MIN_FREQ = 100000000,
	VCO_MAX_FREQ = 432000000,
	SAI_MAX_FREQ = 45000000,
	LCD_MAX_FREQ = 42000000,
	N_MIN = 2,
	N_MAX = 432,
	Q_MIN = 2,
	Q_MAX = 15,
	R_MIN = 2,
	R_MAX = 7
};
}
}
}
#endif

#endif