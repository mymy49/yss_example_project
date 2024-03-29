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

#ifndef YSS_DRV_CLOCK_DEFINE_STM32F1__H_
#define YSS_DRV_CLOCK_DEFINE_STM32F1__H_

namespace define
{
namespace clock
{
namespace clockOutput
{
namespace source
{
enum
{
	DISABLE = 0,
	SYSCLK,
	HSI,
	MSI,
	HSE,
	PLL,
	LSI,
	LSE
};
}

namespace div
{
enum
{
	DIV1 = 0,
	DIV2,
	DIV4,
	DIV8,
	DIV16
};
}
}

namespace scale
{
enum
{
	RANGE1 = 1,
	RANGE2,
	RANGE3
};
}

namespace pll
{
namespace src
{
enum
{
	HSI = 0,
	HSE = 1
};
}

namespace mul
{
enum
{
	MUL3 = 0,
	MUL4,
	MUL6,
	MUL8,
	MUL12,
	MUL16,
	MUL24,
	MUL32,
	MUL48,
};
}

namespace div
{
enum
{
	DIV2 = 1,
	DIV3 = 1,
	DIV4 = 1,
};
}
}

namespace usbclk
{
namespace src
{
enum
{
	MAIN_PLL = 0,
	SAI_PLL = 1,
};
}
}

namespace sysclk
{
namespace src
{
enum
{
	HSI = 0,
	HSE = 1,
	PLL = 2
};
}
}

namespace divFactor
{
namespace ahb
{
enum
{
	NO_DIV = 0,
	DIV2 = 0x8,
	DIV4 = 0x9,
	DIV8 = 0xa,
	DIV16 = 0xb,
	DIV64 = 0xc,
	DIV128 = 0xd,
	DIV256 = 0xe,
	DIV512 = 0xf
};
}

namespace apb
{
enum
{
	NO_DIV = 0,
	DIV2 = 0x4,
	DIV4 = 0x5,
	DIV8 = 0x6,
	DIV16 = 0x7,
};
}

}
}
}

#endif