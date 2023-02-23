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

#include <yss/instance.h>

#if defined(GD32F4) || defined(STM32F4) || defined(STM32F7) || defined(STM32L1) || defined (STM32F0) 

#if defined(GPIOA)
Gpio gpioA((YSS_GPIO_Peri*)GPIOA, 0, 0, define::gpio::exti::PORTA);
#endif

#if defined(GPIOB)
Gpio gpioB((YSS_GPIO_Peri*)GPIOB, 0, 0, define::gpio::exti::PORTB);
#endif

#if defined(GPIOC)
Gpio gpioC((YSS_GPIO_Peri*)GPIOC, 0, 0, define::gpio::exti::PORTC);
#endif

#if defined(GPIOD)
Gpio gpioD((YSS_GPIO_Peri*)GPIOD, 0, 0, define::gpio::exti::PORTD);
#endif

#if defined(GPIOE)
Gpio gpioE((YSS_GPIO_Peri*)GPIOE, 0, 0, define::gpio::exti::PORTE);
#endif

#if defined(GPIOF)
Gpio gpioF((YSS_GPIO_Peri*)GPIOF, 0, 0, define::gpio::exti::PORTF);
#endif

#if defined(GPIOG)
Gpio gpioG((YSS_GPIO_Peri*)GPIOG, 0, 0, define::gpio::exti::PORTG);
#endif

#if defined(GPIOH)
Gpio gpioH((YSS_GPIO_Peri*)GPIOH, 0, 0, define::gpio::exti::PORTH);
#endif

#if defined(GPIOI)
Gpio gpioI((YSS_GPIO_Peri*)GPIOI, 0, 0, define::gpio::exti::PORTI);
#endif

#elif defined(STM32F4_N)

using namespace define::gpio;

#if defined(GPIOA)
const static Drv::Setup gDrvSetupGpioA =
{
	0,	//void (*clockFunc)(bool en);
	0,	//void (*nvicFunc)(bool en);
	0,	//void (*resetFunc)(void);
	0	//uint32_t (*getClockFunc)(void);
};

const static Gpio::Setup gConfigGpioA =
{
	GPIOA,		//YSS_GPIO_Peri *dev;
	exti::PORTA	//uint8_t port;
};

Gpio gpioA(gDrvSetupGpioA, gConfigGpioA);
#endif

#if defined(GPIOB)
const static Drv::Setup gDrvSetupGpioB =
{
	0,	//void (*clockFunc)(bool en);
	0,	//void (*nvicFunc)(bool en);
	0,	//void (*resetFunc)(void);
	0	//uint32_t (*getClockFunc)(void);
};

const static Gpio::Setup gConfigGpioB =
{
	GPIOB,		//YSS_GPIO_Peri *dev;
	exti::PORTB	//uint8_t port;
};

Gpio gpioB(gDrvSetupGpioB, gConfigGpioB);
#endif

#if defined(GPIOC)
const static Drv::Setup gDrvSetupGpioC =
{
	0,	//void (*clockFunc)(bool en);
	0,	//void (*nvicFunc)(bool en);
	0,	//void (*resetFunc)(void);
	0	//uint32_t (*getClockFunc)(void);
};

const static Gpio::Setup gConfigGpioC =
{
	GPIOC,		//YSS_GPIO_Peri *dev;
	exti::PORTC	//uint8_t port;
};

Gpio gpioC(gDrvSetupGpioC, gConfigGpioC);
#endif

#if defined(GPIOD)
const static Drv::Setup gDrvSetupGpioD =
{
	0,	//void (*clockFunc)(bool en);
	0,	//void (*nvicFunc)(bool en);
	0,	//void (*resetFunc)(void);
	0	//uint32_t (*getClockFunc)(void);
};

const static Gpio::Setup gConfigGpioD =
{
	GPIOD,		//YSS_GPIO_Peri *dev;
	exti::PORTD	//uint8_t port;
};

Gpio gpioD(gDrvSetupGpioD, gConfigGpioD);
#endif

#if defined(GPIOE)
const static Drv::Setup gDrvSetupGpioE =
{
	0,	//void (*clockFunc)(bool en);
	0,	//void (*nvicFunc)(bool en);
	0,	//void (*resetFunc)(void);
	0	//uint32_t (*getClockFunc)(void);
};

const static Gpio::Setup gConfigGpioE =
{
	GPIOE,		//YSS_GPIO_Peri *dev;
	exti::PORTE	//uint8_t port;
};

Gpio gpioE(gDrvSetupGpioE, gConfigGpioE);
#endif

#if defined(GPIOF)
const static Drv::Setup gDrvSetupGpioF =
{
	0,	//void (*clockFunc)(bool en);
	0,	//void (*nvicFunc)(bool en);
	0,	//void (*resetFunc)(void);
	0	//uint32_t (*getClockFunc)(void);
};

const static Gpio::Setup gConfigGpioF =
{
	GPIOF,		//YSS_GPIO_Peri *dev;
	exti::PORTF	//uint8_t port;
};

Gpio gpioF(gDrvSetupGpioF, gConfigGpioF);
#endif

#if defined(GPIOG)
const static Drv::Setup gDrvSetupGpioG =
{
	0,	//void (*clockFunc)(bool en);
	0,	//void (*nvicFunc)(bool en);
	0,	//void (*resetFunc)(void);
	0	//uint32_t (*getClockFunc)(void);
};

const static Gpio::Setup gConfigGpioG =
{
	GPIOG,		//YSS_GPIO_Peri *dev;
	exti::PORTG	//uint8_t port;
};

Gpio gpioG(gDrvSetupGpioG, gConfigGpioG);
#endif

#if defined(GPIOH)
const static Drv::Setup gDrvSetupGpioH =
{
	0,	//void (*clockFunc)(bool en);
	0,	//void (*nvicFunc)(bool en);
	0,	//void (*resetFunc)(void);
	0	//uint32_t (*getClockFunc)(void);
};

const static Gpio::Setup gConfigGpioH =
{
	GPIOH,		//YSS_GPIO_Peri *dev;
	exti::PORTH	//uint8_t port;
};

Gpio gpioH(gDrvSetupGpioH, gConfigGpioH);
#endif

#if defined(GPIOI)
const static Drv::Setup gDrvSetupGpioI =
{
	0,	//void (*clockFunc)(bool en);
	0,	//void (*nvicFunc)(bool en);
	0,	//void (*resetFunc)(void);
	0	//uint32_t (*getClockFunc)(void);
};

const static Gpio::Setup gConfigGpioI =
{
	GPIOI,		//YSS_GPIO_Peri *dev;
	exti::PORTI	//uint8_t port;
};

Gpio gpioI(gDrvSetupGpioI, gConfigGpioI);
#endif

#endif

