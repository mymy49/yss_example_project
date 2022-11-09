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

#if defined(STM32G431xx) || defined(STM32G441xx) || \
	defined(STM32G471xx) || defined(STM32G473xx) || defined(STM32G474xx) || defined(STM32G483xx) || defined(STM32G484xx) || defined(STM32GBK1CB)

#include <drv/peripheral.h>
#include <yss/instance.h>

namespace drv
{
uint8_t gHseFreq __attribute__((section(".non_init")));
uint32_t gPllPFreq __attribute__((section(".non_init")));
uint32_t gPllQFreq __attribute__((section(".non_init")));
uint32_t gPllRFreq __attribute__((section(".non_init")));
uint32_t gSaiPllFreq __attribute__((section(".non_init")));
uint32_t gLcdPllFreq __attribute__((section(".non_init")));

static const uint32_t gPpreDiv[8] = {1, 1, 1, 1, 2, 4, 8, 16};
static const uint32_t gHpreDiv[16] = {1, 1, 1, 1, 1, 1, 1, 1, 2, 4, 8, 16, 64, 128, 256, 512};

inline uint8_t getRccPpre1(void)
{
	return (RCC->CFGR & RCC_CFGR_PPRE1_Msk) >> RCC_CFGR_PPRE1_Pos;
}

inline uint8_t getRccPpre2(void)
{
	return (RCC->CFGR & RCC_CFGR_PPRE2_Msk) >> RCC_CFGR_PPRE2_Pos;
}

inline uint8_t getRccHpre(void)
{
	return (RCC->CFGR & RCC_CFGR_HPRE_Msk) >> RCC_CFGR_HPRE_Pos;
}

inline uint8_t getRccSysclkSw(void)
{
	return (RCC->CFGR & RCC_CFGR_SWS_Msk) >> RCC_CFGR_SWS_Pos;
}

bool Clock::setVoltageScale(uint8_t range)
{
	uint32_t cr = PWR->CR1;

	RCC->APB1ENR1 |= RCC_APB1ENR1_PWREN_Msk;

	using namespace define::clock::voltageScale;
	switch (range)
	{
	case RANGE1_NORMAL:
		cr &= ~PWR_CR1_VOS_1;
		cr |= PWR_CR1_VOS_0;
		PWR->CR1 = cr;
		PWR->CR5 |= PWR_CR5_R1MODE_Msk;
		break;
	case RANGE1_BOOST:
		cr &= ~PWR_CR1_VOS_1;
		cr |= PWR_CR1_VOS_0;
		PWR->CR1 = cr;
		PWR->CR5 &= ~PWR_CR5_R1MODE_Msk;
		break;
	case RANGE2:
		cr &= ~PWR_CR1_VOS_0;
		cr |= PWR_CR1_VOS_1;
		PWR->CR1 = cr;
		break;
	default:
		return false;
	}

	return true;
}

uint8_t Clock::getVoltageScale(void)
{
	int32_t  cr1 = PWR->CR1 & PWR_CR1_VOS_Msk;
	int32_t  cr5 = PWR->CR5 & PWR_CR5_R1MODE_Msk;

	using namespace define::clock::voltageScale;
	if (cr1 == PWR_CR1_VOS_1)
		return RANGE2;
	else
	{
		if (cr5)
			return RANGE1_NORMAL;
		else
			return RANGE1_BOOST;
	}
}

uint8_t Clock::getAhbPrescale(void)
{
	return (RCC->CFGR & RCC_CFGR_HPRE_Msk) >> RCC_CFGR_HPRE_Pos;
}

void Clock::setAhbPrescale(uint8_t ahb)
{
	uint32_t cfgr = RCC->CFGR;
	cfgr &= ~RCC_CFGR_HPRE_Msk;
	ahb &= 0x0f;
	cfgr |= ahb << RCC_CFGR_HPRE_Pos;
	RCC->CFGR = cfgr;
}

bool Clock::enableLsi(bool)
{
#if defined(YSS_PERI_REPORT)
	debug_printf("\n########## LSI 장치 설정 ##########\n\n");
	debug_printf("클럭 = %d kHz\n", ec::clock::lsi::FREQ);
#endif

	RCC->CSR |= RCC_CSR_LSION_Msk;

	for (uint32_t i = 0; i < 100000; i++)
	{
		if (RCC->CSR & RCC_CSR_LSIRDY_Msk)
		{
#if defined(YSS_PERI_REPORT)
			debug_printf("장치 설정 완료.\n");
#endif
			return true;
		}
	}

#if defined(YSS_PERI_REPORT)
	debug_printf("장치 설정 실패.\n");
	debug_printf("활성화 대기 시간을 초과했습니다.\n");
#endif
	return false;
}

bool Clock::enableHse(uint8_t hseMhz, bool useOsc)
{
	uint32_t hse = (uint32_t )hseMhz * 1000000;
	gHseFreq = hseMhz;

#if defined(YSS_PERI_REPORT)
	debug_printf("\n########## HSE 장치 설정 ##########\n\n");
	debug_printf("외부 크리스탈 클럭 = %d MHz\n", hseMhz);
#endif

	if (hse < ec::clock::hse::HSE_MIN_FREQ || ec::clock::hse::HSE_MAX_FREQ < hse)
	{
#if defined(YSS_PERI_REPORT)
		debug_printf("장치 설정 실패.\n");
		debug_printf("HSE 클럭이 입력 허용 범위를 초과했습니다. %d kHz(min) < %d kHz(user) < %d kHz(max).\n", ec::clock::hse::HSE_MIN_FREQ / 1000, hse / 1000, ec::clock::hse::HSE_MAX_FREQ / 1000);
#endif
		return false;
	}

	RCC->CR |= RCC_CR_HSEON_Msk;

	for (uint32_t i = 0; i < 100000; i++)
	{
		if (RCC->CR & RCC_CR_HSERDY_Msk)
		{
#if defined(YSS_PERI_REPORT)
			debug_printf("장치 설정 완료.\n");
#endif
			return true;
		}
	}

#if defined(YSS_PERI_REPORT)
	debug_printf("장치 설정 실패.\n");
	debug_printf("활성화 대기 시간을 초과했습니다.\n");
#endif
	return false;
}

uint32_t Clock::getApb1ClkFreq(void)
{
	return (uint32_t)(getSysClkFreq() / gPpreDiv[getRccPpre1()]);
}

uint32_t Clock::getApb2ClkFreq(void)
{
	return (uint32_t)(getSysClkFreq() / gPpreDiv[getRccPpre2()]);
}

uint32_t Clock::getTimerApb1ClkFreq(void)
{
	uint8_t pre = getRccPpre1();
	uint32_t clk = getSysClkFreq() / gPpreDiv[pre];
	if (gPpreDiv[pre] > 1)
		clk <<= 1;
	return clk;
}

uint32_t Clock::getTimerApb2ClkFreq(void)
{
	uint8_t pre = getRccPpre2();
	uint32_t clk = getSysClkFreq() / gPpreDiv[pre];
	if (gPpreDiv[pre] > 1)
		clk <<= 1;
	return clk;
}

uint32_t Clock::getSysClkFreq(void)
{
	uint32_t clk;

	switch (getRccSysclkSw())
	{
	case define::clock::sysclk::src::HSI:
		clk = ec::clock::hsi::FREQ;
		break;
	case define::clock::sysclk::src::HSE:
		clk = gHseFreq;
		break;
	case define::clock::sysclk::src::PLL:
		clk = gPllRFreq;
		break;
	}

	clk /= gHpreDiv[getRccHpre()];

	return clk;
}

void Mainpll::setPEn(bool en)
{
	if (en)
		RCC->PLLCFGR |= RCC_PLLCFGR_PLLPEN_Msk;
	else
		RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLPEN_Msk;
}

void Mainpll::setQEn(bool en)
{
	if (en)
		RCC->PLLCFGR |= RCC_PLLCFGR_PLLQEN_Msk;
	else
		RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLQEN_Msk;
}

void Mainpll::setREn(bool en)
{
	if (en)
		RCC->PLLCFGR |= RCC_PLLCFGR_PLLREN_Msk;
	else
		RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLREN_Msk;
}

bool Mainpll::enable(uint8_t src, uint32_t vcoMhz, uint8_t pDiv, uint8_t qDiv, uint8_t rDiv)
{
	uint32_t vco, p, q, r, n, clk, buf, m, maxVco, minVco, pmin, pmax, qmin, qmax, rmin, rmax;
	uint8_t sw;
#if defined(YSS_PERI_REPORT)
	debug_printf("\n########## Main PLL 장치 설정 ##########\n\n");
#endif

	switch (src)
	{
	case define::clock::pll::src::HSI:
#if defined(YSS_PERI_REPORT)
		debug_printf("클럭 소스 = HSI 내부 RC 16MHz\n");
#endif
		clk = ec::clock::hsi::FREQ / 1000000;
		break;
	case define::clock::pll::src::HSE:
#if defined(YSS_PERI_REPORT)
		debug_printf("클럭 소스 = HSE 외부 크리스탈\n");
#endif
		if ((RCC->CR & RCC_CR_HSERDY_Msk) == 0)
		{
#if defined(YSS_PERI_REPORT)
			debug_printf("장치 설정 실패.\n");
			debug_printf("HSE 클럭이 활성화되지 않았습니다.\n");
#endif
			goto error;
		}
		clk = gHseFreq;
		break;
	default:
#if defined(YSS_PERI_REPORT)
		debug_printf("장치 설정 실패.\n");
		debug_printf("클럭 소스의 선택 범위를 초과했습니다.\n");
#endif
		goto error;
	}

	using namespace define::clock::voltageScale;
	switch (clock.getVoltageScale())
	{
	case RANGE2:
		minVco = ec::clock::pll::RANGE2_VCO_MIN_FREQ;
		maxVco = ec::clock::pll::RANGE2_VCO_MAX_FREQ;
		pmin = ec::clock::pll::RANGE2_PLLP_MIN_FREQ;
		pmax = ec::clock::pll::RANGE2_PLLP_MAX_FREQ;
		qmin = ec::clock::pll::RANGE2_PLLQ_MIN_FREQ;
		qmax = ec::clock::pll::RANGE2_PLLQ_MAX_FREQ;
		rmin = ec::clock::pll::RANGE2_PLLR_MIN_FREQ;
		rmax = ec::clock::pll::RANGE2_PLLR_MAX_FREQ;
		break;
	case RANGE1_NORMAL:
		minVco = ec::clock::pll::RANGE1_NORMAL_VCO_MIN_FREQ;
		maxVco = ec::clock::pll::RANGE1_NORMAL_VCO_MAX_FREQ;
		pmin = ec::clock::pll::RANGE1_NORMAL_PLLP_MIN_FREQ;
		pmax = ec::clock::pll::RANGE1_NORMAL_PLLP_MAX_FREQ;
		qmin = ec::clock::pll::RANGE1_NORMAL_PLLQ_MIN_FREQ;
		qmax = ec::clock::pll::RANGE1_NORMAL_PLLQ_MAX_FREQ;
		rmin = ec::clock::pll::RANGE1_NORMAL_PLLR_MIN_FREQ;
		rmax = ec::clock::pll::RANGE1_NORMAL_PLLR_MAX_FREQ;
		break;
	case RANGE1_BOOST:
		minVco = ec::clock::pll::RANGE1_BOOST_VCO_MIN_FREQ;
		maxVco = ec::clock::pll::RANGE1_BOOST_VCO_MAX_FREQ;
		pmin = ec::clock::pll::RANGE1_BOOST_PLLP_MIN_FREQ;
		pmax = ec::clock::pll::RANGE1_BOOST_PLLP_MAX_FREQ;
		qmin = ec::clock::pll::RANGE1_BOOST_PLLQ_MIN_FREQ;
		qmax = ec::clock::pll::RANGE1_BOOST_PLLQ_MAX_FREQ;
		rmin = ec::clock::pll::RANGE1_BOOST_PLLR_MIN_FREQ;
		rmax = ec::clock::pll::RANGE1_BOOST_PLLR_MAX_FREQ;
		break;
	}

	vco = vcoMhz * 1000000;
	using namespace ec::clock::pll;
	if (vco < minVco || maxVco < vco)
	{
#if defined(YSS_PERI_REPORT)
		debug_printf("장치 설정 실패.\n");
		debug_printf("VCO 클럭이 허용 범위를 벗어났습니다. %d kHz(min) < %d kHz(user) < %d kHz(max).\n", minVco / 1000, vco / 1000, maxVco / 1000);
#endif
		goto error;
	}

	using namespace define::clock::sysclk;
	sw = (RCC->CFGR & RCC_CFGR_SWS_Msk) >> RCC_CFGR_SWS_Pos;
	p = RCC->CR & RCC_CR_PLLRDY_Msk;

	if (p || sw == src::PLL)
	{
#if defined(YSS_PERI_REPORT)
		debug_printf("장치 설정 실패.\n");
		debug_printf("장치가 이미 활성화되어 시스템 클럭으로 설정되어 있습니다.\n");
#endif
		goto error;
	}

	for (int32_t  i = 1; i < clk; i++)
	{
		if (clk % i == 0)
		{
			buf = clk / i;
			if (vcoMhz % buf == 0)
			{
				m = i;
				goto mCalComplete;
			}
		}
	}

#if defined(YSS_PERI_REPORT)
	debug_printf("장치 설정 실패.\n");
	debug_printf("M의 계산 값을 찾을 수 없습니다. 입력 소스의 클럭과 설정한 VCO 클럭이 정확한지 확인해주세요.\n");
#endif

mCalComplete:
	buf = clk / m;
	if (buf <= (ec::clock::pll::VCO_IN_MIN_FREQ / 1000000) && (ec::clock::pll::VCO_IN_MAX_FREQ / 1000000) <= buf)
	{
#if defined(YSS_PERI_REPORT)
		debug_printf("장치 설정 실패.\n");
		debug_printf("VCO 입력 클럭이 너무 낮습니다. 입력 소스의 클럭과 설정한 VCO 클럭이 정확한지 확인해주세요.\n");
#endif
	}

	using namespace ec::clock::pll;
	if (M_MIN > (m - 1) || (m - 1) > M_MAX)
	{
#if defined(YSS_PERI_REPORT)
		debug_printf("장치 설정 실패.\n");
		debug_printf("계산 값 M이 허용 범위를 초과했습니다. 입력 소스의 클럭이 정확한지 확인해주세요. %d(min) < %d(user) < %d(max).\n", M_MIN, m, M_MAX);
#endif
		goto error;
	}

	n = vcoMhz / (clk / m);
	if (N_MIN > n || n > N_MAX)
	{
#if defined(YSS_PERI_REPORT)
		debug_printf("장치 설정 실패.\n");
		debug_printf("계산 값 N이 허용 범위를 초과했습니다. 인자 vcoMhz의 값을 확인해주세요. %d(min) < %d(user) < %d(max).\n", N_MIN, n, N_MAX);
#endif
		goto error;
	}

	if (P_MIN > pDiv || pDiv > P_MAX)
	{
#if defined(YSS_PERI_REPORT)
		debug_printf("장치 설정 실패.\n");
		debug_printf("인자 pDiv의 설정이 허용 범위를 벗어났습니다. %d(min) < %d(user) < %d(max).\n", P_MIN, pDiv, P_MAX);
#endif
		goto error;
	}

	if (qDiv > Q_R_MAX)
	{
#if defined(YSS_PERI_REPORT)
		debug_printf("장치 설정 실패.\n");
		debug_printf("인자 qDiv의 설정이 허용 범위를 벗어났습니다. %d(user) < %d(max).\n", qDiv, Q_R_MAX);
#endif
		goto error;
	}

	if (rDiv > Q_R_MAX)
	{
#if defined(YSS_PERI_REPORT)
		debug_printf("장치 설정 실패.\n");
		debug_printf("인자 rDiv의 설정이 허용 범위를 벗어났습니다. %d(user) < %d(max).\n", rDiv, Q_R_MAX);
#endif
		goto error;
	}

	p = vco / pDiv;
	if (pmin > p || p > pmax)
	{
#if defined(YSS_PERI_REPORT)
		debug_printf("장치 설정 실패.\n");
		debug_printf("Main PLL의 P 출력 주파수가 허용 범위를 벗어났습니다. %d kHz(min) < %d kHz(user) < %d kHz(max).\n", pmin / 1000, p / 1000, pmax / 1000);
#endif
		goto error;
	}

	q = vco / (2 + qDiv * 2);
	if (qmin > q || q > qmax)
	{
#if defined(YSS_PERI_REPORT)
		debug_printf("장치 설정 실패.\n");
		debug_printf("Main PLL의 Q 출력 주파수가 허용 범위를 벗어났습니다. %d kHz(min) < %d kHz(user) < %d kHz(max).\n", qmin / 1000, q / 1000, qmax / 1000);
#endif
		goto error;
	}

	r = vco / (2 + rDiv * 2);
	if (rmin > r || r > rmax)
	{
#if defined(YSS_PERI_REPORT)
		debug_printf("장치 설정 실패.\n");
		debug_printf("Main PLL의 R 출력 주파수가 허용 범위를 벗어났습니다. %d kHz(min) < %d kHz(user) < %d kHz(max).\n", rmin / 1000, r / 1000, rmax / 1000);
#endif
		goto error;
	}

	RCC->PLLCFGR = pDiv << RCC_PLLCFGR_PLLPDIV_Pos |
				   rDiv << RCC_PLLCFGR_PLLR_Pos |
				   qDiv << RCC_PLLCFGR_PLLQ_Pos |
				   n << RCC_PLLCFGR_PLLN_Pos |
				   (m - 1) << RCC_PLLCFGR_PLLM_Pos |
				   src << RCC_PLLCFGR_PLLSRC_Pos;

	RCC->CR |= RCC_CR_PLLON_Msk;

	for (uint32_t i = 0; i < 10000; i++)
	{
		if (RCC->CR & RCC_CR_PLLRDY_Msk)
		{
#if defined(YSS_PERI_REPORT)
			debug_printf("PLL P(ADC) = %d kHz\n", p / 1000);
			debug_printf("PLL Q(USB, SAI, I2S, CAN, RNG, QSPI) = %d kHz\n", q / 1000);
			debug_printf("PLL R(SYSCLK) = %d kHz\n", r / 1000);
			debug_printf("장치 설정 완료.\n");
#endif
			gPllPFreq = p;
			gPllQFreq = q;
			gPllRFreq = r;
			return true;
		}
	}

#if defined(YSS_PERI_REPORT)
	debug_printf("장치 설정 실패.\n");
	debug_printf("활성화 대기 시간을 초과했습니다.\n");
#endif
error:
	gPllPFreq = 0;
	gPllQFreq = 0;
	gPllRFreq = 0;
	return false;
}

bool Clock::setSysclk(uint8_t sysclkSrc, uint8_t ahb, uint8_t apb1, uint8_t apb2, uint8_t vcc)
{
	uint32_t clk, ahbClk, apb1Clk, apb2Clk, adcClk, cfgr;
	uint32_t ahbMax, apb1Max, apb2Max;

#if defined(YSS_PERI_REPORT)
	debug_printf("\n##########  시스템 클럭 설정 ##########\n\n");
#endif

	using namespace define::clock::sysclk::src;
	switch (sysclkSrc)
	{
	case HSI:
#if defined(YSS_PERI_REPORT)
		debug_printf("클럭 소스 = HSI 내부 RC 16MHz\n");
#endif
		clk = ec::clock::hsi::FREQ;
		break;
	case HSE:
#if defined(YSS_PERI_REPORT)
		debug_printf("클럭 소스 = HSE 외부 크리스탈\n");
#endif
		if ((RCC->CR & RCC_CR_HSERDY_Msk) == 0)
		{
#if defined(YSS_PERI_REPORT)
			debug_printf("장치 설정 실패.\n");
			debug_printf("외부 크리스탈이 현재 동작중이 아닙니다. 먼저 외부 크리스탈을 활성화 해주세요.\n");
#endif
			return false;
		}
		clk = gHseFreq * 1000000;
		break;
	case PLL:
#if defined(YSS_PERI_REPORT)
		debug_printf("클럭 소스 = PLL\n");
#endif
		if ((RCC->CR & RCC_CR_PLLRDY_Msk) == 0)
		{
#if defined(YSS_PERI_REPORT)
			debug_printf("장치 설정 실패.\n");
			debug_printf("PLL이 현재 동작중이 아닙니다. 먼저 PLL을 활성화 해주세요.\n");
#endif
			return false;
		}
		clk = gPllRFreq;
		break;
	default:
#if defined(YSS_PERI_REPORT)
		debug_printf("장치 설정 실패.\n");
		debug_printf("지정된 클럭 소스가 유효하지 않습니다.\n");
#endif
		return false;
	}

	using namespace define::clock::voltageScale;
	switch (getVoltageScale())
	{
	case RANGE1_NORMAL:
		ahbMax = ec::clock::sysclk::RANGE1_NORMAL_MAX_FREQ;
		apb1Max = ec::clock::apb1::RANGE1_NORMAL_MAX_FREQ;
		apb2Max = ec::clock::apb2::RANGE1_NORMAL_MAX_FREQ;
		break;
	case RANGE1_BOOST:
		ahbMax = ec::clock::sysclk::RANGE1_BOOST_MAX_FREQ;
		apb1Max = ec::clock::apb1::RANGE1_BOOST_MAX_FREQ;
		apb2Max = ec::clock::apb2::RANGE1_BOOST_MAX_FREQ;
		break;
	case RANGE2:
		ahbMax = ec::clock::sysclk::RANGE2_MAX_FREQ;
		apb1Max = ec::clock::apb1::RANGE2_MAX_FREQ;
		apb2Max = ec::clock::apb2::RANGE2_MAX_FREQ;
		break;
	default:
		return false;
	}

	ahbClk = clk / gHpreDiv[ahb];
	if (ahbClk > ahbMax)
	{
#if defined(YSS_PERI_REPORT)
		debug_printf("장치 설정 실패.\n");
		debug_printf("Sysclk 주파수가 허용 범위를 초과했습니다. %d(user) < %d(max).\n", ahbClk, ahbMax);
#endif
		return false;
	}

	apb1Clk = ahbClk / gPpreDiv[apb1];
	if (apb1Clk > apb1Max)
	{
#if defined(YSS_PERI_REPORT)
		debug_printf("장치 설정 실패.\n");
		debug_printf("APB1 주파수가 허용 범위를 초과했습니다. %d(user) < %d(max).\n", apb1Clk, apb1Max);
#endif
		return false;
	}

	apb2Clk = ahbClk / gPpreDiv[apb2];
	if (apb2Clk > apb2Max)
	{
#if defined(YSS_PERI_REPORT)
		debug_printf("장치 설정 실패.\n");
		debug_printf("APB2 주파수가 허용 범위를 초과했습니다. %d(user) < %d(max).\n", apb2Max);
		return false;
#endif
	}

	setLatency(ahbClk, vcc);

	cfgr = RCC->CFGR;
	cfgr &= ~(RCC_CFGR_PPRE1_Msk | RCC_CFGR_PPRE2_Msk | RCC_CFGR_HPRE_Msk | RCC_CFGR_SW_Msk);
	cfgr |= (apb1 << RCC_CFGR_PPRE1_Pos) | (apb2 << RCC_CFGR_PPRE2_Pos) | (ahb << RCC_CFGR_HPRE_Pos) | (sysclkSrc << RCC_CFGR_SW_Pos);
	RCC->CFGR = cfgr;

#if defined(YSS_PERI_REPORT)
	debug_printf("Sysclk = %d kHz\n", ahbClk / 1000);
	debug_printf("APB1 = %d kHz\n", apb1Clk / 1000);
	debug_printf("APB2 = %d kHz\n", apb2Clk / 1000);
	debug_printf("장치 설정 완료.\n");
#endif

	return true;
}

void Clock::setLatency(uint32_t freq, uint8_t vcc)
{
	uint8_t wait;
	uint32_t ws0, ws1, ws2, ws3, ws4, acr;

	switch (clock.getVoltageScale())
	{
	case define::clock::voltageScale::RANGE2:
		ws0 = 12 * 1000000;
		ws1 = 24 * 1000000;
		ws2 = 26 * 1000000;
		ws3 = 999 * 1000000;
		ws4 = 999 * 1000000;
		break;
	case define::clock::voltageScale::RANGE1_NORMAL:
		ws0 = 30 * 1000000;
		ws1 = 60 * 1000000;
		ws2 = 90 * 1000000;
		ws3 = 120 * 1000000;
		ws4 = 150 * 1000000;
		break;
	case define::clock::voltageScale::RANGE1_BOOST:
		ws0 = 34 * 1000000;
		ws1 = 68 * 1000000;
		ws2 = 102 * 1000000;
		ws3 = 136 * 1000000;
		ws4 = 170 * 1000000;
		break;
	}

	if (freq <= ws0)
	{
		wait = 0;
	}
	else if (freq <= ws1)
	{
		wait = 1;
	}
	else if (freq <= ws2)
	{
		wait = 2;
	}
	else if (freq <= ws3)
	{
		wait = 3;
	}
	else if (freq <= ws4)
	{
		wait = 4;
	}

	acr = FLASH->ACR;
	acr &= ~FLASH_ACR_LATENCY_Msk;
	acr |= wait << FLASH_ACR_LATENCY_Pos;
	FLASH->ACR = acr;
}
}

#endif