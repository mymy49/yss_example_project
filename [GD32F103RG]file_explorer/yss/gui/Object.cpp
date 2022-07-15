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

//#include <__cross_studio_io.h>
#include <yss/gui.h>

#if defined(DMA2D) && USE_GUI && YSS_L_HEAP_USE

Mutex Object::mMutex;

Object::Object(void)
{
	mPos.x = 0;
	mPos.y = 0;
	mParent = 0;
	mFrame = 0;
	mVisibleFlag = true;
}

void Object::update(Pos pos, Size size)
{
	if (mFrame)
		mFrame->update(pos, size);
	else if (mParent)
		mParent->update(pos, size);
}

void Object::update(Pos beforePos, Size beforeSize, Pos currentPos, Size currentSize)
{
	if (mFrame)
		mFrame->update(beforePos, beforeSize, currentPos, currentSize);
	else if (mParent)
		mParent->update(beforePos, beforeSize, currentPos, currentSize);
}

void Object::update(void)
{
	if (mFrame)
		mFrame->update(mPos, mSize);
	else if (mParent)
		mParent->update(mPos, mSize);
}

void Object::setPos(Pos pos)
{
	setPos(pos.x, pos.y);
}

void Object::setPos(signed short x, signed short y)
{
	mMutex.lock();
	Pos before = mPos;
	mPos = Pos{x, y};
	update(before, FrameBuffer::mSize, mPos, FrameBuffer::mSize);
	mMutex.unlock();
}

Pos Object::getPos(void)
{
	return mPos;
}

void Object::setSize(Size size)
{
	mMutex.lock();
	FrameBuffer::setSize(size.width, size.height);
	paint();
	update(mPos, mSize, mPos, size);
	mSize = size;
	mMutex.unlock();
}

void Object::setSize(unsigned short width, unsigned short height)
{
	setSize(Size{width, height});
}

void Object::setVisible(bool on)
{
	mVisibleFlag = on;
	update(mPos, FrameBuffer::mSize);
}

bool Object::isVisible(void)
{
	return mVisibleFlag;
}

void Object::setParent(Container *parent)
{
	mParent = parent;
}

void Object::setFrame(Frame *frame)
{
	mFrame = frame;
}

Object *Object::handlerPush(Pos pos)
{
	return this;
}

Object *Object::handlerDrag(Pos pos)
{
	return this;
}

Object *Object::handlerUp(void)
{
	return this;
}

Pos Object::getAbsolutePos(void)
{
	Pos pos = Pos{0, 0};

	if (mParent)
		pos = mParent->getAbsolutePos();

	pos.x = mPos.x;
	pos.y = mPos.y;

	return pos;
}

#endif