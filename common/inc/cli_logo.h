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

#ifndef COMMON_CLI_LOGO__H_
#define COMMON_CLI_LOGO__H_

#include <util/CommandLineInterface.h>
#include <util/FunctionQueue.h>
#include <drv/Ltdc.h>
#include <yss/error.h>

#if !defined(YSS_DRV_LTDC_UNSUPPORTED)

// TFT LCD에 Logo 화면을 띄우는 CLI를 명령을 생성한다.
// 사용 순서는 아래와 같다.
// 1. setFunctionQueue() 함수를 사용해 현재 프로젝트의 FunctionQueue의 인스턴스를 등록한다.
// 2. registerCli() 함수를 사용해 CommandLineInterface class의 instance에 등록한다.
namespace Cli
{
namespace Logo
{
	// 현재 프로젝트에 선언된 FunctionQueue의 인스턴스를 등록한다.
	// 
	// FunctionQueue &obj
	//		현재 프로젝트에 선언된 FunctionQueuE의 인스턴스를 설정한다.
	void setFunctionQueue(FunctionQueue &obj);

	// CommandLineInterface class의 instance에 명령어와 관련 함수등을 등록한다.
	// 
	// CommandLineInterface &cli
	//		등록할 CLI를 설정한다.
	void registerCli(CommandLineInterface &cli);
}
}

#endif

#endif

