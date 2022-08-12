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

#ifndef YSS_FAT32_DIRECTORY_ENTRY__H_
#define YSS_FAT32_DIRECTORY_ENTRY__H_

#include "Fat32Cluster.h"

class Fat32DirectoryEntry
{
	struct LongFileName
	{
		char order;
		char name1[10];
		char attr;
		char type;
		char checksum;
		char name2[12];
		char zero[2];
		char name3[4];
	};

	struct DirectoryEntry
	{
		char name[8];
		char extention[3];
		char attr;
		char reserved[2];
		unsigned short createdTime;
		unsigned short createdDate;
		unsigned short lastAccessedDate;
		unsigned short startingClusterHigh;
		unsigned short lastModifiedTime;
		unsigned short lastModifiedDate;
		unsigned short startingClusterLow;
		unsigned int fileSize;
	};
	
	enum
	{
		MAX_LFN = 20
	};

	Fat32Cluster *mCluster;
	DirectoryEntry *mEntryBuffer;
	unsigned short mSectorSize;
	signed short mIndex;
	unsigned char mCurrentAttribute, mLfnCount;
	LongFileName mLfn[MAX_LFN];

	error insertEntry(unsigned char lfnLen, DirectoryEntry *src);
	unsigned int translateUtf16ToUtf8(const char *utf16);
	unsigned int translateMixedUtf16ToUtf8(const char *utf16);
	unsigned short translateUtf8ToUtf16(const char *utf8);
	int strlen(const char *src);
	void setShortName(void *des, const char *src);
	unsigned char calculateChecksum(DirectoryEntry *src);
	void copyStringUtf8ToLfnBuffer(const char *utf8, signed int len);
	DirectoryEntry getCurrentDirectoryEntry(void);

	error prepareInsert(unsigned int &cluster, DirectoryEntry &sfn, unsigned char attribute, const char *name, unsigned int len);

public:
	Fat32DirectoryEntry(void);
	void init(Fat32Cluster &cluster, void* sectorBuffer);
	error moveToRoot(void);
	error moveToStart(void);
	error moveToNext(void);
	error moveToEnd(void);
	error setRootCluster(unsigned int cluster);
	error setCluster(unsigned int cluster);
	error append(void);
	unsigned int getRootCluster(void);
	
	error makeDirectory(const char *name);
	error makeFile(const char *name);

	error getTargetName(void *des, unsigned int size);
	unsigned char getTargetAttribute(void);
	unsigned int getTargetCluster(void);
	unsigned int getTargetFileSize(void);
	void setTargetFileSize(unsigned int size);
	bool comapreTargetName(const char *utf8);
	error saveEntry(void);
};

#endif
