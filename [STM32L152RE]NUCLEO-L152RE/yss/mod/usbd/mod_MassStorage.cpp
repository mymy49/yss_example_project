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

#include <drv/peripheral.h>
/*
#if	defined(USB_OTG_FS) ||	defined(USB_OTG_HS)

#include <drv/peripherals.h>
#include <mod/usbd/MassStorage.h>
#include <string.h>
#include <yss/malloc.h>

#define OPCODE_TEST_UNIT_READY			0x00
#define OPCODE_REQUEST_SENSE			0x03
#define OPCODE_INQUIRY					0x12
#define OPCODE_MODE_SENSE6				0x1a
#define OPCODE_START_STOP				0x1b
#define OPCODE_ALLOW_MEDIUM_REMOVAL		0x1e
#define OPCODE_READ_FORMAT_CAPACITIES	0x23
#define OPCODE_READ_CAPACITY10			0x25
#define OPCODE_READ10					0x28
#define OPCODE_WRITE10					0x2a

#define RECEIVE_BUFFER_SIZE				8*1024

struct CommandBlockWrapper
{
	uint32_t dCBWSignature;
	uint32_t dCBWTag;
	uint32_t dCBWDataTransferLength;
	uint8_t bmCBWFlags;
	uint8_t bCBWLUN;
	uint8_t bCBWCBLength;
	uint8_t CBWCB[16];
}__attribute__((packed));

struct CommandStatusWrapper
{
	uint32_t dCSWSignature;
	uint32_t dCSWTag;
	uint32_t dCSWDataResidue;
	uint8_t bCSWStatus;
}__attribute__((packed));

struct Inquiry
{
	uint8_t opcode;
	unsigned reserved1 : 6;
	unsigned cmdDt : 1;
	unsigned evpd : 1;
	uint8_t pageOrOpcode;
	uint8_t reserved2;
	uint8_t allocationLength;
	uint8_t control;
}__attribute__((packed));

struct Read10
{
	uint8_t opcode;
	unsigned obsolete : 1;
	unsigned fuaNv : 1;
	unsigned reserved1 : 1;
	unsigned fua : 1;
	unsigned dpo : 1;
	unsigned rdProtect : 3;
	uint8_t logicalBlockAddress[4];
	unsigned groupNumber : 5;
	unsigned reserved2 : 3;
	uint8_t transferLength[2];
	uint8_t control;
}__attribute__((packed));

struct Write10
{
	uint8_t opcode;
	unsigned obsolete : 1;
	unsigned fuaNv : 1;
	unsigned reserved1 : 1;
	unsigned fua : 1;
	unsigned dpo : 1;
	unsigned wrProtect : 3;
	uint8_t logicalBlockAddress[4];
	unsigned groupNumber : 5;
	unsigned reserved2 : 3;
	uint8_t transferLength[2];
	uint8_t control;
}__attribute__((packed));

struct ReadFormatCapacities
{
	uint8_t opcode;
	unsigned reserved1 : 5;
	unsigned logicalUnitNumber : 3;
	uint8_t reserved2[5];
	uint8_t allocationLength[2];
	uint8_t reserved3[3];
}__attribute__((packed));

struct InquiryResponse
{
	unsigned peripheralDeviceType : 5;
	unsigned peripheralQualifier : 3;
	unsigned reserved1 : 7;
	unsigned rmb : 1;
	uint8_t version;
	unsigned responseDataFormat : 4;
	unsigned hiSup : 1;
	unsigned normACA : 1;
	unsigned obsolete1 : 1;
	unsigned aerc : 1;
	uint8_t additionalLength;
	unsigned reserved2 : 7;
	unsigned sccs : 1;
	unsigned addr16 : 1;
	unsigned obsolete2 : 2;
	unsigned mChngr : 1;
	unsigned multiP : 1;
	unsigned vs1 : 1;
	unsigned encServ : 1;
	unsigned bQue : 1;
	unsigned vs2 : 1;
	unsigned cmdQue : 1;
	unsigned obsolete3 : 1;
	unsigned linked : 1;
	unsigned sync : 1;
	unsigned wbus16 : 1;
	unsigned obsolete4 : 1;
	unsigned relAdr : 1;
	uint8_t vendorIdentification[8];
	uint8_t productIdentification[16];
	uint8_t productRevisionLevel[4];
}__attribute__((packed));

struct RequestSenseResponse
{
	unsigned responseCode : 7;				// 0
	unsigned valid : 1;
	uint8_t obsolate;					// 1
	unsigned senseKey : 4;					// 2
	unsigned reserved1 : 1;
	unsigned ili : 1;
	unsigned eom : 1;
	unsigned filmrk : 1;
	uint8_t information[4];			// 3
	uint8_t additionalSenseLength;	// 7
	uint8_t commandInformation[4];	// 8
	uint8_t addtionalSenseCode;		// 12
	uint8_t addtionalSenseCodeQualifier;	// 13
	uint8_t fieldReplaceableUnitCode;	// 14
	unsigned reserved2 : 7;					// 15
	unsigned sksv : 1;
	uint8_t senseKeySpecific;			// 16
	uint8_t addtionalSenseBytes;		// 17
}__attribute__((packed));

struct CurrentMaximumCapacityDescriptor
{
	uint8_t numOfBlocks[4];
	unsigned descriptorCode : 2;
	unsigned reserved : 6;
	uint8_t blockLength[3];
}__attribute__((packed));

struct CapacityListHeader
{
	uint8_t reserved[3];
	uint8_t capacityListLength;
}__attribute__((packed));

struct FormattableCapacityDescriptor
{
	uint8_t numOfBlocks[4];
	uint8_t reserved;
	uint8_t blockLength[3];
}__attribute__((packed));

struct ReadFormatCapacitiesResponse
{
	CapacityListHeader capacityListHeader;
	CurrentMaximumCapacityDescriptor currentMaximumCapacityHeader;
}__attribute__((packed));

static config::usbd::DeviceDescriptor gDeviceDescriptor =
{
	sizeof(config::usbd::DeviceDescriptor),	// uint8_t bLength;
	0x01,		// uint8_t bDescriptorType;
	0x0200,		// uint16_t bcdUSB;
	0,			// uint8_t bDeviceClass;
	0,			// uint8_t bDeviceSubclass;
	0,			// uint8_t bDeviceProtocol;
	64,			// uint8_t bMaxPacketSize0;
	0,			// uint16_t idVendor;
	0,			// uint16_t idProduct;
	0,			// uint16_t bcdDevice;
	1,			// uint8_t iManufacturer;
	2,			// uint8_t iProduct;
	3,			// uint8_t iSerialNumber;
	1			// uint8_t bNumConfigurations;
};

static config::usbd::InterfaceDescriptor gInterfaceDescriptor =
{
	sizeof(config::usbd::InterfaceDescriptor),	// uint8_t bLength;
	4,		// uint8_t bDescriptorType;
	0,		// uint8_t bInterfaceNumber;
	0,		// uint8_t bAlternateSetting;
	2,		// uint8_t bNumEndpoints;
	0x08,	// uint8_t bInterfaceClass;
	0x06,	// uint8_t bInterfaceSubClass;
	0x50,	// uint8_t bInterfaceProtocol;
	5		// uint8_t iInterface;
};

static config::usbd::ConfigurationDescriptor gConfigurationDescriptor =
{
	sizeof(config::usbd::ConfigurationDescriptor),	// uint8_t bLength;
	2,										// uint8_t bDescriptorType;
	sizeof(config::usbd::ConfigurationDescriptor) +
	sizeof(config::usbd::InterfaceDescriptor) +
	sizeof(config::usbd::EndpointDescriptor) * 2,		// uint16_t wTotalLength;
	1,										// uint8_t bNumInterfaces;
	1,										// uint8_t bConfigurationValue;
	4,										// uint8_t bConfiguration;
	0,										// uint8_t bmAttributes;
	0										// uint8_t bMaxPower;
};

static config::usbd::EndpointDescriptor gOutEndpointDescriptor =
{
	sizeof(config::usbd::EndpointDescriptor),	// uint8_t bLength;
	0x05,	// uint8_t bDescriptorType;
	0x01,	// uint8_t bEndpointAddress;
	0x02,	// uint8_t bmAttributes;
	64,		// uint16_t wMaxPacketSize;
	0,		// uint8_t bInterval;
};

static config::usbd::EndpointDescriptor gInEndpointDescriptor =
{
	sizeof(config::usbd::EndpointDescriptor),	// uint8_t bLength;
	0x05,	// uint8_t bDescriptorType;
	0x82,	// uint8_t bEndpointAddress;
	0x02,	// uint8_t bmAttributes;
	64,		// uint16_t wMaxPacketSize;
	0,		// uint8_t bInterval;
};

namespace mod
{
namespace usbd
{
namespace massStorage
{
	static void thread_handleMessage(void);
	static void trigger_handleSetup(config::usbd::SetupRequest *request);

	drv::Usbd *gPeri;
	config::mod::usbd::massStorage::Config *gConfig;
	int32_t gThreadId;

	bool init(drv::Usbd &peri, config::mod::usbd::massStorage::Config &config)
	{
		gPeri = &peri;
		gConfig = &config;

		if(gPeri == 0 || gConfig == 0)
			return false;

		gThreadId = thread::add(thread_handleMessage, 512);
		if(gThreadId < 0)
			return false;

		gPeri->mallocOutEndpoint(1, RECEIVE_BUFFER_SIZE);
		return gPeri->init(trigger_handleSetup);
	}

	void writeDeviceDescriptor(config::usbd::SetupRequest *request)
	{
		uint8_t *src = (uint8_t*)&gDeviceDescriptor;
		uint8_t buf[sizeof(gDeviceDescriptor)];
		uint8_t *des = (uint8_t*)buf;
		uint16_t *sDes;

		for(uint16_t i=0;i<sizeof(gDeviceDescriptor);i++)
			*des++ = *src++;

		sDes = (uint16_t*)&buf[8];
		*sDes = gConfig->vid;
		sDes = (uint16_t*)&buf[10];
		*sDes = gConfig->pid;
		sDes = (uint16_t*)&buf[12];
		*sDes = gConfig->bcdVersion;

		gPeri->writeSetup(buf, sizeof(gDeviceDescriptor), 1000);
	}

	void writeDeviceConfiguration(config::usbd::SetupRequest *request)
	{
		uint8_t *src = (uint8_t*)&gConfigurationDescriptor;
		uint8_t buf[128];
		uint8_t *des = (uint8_t*)buf;
		uint16_t *sDes;
		uint16_t configSize;

		for(uint16_t i=0;i<sizeof(gConfigurationDescriptor);i++)
			*des++ = *src++;

		sDes = (uint16_t*)&buf[2];
		configSize = *sDes;

		buf[7] = (1 << 7) | (gConfig->selfPowered << 6);
		buf[8] = gConfig->maxPower >> 1;

		if(request->wLength == sizeof(gConfigurationDescriptor))
		{
			gPeri->writeSetup(buf, sizeof(gConfigurationDescriptor), 1000);
		}
		else if(configSize <= request->wLength)
		{
			src = (uint8_t*)&gInterfaceDescriptor;
			for(uint16_t i=0;i<sizeof(gInterfaceDescriptor);i++)
				*des++ = *src++;

			src = (uint8_t*)&gOutEndpointDescriptor;
			for(uint16_t i=0;i<sizeof(gOutEndpointDescriptor);i++)
				*des++ = *src++;

			src = (uint8_t*)&gInEndpointDescriptor;
			for(uint16_t i=0;i<sizeof(gInEndpointDescriptor);i++)
				*des++ = *src++;

			gPeri->writeSetup(buf, configSize, 1000);
		}
		else
		{
			gPeri->stall(0);
		}
	}

	void writeStringDescriptor(config::usbd::SetupRequest *request)
	{
		uint8_t *src = 0;
		uint8_t buf[128];
		uint8_t *des = (uint8_t*)buf;
		uint16_t *sDes, len, size;

		switch(request->wValue)
		{
		case 0x0300 :
			*des++ = 0x04;
			*des++ = 0x03;
			*des++ = 0x09;
			*des++ = 0x04;
			gPeri->writeSetup(buf, 4, 1000);
			break;
		case 0x0301 :
			src = (uint8_t*)gConfig->manufacturer;
			break;
		case 0x0302 :
			src = (uint8_t*)gConfig->product;
			break;
		case 0x0303 :
			src = (uint8_t*)"000000000000";
			break;
		case 0x0304 :
			src = (uint8_t*)"MSC CONFIG";
			break;
		case 0x0305 :
			src = (uint8_t*)"MSC INTERFACE";
			break;
		default :
			gPeri->stall();
			break;
		}

		if(src)
		{
			len = strlen((const int8_t*)src);
			size = len * 2 + 2;
			*des++ = size;
			*des++ = 3;
			sDes = (uint16_t*)des;

			for(uint8_t i=0;i<len;i++)
				*sDes++ = (uint16_t)*src++;

			if(request->wLength < size)
				gPeri->writeSetup(buf, request->wLength, 1000);
			else
				gPeri->writeSetup(buf, size, 1000);
		}
	}

	void handleGetDescriptor(config::usbd::SetupRequest *request)
	{
//		debug_printf("wValue 0x%04x\n", request->wValue);

		switch(request->wValue)
		{
		case 0x0100 : // Device
			writeDeviceDescriptor(request);
			break;
		case 0x0200 : // Configuration
			writeDeviceConfiguration(request);
			break;
		case 0x0300 : // String language ID descriptor
		case 0x0301 : // String descriptor
		case 0x0302 : // String descriptor
		case 0x0303 : // String descriptor
		case 0x0304 : // String descriptor
		case 0x0305 : // String descriptor
			writeStringDescriptor(request);
			break;
		case 0x0600 : // Qualifier
		default :
			gPeri->stall();
			break;
		}
	}

	void trigger_handleSetup(config::usbd::SetupRequest *request)
	{
		uint8_t data = 0;

//		debug_printf("request 0x%04x\n", request->wRequest);

		switch(request->wRequest)
		{
		case 0x0680 : // Get Descriptor
			handleGetDescriptor(request);
			break;
		case 0x0500 : // Set Address
			gPeri->setAddress(request->wValue);
			gPeri->writeSetup(0, 0, 1000);
			break;
		case 0x0900 : // Set Configuration
			gPeri->activeOutEndpoint(1, define::usbd::endpointType::BULK, 64);
			gPeri->setOutEpDataReceive(1);
			gPeri->activeInEndpoint(2, define::usbd::endpointType::BULK, 64);
			gPeri->writeSetup(0, 0, 1000);
			break;
		case 0x0b01 : // Set Interface
			gPeri->writeSetup(0, 0, 1000);
			break;
		case 0xfea1 : // Set Interface
			data = 0;
			gPeri->writeSetup(&data, 1, 1000);
			break;
		default :
			gPeri->stall();
			break;
		}
	}

	uint8_t read(void)
	{
		int16_t data;
		drv::UsbOutData *outData = gPeri->getOutEndpointData(1);

		while(1)
		{
			data = outData->pop();

			if(data >= 0)
			{
//				debug_printf("data = 0x%02x\n", data);
				return (uint8_t)data;
			}
			else
				thread::yield();
		}
	}

	bool read(void *des, uint32_t size, uint32_t timeOut)
	{
		drv::UsbOutData *outData = gPeri->getOutEndpointData(1);
		return outData->pop(des, size, timeOut);
	}


	static uint8_t *sendBuf;
	static void thread_handleMessage(void)
	{
		CommandBlockWrapper cbw;
		CommandStatusWrapper csw;
		bool result = false;
		Inquiry *inquiry = (Inquiry*)cbw.CBWCB;
		Read10 *read10 = (Read10*)cbw.CBWCB;
		Write10 *write10 = (Write10*)cbw.CBWCB;
		ReadFormatCapacities *readFormatCapacities = (ReadFormatCapacities*)cbw.CBWCB;
		InquiryResponse *inquiryResponse;
		RequestSenseResponse *requestSenseResponse;
		ReadFormatCapacitiesResponse *readFormatCapacitiesResponse;
		uint16_t transferLen, allocationLength;
		uint32_t logicalBlockAddress, lTemp;
//		uint8_t *readBuffer = (uint8_t*)lmalloc(4096);
		uint8_t *readBuffer = new uint8_t[4096];
		uint32_t numOfBlock = gConfig->storage.getNumOfBlock();
		uint32_t blockSize = gConfig->storage.getBlockSize();

		for(uint16_t i=0;i<4096;i++)
			readBuffer[i] = i;

		if(sendBuf)
			delete sendBuf;

		sendBuf = new uint8_t[blockSize];

		uint8_t *cbwRcvPnt = (uint8_t*)&cbw;

		csw.dCSWSignature = 0x53425355;

		thread::delay(100);

		while(1)
		{
//			while(1)
//				thread::switchContext();
start:
			cbwRcvPnt = (uint8_t*)&cbw;

			for(uint8_t i=0;i<4;i++)
				*cbwRcvPnt++ = read();

			if(cbw.dCBWSignature != 0x43425355)
				goto start;

			for(uint8_t i=0;i<27;i++)
				*cbwRcvPnt++ = read();

//			debug_printf("0x%02x\n", cbw.CBWCB[0]);
			switch(cbw.CBWCB[0])
			{
			case OPCODE_INQUIRY :
				inquiryResponse = (InquiryResponse*)sendBuf;

				for(uint8_t i=0;i<sizeof(InquiryResponse);i++)
					sendBuf[i] = 0;

				inquiryResponse->peripheralQualifier = 0;
				inquiryResponse->peripheralDeviceType = 0x0;
				inquiryResponse->rmb = true;
				inquiryResponse->version = 0x02;
				inquiryResponse->aerc = false;
				inquiryResponse->normACA = false;
				inquiryResponse->hiSup = false;
				inquiryResponse->responseDataFormat = 2;
				inquiryResponse->additionalLength = 0x1f;
				inquiryResponse->sccs = false;
				inquiryResponse->bQue = false;
				inquiryResponse->encServ = false;
				inquiryResponse->multiP = false;
				inquiryResponse->mChngr = false;
				inquiryResponse->addr16 = false;
				inquiryResponse->relAdr = false;
				inquiryResponse->wbus16 = false;
				inquiryResponse->sync = false;
				inquiryResponse->linked = false;
				inquiryResponse->cmdQue = false;
				inquiryResponse->vendorIdentification[0] = 'Y';
				inquiryResponse->vendorIdentification[1] = 'S';
				inquiryResponse->vendorIdentification[2] = 'S';
				inquiryResponse->vendorIdentification[3] = ' ';
				inquiryResponse->vendorIdentification[4] = ' ';
				inquiryResponse->vendorIdentification[5] = ' ';
				inquiryResponse->vendorIdentification[6] = ' ';
				inquiryResponse->vendorIdentification[7] = ' ';
				inquiryResponse->productIdentification[0] = 'M';
				inquiryResponse->productIdentification[1] = 'A';
				inquiryResponse->productIdentification[2] = 'S';
				inquiryResponse->productIdentification[3] = 'S';
				inquiryResponse->productIdentification[4] = ' ';
				inquiryResponse->productIdentification[5] = 'S';
				inquiryResponse->productIdentification[6] = 'T';
				inquiryResponse->productIdentification[7] = 'O';
				inquiryResponse->productIdentification[8] = 'R';
				inquiryResponse->productIdentification[9] = 'A';
				inquiryResponse->productIdentification[10] = 'G';
				inquiryResponse->productIdentification[11] = 'E';
				inquiryResponse->productIdentification[12] = ' ';
				inquiryResponse->productIdentification[13] = ' ';
				inquiryResponse->productIdentification[14] = ' ';
				inquiryResponse->productIdentification[15] = ' ';
				inquiryResponse->productRevisionLevel[0] = '0';
				inquiryResponse->productRevisionLevel[1] = '.';
				inquiryResponse->productRevisionLevel[2] = '0';
				inquiryResponse->productRevisionLevel[3] = '1';
//				thread::delay(100);
				gPeri->write(2, sendBuf, sizeof(InquiryResponse), 1000);
				result = true;
				csw.dCSWDataResidue = 0;
				break;
			case OPCODE_REQUEST_SENSE :
				requestSenseResponse = (RequestSenseResponse*)sendBuf;

				for(uint8_t i=0;i<sizeof(requestSenseResponse);i++)
					sendBuf[i] = 0;

				requestSenseResponse->valid = 0;
				requestSenseResponse->responseCode = 0x70;
				requestSenseResponse->obsolate = 0;
				requestSenseResponse->filmrk = 0;
				requestSenseResponse->eom = 0;
				requestSenseResponse->ili = 0;
				requestSenseResponse->reserved1 = 0;
				requestSenseResponse->senseKey = 5;
				requestSenseResponse->information[0] = 0;
				requestSenseResponse->information[1] = 0;
				requestSenseResponse->information[2] = 0;
				requestSenseResponse->information[3] = 0;
				requestSenseResponse->additionalSenseLength = 0xc;
				requestSenseResponse->commandInformation[0] = 0;
				requestSenseResponse->commandInformation[1] = 0;
				requestSenseResponse->commandInformation[2] = 0;
				requestSenseResponse->commandInformation[3] = 0;
				requestSenseResponse->addtionalSenseCode = 0x21;
				requestSenseResponse->addtionalSenseCodeQualifier = 0;
				requestSenseResponse->fieldReplaceableUnitCode = 0;
				requestSenseResponse->sksv = 0;
				requestSenseResponse->reserved2 = 0;
				requestSenseResponse->senseKeySpecific = 0;
				requestSenseResponse->addtionalSenseBytes = 0;
				gPeri->write(2, sendBuf, sizeof(RequestSenseResponse), 1000);
				result = true;
				csw.dCSWDataResidue = 0;
				break;
			case OPCODE_TEST_UNIT_READY :
			case OPCODE_ALLOW_MEDIUM_REMOVAL :
			case OPCODE_START_STOP :
				result = true;
				csw.dCSWDataResidue = 0;
				break;
			case OPCODE_READ_CAPACITY10 :
				sendBuf[0] = (numOfBlock >> 24) & 0xff;
				sendBuf[1] = (numOfBlock >> 16) & 0xff;
				sendBuf[2] = (numOfBlock >> 8) & 0xff;
				sendBuf[3] = numOfBlock & 0xff;
				sendBuf[4] = (blockSize >> 24) & 0xff;
				sendBuf[5] = (blockSize >> 16) & 0xff;
				sendBuf[6] = (blockSize >> 8) & 0xff;
				sendBuf[7] = blockSize & 0xff;
				gPeri->write(2, sendBuf, 8, 1000);
				result = true;
				csw.dCSWDataResidue = 0;
				break;
			case OPCODE_MODE_SENSE6 :
				for(uint8_t i=0;i<8;i++)
					sendBuf[i] = 0;
				gPeri->write(2, sendBuf, 8, 1000);
				result = true;
				csw.dCSWDataResidue = 0xb8;
				break;
			case OPCODE_READ10 :
				transferLen = read10->transferLength[0] << 8;
				transferLen |= read10->transferLength[1];
				logicalBlockAddress = read10->logicalBlockAddress[0] << 24;
				logicalBlockAddress |= read10->logicalBlockAddress[1] << 16;
				logicalBlockAddress |= read10->logicalBlockAddress[2] << 8;
				logicalBlockAddress |= read10->logicalBlockAddress[3];

				for(uint16_t i=0;i<transferLen;i++)
				{
					bool rt = gConfig->storage.read(logicalBlockAddress++, sendBuf);
//					debug_printf("rt = %d\n", rt);
					gPeri->write(2, sendBuf, blockSize, 1000);
//					gPeri->write(2, readBuffer, gConfig->sizePerBlock, 1000);
				}
				result = true;
				csw.dCSWDataResidue = 0;
				break;
			case OPCODE_WRITE10 :
				transferLen = write10->transferLength[0] << 8;
				transferLen |= write10->transferLength[1];
				logicalBlockAddress = write10->logicalBlockAddress[0] << 24;
				logicalBlockAddress |= write10->logicalBlockAddress[1] << 16;
				logicalBlockAddress |= write10->logicalBlockAddress[2] << 8;
				logicalBlockAddress |= write10->logicalBlockAddress[3];

				for(uint16_t i=0;i<transferLen;i++)
				{
					read(sendBuf, blockSize, 3000);

					if(!gConfig->storage.write(logicalBlockAddress++, sendBuf))
					{
						result = false;
					}
					else
						result = true;
				}
				csw.dCSWDataResidue = 0;
				break;
			case OPCODE_READ_FORMAT_CAPACITIES :
				readFormatCapacitiesResponse = (ReadFormatCapacitiesResponse*)sendBuf;
//				allocationLength = readFormatCapacities->allocationLength[0] << 8;
//				allocationLength |= readFormatCapacities->allocationLength[1];

				for(uint8_t i=0;i<sizeof(ReadFormatCapacitiesResponse);i++)
					sendBuf[i] = 0;

				readFormatCapacitiesResponse->capacityListHeader.capacityListLength = 8;
				readFormatCapacitiesResponse->currentMaximumCapacityHeader.numOfBlocks[0] = (numOfBlock >> 24) & 0xff;
				readFormatCapacitiesResponse->currentMaximumCapacityHeader.numOfBlocks[1] = (numOfBlock >> 16) & 0xff;
				readFormatCapacitiesResponse->currentMaximumCapacityHeader.numOfBlocks[2] = (numOfBlock >> 8) & 0xff;
				readFormatCapacitiesResponse->currentMaximumCapacityHeader.numOfBlocks[3] = (numOfBlock) & 0xff;
				readFormatCapacitiesResponse->currentMaximumCapacityHeader.descriptorCode = 2;
				readFormatCapacitiesResponse->currentMaximumCapacityHeader.blockLength[0] = (blockSize >> 16) & 0xff;
				readFormatCapacitiesResponse->currentMaximumCapacityHeader.blockLength[1] = (blockSize >> 8) & 0xff;
				readFormatCapacitiesResponse->currentMaximumCapacityHeader.blockLength[2] = (blockSize) & 0xff;

				gPeri->write(2, sendBuf, sizeof(ReadFormatCapacitiesResponse), 1000);
				result = true;
				csw.dCSWDataResidue = 0;
				break;
			default :
//				debug_printf("0x%02x\n", cbw.CBWCB[0]);
				result = false;
				break;
			}

			csw.dCSWTag = cbw.dCBWTag;

			if(result)
			{
				csw.bCSWStatus = 0;
			}
			else
			{
				csw.bCSWStatus = 1;
			}

//			thread::delay(100);
			gPeri->write(2, (uint8_t*)&csw, sizeof(csw), 1000);
//			thread::delay(100);
		}

		while(1)
			thread::yield();
	}
}
}
}

#endif
*/