/*****************************************************************
 *             U S B   H I D   U Y G U L A M A S I               * 
 *****************************************************************
 * Dosya Ismi:      usbhid.c - Hid istek ve i�lemleri            *
 *															     *
 * Version:         1.00                                         *
 *																 *
 * Tarih:           06.05.2007                                   *
 *																 *
 * Author:          Ahmet ATAR								     *
 *****************************************************************
 * A��klama:        Bu dosyada HID transferi i�in istek yan�tla- *
 *                  malar�n� i�eren fonksiyonlar ve hid g�nde    *
 *                  rim/al�m i�lemleri i�in fonksiyonlar tan�ml�-*
 *                  d�r.                                         *
 *****************************************************************/
//I N C L U D E S 
#include "DSC.h"
#include "pubfunc.h"
#include "mem_alloc.h"
#include "usbmcr.h"

//� Z E L   D E � � � K E N L E R 
unsigned char bIdleRate;
unsigned char bActiveProtocol;

//� Z E L   F O N K S � Y O N   P R O T O T I P 
void HidGetReportHandler( void );
void HidSetReportHandler( void );

/*****************************************************************
 * Fonksiyon Ismi:   HidInitEndpoint()                           *
 *																 *
 * Parametreler:     void										 *
 *																 *
 * D�nen De�er:      void										 *
 *																 *
 * A��klama:         Bu fonksiyon U�nokta1'i Hid Rapor Tamponla- *
 *                   r� i�in yap�land�r�r.                       *
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
 * Parametreler:     Tampona i�aret eden i�aret�i ve yaz�lacak   *
 *                   veri adedi                                  *
 *																 *
 * D�nen De�er:      void										 *
 *																 *
 * A��klama:         Bu fonksiyon buffer ile belirtilen adres-   *
 *                   ten itibaren Length kadar veriyi u�nokta1   *
 *                   den PC'ye g�nderir.                         *
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
 * Parametreler:     Veri tamponuna i�aret eden i�aret�i, ve o-  *
 *                   kunmak istenen veri adedi                   *
 *																 *
 * D�nen De�er:      Okunan veri adedi                           *
 *																 *
 * A��klama:         Bu fonksiyon Buffer ile i�aret edilen bel-  *
 *                   lek b�lgesine Length kadar veri okur.       *
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
 * D�nen De�er:      void										 *
 *																 *
 * A��klama:         Bu fonksiyon s�n�fa �zg� istekleri yan�tlar *
 *                   Kontrol tipi blok transferler ve di�er is-  *
 *                   tekler burdan y�r�t�l�r.                    *
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
 * D�nen De�er:      void										 *
 *																 *
 * A��klama:         Bu fonksiyon s�n�fa �zg� bilgi okuma iste�i *
 *                   Get_Report geldi�inde i�letilir ve PC'ye    *
 *                   HidReportIn tamponundaki verileri g�nderir. *
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
 * D�nen De�er:      void										 *
 *																 *
 * A��klama:         Bu fonksiyon s�n�fa �zg� bilgi g�nderme is- *
 *                   te�i Set_Report geldi�inde i�letilir ve PC  *
 *                   gelen verileri tampona yerle�tirir.         *
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
