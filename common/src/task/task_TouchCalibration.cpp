////////////////////////////////////////////////////////////////////////////////////////
//
// 저작권 표기 License_ver_3.1
// 본 소스 코드의 소유권은 홍윤기에게 있습니다.
// 어떠한 형태든 기여는 기증으로 받아들입니다.
// 본 소스 코드는 아래 사항에 동의할 경우에 사용 가능합니다.
// 아래 사항에 대해 동의하지 않거나 이해하지 못했을 경우 사용을 금합니다.
// 본 소스 코드를 사용하였다면 아래 사항을 모두 동의하는 것으로 자동 간주 합니다.
// 본 소스 코드의 상업적 또는 비 상업적 이용이 가능합니다.
// 본 소스 코드의 내용을 임의로 수정하여 재배포하는 행위를 금합니다.
// 본 소스 코드의 사용으로 인해 발생하는 모든 사고에 대해서 어떠한 법적 책임을 지지 않습니다.
//
// Home Page : http://cafe.naver.com/yssoperatingsystem
// Copyright 2022. 홍윤기 all right reserved.
//
////////////////////////////////////////////////////////////////////////////////////////

#include <task.h>
#include <yss.h>
#include <yss/event.h>
#include <yss/PointerEvent.h>
#include "../font/Abyssinica_SIL_14.h"

#if USE_GUI && YSS_L_HEAP_USE

namespace Task
{
	sac::Rtouch *gTouch;

	void setTouchScreen(sac::Rtouch *touch)
	{
		gTouch = touch;
	}

	error calibrateTouchScreen(FunctionQueue *obj)
	{
		// gTouch의 값이 설정되어 있지 않다면 교정이 필요 없다고 간주하고 error::ERROR_NONE으로 리턴한다.
		if(gTouch)
			return error::ERROR_NONE;

		PointerEvent *pointerEvent = new PointerEvent(256);

		lock(); // unlock()을 만날 때까지 외부에서 이 함수를 강제 종료 시키지 못한다.
		clearTask();

		Frame *frame = new Frame;
		Label *label = new Label;
		
		//gTouch->setInterface(*pointerEvent, -1);
		//event::setPointerDevice(*gTouch);
	
		label->setSize(200, 20);
		label->setFont(Font_Abyssinica_SIL_14);
		label->setText("터치 스크린 교정을 시작합니다.");
		label->setBackgroundColor(0xFF, 0xFF, 0xFF);
		label->setFontColor(0x00, 0x00, 0x00);
		label->setPosition(Position{20, 160});
		frame->add(label);

		setFrame(frame);
		
		thread::delay(10000);

		delete pointerEvent;
		unlock();

		return error::ERROR_NONE;
	}
}

#endif

