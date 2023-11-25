/*****************************************************************
 *             U S B   H I D   U Y G U L A M A S I               * 
 *****************************************************************
 * Dosya Ismi:      stdRequest.c - Standart Ýstekler             *
 *															     *
 * Version:         1.00                                         *
 *																 *
 * Tarih:           06.05.2007                                   *
 *																 *
 * Author:          Ahmet ATAR								     *
 *****************************************************************
 * Açýklama:        Bu dosyada windows tarafýndan istenen stan-  *
 *                  dart istekler tespit edilir ve gerekli veri- *
 *                  ler tamponlara yerleþtirilir.                *
 *****************************************************************/
//I N C L U D E S 
#include "DSC.h"
#include "pubfunc.h"
#include "mem_alloc.h"
#include "usbdefs.h"
#include "usbmcr.h"

//Ö Z E L   F O N K S Ý Y O N   P R O T O T Ý P 
void GetDescriptorRequestHandler( void );
void SetConfigurationRequestHandler( void );
void GetStatusRequestHandler( void );
void SetFeatureRequestHandler( void );

/*****************************************************************
 * Fonksiyon Ismi:   CheckStandartRequest()                      *
 *																 *
 * Parametreler:     void										 *
 *																 *
 * Dönen Deðer:      void										 *
 *																 *
 * Açýklama:         Bu fonksiyon USB'nin 11 standart isteðin-   *
 *                   den sadece desteklediklerini çözümler ve    *
 *                   gerekli bilgileri tampona yerleþtiren fonk- *
 *                   siyonlarý çaðýrýr.                          *
 *****************************************************************/
void CheckStandartRequest( void )
{
	if(SetupPacket.RequestType != STANDART) return;
	
	switch(SetupPacket.bRequest)
	{
		case SET_ADDRESS:
			 bSessionOwner = SESSION_CNTRL;
			 bDeviceState  = ADR_PENDIG;
			 break;
		case GET_DESCRIPTOR:
			 GetDescriptorRequestHandler();
			 break;
		case SET_CONFIGURATION:
			 SetConfigurationRequestHandler();
			 break;
		case GET_CONFIGURATION:
			 bSessionOwner = SESSION_CNTRL;
			 hSource.bHandle = (unsigned char*)&bActiveConfiguration;
			 UsbDeviceStatus.HandleType = RAM_TYPE;
			 iDataCount = 1;
			 break;
		case GET_STATUS:
			 GetStatusRequestHandler();
			 break;
		case SET_FEATURE:
		     bSessionOwner = SESSION_CNTRL;
			 SetFeatureRequestHandler();
			 break;
		case GET_INTERFACE:
			 bSessionOwner = SESSION_CNTRL;
			 hSource.bHandle = (unsigned char*)&bAlternateInterface + SetupPacket.bInterfaceIdLow;
			 UsbDeviceStatus.HandleType = RAM_TYPE;
			 iDataCount = 1;
			 break;
		case SET_INTERFACE:
			 bSessionOwner = SESSION_CNTRL;
			 bAlternateInterface[SetupPacket.bInterfaceIdLow] = SetupPacket.bAlternateIdLow;
			 break;
			 
		default:
			 break;
	}
}

/*****************************************************************
 * Fonksiyon Ismi:   GetDescriptorRequestHandler()               *
 * 																 *
 * Parametreler:     void										 *
 *																 *
 * Dönen Deðer:		 void										 *
 *															     *
 * Açýklama:		 Bu fonksiyon windows'un Get_Descriptor is-  *
 *                   teði sonrasý çaðrýlýr ve hangi künye'nin    *
 *                   istendiði tespit edilir.Daha sonra bu kün-  *
 *                   yelerin bulunduðu tamponlarýn handle'larý   *
 *                   HANDLE tipindeki deðiþkene alýnýr.Tüm bil-  *
 *                   giler rom'da saklandýðýndan transfe'de      *
 *					 ROM_TYPE kullanýlýr.                        *
 *****************************************************************/
void GetDescriptorRequestHandler( void )
{
	//0x80 : 7.bit [1]IN, 6-5.bitler [00]Standart Ýstek, 4-0.bitler [0000]Ýstek
	//doðrudan cihaz'a yönelik
	if(SetupPacket.bmRequestType == 0x80)
	{
		switch(SetupPacket.bDescriptorType)
		{
			case DEVICE:
				 bSessionOwner = SESSION_CNTRL;
				 hSource.brHandle = (rom unsigned char*)&DeviceDescriptor;
				 iDataCount = sizeof(DeviceDescriptor);
				 break;
			case CONFIGURATION:
			     bSessionOwner = SESSION_CNTRL;
			     //Konfigrasyon Künyesinden istenen künye index'e göre belirleniyor.
			     //Index=0 Konfigrasyon, Index=1, Arabirim ... (dsc.c'ye bakýn)
				 hSource.brHandle = *(USBConfigDescPtr + SetupPacket.bDescriptorIndex);
				 iDataCount = *(hSource.wrHandle + 1);
				 break;
			case STRING:
			     bSessionOwner = SESSION_CNTRL;
			     //Konfigrasyon isteðinde olduðu gibi.Ýstenen index'e göre string künyeleri
			     //gönderiliyor.
				 hSource.brHandle = *(USBStringDescPtr + SetupPacket.bDescriptorIndex);
				 iDataCount = *hSource.brHandle;
				 break;
				 
			default:
				 break;
		}
		UsbDeviceStatus.HandleType = ROM_TYPE;
	}
}

/*****************************************************************
 * Fonksiyon Ismi:   SetConfigurationRequestHandler()            *   
 *																 *
 * Parametreler:     void										 *
 *																 *
 * Dönen Deðer:      void										 *
 *																 *
 * Açýklama:         Bu fonksiyon windows Set_Configuration is-  *
 *                   ðinde bulunduðunda çaðrýlýr.Windows ilk o-  *
 *                   larak Get_Configuration isteði ile mevcut   *
 *                   konfigrasyon deðerini okur ve ardýndan uy-  *
 *                   gunsa set eder.BU fonksiyonda bu deðeri     *
 *                   saklar.0 içerikli bir konfigrasyon deðeri   *
 *                   cihazýn adresli duruma geçmesine yol açar.  *
 *                   Aksi halde cihaz kurulu duruma geçer.       *
 *                   Bu durumda cihaz haberleþmeye hazýr olduðun-*
 *                   dan uçnokta HID'ýn kullandýðý uçnokta da    *
 *                   kurulur.                                    *
 *****************************************************************/
void SetConfigurationRequestHandler( void )
{
	bSessionOwner = SESSION_CNTRL;
	
	mDisableAllEndPoint();
	FlushBuffer((unsigned char*)&bAlternateInterface, MAX_INTERFACE_NUM);
	bActiveConfiguration = SetupPacket.bConfigValue;
	if(bActiveConfiguration == 0) bDeviceState = ADDRESS; else bDeviceState = CONFIGURED;
	HidInitEndpoint();
}

/*****************************************************************
 * Fonksiyon Ismi:   GetStatusRequestHandler()                   *
 *																 *
 * Parametreler:     void										 *
 *																 *
 * Dönen Deðer:      void										 *
 * 																 *
 * Açýklama:         Bu fonksiyon Get_Status isteði üzerine çað- *
 *                   rýlýr.Bu istekle PC cihazdan bir arabiri-   *
 *                   nim yada uçnoktanýn status'unu ister.Index  *
 *                   alýnýnýn deðerine göre Cihaz için 0, arabi- *
 *                   rim ve uçnokta için ise numaralarý dikkate  *
 *                   alýnarak status bilgisi gönderilir.Cihaz    *
 *                   Ýlk byte'ýn 0.biti Self-Power'dýr.Bu bit 0  *
 *                   ise cihaz BUS'dan besleniyordur, 1 ise ken- *
 *                   di güç kaynaðý vardýr.1.bit ise Uzaktan u-  *
 *                   yandýrma'dýr.PC tarafýndan daha önce set e- *
 *                   dilmiþ yada edilmemiþtir.Uçnokta için sade- *
 *                   ce 0.bit tanýmlýdýr.Bu bit'in 1 olmasý HALT *
 *                   koþulunu gösterir.                          *
 *****************************************************************/
void GetStatusRequestHandler( void )
{
	CntrlDataPacket.Data0 = CLEAR;
	CntrlDataPacket.Data1 = CLEAR;
	
	switch(SetupPacket.Recipient)
	{
		case RCPT_DEVICE:
		     bSessionOwner = SESSION_CNTRL;
			 if(UsbDeviceStatus.RemoteWakeup == true) CntrlDataPacket.Data0 |= 0x02;
			 break;
		case RCPT_INTERFACE:
			 bSessionOwner = SESSION_CNTRL;
			 break;
		case RCPT_ENDPOINT:
		     bSessionOwner = SESSION_CNTRL;
		     /*
		      * EndPoint0BdtOut'dan sonraki adresler EndPoint0BdtIn, EndPoint1BdtOut ... þeklinde gider.
		      * Her register 8 bit olduðuna göre uçnokta numarasý 8 ile çarpýlýp EndPoint0BdtOut adresi çarpýldýðýnda
		      * ilgili uçnokta buffer descriptor table'a gidilmiþ olunur.Uçnokta yönü 4 ile çarpýlýp gidilen yerdeki adres ile
		      * toplanýrsa Buffer Descriptor Table'ýn BSTALL bitine gidilir.
			  */
			 hDestination.bHandle = (unsigned char*)&EndPoint0BdtOut + (SetupPacket.EndPointNum * 8) + (SetupPacket.EndPointDir * 4);
			 if(*hDestination.bHandle & _BSTALL) CntrlDataPacket.Data0 = 0x01;
			 break;
	}
	if(bSessionOwner == SESSION_CNTRL)
	{
		hSource.bHandle = (unsigned char*)&CntrlDataPacket;
		UsbDeviceStatus.HandleType = RAM_TYPE;
		iDataCount = 2;
	}
}

/*****************************************************************
 * Fonksiyon Ismi:   SetFeatureRequestHandler()                  * 
 *																 *
 * Parametreler:     void										 *
 *																 *
 * Dönen Deðer:      void										 *
 *																 *
 * Açýklama:         Bu fonksiyon Set_Feature isteði geldiðinde  *
 *                   iþletilir.Windows cihazdan belirli bir ö-   *
 *                   zelliðini set etmesini ister.Ýki tip özel-  *
 *                   lik bulunur.Birincisi DEVICE_REMOTE_WAKEUP  *
 *                   ENDPOINT_HALT'dýr.                          *
 *****************************************************************/
void SetFeatureRequestHandler( void )
{
	if((SetupPacket.bFeature == DEVICE_REMOTE_WAKEUP) && (SetupPacket.Recipient == RCPT_DEVICE))
	{
		bSessionOwner = SESSION_CNTRL;
		UsbDeviceStatus.RemoteWakeup = true;
	}
		
	if((SetupPacket.bFeature == ENDPOINT_HALT) && (SetupPacket.Recipient == RCPT_ENDPOINT) && (SetupPacket.EndPointNum != 0))
	{
		bSessionOwner = SESSION_CNTRL;
		hDestination.bHandle=(unsigned char*)&EndPoint0BdtOut + (SetupPacket.EndPointNum * 8) + (SetupPacket.EndPointDir * 4);
		*hDestination.bHandle = _USIE|_BSTALL;
	}
}