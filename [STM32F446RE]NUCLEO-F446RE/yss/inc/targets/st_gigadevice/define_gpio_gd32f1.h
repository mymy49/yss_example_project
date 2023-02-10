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

#ifndef YSS_DRV_GPIO_ST_TYPE_B_DEFINE__H_
#define YSS_DRV_GPIO_ST_TYPE_B_DEFINE__H_

namespace define
{
namespace gpio
{
namespace exti
{
enum
{
	PORTA = 0,
	PORTB = 1,
	PORTC = 2,
	PORTD = 3,
	PORTE = 4,
	PORTF = 5,
	PORTG = 6,
};
}

namespace altfunc
{
enum
{
	PA0_WKUP,
	PA0_USART2_CTS,
	PA0_TIM2_CH1_ETR,
	PA0_TIM5_CH1,
	PA0_TIM8_ETR,
	PA1_USART2_RTS,
	PA1_TIM5_CH2,
	PA1_TIM2_CH2,
	PA2_USART2_TX,
	PA2_TIM5_CH3,
	PA2_TIM9_CH1,
	PA2_TIM2_CH3,
	PA3_USART2_RX,
	PA3_TIM5_CH4,
	PA3_TIM2_CH4,
	PA3_TIM9_CH2,
	PA4_SPI1_NSS,
	PA4_USART2_CK,
	PA4_DAC_OUT1,
	PA5_SPI1_SCK,
	PA5_DAC_OUT2,
	PA6_SPI1_MISO,
	PA6_TIM8_BKIN,
	PA6_TIM3_CH1,
	PA6_TIM13_CH1,
	PA7_SPI1_MOSI,
	PA7_TIM8_CH1N,
	PA7_TIM3_CH2,
	PA7_TIM14_CH1,
	PA8_USART1_CK,
	PA8_TIM1_CH1,
	PA8_MCO,
	PA9_USART1_TX,
	PA9_TIM1_CH2,
	PA10_USART1_RX,
	PA10_TIM1_CH3,
	PA11_USART1_CTS,
	PA11_USB_DM,
	PA11_CAN_RX,
	PA11_TIM1_CH4,
	PA12_USART1_RTS,
	PA12_USB_DP,
	PA12_CAN_TX,
	PA12_TIM1_ETR,
	PA13,
	PA13_JTMS_SWDIO,
	PA14,
	PA14_JTCK_SWCLK,
	PA15,
	PA15_JTDI,
	PA15_SPI3_NSS,
	PA15_I2S3_WS,
	PA15_TIM2_CH1_ETR,
	PA15_SPI1_NSS,

	PB0_TIM3_CH3,
	PB0_TIM8_CH2N,
	PB0_TIM1_CH2N,
	PB1_TIM3_CH4,
	PB1_TIM8_CH3N,
	PB1_TIM1_CH3N,
	PB3,
	PB3_SPI3_SCK,
	PB3_I2S3_CK,
	PB3_TRACESWO,
	PB3_TIM2_CH2,
	PB3_SPI1_SCK,
	PB4,
	PB4_SPI3_MISO,
	PB4_TIM3_CH1,
	PB4_SPI1_MISO,
	PB5_I2C1_SMBA,
	PB5_SPI3_MOSI,
	PB5_I2S3_SD,
	PB5_TIM3_CH2,
	PB5_SPI1_MOSI,
	PB6_I2C1_SCL,
	PB6_TIM4_CH1,
	PB6_USART1_TX,
	PB7_I2C1_SDA,
	PB7_FSMC_NADV,
	PB7_TIM4_CH2,
	PB7_USART1_RX,
	PB8_TIM4_CH3,
	PB8_SDIO_D4,
	PB8_TIM10_CH1,
	PB8_I2C1_SCL,
	PB8_CAN_RX,
	PB9_TIM4_CH4,
	PB9_SDIO_D5,
	PB9_TIM11_CH1,
	PB9_I2C1_SDA,
	PB9_CAN_TX,
	PB10_I2C2_SCL,
	PB10_USART3_TX,
	PB10_TIM2_CH3,
	PB11_I2C2_SDA,
	PB11_USART3_RX,
	PB11_TIM2_CH4,
	PB12_SPI2_NSS,
	PB12_I2S2_WS,
	PB12_I2C2_SMBA,
	PB12_USART3_CK,
	PB12_TIM1_BKIN,
	PB13_SPI2_SCK,
	PB13_I2S2_CK,
	PB13_USART3_CTS,
	PB13_TIM1_CH1N,
	PB14_SPI2_MISO,
	PB14_TIM1_CH2N,
	PB14_USART3_RTS,
	PB14_TIM12_CH1,
	PB15_SPI2_MOSI,
	PB15_I2S2_SD,
	PB15_TIM1_CH3N,
	PB15_TIM12_CH2,

	PC6_I2S2_MCK,
	PC6_TIM8_CH1,
	PC6_SDIO_D6,
	PC6_TIM3_CH1,
	PC7_I2S3_MCK,
	PC7_TIM8_CH2,
	PC7_SDIO_D7,
	PC7_TIM3_CH2,
	PC8_TIM8_CH3,
	PC8_SDIO_D0,
	PC8_TIM3_CH3,
	PC9_TIM8_CH4,
	PC9_SDIO_D1,
	PC9_TIM3_CH4,
	PC10_UART4_TX,
	PC10_SDIO_D2,
	PC10_USART3_TX,
	PC11_UART4_RX,
	PC11_SDIO_D3,
	PC11_USART3_RX,
	PC12_UART5_TX,
	PC12_SDIO_CK,
	PC12_USART3_CK,
	PC13_TAMPER_RTC,
	PC14_OSC32_IN,
	PC15_OSC32_OUT,

	PD0,
	PD0_FSMC_D2,
	PD0_CAN_RX,
	PD0_OSC_IN,
	PD1,
	PD1_FSMC_D3,
	PD1_CAN_TX,
	PD1_OSC_OUT,
	PD2_TIM3_ETR,
	PD2_USART5_RX,
	PD2_SDIO_CMD,
	PD3_FSMC_CLK,
	PD3_USART2_CTS,
	PD4_FSMC_NOE,
	PD4_USART2_RTS,
	PD5_FSMC_NWE,
	PD5_USART2_TX,
	PD6_FSMC_NWAIT,
	PD6_USART2_RX,
	PD7_FSMC_NE1,
	PD7_FSMC_NCE2,
	PD7_USART2_CK,
	PD8_FSMC_D13,
	PD8_USART3_TX,
	PD9_FSMC_D14,
	PD9_USART3_RX,
	PD10_FSMC_D15,
	PD10_USART3_CK,
	PD11_FSMC_A16,
	PD11_USART3_CTS,
	PD12_FSMC_A17,
	PD12_TIM4_CH1,
	PD12_USART3_RTS,
	PD13_FSMC_A18,
	PD13_TIM4_CH2,
	PD14_FSMC_D0,
	PD14_TIM4_CH3,
	PD15_FSMC_D1,
	PD15_TIM4_CH4,

	PE0_TIM4_ETR,
	PE2_TRACECK,
	PE2_FSMC_A23,
	PE3_TRACED0,
	PE3_FSMC_A19,
	PE4_TRACED1,
	PE4_FSMC_A20,
	PE5_TRACED2,
	PE5_FSMC_A21,
	PE5_TIM9_CH1,
	PE6_TRACED3,
	PE6_FSMC_A22,
	PE6_TIM9_CH2,
	PE7_FSMC_D4,
	PE7_TIM1_ETR,
	PE8_FSMC_D5,
	PE8_TIM1_CH1N,
	PE9_FSMC_D6,
	PE9_TIM1_CH1,
	PE10_FSMC_D7,
	PE10_TIM1_CH2N,
	PE11_FSMC_D8,
	PE11_TIM1_CH2,
	PE12_FSMC_D9,
	PE12_TIM1_CH3N,
	PE13_FSMC_D10,
	PE13_TIM1_CH3,
	PE14_FSMC_D11,
	PE14_TIM1_CH4,
	PE15_FSMC_D12,
	PE15_TIM1_BKIN,

	PF0_FSMC_A0,
	PF1_FSMC_A1,
	PF2_FSMC_A2,
	PF3_FSMC_A3,
	PF4_FSMC_A4,
	PF5_FSMC_A5,
	PF6_FSMC_NIORD,
	PF6_TIM10_CH1,
	PF7_FSMC_NREG,
	PF7_TIM11_CH1,
	PF8_FSMC_NIOWR,
	PF8_TIM13_CH1,
	PF9_FSMC_CD,
	PF9_TIM14_CH1,
	PF10_FSMC_INTR,
	PF11_FSMC_NIOS16,
	PF12_FSMC_A6,
	PF13_FSMC_A7,
	PF14_FSMC_A8,
	PF15_FSMC_A9,

	PG0_FSMC_A10,
	PG1_FSMC_A11,
	PG2_FSMC_A12,
	PG3_FSMC_A13,
	PG4_FSMC_A14,
	PG5_FSMC_A15,
	PG6_FSMC_INT2,
	PG7_FSMC_INT3,
	PG9_FSMC_NE2,
	PG9_FSMC_NCE3,
	PG10_NCE4_1,
	PG10_NE3,
	PG11_NCE4_2,
	PG12_NE4,
	PG13_A24,
	PG14_A25,
};
}

namespace ospeed
{
enum
{
	LOW = 2,
	MID = 1,
	FAST = 3,
};
}

namespace otype
{
enum
{
	PUSH_PULL = 0,
	OPEN_DRAIN = 1,
};
}

namespace mode
{
enum
{
	INPUT = 0,
	OUTPUT = 1,
	ALT_FUNC = 2,
	ANALOG = 3
};
}

namespace pupd
{
enum
{
	NONE = 0,
	PULL_UP,
	PULL_DOWN
};
}
}
}

#define setGpioA(x)		GPIOA->DOR = x
#define setGpioA0()		GPIOA->BOR = (1 << 0)
#define setGpioA1()		GPIOA->BOR = (1 << 1)
#define setGpioA2()		GPIOA->BOR = (1 << 2)
#define setGpioA3()		GPIOA->BOR = (1 << 3)
#define setGpioA4()		GPIOA->BOR = (1 << 4)
#define setGpioA5()		GPIOA->BOR = (1 << 5)
#define setGpioA6()		GPIOA->BOR = (1 << 6)
#define setGpioA7()		GPIOA->BOR = (1 << 7)
#define setGpioA8()		GPIOA->BOR = (1 << 8)
#define setGpioA9()		GPIOA->BOR = (1 << 9)
#define setGpioA10()	GPIOA->BOR = (1 << 10)
#define setGpioA11()	GPIOA->BOR = (1 << 11)
#define setGpioA12()	GPIOA->BOR = (1 << 12)
#define setGpioA13()	GPIOA->BOR = (1 << 13)
#define setGpioA14()	GPIOA->BOR = (1 << 14)
#define setGpioA15()	GPIOA->BOR = (1 << 15)

#define resetGpioA0()	GPIOA->BCR = (1 << 0)
#define resetGpioA1()	GPIOA->BCR = (1 << 1)
#define resetGpioA2()	GPIOA->BCR = (1 << 2)
#define resetGpioA3()	GPIOA->BCR = (1 << 3)
#define resetGpioA4()	GPIOA->BCR = (1 << 4)
#define resetGpioA5()	GPIOA->BCR = (1 << 5)
#define resetGpioA6()	GPIOA->BCR = (1 << 6)
#define resetGpioA7()	GPIOA->BCR = (1 << 7)
#define resetGpioA8()	GPIOA->BCR = (1 << 8)
#define resetGpioA9()	GPIOA->BCR = (1 << 9)
#define resetGpioA10()	GPIOA->BCR = (1 << 10)
#define resetGpioA11()	GPIOA->BCR = (1 << 11)
#define resetGpioA12()	GPIOA->BCR = (1 << 12)
#define resetGpioA13()	GPIOA->BCR = (1 << 13)
#define resetGpioA14()	GPIOA->BCR = (1 << 14)
#define resetGpioA15()	GPIOA->BCR = (1 << 15)

#define setGpioB(x)		GPIOB->DOR = x
#define setGpioB0()		GPIOB->BOR = (1 << 0)
#define setGpioB1()		GPIOB->BOR = (1 << 1)
#define setGpioB2()		GPIOB->BOR = (1 << 2)
#define setGpioB3()		GPIOB->BOR = (1 << 3)
#define setGpioB4()		GPIOB->BOR = (1 << 4)
#define setGpioB5()		GPIOB->BOR = (1 << 5)
#define setGpioB6()		GPIOB->BOR = (1 << 6)
#define setGpioB7()		GPIOB->BOR = (1 << 7)
#define setGpioB8()		GPIOB->BOR = (1 << 8)
#define setGpioB9()		GPIOB->BOR = (1 << 9)
#define setGpioB10()	GPIOB->BOR = (1 << 10)
#define setGpioB11()	GPIOB->BOR = (1 << 11)
#define setGpioB12()	GPIOB->BOR = (1 << 12)
#define setGpioB13()	GPIOB->BOR = (1 << 13)
#define setGpioB14()	GPIOB->BOR = (1 << 14)
#define setGpioB15()	GPIOB->BOR = (1 << 15)

#define resetGpioB0()	GPIOB->BCR = (1 << 0)
#define resetGpioB1()	GPIOB->BCR = (1 << 1)
#define resetGpioB2()	GPIOB->BCR = (1 << 2)
#define resetGpioB3()	GPIOB->BCR = (1 << 3)
#define resetGpioB4()	GPIOB->BCR = (1 << 4)
#define resetGpioB5()	GPIOB->BCR = (1 << 5)
#define resetGpioB6()	GPIOB->BCR = (1 << 6)
#define resetGpioB7()	GPIOB->BCR = (1 << 7)
#define resetGpioB8()	GPIOB->BCR = (1 << 8)
#define resetGpioB9()	GPIOB->BCR = (1 << 9)
#define resetGpioB10()	GPIOB->BCR = (1 << 10)
#define resetGpioB11()	GPIOB->BCR = (1 << 11)
#define resetGpioB12()	GPIOB->BCR = (1 << 12)
#define resetGpioB13()	GPIOB->BCR = (1 << 13)
#define resetGpioB14()	GPIOB->BCR = (1 << 14)
#define resetGpioB15()	GPIOB->BCR = (1 << 15)

#define setGpioC(x)		GPIOC->DOR = x
#define setGpioC0()		GPIOC->BOR = (1 << 0)
#define setGpioC1()		GPIOC->BOR = (1 << 1)
#define setGpioC2()		GPIOC->BOR = (1 << 2)
#define setGpioC3()		GPIOC->BOR = (1 << 3)
#define setGpioC4()		GPIOC->BOR = (1 << 4)
#define setGpioC5()		GPIOC->BOR = (1 << 5)
#define setGpioC6()		GPIOC->BOR = (1 << 6)
#define setGpioC7()		GPIOC->BOR = (1 << 7)
#define setGpioC8()		GPIOC->BOR = (1 << 8)
#define setGpioC9()		GPIOC->BOR = (1 << 9)
#define setGpioC10()	GPIOC->BOR = (1 << 10)
#define setGpioC11()	GPIOC->BOR = (1 << 11)
#define setGpioC12()	GPIOC->BOR = (1 << 12)
#define setGpioC13()	GPIOC->BOR = (1 << 13)
#define setGpioC14()	GPIOC->BOR = (1 << 14)
#define setGpioC15()	GPIOC->BOR = (1 << 15)

#define resetGpioC0()	GPIOC->BCR = (1 << 0)
#define resetGpioC1()	GPIOC->BCR = (1 << 1)
#define resetGpioC2()	GPIOC->BCR = (1 << 2)
#define resetGpioC3()	GPIOC->BCR = (1 << 3)
#define resetGpioC4()	GPIOC->BCR = (1 << 4)
#define resetGpioC5()	GPIOC->BCR = (1 << 5)
#define resetGpioC6()	GPIOC->BCR = (1 << 6)
#define resetGpioC7()	GPIOC->BCR = (1 << 7)
#define resetGpioC8()	GPIOC->BCR = (1 << 8)
#define resetGpioC9()	GPIOC->BCR = (1 << 9)
#define resetGpioC10()	GPIOC->BCR = (1 << 10)
#define resetGpioC11()	GPIOC->BCR = (1 << 11)
#define resetGpioC12()	GPIOC->BCR = (1 << 12)
#define resetGpioC13()	GPIOC->BCR = (1 << 13)
#define resetGpioC14()	GPIOC->BCR = (1 << 14)
#define resetGpioC15()	GPIOC->BCR = (1 << 15)

#define setGpioD(x)		GPIOD->DOR = x
#define setGpioD0()		GPIOD->BOR = (1 << 0)
#define setGpioD1()		GPIOD->BOR = (1 << 1)
#define setGpioD2()		GPIOD->BOR = (1 << 2)
#define setGpioD3()		GPIOD->BOR = (1 << 3)
#define setGpioD4()		GPIOD->BOR = (1 << 4)
#define setGpioD5()		GPIOD->BOR = (1 << 5)
#define setGpioD6()		GPIOD->BOR = (1 << 6)
#define setGpioD7()		GPIOD->BOR = (1 << 7)
#define setGpioD8()		GPIOD->BOR = (1 << 8)
#define setGpioD9()		GPIOD->BOR = (1 << 9)
#define setGpioD10()	GPIOD->BOR = (1 << 10)
#define setGpioD11()	GPIOD->BOR = (1 << 11)
#define setGpioD12()	GPIOD->BOR = (1 << 12)
#define setGpioD13()	GPIOD->BOR = (1 << 13)
#define setGpioD14()	GPIOD->BOR = (1 << 14)
#define setGpioD15()	GPIOD->BOR = (1 << 15)

#define resetGpioD0()	GPIOD->BCR = (1 << 0)
#define resetGpioD1()	GPIOD->BCR = (1 << 1)
#define resetGpioD2()	GPIOD->BCR = (1 << 2)
#define resetGpioD3()	GPIOD->BCR = (1 << 3)
#define resetGpioD4()	GPIOD->BCR = (1 << 4)
#define resetGpioD5()	GPIOD->BCR = (1 << 5)
#define resetGpioD6()	GPIOD->BCR = (1 << 6)
#define resetGpioD7()	GPIOD->BCR = (1 << 7)
#define resetGpioD8()	GPIOD->BCR = (1 << 8)
#define resetGpioD9()	GPIOD->BCR = (1 << 9)
#define resetGpioD10()	GPIOD->BCR = (1 << 10)
#define resetGpioD11()	GPIOD->BCR = (1 << 11)
#define resetGpioD12()	GPIOD->BCR = (1 << 12)
#define resetGpioD13()	GPIOD->BCR = (1 << 13)
#define resetGpioD14()	GPIOD->BCR = (1 << 14)
#define resetGpioD15()	GPIOD->BCR = (1 << 15)

#define setGpioE(x)		GPIOE->DOR = x
#define setGpioE0()		GPIOE->BOR = (1 << 0)
#define setGpioE1()		GPIOE->BOR = (1 << 1)
#define setGpioE2()		GPIOE->BOR = (1 << 2)
#define setGpioE3()		GPIOE->BOR = (1 << 3)
#define setGpioE4()		GPIOE->BOR = (1 << 4)
#define setGpioE5()		GPIOE->BOR = (1 << 5)
#define setGpioE6()		GPIOE->BOR = (1 << 6)
#define setGpioE7()		GPIOE->BOR = (1 << 7)
#define setGpioE8()		GPIOE->BOR = (1 << 8)
#define setGpioE9()		GPIOE->BOR = (1 << 9)
#define setGpioE10()	GPIOE->BOR = (1 << 10)
#define setGpioE11()	GPIOE->BOR = (1 << 11)
#define setGpioE12()	GPIOE->BOR = (1 << 12)
#define setGpioE13()	GPIOE->BOR = (1 << 13)
#define setGpioE14()	GPIOE->BOR = (1 << 14)
#define setGpioE15()	GPIOE->BOR = (1 << 15)

#define resetGpioE0()	GPIOE->BCR = (1 << 0)
#define resetGpioE1()	GPIOE->BCR = (1 << 1)
#define resetGpioE2()	GPIOE->BCR = (1 << 2)
#define resetGpioE3()	GPIOE->BCR = (1 << 3)
#define resetGpioE4()	GPIOE->BCR = (1 << 4)
#define resetGpioE5()	GPIOE->BCR = (1 << 5)
#define resetGpioE6()	GPIOE->BCR = (1 << 6)
#define resetGpioE7()	GPIOE->BCR = (1 << 7)
#define resetGpioE8()	GPIOE->BCR = (1 << 8)
#define resetGpioE9()	GPIOE->BCR = (1 << 9)
#define resetGpioE10()	GPIOE->BCR = (1 << 10)
#define resetGpioE11()	GPIOE->BCR = (1 << 11)
#define resetGpioE12()	GPIOE->BCR = (1 << 12)
#define resetGpioE13()	GPIOE->BCR = (1 << 13)
#define resetGpioE14()	GPIOE->BCR = (1 << 14)
#define resetGpioE15()	GPIOE->BCR = (1 << 15)

#define setGpioF(x)		GPIOF->DOR = x
#define setGpioF0()		GPIOF->BOR = (1 << 0)
#define setGpioF1()		GPIOF->BOR = (1 << 1)
#define setGpioF2()		GPIOF->BOR = (1 << 2)
#define setGpioF3()		GPIOF->BOR = (1 << 3)
#define setGpioF4()		GPIOF->BOR = (1 << 4)
#define setGpioF5()		GPIOF->BOR = (1 << 5)
#define setGpioF6()		GPIOF->BOR = (1 << 6)
#define setGpioF7()		GPIOF->BOR = (1 << 7)
#define setGpioF8()		GPIOF->BOR = (1 << 8)
#define setGpioF9()		GPIOF->BOR = (1 << 9)
#define setGpioF10()	GPIOF->BOR = (1 << 10)
#define setGpioF11()	GPIOF->BOR = (1 << 11)
#define setGpioF12()	GPIOF->BOR = (1 << 12)
#define setGpioF13()	GPIOF->BOR = (1 << 13)
#define setGpioF14()	GPIOF->BOR = (1 << 14)
#define setGpioF15()	GPIOF->BOR = (1 << 15)

#define resetGpioF0()	GPIOF->BCR = (1 << 0)
#define resetGpioF1()	GPIOF->BCR = (1 << 1)
#define resetGpioF2()	GPIOF->BCR = (1 << 2)
#define resetGpioF3()	GPIOF->BCR = (1 << 3)
#define resetGpioF4()	GPIOF->BCR = (1 << 4)
#define resetGpioF5()	GPIOF->BCR = (1 << 5)
#define resetGpioF6()	GPIOF->BCR = (1 << 6)
#define resetGpioF7()	GPIOF->BCR = (1 << 7)
#define resetGpioF8()	GPIOF->BCR = (1 << 8)
#define resetGpioF9()	GPIOF->BCR = (1 << 9)
#define resetGpioF10()	GPIOF->BCR = (1 << 10)
#define resetGpioF11()	GPIOF->BCR = (1 << 11)
#define resetGpioF12()	GPIOF->BCR = (1 << 12)
#define resetGpioF13()	GPIOF->BCR = (1 << 13)
#define resetGpioF14()	GPIOF->BCR = (1 << 14)
#define resetGpioF15()	GPIOF->BCR = (1 << 15)

#define setGpioG(x)		GPIOG->DOR = x
#define setGpioG0()		GPIOG->BOR = (1 << 0)
#define setGpioG1()		GPIOG->BOR = (1 << 1)
#define setGpioG2()		GPIOG->BOR = (1 << 2)
#define setGpioG3()		GPIOG->BOR = (1 << 3)
#define setGpioG4()		GPIOG->BOR = (1 << 4)
#define setGpioG5()		GPIOG->BOR = (1 << 5)
#define setGpioG6()		GPIOG->BOR = (1 << 6)
#define setGpioG7()		GPIOG->BOR = (1 << 7)
#define setGpioG8()		GPIOG->BOR = (1 << 8)
#define setGpioG9()		GPIOG->BOR = (1 << 9)
#define setGpioG10()	GPIOG->BOR = (1 << 10)
#define setGpioG11()	GPIOG->BOR = (1 << 11)
#define setGpioG12()	GPIOG->BOR = (1 << 12)
#define setGpioG13()	GPIOG->BOR = (1 << 13)
#define setGpioG14()	GPIOG->BOR = (1 << 14)
#define setGpioG15()	GPIOG->BOR = (1 << 15)

#define resetGpioG0()	GPIOG->BCR = (1 << 0)
#define resetGpioG1()	GPIOG->BCR = (1 << 1)
#define resetGpioG2()	GPIOG->BCR = (1 << 2)
#define resetGpioG3()	GPIOG->BCR = (1 << 3)
#define resetGpioG4()	GPIOG->BCR = (1 << 4)
#define resetGpioG5()	GPIOG->BCR = (1 << 5)
#define resetGpioG6()	GPIOG->BCR = (1 << 6)
#define resetGpioG7()	GPIOG->BCR = (1 << 7)
#define resetGpioG8()	GPIOG->BCR = (1 << 8)
#define resetGpioG9()	GPIOG->BCR = (1 << 9)
#define resetGpioG10()	GPIOG->BCR = (1 << 10)
#define resetGpioG11()	GPIOG->BCR = (1 << 11)
#define resetGpioG12()	GPIOG->BCR = (1 << 12)
#define resetGpioG13()	GPIOG->BCR = (1 << 13)
#define resetGpioG14()	GPIOG->BCR = (1 << 14)
#define resetGpioG15()	GPIOG->BCR = (1 << 15)

#endif