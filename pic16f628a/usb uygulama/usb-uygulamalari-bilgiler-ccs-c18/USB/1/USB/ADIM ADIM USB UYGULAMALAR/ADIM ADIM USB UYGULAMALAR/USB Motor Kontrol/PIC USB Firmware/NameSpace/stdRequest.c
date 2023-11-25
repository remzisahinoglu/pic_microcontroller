/*****************************************************************
 *             U S B   H I D   U Y G U L A M A S I               * 
 *****************************************************************
 * Dosya Ismi:      stdRequest.c - Standart �stekler             *
 *															     *
 * Version:         1.00                                         *
 *																 *
 * Tarih:           06.05.2007                                   *
 *																 *
 * Author:          Ahmet ATAR								     *
 *****************************************************************
 * A��klama:        Bu dosyada windows taraf�ndan istenen stan-  *
 *                  dart istekler tespit edilir ve gerekli veri- *
 *                  ler tamponlara yerle�tirilir.                *
 *****************************************************************/
//I N C L U D E S 
#include "DSC.h"
#include "pubfunc.h"
#include "mem_alloc.h"
#include "usbdefs.h"
#include "usbmcr.h"

//� Z E L   F O N K S � Y O N   P R O T O T � P 
void GetDescriptorRequestHandler( void );
void SetConfigurationRequestHandler( void );
void GetStatusRequestHandler( void );
void SetFeatureRequestHandler( void );

/*****************************************************************
 * Fonksiyon Ismi:   CheckStandartRequest()                      *
 *																 *
 * Parametreler:     void										 *
 *																 *
 * D�nen De�er:      void										 *
 *																 *
 * A��klama:         Bu fonksiyon USB'nin 11 standart iste�in-   *
 *                   den sadece desteklediklerini ��z�mler ve    *
 *                   gerekli bilgileri tampona yerle�tiren fonk- *
 *                   siyonlar� �a��r�r.                          *
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
 * D�nen De�er:		 void										 *
 *															     *
 * A��klama:		 Bu fonksiyon windows'un Get_Descriptor is-  *
 *                   te�i sonras� �a�r�l�r ve hangi k�nye'nin    *
 *                   istendi�i tespit edilir.Daha sonra bu k�n-  *
 *                   yelerin bulundu�u tamponlar�n handle'lar�   *
 *                   HANDLE tipindeki de�i�kene al�n�r.T�m bil-  *
 *                   giler rom'da sakland���ndan transfe'de      *
 *					 ROM_TYPE kullan�l�r.                        *
 *****************************************************************/
void GetDescriptorRequestHandler( void )
{
	//0x80 : 7.bit [1]IN, 6-5.bitler [00]Standart �stek, 4-0.bitler [0000]�stek
	//do�rudan cihaz'a y�nelik
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
			     //Konfigrasyon K�nyesinden istenen k�nye index'e g�re belirleniyor.
			     //Index=0 Konfigrasyon, Index=1, Arabirim ... (dsc.c'ye bak�n)
				 hSource.brHandle = *(USBConfigDescPtr + SetupPacket.bDescriptorIndex);
				 iDataCount = *(hSource.wrHandle + 1);
				 break;
			case STRING:
			     bSessionOwner = SESSION_CNTRL;
			     //Konfigrasyon iste�inde oldu�u gibi.�stenen index'e g�re string k�nyeleri
			     //g�nderiliyor.
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
 * D�nen De�er:      void										 *
 *																 *
 * A��klama:         Bu fonksiyon windows Set_Configuration is-  *
 *                   �inde bulundu�unda �a�r�l�r.Windows ilk o-  *
 *                   larak Get_Configuration iste�i ile mevcut   *
 *                   konfigrasyon de�erini okur ve ard�ndan uy-  *
 *                   gunsa set eder.BU fonksiyonda bu de�eri     *
 *                   saklar.0 i�erikli bir konfigrasyon de�eri   *
 *                   cihaz�n adresli duruma ge�mesine yol a�ar.  *
 *                   Aksi halde cihaz kurulu duruma ge�er.       *
 *                   Bu durumda cihaz haberle�meye haz�r oldu�un-*
 *                   dan u�nokta HID'�n kulland��� u�nokta da    *
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
 * D�nen De�er:      void										 *
 * 																 *
 * A��klama:         Bu fonksiyon Get_Status iste�i �zerine �a�- *
 *                   r�l�r.Bu istekle PC cihazdan bir arabiri-   *
 *                   nim yada u�noktan�n status'unu ister.Index  *
 *                   al�n�n�n de�erine g�re Cihaz i�in 0, arabi- *
 *                   rim ve u�nokta i�in ise numaralar� dikkate  *
 *                   al�narak status bilgisi g�nderilir.Cihaz    *
 *                   �lk byte'�n 0.biti Self-Power'd�r.Bu bit 0  *
 *                   ise cihaz BUS'dan besleniyordur, 1 ise ken- *
 *                   di g�� kayna�� vard�r.1.bit ise Uzaktan u-  *
 *                   yand�rma'd�r.PC taraf�ndan daha �nce set e- *
 *                   dilmi� yada edilmemi�tir.U�nokta i�in sade- *
 *                   ce 0.bit tan�ml�d�r.Bu bit'in 1 olmas� HALT *
 *                   ko�ulunu g�sterir.                          *
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
		      * EndPoint0BdtOut'dan sonraki adresler EndPoint0BdtIn, EndPoint1BdtOut ... �eklinde gider.
		      * Her register 8 bit oldu�una g�re u�nokta numaras� 8 ile �arp�l�p EndPoint0BdtOut adresi �arp�ld���nda
		      * ilgili u�nokta buffer descriptor table'a gidilmi� olunur.U�nokta y�n� 4 ile �arp�l�p gidilen yerdeki adres ile
		      * toplan�rsa Buffer Descriptor Table'�n BSTALL bitine gidilir.
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
 * D�nen De�er:      void										 *
 *																 *
 * A��klama:         Bu fonksiyon Set_Feature iste�i geldi�inde  *
 *                   i�letilir.Windows cihazdan belirli bir �-   *
 *                   zelli�ini set etmesini ister.�ki tip �zel-  *
 *                   lik bulunur.Birincisi DEVICE_REMOTE_WAKEUP  *
 *                   ENDPOINT_HALT'd�r.                          *
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