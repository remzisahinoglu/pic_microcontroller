/*****************************************************************
 *             U S B   H I D   U Y G U L A M A S I               * 
 *****************************************************************
 * Dosya Ismi:      mem_alloc.h - USB Ram yerleþimi              *
 *																 *
 * Version:         1.00                                         *
 *																 *
 * Tarih:           06.05.2007                                   *
 *																 *
 * Author:          Ahmet ATAR								     *
 *****************************************************************
 * Açýklama:        Bu dosyada USB ram yerleþiminde kullanýlacak *
 *                  yapýlar tanýmlanmýþtýr.mem_alloc.c dosyasýn- *
 *                  da bu yapýlar kullanýlarak, usb için ayrýlan *
 *                  ram bölgesi 0x400 adresinden itibaren þekil- *
 *                  lendirilmiþtir.                              *
 *****************************************************************/
#ifndef MEM_ALLOC_H
#define MEM_ALLOC_H

//I N C L U D E S
#include "ep0_alloc.h"
#include "usbdefs.h"

//Cihaz özellikleri.Sadece uzaktan uyandýrma tanýmlýdýr.
typedef union _DEVICE_STATUS
{
	unsigned char DevStat;
	struct 
	{ 
		unsigned RemoteWakeup:1;
		unsigned HandleType:1;
	};
} DEVICE_STATUS, *PDEVICE_STATUS;
	
/*
 * Bu alan ile ilgili tanýmalamalar ve açýklamalar þekilleriyle
 * birlikte 18f4550 DataSheet'inin 173. sayfasýnda "Buffer Descriptors and
 * the Buffer Descriptor Table" bölümünde yer almaktadýr.
 */

//B U F F E R   D E S C R I P T O R   S T A T U S 
typedef union _BUFF_DESC_STAT
{
	unsigned char BdsArea;
	struct
	{
        unsigned BC8:1;                    //Byte count 8 (1024 byte veri için)
        unsigned BC9:1;                    //Byte count 9
        unsigned BSTALL:1;                 //Buffer stall enable bit
        unsigned DTSEN:1;                  //Veri deðiþim senkronizasyonu aktif bit
        unsigned INCDIS:1;                 //Adres arttýrma bit'i
        unsigned KEN:1;                    //Keep Enable bit
        unsigned DTS:1;                    //Veri deðiþim senkronizasyon bit
        unsigned UOWN:1;                   //USB Own bit
	};//Buffer Descriptor Status Register CPU Mod
	struct
	{
		unsigned :2;
		unsigned PID:4;
		unsigned :2;	
	};
} BUFF_DESC_STAT, *PBUFF_DESC_STAT;

//B U F F E R   D E S C R I P T O R   T A B L E 
typedef union _BUFF_DESC_TAB
{
	struct
	{
		BUFF_DESC_STAT BDnSTAT;            //Buffer descriptor status register
		unsigned char  BDnCNT;             //Buffer descriptor byte count register
		unsigned char  BDnADRL;            //Buffer adres low
		unsigned char  BDnADRH;            //Buffer adres high
	};
	struct
	{
		unsigned :8;
		unsigned :8;
		unsigned char *ADDR;
	};
} BUFF_DESC_TAB, *PBUFF_DESC_TAB;

//H A N D L E 
typedef union _HANDLE
{
	unsigned char *bHandle;
	unsigned int  *wHandle;

	rom unsigned char *brHandle;
	rom unsigned int  *wrHandle;
} HANDLE;

//************************************************************************
// E X T E R N S 
//************************************************************************
extern unsigned char bTransferState;
extern unsigned char bHidReportLength;
extern unsigned char bDeviceState;
extern unsigned char bActiveConfiguration;
extern unsigned char bAlternateInterface[MAX_INTERFACE_NUM];
extern unsigned char bSessionOwner;
extern unsigned int  iDataCount;
extern HANDLE hSource;
extern HANDLE hDestination;
extern DEVICE_STATUS UsbDeviceStatus;

extern volatile far BUFF_DESC_TAB   EndPoint0BdtOut;
extern volatile far BUFF_DESC_TAB	EndPoint0BdtIn;
extern volatile far BUFF_DESC_TAB	EndPoint1BdtOut;
extern volatile far BUFF_DESC_TAB	EndPoint1BdtIn;

extern volatile far SETUP_PACKET    SetupPacket;
extern volatile far DATA_PACKET		CntrlDataPacket;

extern volatile far unsigned char   HidReportOut[HID_OUT_ENDPT_SIZE];
extern volatile far unsigned char   HidReportIn[HID_IN_ENDPT_SIZE];
extern volatile far unsigned char   HidFeatureReport[ENDPOINT0_BUFFER_SIZE];

#endif