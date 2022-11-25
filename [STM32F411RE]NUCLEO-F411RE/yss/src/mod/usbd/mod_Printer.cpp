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
#include <mod/usbd/Printer.h>
#include <string.h>
#include <yss/malloc.h>

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
	0x07,	// uint8_t bInterfaceClass;
	0x01,	// uint8_t bInterfaceSubClass;
	0x02,	// uint8_t bInterfaceProtocol;
	0		// uint8_t iInterface;
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
	0,										// uint8_t bConfiguration;
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
namespace printer
{
	static void trigger_handleSetup(config::usbd::SetupRequest *request);

	drv::Usbd *gPeri;
	config::mod::usbd::printer::Config *gConfig;

	bool init(drv::Usbd &peri)
	{
		gPeri = &peri;
		gPeri->mallocOutEndpoint(1, 1024*1024);
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
		debug_printf("wValue 0x%04x\n", request->wValue);

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
}
}
}
#endif
*/
