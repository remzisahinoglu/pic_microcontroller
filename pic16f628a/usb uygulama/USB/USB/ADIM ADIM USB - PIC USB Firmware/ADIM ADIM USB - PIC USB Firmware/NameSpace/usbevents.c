/*****************************************************************
 *             U S B   H I D   U Y G U L A M A S I               * 
 *****************************************************************
 * Dosya Ismi:      usbevents.h - USB olaylarý                   *
 *                                                               *
 * Version:         1.00                                         *
 *                                                               *
 * Tarih:           06.05.2007                                   *
 *                                                               *
 * Author:          Ahmet ATAR								     *
 *****************************************************************
 * Açýklama:        Bu dosyada USB haberleþmesi esnasýnda oluþan *
 *                  olaylar(kesmeler)denetlenmektedir.Bu oluþan  *
 *                  olaylara göre ilgili fonksiyonlar iþletilir. *
 *****************************************************************/
//I N C L U D E S 
#include <p18cxxx.h>
#include "usbmcr.h"
#include "pubfunc.h"

//Ö Z E L   F O N K S Ý Y O N    P R O T O T Ý P
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
 * Dönen Deðer:      void                                        *
 *                                                               *
 * Açýklama:         Bu fonksiyon cihaz'ýn USB Bus'ýna baðlý o-  *
 *                   lup olmadýðýný denetler.Eðer baðlý ise USB  *
 *                   modülü aktif eder ve reset kesmesini kurar. *
 *****************************************************************/
void USBCheckBus( void )
{
	//Cihaz BUS'a takýlý ise USB_ATTACH_REMOVE_PIN'de set olur.
	//Çünkü bu bit doðrudan VBus'a baðlanmýþ olur.
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
 * Dönen Deðer:      void                                        *
 * 																 *
 * Açýklama:         Bu fonksiyon USB haberleþmesinde oluþan kes-*
 *                   me bitlerinin durumunu sürekli takip eder.  *
 *                   Ýlgili olaylar oluþtuðunda ise gerekli fonk-*
 *                   siyonlarý iþletir.                          *
 *****************************************************************/
void USBService( void )
{
	//Eðer cihaz bus'a baðlý deðil ise geri dön
	if(bDeviceState == DETACHED) return;
	
	//Bus'da faliyet baþladýðý zaman ACTVIF biti set olur.Bu durumda
	//cihaz suspend durumda ise çýkmasý gerekir.
	if(isActive()) WakeUpSuspendEventHandler();
	
	//Cihaz suspend modundaysa hiçbir iþlem yapma ve fonksiyondan çýk
	if(UCONbits.SUSPND) return;
	
	/*
	 * Reset durumu algýlandýðýnda eðer cihaz suspend modda ise ilk olarak
	 * ACTVIF biti set olur.Bu durumda cihaz ilk olarak suspend moddan çýkartýlmalýdýr.
	 * Daha sonra URSTIF biti set olur.
	 */
	if(isReset())   ResetEventHandler();
	
	//Diðer kesmeler
	if(isSuspend()) SuspendEventHandler();
	if(isSof())     StartOfFrameEventHandler();
	if(isStall())   StallEventHandler();
	if(isError())   ErrorEventHandler();
	
	/*
	 * Bir iþlem en az üç evreden oluþur.Windows tarafýndan ilk olarak bir jeton paketi
	 * gönderilir.Ardýndan bir veri paketi gönderir.Alýcý taraf cihaz olduðunda PC'ye bir 
	 * ACK gönderilir ve TRNIF biti set olur.SIE gelen veriyi 0x400 adresinden itibaren 
	 * yerleþtirmiþtir.
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
 * Parametreler:    Modülü aktif/pasif yapacak bool deðeri       *
 *																 *
 * Dönen Deðer:     void                                         *
 *																 *
 * Açýklama:        Bu fonksiyon enable parametresine argüman o- *
 *                  larak aktýralacak deðer true ise USB modülü  *
 *                  aktif yapar, aksi halde kapatýr.             *
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
 * Dönen Deðer:      void                                        *
 *																 *
 * Açýklama:         Bu fonksiyon windows tarafýndan D- ve D+    *
 *                   hatlarý reset durumuna getirildiðinde çað-  *
 *                   rýlýr.Tüm hata bitleri kurulur.Ayný zaman-  *
 *                   da ACTVIE biti hariç diðer tüm kesmeler ku- *
 *                   rulur.Tüm uçnoktalar pasif yapýlýr ve sade- *
 *                   kontrol transferi için uçnokta0 kurulur.    *
 *                   AYGIT DURUMU ve konfigrasyon gibi ayarlar   *
 *                   resetlenir ve cihaz DEFAULT duruma kurulur. *
 *****************************************************************/
void ResetEventHandler( void )
{
	UIR	 = CLEAR;
	UEIR = CLEAR;
	UIE  = _SOFIE|_STALLIE|_IDLEIE|_TRNIE|_UERRIE|_URSTIE;   //USB kesmelerini kur
	UEIE = _BTSEE|_BTOEE|_DFN8EE|_CRC16EE|_CRC5EE|_PIDEE;    //USB hata kesmelerini kur
	
	UADDR = CLEAR;                                           //Reset sonrasý varsayýlan adres 0
	mDisableAllEndPoint();                                   //Tüm uçnoktalarý pasif yapan makro
	UEP0 = EP_CTRL|HSHK_EN;                                  //Uçnokta0 kontrol uçnoktasý ve el sýkýþma aktif
	/*
	 * Paket alýmý aktif.Bir paket alýnýp ACK gönderildikten sonra 
	 * paket içeriðinin tutulduðu tampona, üstüne yazýlmamasý için yeni paket alýmýna 
	 * izin verilmez.Dolasýyla PKTDIS biti otomatik olarak set olur.Her paket alýmý ardýndan
	 * yeni paketin alýnmasý için bu bitin temizlenmesi gerekir.
	 */
	UCONbits.PKTDIS = false;                                 //Paket alýmý aktif
	USBInitNextSetupTrf();                                   //Uçnoktayý setup iþlemi için hazýrla
	UsbDeviceStatus.RemoteWakeup = false;                    //Cihaz özelliklerini resetle
	bActiveConfiguration = CLEAR;
	bDeviceState = DEFAULT;
}

/*****************************************************************
 * Fonksiyon Ismi:   StallEventHandler()                         *
 *																 *
 * Parametreler:     void                                        *
 *																 *
 * Dönen Deðer:      void                                        *
 *																 *
 * Açýklama:         Bu fonksiyon uçnokta0 bir istek sonrasý     *
 *                   stall döndürdüðünde çaðrýlýr.Uçnokta0 des-  *
 *                   teklenmeyen bir istek aldýðý yada uçnokta   *
 *                   çakýlmasý(endpoint halted) gibi durumlarda  *
 *                   STALL ile yanýt verebilir.                  *
 *                   Bu olaydan sonra stall biti ve kesme biti   *
 *                   temizlenir ve uçnokta0 bir sonraki iþleme   *
 *                   hazýrlanýr.                                 *
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
 * Dönen Deðer:      void                                        *
 *																 *
 * Açýklama:         Bu fonksiyon bir hata oluþtuðunda çaðrýlýr  *
 *                   ve sadece hata kesme bayraðýný temizler.    *
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
 * Dönen Deðer:      void                                        *
 *																 *
 * Açýklama:         Bu fonksiyon Bus aktif olduktan sonra iþle- *
 *                   tilir ve cihaz suspend modundan çýkartýlýr. *
 *****************************************************************/
void WakeUpSuspendEventHandler( void )
{
	UCONbits.SUSPND	= false;       //SIE clock aktif
	UIEbits.ACTVIE  = false;       //Bus aktif kesmesi pasif
	UIRbits.ACTVIF	= false;       //Bus aktif kesme bayraðýný temizle
}

/*****************************************************************
 * Fonksiyon Ismi:   SuspendEventHandler()                       * 
 * 																 *   
 * Parametreler:     void                                        *
 *																 *
 * Dönen Deðer:      void                                        *
 *																 *
 * Açýklama:         Cihaz BUS'da 3 milisaniye iþlem görmediði   *
 *                   zaman IDLEIF biti set olur.Ardýndan bu fonk-*
 *                   siyon çaðrýlýr ve ACTVIE biti set edilerek  *
 *                   BUS aktif kesmesi kurulur.Bus'da tekrar fa- *
 *                   liyet baþladýðýnda bu bit ACTVIF biti set   *
 *                   olur.Son olarak UCON register'ýnýn SUSPND   *
 *                   biti set edilir ve SIE'nin saat kaynaðý pa- *
 *                   sif yapýlýr.                                *
 *****************************************************************/
void SuspendEventHandler( void )
{
	UIEbits.ACTVIE  = true;      //Bus aktif kesmesini kur
	UIRbits.IDLEIF  = false;     //Aylak bitini temizle
	UCONbits.SUSPND = true;      //SIE pasif yapýldý ve cihaz suspend mod'da
}

/*****************************************************************
 * Fonksiyon Ismi:   StartOfFrameEventHandler()                  *
 *																 *
 * Parametreler:     void                                        *
 *																 *
 * Dönen Deðer:      void                                        *
 *																 *
 * Açýklama:         Bu fonksiyon iþlem baþlangýcýndan SOF alýn- *
 *                   dýðýnda çaðrýlýr ve sadece bayrak bitini    *
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
 * Dönen Deðer:      void                                        *
 *																 *
 * Açýklama:         Bu fonksiyon eðer cihazýn uzaktan uyandýrma *
 *                   özelliði aktif edildiyse kullanýlabilir.    *
 *                   Aský durumundaki bir cihaz Bus'da faliyet   *
 *                   baþlatmak için, yani haberleþmeyi tekrar    *
 *                   canlandýrmak için Rresume sinyali göndere-  *
 *                   bilir.Spec. 2.0'da bu süre 1-15ms olarak    *
 *                   belirtilmektedir.                           *
 *****************************************************************/
void WakeUpRemoteEventHandler( void )
{
	static unsigned int iResCount;
	
	//Eðer uzaktan uyandýrma özelliði aktif ise 
	//resume sinyali gönderilebilir.
	if(UsbDeviceStatus.RemoteWakeup == true)
	{
		WakeUpSuspendEventHandler();
		UCONbits.RESUME = true;          //Resume sinyali göndermeyi baþlat
		
		iResCount = 1800U;               //1-13ms arasý
		mWaitResumeSignal(iResCount);
		UCONbits.RESUME = false;         //Resume sinyali göndermeyi bitir.
	}
}

/*****************************************************************
 * Fonksiyon Ismi:   FlushBuffer()                               *
 *																 *
 * Parametreler:     Buffer start adresi ve uzunluk              *
 *																 *
 * Dönen Deðer:      void                                        *
 *																 *
 * Açýklama:         Bu fonksiyon baþlangýç adresi verilen adres-*
 *                   ten itibaren uzunluk kadar alaný temizler.  *
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