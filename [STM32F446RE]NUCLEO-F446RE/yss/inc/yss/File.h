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

#ifndef YSS_FILE__H_
#define YSS_FILE__H_

#include <sac/FileSystem.h>

class File
{
	sac::FileSystem *mFileSystem;
	bool mOpenFlag;
	uint8_t *mBuffer, mOpenMode;
	uint32_t mFileSize, mBufferCount;

	bool checkFileName(const char *fileName);
	bool bringOneName(char *des, const char **src);
	error enterDirectory(const char *name);
	error findFile(const char *name);
public:
	enum
	{
		WRITE_ONLY = 0,
		READ_ONLY,
	};

	File(sac::FileSystem &fileSystem);
	File(sac::FileSystem *fileSystem);
	error init(void);
	error open(const char *fileName, uint8_t mode);
	error open(int32_t  cluster, uint8_t mode);
	error setPath(uint32_t cluster);
	uint32_t read(void *des, uint32_t size);
	uint32_t write(void *src, uint32_t size);
	uint32_t getSize(void);
	error moveToStart(void);
	error moveToEnd(void);
	error moveTo(uint32_t position);
	error makeFile(const char *fileName);
	error close(void);
};

#endif