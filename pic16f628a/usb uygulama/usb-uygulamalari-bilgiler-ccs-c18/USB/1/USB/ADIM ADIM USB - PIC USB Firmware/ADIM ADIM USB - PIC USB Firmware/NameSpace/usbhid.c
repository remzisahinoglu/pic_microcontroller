/*****************************************************************
 *             U S B   H I D   U Y G U L A M A S I               * 
 *****************************************************************
 * Dosya Ismi:      usbhid.c - Hid istek ve iþlemleri            *
 *															     *
 * Version:         1.00                                         *
 *																 *
 * Tarih:           06.05.2007                                   *
 *																 *
 * Author:          Ahmet ATAR								     *
 *****************************************************************
 * Açýklama:        Bu dosyada HID transferi için istek yanýtla- *
 *                  malarýný içeren fonksiyonlar ve hid gönde    *
 *                  rim/alým iþlemleri için fonksiyonlar tanýmlý-*
 *                  dýr.                                         *
 *****************************************************************/
//I N C L U D E S 
#include "DSC.h"
#include "pubfunc.h"
#include "mem_alloc.h"
#include "usbmcr.h"

//Ö Z E L   D E Ð Ý Þ K E N L E R 
unsigned char bIdleRate;
unsigned char bActiveProtocol;

//Ö Z E L   F O N K S Ý Y O N   P R O T O T I P 
void HidGetReportHandler( void );
void HidSetReportHandler( void );

/*****************************************************************
 * Fonksiyon Ismi:   HidInitEndpoint()                           *
 *																 *
 * Parametreler:     void										 *
 *																 *
 * Dönen Deðer:      void										 *
 *																 *
 * Açýklama:         Bu fonksiyon Uçnokta1'i Hid Rapor Tamponla- *
 *                   rý için yapýlandýrýr.                       *
 *****************************************************************/
void HidInitEndpoint( void )
{
	bHidReportLength = 0;
	UEP1 = EP_OUT_IN|HSHK_EN;
	
	EndPoint1BdtOut.BDnCNT  = sizeof(HidReportOut);
	EndPoint1BdtOut.ADDR    = (unsigned char*)&HidReportOut;
	EndPoint1BdtOut.BDnSTAT.BdsArea = _USIE|_DAT0|_DTSEN;
	
	EndPoint1BdtIn.ADDR = (unsigned char*)&HidReportIn;
	EndPoint1BdtIn.BDnSTAT.BdsArea = _UCPU|_DAT1;
	
	UCONbits.PKTDIS = 0;
}

/*****************************************************************
 * Fonksiyon Ismi:   HidTransferWriteService()                   *
 *																 *
 * Parametreler:     Tampona iþaret eden iþaretçi ve yazýlacak   *
 *                   veri adedi                                  *
 *																 *
 * Dönen Deðer:      void										 *
 *																 *
 * Açýklama:         Bu fonksiyon buffer ile belirtilen adres-   *
 *                   ten itibaren Length kadar veriyi uçnokta1   *
 *                   den PC'ye gönderir.                         *
 *****************************************************************/
void HidTransferWriteService(char *buffer, unsigned char Length)
{
	unsigned char i;
	
	if(Length > HID_IN_ENDPT_SIZE) Length = HID_IN_ENDPT_SIZE;
	for(i = 0; i < Length; i++) HidReportIn[i] = buffer[i];
	
	EndPoint1BdtIn.BDnCNT = Length;
	mInitEndPoint(EndPoint1BdtIn);
}

/*****************************************************************
 * Fonksiyon Ismi:   HidTransferReadService()                    *
 *																 *
 * Parametreler:     Veri tamponuna iþaret eden iþaretçi, ve o-  *
 *                   kunmak istenen veri adedi                   *
 *																 *
 * Dönen Deðer:      Okunan veri adedi                           *
 *																 *
 * Açýklama:         Bu fonksiyon Buffer ile iþaret edilen bel-  *
 *                   lek bölgesine Length kadar veri okur.       *
 *****************************************************************/
unsigned char HidTransferReadService(char *buffer, unsigned char Length)
{
	bHidReportLength = CLEAR;
	
	if(!EndPoint1BdtOut.BDnSTAT.UOWN)
	{
		if(Length > EndPoint1BdtOut.BDnCNT) Length = EndPoint1BdtOut.BDnCNT;
		for(bHidReportLength = 0; bHidReportLength < Length; bHidReportLength++)
			buffer[bHidReportLength] = HidReportOut[bHidReportLength];
			
		EndPoint1BdtOut.BDnCNT = sizeof(HidReportOut);
		mInitEndPoint(EndPoint1BdtOut);
	}
	
	return bHidReportLength;
}

/*****************************************************************
 * Fonksiyon Ismi:   CheckHidRequest()                           *
 *																 *
 * Parametreler:     void										 *
 *																 *
 * Dönen Deðer:      void										 *
 *																 *
 * Açýklama:         Bu fonksiyon sýnýfa özgü istekleri yanýtlar *
 *                   Kontrol tipi blok transferler ve diðer is-  *
 *                   tekler burdan yürütülür.                    *
 *****************************************************************/
void CheckHidRequest( void )
{	
	if(SetupPacket.Recipient != RCPT_INTERFACE) return;
	if(SetupPacket.bInterfaceIdLow != HID_INTERFACE_ID) return;
	
	if(SetupPacket.bRequest == GET_DESCRIPTOR)
	{
		switch(SetupPacket.bDescriptorType)
		{
			case HID_DESC:
				 bSessionOwner = SESSION_HID;
				 mGetHidDscAdr(hSource.brHandle);
				 iDataCount = sizeof(HID_DESCRIPTOR);
				 break;
			case REPORT_DESC:
				 bSessionOwner = SESSION_HID;
				 mGetHidReportDscAdr(hSource.brHandle);
				 mGetHidReportDescriptorSize(iDataCount);
				 break;
		}
		UsbDeviceStatus.HandleType = ROM_TYPE;
	}
	
	if(SetupPacket.RequestType != CLASS) return;
	switch(SetupPacket.bRequest)
	{
		case GET_REPORT:
			 HidGetReportHandler();
			 break;
		case SET_REPORT:
			 HidSetReportHandler();
			 break;
		case GET_IDLE:
			 bSessionOwner = SESSION_HID;
			 hSource.bHandle = (unsigned char*)&bIdleRate;
			 UsbDeviceStatus.HandleType = RAM_TYPE;
			 iDataCount = 1;
			 break;
		case SET_IDLE:
			 bSessionOwner = SESSION_HID;
			 bIdleRate = MSB(SetupPacket.wValue);
			 break;
		case GET_PROTOCOL:
			 bSessionOwner = SESSION_HID;
			 hSource.bHandle = (unsigned char*)&bActiveProtocol;
			 UsbDeviceStatus.HandleType = RAM_TYPE;
			 iDataCount = 1;
			 break;
		case SET_PROTOCOL:
			 bSessionOwner = SESSION_HID;
			 bActiveProtocol = LSB(SetupPacket.wValue);
			 break;
		default:
			 break;
	}
}

/*****************************************************************
 * Fonksiyon Ismi:   HidGetReportHandler()                       *
 *																 *
 * Parametreler:     void										 *
 *																 *
 * Dönen Deðer:      void										 *
 *																 *
 * Açýklama:         Bu fonksiyon sýnýfa özgü bilgi okuma isteði *
 *                   Get_Report geldiðinde iþletilir ve PC'ye    *
 *                   HidReportIn tamponundaki verileri gönderir. *
 *****************************************************************/
void HidGetReportHandler( void )
{
	unsigned char wValueLsb, wValueMsb;
	unsigned char bDataCount;
	
	wValueLsb = LSB(SetupPacket.wValue);
	wValueMsb = MSB(SetupPacket.wValue);
	
	switch(wValueMsb)
	{
		case INPUT_REPORT:
			 if(wValueLsb == 0)
			 {
				 bSessionOwner = SESSION_HID;
			 	 hSource.bHandle = (unsigned char*)&HidReportIn;
			 	 iDataCount = HID_IN_ENDPT_SIZE;
			 }
			 break;
		case FEATURE_REPORT:
			 if(wValueLsb == 0)
			 {
				 bSessionOwner = SESSION_HID;
				 hSource.bHandle = (unsigned char*)&HidFeatureReport;
				 iDataCount = ENDPOINT0_BUFFER_SIZE;
			 }
			 break;
	}
	UsbDeviceStatus.HandleType = RAM_TYPE;
}

/*****************************************************************
 * Fonksiyon Ismi:   HidSetReportHandler()                       *  
 *																 *
 * Parametreler:     void										 *
 *																 *
 * Dönen Deðer:      void										 *
 *																 *
 * Açýklama:         Bu fonksiyon sýnýfa özgü bilgi gönderme is- *
 *                   teði Set_Report geldiðinde iþletilir ve PC  *
 *                   gelen verileri tampona yerleþtirir.         *
 *****************************************************************/
void HidSetReportHandler( void )
{
	unsigned char wValueLsb, wValueMsb;
	
	wValueLsb = LSB(SetupPacket.wValue);
	wValueMsb = MSB(SetupPacket.wValue);
	
	switch(wValueMsb)
	{
		case OUTPUT_REPORT:
			 if(wValueLsb == 0)
			 {
				 bSessionOwner = SESSION_HID;
			 	 hDestination.bHandle = (unsigned char*)&HidReportOut; 
			 	 iDataCount = HID_OUT_ENDPT_SIZE;
			 }
			 break;
		case FEATURE_REPORT:
			 if(wValueLsb == 0)
			 {
				 bSessionOwner = SESSION_HID;
				 hDestination.bHandle = (unsigned char*)&HidFeatureReport;
			 }
			 break;
	}
}
