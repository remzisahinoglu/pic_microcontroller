/*****************************************************************
 *             U S B   H I D   U Y G U L A M A S I               * 
 *****************************************************************
 * Dosya Ismi:      usbevents.h - USB olaylar�                   *
 *                                                               *
 * Version:         1.00                                         *
 *                                                               *
 * Tarih:           06.05.2007                                   *
 *                                                               *
 * Author:          Ahmet ATAR								     *
 *****************************************************************
 * A��klama:        Bu dosyada USB haberle�mesi esnas�nda olu�an *
 *                  olaylar(kesmeler)denetlenmektedir.Bu olu�an  *
 *                  olaylara g�re ilgili fonksiyonlar i�letilir. *
 *****************************************************************/
//I N C L U D E S 
#include <p18cxxx.h>
#include "usbmcr.h"
#include "pubfunc.h"

//� Z E L   F O N K S � Y O N    P R O T O T � P
void ResetEventHandler( void );
void StallEventHandler( void );
void ErrorEventHandler( void );
void SuspendEventHandler( void );
void USBModuleControl(bool enable);
void WakeUpRemoteEventHandler( void );
void StartOfFrameEventHandler( void );
void WakeUpSuspendEventHandler( void );

/*****************************************************************
 * Fonksiyon Ismi:   USBCheckBus()                               *
 *                                                               *
 * Parametreler:     void                                        *
 *                                                               *
 * D�nen De�er:      void                                        *
 *                                                               *
 * A��klama:         Bu fonksiyon cihaz'�n USB Bus'�na ba�l� o-  *
 *                   lup olmad���n� denetler.E�er ba�l� ise USB  *
 *                   mod�l� aktif eder ve reset kesmesini kurar. *
 *****************************************************************/
void USBCheckBus( void )
{
	//Cihaz BUS'a tak�l� ise USB_ATTACH_REMOVE_PIN'de set olur.
	//��nk� bu bit do�rudan VBus'a ba�lanm�� olur.
	if(USB_ATTACH_REMOVE_PIN == BUS_ATTACHED)
	{
		if(!UCONbits.USBEN) USBModuleControl(true);
	}
	else
	{
		if(UCONbits.USBEN)  USBModuleControl(false);
	}
	
	if(bDeviceState == ATTACHED)
	{
		if(!UCONbits.SE0)
		{
			UIE = CLEAR;
			UIR = CLEAR;
			UIEbits.IDLEIE = true;
			UIEbits.URSTIE = true;
			bDeviceState = POWERED;
		}
	}
}

/*****************************************************************
 * Fonksiyon Ismi:   USBService()                                *
 *           													 *
 * Parametreler:     void                                        *
 *																 *
 * D�nen De�er:      void                                        *
 * 																 *
 * A��klama:         Bu fonksiyon USB haberle�mesinde olu�an kes-*
 *                   me bitlerinin durumunu s�rekli takip eder.  *
 *                   �lgili olaylar olu�tu�unda ise gerekli fonk-*
 *                   siyonlar� i�letir.                          *
 *****************************************************************/
void USBService( void )
{
	//E�er cihaz bus'a ba�l� de�il ise geri d�n
	if(bDeviceState == DETACHED) return;
	
	//Bus'da faliyet ba�lad��� zaman ACTVIF biti set olur.Bu durumda
	//cihaz suspend durumda ise ��kmas� gerekir.
	if(isActive()) WakeUpSuspendEventHandler();
	
	//Cihaz suspend modundaysa hi�bir i�lem yapma ve fonksiyondan ��k
	if(UCONbits.SUSPND) return;
	
	/*
	 * Reset durumu alg�land���nda e�er cihaz suspend modda ise ilk olarak
	 * ACTVIF biti set olur.Bu durumda cihaz ilk olarak suspend moddan ��kart�lmal�d�r.
	 * Daha sonra URSTIF biti set olur.
	 */
	if(isReset())   ResetEventHandler();
	
	//Di�er kesmeler
	if(isSuspend()) SuspendEventHandler();
	if(isSof())     StartOfFrameEventHandler();
	if(isStall())   StallEventHandler();
	if(isError())   ErrorEventHandler();
	
	/*
	 * Bir i�lem en az �� evreden olu�ur.Windows taraf�ndan ilk olarak bir jeton paketi
	 * g�nderilir.Ard�ndan bir veri paketi g�nderir.Al�c� taraf cihaz oldu�unda PC'ye bir 
	 * ACK g�nderilir ve TRNIF biti set olur.SIE gelen veriyi 0x400 adresinden itibaren 
	 * yerle�tirmi�tir.
	 */ 
	if(isComplate())
	{
		EndPointService();
		UIRbits.TRNIF = false;
	}
}

/*****************************************************************
 * Fonksiyon Ismi:  USBModuleControl                             *
 *																 *
 * Parametreler:    Mod�l� aktif/pasif yapacak bool de�eri       *
 *																 *
 * D�nen De�er:     void                                         *
 *																 *
 * A��klama:        Bu fonksiyon enable parametresine arg�man o- *
 *                  larak akt�ralacak de�er true ise USB mod�l�  *
 *                  aktif yapar, aksi halde kapat�r.             *
 *****************************************************************/
void USBModuleControl(bool enable)
{
	if(enable)
	{
		UIE  = CLEAR;
		UCON = CLEAR;
		UCONbits.USBEN = true;
		bDeviceState = ATTACHED;
	}
	else
	{
		UIE  = CLEAR;
		UCON = CLEAR;
		bDeviceState = DETACHED;
	}
}

/*****************************************************************
 * Fonksiyon Ismi:   ResetEventHandler()                         *
 *																 *
 * Parametreler:     void                                        *
 *																 * 
 * D�nen De�er:      void                                        *
 *																 *
 * A��klama:         Bu fonksiyon windows taraf�ndan D- ve D+    *
 *                   hatlar� reset durumuna getirildi�inde �a�-  *
 *                   r�l�r.T�m hata bitleri kurulur.Ayn� zaman-  *
 *                   da ACTVIE biti hari� di�er t�m kesmeler ku- *
 *                   rulur.T�m u�noktalar pasif yap�l�r ve sade- *
 *                   kontrol transferi i�in u�nokta0 kurulur.    *
 *                   AYGIT DURUMU ve konfigrasyon gibi ayarlar   *
 *                   resetlenir ve cihaz DEFAULT duruma kurulur. *
 *****************************************************************/
void ResetEventHandler( void )
{
	UIR	 = CLEAR;
	UEIR = CLEAR;
	UIE  = _SOFIE|_STALLIE|_IDLEIE|_TRNIE|_UERRIE|_URSTIE;   //USB kesmelerini kur
	UEIE = _BTSEE|_BTOEE|_DFN8EE|_CRC16EE|_CRC5EE|_PIDEE;    //USB hata kesmelerini kur
	
	UADDR = CLEAR;                                           //Reset sonras� varsay�lan adres 0
	mDisableAllEndPoint();                                   //T�m u�noktalar� pasif yapan makro
	UEP0 = EP_CTRL|HSHK_EN;                                  //U�nokta0 kontrol u�noktas� ve el s�k��ma aktif
	/*
	 * Paket al�m� aktif.Bir paket al�n�p ACK g�nderildikten sonra 
	 * paket i�eri�inin tutuldu�u tampona, �st�ne yaz�lmamas� i�in yeni paket al�m�na 
	 * izin verilmez.Dolas�yla PKTDIS biti otomatik olarak set olur.Her paket al�m� ard�ndan
	 * yeni paketin al�nmas� i�in bu bitin temizlenmesi gerekir.
	 */
	UCONbits.PKTDIS = false;                                 //Paket al�m� aktif
	USBInitNextSetupTrf();                                   //U�noktay� setup i�lemi i�in haz�rla
	UsbDeviceStatus.RemoteWakeup = false;                    //Cihaz �zelliklerini resetle
	bActiveConfiguration = CLEAR;
	bDeviceState = DEFAULT;
}

/*****************************************************************
 * Fonksiyon Ismi:   StallEventHandler()                         *
 *																 *
 * Parametreler:     void                                        *
 *																 *
 * D�nen De�er:      void                                        *
 *																 *
 * A��klama:         Bu fonksiyon u�nokta0 bir istek sonras�     *
 *                   stall d�nd�rd���nde �a�r�l�r.U�nokta0 des-  *
 *                   teklenmeyen bir istek ald��� yada u�nokta   *
 *                   �ak�lmas�(endpoint halted) gibi durumlarda  *
 *                   STALL ile yan�t verebilir.                  *
 *                   Bu olaydan sonra stall biti ve kesme biti   *
 *                   temizlenir ve u�nokta0 bir sonraki i�leme   *
 *                   haz�rlan�r.                                 *
 *****************************************************************/
void StallEventHandler( void )
{
	if(UEP0bits.EPSTALL)
	{
		USBInitNextSetupTrf;
		UEP0bits.EPSTALL = false;
	}
	UIRbits.STALLIF = false;
}

/*****************************************************************
 * Fonksiyon Ismi:   ErrorEventHandler()                         *
 *																 *
 * Parametreler:     void                                        *
 *															     *
 * D�nen De�er:      void                                        *
 *																 *
 * A��klama:         Bu fonksiyon bir hata olu�tu�unda �a�r�l�r  *
 *                   ve sadece hata kesme bayra��n� temizler.    *
 *****************************************************************/
void ErrorEventHandler( void )
{
	UIRbits.UERRIF = false;
}

/*****************************************************************
 * Fonksiyon Ismi:   WakeUpEventHandler()                        *
 *                                                               *
 * Parametreler:     void                                        *
 *															     *
 * D�nen De�er:      void                                        *
 *																 *
 * A��klama:         Bu fonksiyon Bus aktif olduktan sonra i�le- *
 *                   tilir ve cihaz suspend modundan ��kart�l�r. *
 *****************************************************************/
void WakeUpSuspendEventHandler( void )
{
	UCONbits.SUSPND	= false;       //SIE clock aktif
	UIEbits.ACTVIE  = false;       //Bus aktif kesmesi pasif
	UIRbits.ACTVIF	= false;       //Bus aktif kesme bayra��n� temizle
}

/*****************************************************************
 * Fonksiyon Ismi:   SuspendEventHandler()                       * 
 * 																 *   
 * Parametreler:     void                                        *
 *																 *
 * D�nen De�er:      void                                        *
 *																 *
 * A��klama:         Cihaz BUS'da 3 milisaniye i�lem g�rmedi�i   *
 *                   zaman IDLEIF biti set olur.Ard�ndan bu fonk-*
 *                   siyon �a�r�l�r ve ACTVIE biti set edilerek  *
 *                   BUS aktif kesmesi kurulur.Bus'da tekrar fa- *
 *                   liyet ba�lad���nda bu bit ACTVIF biti set   *
 *                   olur.Son olarak UCON register'�n�n SUSPND   *
 *                   biti set edilir ve SIE'nin saat kayna�� pa- *
 *                   sif yap�l�r.                                *
 *****************************************************************/
void SuspendEventHandler( void )
{
	UIEbits.ACTVIE  = true;      //Bus aktif kesmesini kur
	UIRbits.IDLEIF  = false;     //Aylak bitini temizle
	UCONbits.SUSPND = true;      //SIE pasif yap�ld� ve cihaz suspend mod'da
}

/*****************************************************************
 * Fonksiyon Ismi:   StartOfFrameEventHandler()                  *
 *																 *
 * Parametreler:     void                                        *
 *																 *
 * D�nen De�er:      void                                        *
 *																 *
 * A��klama:         Bu fonksiyon i�lem ba�lang�c�ndan SOF al�n- *
 *                   d���nda �a�r�l�r ve sadece bayrak bitini    *
 *                   temizler.                                   *
 *****************************************************************/
void StartOfFrameEventHandler( void )
{
	UIRbits.SOFIF = false;
}

/*****************************************************************
 * Fonksiyon Ismi:   WakeUpRemoteEventHandler()                  *
 *																 *
 * Parametreler:     void                                        *
 *																 *
 * D�nen De�er:      void                                        *
 *																 *
 * A��klama:         Bu fonksiyon e�er cihaz�n uzaktan uyand�rma *
 *                   �zelli�i aktif edildiyse kullan�labilir.    *
 *                   Ask� durumundaki bir cihaz Bus'da faliyet   *
 *                   ba�latmak i�in, yani haberle�meyi tekrar    *
 *                   canland�rmak i�in Rresume sinyali g�ndere-  *
 *                   bilir.Spec. 2.0'da bu s�re 1-15ms olarak    *
 *                   belirtilmektedir.                           *
 *****************************************************************/
void WakeUpRemoteEventHandler( void )
{
	static unsigned int iResCount;
	
	//E�er uzaktan uyand�rma �zelli�i aktif ise 
	//resume sinyali g�nderilebilir.
	if(UsbDeviceStatus.RemoteWakeup == true)
	{
		WakeUpSuspendEventHandler();
		UCONbits.RESUME = true;          //Resume sinyali g�ndermeyi ba�lat
		
		iResCount = 1800U;               //1-13ms aras�
		mWaitResumeSignal(iResCount);
		UCONbits.RESUME = false;         //Resume sinyali g�ndermeyi bitir.
	}
}

/*****************************************************************
 * Fonksiyon Ismi:   FlushBuffer()                               *
 *																 *
 * Parametreler:     Buffer start adresi ve uzunluk              *
 *																 *
 * D�nen De�er:      void                                        *
 *																 *
 * A��klama:         Bu fonksiyon ba�lang�� adresi verilen adres-*
 *                   ten itibaren uzunluk kadar alan� temizler.  *
 *****************************************************************/
void FlushBuffer(unsigned char *buffer, unsigned char length)
{
	while(length)
	{
		*buffer = CLEAR;
		buffer++;
		length--;
	}
}