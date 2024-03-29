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

#include <yss/File.h>
#include <string.h>

File::File(sac::FileSystem &fileSystem)
{
	mFileSystem = &fileSystem;
	mOpenFlag = false;
	mBuffer = (unsigned char*)mFileSystem->getSectorBuffer();
	mFileSize = 0;
	mOpenMode = READ_ONLY;
	mBufferCount = 0;
}

File::File(sac::FileSystem *fileSystem)
{
	mFileSystem = fileSystem;
	mOpenFlag = false;
	mBuffer = (unsigned char*)mFileSystem->getSectorBuffer();
	mFileSize = 0;
	mOpenMode = READ_ONLY;
	mBufferCount = 0;
}

// 새로운 SD 메모리가 장착되면 init()을 먼저 호출해줘야 한다.
// SD 메모리의 기본 정보를 읽어오고 루트 디렉토리를 찾는다.
error File::init(void)
{
	return mFileSystem->init();
}

// 파일을 오픈하는 함수이다.
// 파일 저장소의 루트 디렉토리는 파일명의 첫 글짜에 '/' 문자를 사용한다.
// 루트 디렉토리 지정이 없을 경우 현재 디렉토리를 기준의 탐색 한다.
// 현재 디렉토리는 setPath() 함수를 사용한다.
// 현재 디렉토리를 루트로 옮기기 위해서 moveToRoot() 함수 호출이 필요하다.
//
// 반환형 error
//		/inc/yss/error.h에 정의된 에러를 반환한다.
//
// const char *fileName
//		오픈할 파일명을 지정한다. utf8 엔코딩을 사용한다.
//
// unsigned char mode
//		오픈할 파일의 모드를 설정한다. 읽기 또는 쓰기 등의 설정을 한다.
//		사용 가능한 설정은 File class에 정의되어 있다.
error File::open(const char *fileName, unsigned char mode)
{
	if(mOpenFlag)
		return Error::BUSY;

	switch(mode)
	{
	case WRITE_ONLY :
		mFileSize = 0;
		break;
	case READ_ONLY :
		break;
	default :
		return Error::UNSUPPORTED_MODE;
	} 

	const char *src = fileName;
	mOpenMode = mode;

	if(checkFileName(fileName) == false)
		return Error::WRONG_FILE_NAME;

	error result;
	thread::protect();
	char *name = new char[256];
	
	if(*src == '/')
	{
		mFileSystem->moveToRootDirectory();
		src++;
	}

	while(*src != 0)
	{
		if(bringOneName(name, &src))
		{
			result = enterDirectory(name);
			if(result != Error::NONE)
			{
				goto error_handler;
			}
			if(*src != '/')
				break;;
		}
		else
		{
			result = findFile(name);
			
			switch(mOpenMode)
			{
			case READ_ONLY :
				if(result != Error::NONE)
					goto error_handler;

				result = mFileSystem->open();
				if(result == Error::NONE)
					mOpenFlag = true;
				mFileSize = mFileSystem->getFileSize();
				mBufferCount = 0;
				break;

			case WRITE_ONLY :
				if(result == Error::NONE)
				{
					result = mFileSystem->open();
					if(result == Error::NONE)
						mOpenFlag = true;
				}
				else if(result == Error::NOT_EXIST_NAME)
				{
					result = mFileSystem->makeFile(name);
					if(result != Error::NONE)
						goto error_handler;

					result = findFile(name);
					if(result != Error::NONE)
						goto error_handler;

					result = mFileSystem->open();
					if(result == Error::NONE)
						mOpenFlag = true;
				}
				mBufferCount = 0;
				break;

				
			}
			
			// 정상 종료지만 코드 재활용을 위해 error_handler 호출
			goto error_handler;
		}
		
		src++;
	}

	result = Error::INDEX_OVER;

error_handler:
	thread::unprotect();
	return result;
}

// Directory class의 getCurrentDirectoryCluster() 함수로부터 현재 디렉토리 경로에 해당하는 cluster 번호를 
// 사용해 현재 경로를 지정하는 함수이다.
//
// 반환형 error
//		/inc/yss/error.h에 정의된 에러를 반환한다.
//
// unsigned int cluster
//		디렉토리에 할당된 clsuter 번호를 지정한다.
error File::setPath(unsigned int cluster)
{
	return mFileSystem->moveToCluster(cluster);
}

bool File::checkFileName(const char *fileName)
{
	while(*fileName)
	{
		if(	*fileName == '\\' ||
			*fileName == '?' ||
			*fileName == '%' ||
			*fileName == '*' ||
			*fileName == ';' ||
			*fileName == '|' ||
			*fileName == '"' ||
			*fileName == '<' ||
			*fileName == '>'
		)
			return false;
		
		fileName++;
	}

	return true;
}

bool File::bringOneName(char *des, const char **src)
{
	while(**src != 0 && **src != '/')
	{
		*des++ = *(*src)++;
	}
	
	*des++ = 0;
	return *(*src) == '/';
}

error File::enterDirectory(const char *name)
{
	error result;

	if(mOpenFlag)
		return Error::BUSY;

	result = mFileSystem->moveToStart();
	if(result != Error::NONE)
		return result;
	
	if(mFileSystem->isDirectory() == false)
		result = mFileSystem->moveToNextDirectory();
	if(result != Error::NONE)
		return result;

	while(1)
	{
		if(mFileSystem->compareName(name) == false)
		{
			result = mFileSystem->enterDirectory();
			if(result != Error::NONE)
				return result;

			return Error::NONE;
		}

		result = mFileSystem->moveToNextDirectory();
		if(result != Error::NONE)
			return result;
	}
	
	return Error::NOT_EXIST_NAME;
}

error File::findFile(const char *name)
{
	error result;

	result = mFileSystem->moveToStart();
	if(result != Error::NONE)
		return result;
	
	if(mFileSystem->isFile() == false)
		result = mFileSystem->moveToNextFile();
	if(result == Error::INDEX_OVER)
		return Error::NOT_EXIST_NAME;
	else if(result != Error::NONE)
		return result;

	while(1)
	{
		if(mFileSystem->compareName(name) == false)
			return Error::NONE;

		result = mFileSystem->moveToNextFile();
		if(result == Error::INDEX_OVER)
			return Error::NOT_EXIST_NAME;
		else if(result != Error::NONE)
			return result;
	}
	
	return Error::NOT_EXIST_NAME;
}

unsigned int File::read(void *des, unsigned int size)
{
	if(!mOpenFlag)
		return 0;
	
	switch(mOpenMode)
	{
	case READ_ONLY :
		break;
	default :
		return 0;
	}

	char *src, *cDes = (char*)des;
	unsigned int tmp, len = 0;
	error result;

	while(size)
	{
		if(mBufferCount > 0)
		{
			src = (char*)&mBuffer[512 - mBufferCount];

			if(size >= mBufferCount)
			{
				tmp = mBufferCount;
				size -= tmp;
				mBufferCount = 0;
			}
			else
			{
				tmp = size;
				mBufferCount -= size;
				size = 0;
			}

			len += tmp;
			memcpy(cDes, src, tmp);
			cDes += tmp;

			if(size == 0)
				return len;
		}

		result = mFileSystem->read(mBuffer);
		mBufferCount = 512;
		if(result == Error::INDEX_OVER)
			return len;
		else if(result != Error::NONE)
			return 0;
	}
	
	return len;
}

unsigned int File::write(void *src, unsigned int size)
{
	if(!mOpenFlag)
		return 0;
	
	switch(mOpenMode)
	{
	case WRITE_ONLY :
		break;
	default :
		return 0;
	}

	char *des, *cSrc = (char*)src;
	unsigned int tmp, len = 0;
	error result;

	while(size)
	{
		des = (char*)&mBuffer[mBufferCount];

		if(size >= (512-mBufferCount))
		{
			tmp = (512-mBufferCount);
			size -= tmp;
			mBufferCount = 512;
		}
		else
		{
			tmp = size;
			mBufferCount += size;
			size = 0;
		}

		len += tmp;
		memcpy(des, cSrc, tmp);
		cSrc += tmp;
		mFileSize += tmp;
		
		if(mBufferCount >= 512)
		{
			result = mFileSystem->write(mBuffer);
			if(result != Error::NONE)
				return 0;
			mBufferCount = 0;
		}
	}
	
	return len;
}

unsigned int File::getSize(void)
{
	if(!mOpenFlag)
		return Error::FILE_NOT_OPENED;

	return mFileSize;
}

error File::moveToStart(void)
{
	if(!mOpenFlag)
		return Error::FILE_NOT_OPENED;

	return mFileSystem->moveToStart();
}

error File::moveToEnd(void)
{
	unsigned int movingSector = mFileSize / 512;
	error result;

	if(!mOpenFlag)
		return Error::FILE_NOT_OPENED;

	moveToStart();
	
	for(unsigned int i=0;i<movingSector;i++)
	{
		result = mFileSystem->moveToNextSector();
		if(result != Error::NONE)
			return result;
	}

	mBufferCount = mFileSize % 512;

	return Error::NONE;
}

error File::moveTo(unsigned int position)
{
	error result;
	unsigned int movingSector;

	if(!mOpenFlag)
		return Error::FILE_NOT_OPENED;

	if(position > mFileSize)
		return moveToEnd();
	
	movingSector = position / 512;

	moveToStart();
	
	for(unsigned int i=0;i<movingSector;i++)
	{
		result = mFileSystem->moveToNextSector();
		if(result != Error::NONE)
			return result;
	}

	mBufferCount = position % 512;

	return Error::NONE;
}

error File::makeFile(const char *fileName)
{
	error result;

	if(mOpenFlag)
		return Error::BUSY;

	result = findFile(fileName);
	if(result == Error::NOT_EXIST_NAME)
	{
		result = mFileSystem->makeFile(fileName);
		if(result != Error::NONE)
			return result;
	}

	return Error::NONE;
}

error File::close(void)
{
	if(!mOpenFlag)
		return Error::FILE_NOT_OPENED;

	error result;
	mOpenFlag = false;

	switch(mOpenMode)
	{
	case WRITE_ONLY :
		if(mBufferCount)
		{
			result = mFileSystem->write(mBuffer);
			if(result != Error::NONE)
				return result;
			mBufferCount = 0;
			result = mFileSystem->close(mFileSize);
			if(result != Error::NONE)
				return result;
		}
		break;
	case READ_ONLY :
		mFileSystem->close();
		break;
	default :
		return Error::UNSUPPORTED_MODE;
	}

	return Error::NONE;
}
