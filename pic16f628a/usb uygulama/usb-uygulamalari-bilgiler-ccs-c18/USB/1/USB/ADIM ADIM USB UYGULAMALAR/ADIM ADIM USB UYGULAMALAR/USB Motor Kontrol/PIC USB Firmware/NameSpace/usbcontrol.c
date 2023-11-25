/*****************************************************************
 *             U S B   H I D   U Y G U L A M A S I               * 
 *****************************************************************
 * Dosya Ismi:      usbcontrol.c - Kontrol transferleri          *
 *															     *
 * Version:         1.00                                         *
 *																 *
 * Tarih:           06.05.2007                                   *
 *																 *
 * Author:          Ahmet ATAR								     *
 *****************************************************************
 * Açýklama:        Bu dosyada kontrol transferleri için gerekli *
 *                  fonksiyonlar yer almaktadýr.                 *
 *****************************************************************/
//I N C L U D E S 
#include <p18cxxx.h>
#include "mem_alloc.h"
#include "usbdefs.h"
#include "pubfunc.h"
#include "usbmcr.h"

//Ö Z E L   F O N K S Ý Y O N   P R O T O T Ý P 
void USBControlInHandler( void );
void USBControlOutHandler( void );
void USBControlSetupHandler( void );

/*****************************************************************
 * Fonksiyon Ismi:   USBControlInHandler()                       *
 *																 *
 * Parametreler:     void										 *
 *     															 *
 * Dönen Deðer:      void										 *
 * 																 *
 * Açýklama:         Bu fonksiyon control iþlemi IN ise iþleti-  *
 *                   lir ve SETUP paketi alýndýktan sonra çözü-  *
 *                   len isteðin gerektirdiði veriler tampona    *
 *                   yerleþtirildikten sonra bu fonksiyon bu tam-*
 *                   pondaki verileri PC'ye gönderir.SETUP pake- *
 *                   ti bir Set_Address isteði içeriyorsa cihaz  *
 *                   ADDRESS_PENDING durumuna alýnýr.Bir sonraki *
 *                   IN iþleminde ise cihazýn adresli olup olma- *
 *                   dýðý kontrol edilir.Deðilse wValue alaný    *
 *                   okunarak cihaz adresli duruma getirilir.    *
 *****************************************************************/
void USBControlInHandler( void )
{
	mCheckAddressPending();
	if(bTransferState == CTRL_SEND)
	{
		ControlTransferWriteService();
		if(!EndPoint0BdtIn.BDnSTAT.DTS)
		   EndPoint0BdtIn.BDnSTAT.BdsArea = _USIE|_DAT1|_DTSEN;
		else
		   EndPoint0BdtIn.BDnSTAT.BdsArea = _USIE|_DAT0|_DTSEN;
	}
	else
	   USBInitNextSetupTrf();
}

/*****************************************************************
 * Fonksiyon Ismi:   USBControlOutHandler()                      *
 *																 *
 * Parametreler:     void										 *
 *															     *
 * Dönen Deðer:      void										 *
 *																 *
 * Açýklama:         Bu fonksiyon control transfer iþlemi OUT    *
 *                   ise iþletilir ve SIE tarafýndan doldurulan  *
 *                   CntrlDataPacket alanýný okumak üzere ilgi-  *
 *                   li fonksiyonu çaðýrýr.Ardýn senkronizasyon  *
 *                   bitinin durumunu deðiþtirir.                *
 *****************************************************************/
void USBControlOutHandler( void )
{
	if(bTransferState == CTRL_REC)
	{
		ControlTransferReadService();
		if(!EndPoint0BdtOut.BDnSTAT.DTS)
		   EndPoint0BdtOut.BDnSTAT.BdsArea = _USIE|_DAT1|_DTSEN;
		else
		   EndPoint0BdtOut.BDnSTAT.BdsArea = _USIE|_DAT0|_DTSEN;
	}
	else
	   USBInitNextSetupTrf();
}

/*****************************************************************
 * Fonksiyon Ismi:   USBControlSetupHandler()                    *
 *																 *
 * Parametreler:     void                                        *
 *															 	 *
 * Dönen Deðer:      void										 *
 *                   											 *
 * Açýklama:         Bu fonksiyon gönderilen setup paketini in-  *
 *                   celeyip gerekli fonksiyonlarý çaðýracak o-  *
 *                   lan fonksiyonlarý çaðýrýr.                  *
 *                   Transfer iþlemi sahibi baþlangýçta SESSION  *
 *                   NULL olarak ayarlanýr.Ýlgili fonksiyonlar   *
 *                   çaðrýldýktan sonra eðer istek standart is-  *
 *                   teklerden biri ise transfer iþlemi sahibi   *
 *                   SESSION_CNTRL olarak atanýr.Eðer standart   *
 *                   isteklerden biri deðilse transfer sahibi    *
 *                   SESSION_NULL olarak kalacak ve bu sefer ö-  *
 *                   zel sýnýf istekleri(HID) kontrol edilecek-  *
 *                   tir.Son olarak iþlemleri tamamlayacak fonk- *
 *                   siyon çaðrýlýr.                             * 
 *****************************************************************/
void USBControlSetupHandler( void )
{
	bTransferState = WAIT;
	bSessionOwner  = SESSION_NULL;
	iDataCount     = CLEAR;
	
	ToggleLed();     //Setup paketleri geldiði zaman led'i toggle yap
	
	CheckStandartRequest();
	if(bSessionOwner == SESSION_NULL) CheckHidRequest();
	ControlTransferComplateService();
}

/*****************************************************************
 * Fonksiyon Ismi:   EndPointService()                           *
 *																 *
 * Parametreler:     void                                        *
 *																 *
 * Dönen Deðer:      void										 *
 *        														 *
 * Açýklama:         Bu fonksiyon kontrol transfer tipini tes-   *
 *                   pit eder ve ilgili fonksiyonlarý çaðýrýr.   *
 *                   Sadece uçnokta 0'a gelen istekler deðerlen- *
 *                   dirilir.Bu istekler SETUP, OUT, ve IN'dir.  *
 *                   Cihaz kurulu durumdaysa Uçnokta1'e gelen    *
 *                   paketleride deðerlendirir.Bu paketler sade- *
 *                   ce IN veya OUT olabilir.                    *
 *****************************************************************/
void EndPointService( void )
{
	//Kontrol Transfer Ýþlemleri : OUT
	if(USTAT == EP0_OUT)
	{
		if(EndPoint0BdtOut.BDnSTAT.PID == SETUP_TOKEN) 
		   USBControlSetupHandler(); 
		else 
		   USBControlOutHandler();
	} //IN
	else if(USTAT == EP0_IN) USBControlInHandler();
	
	//Kesme transfer iþlemleri
	if(USTAT == EP1_OUT) 
	{ 
		UserProc(INTERRUPT_OUT); 
		UCONbits.PKTDIS = false; 
	}
	if(USTAT == EP1_IN)  
	{ 
		UserProc(INTERRUPT_IN);  
		UCONbits.PKTDIS = false;
	}
}

/*****************************************************************
 * Fonksiyon Ismi:   ControlTransferWriteService()               *
 *																 *
 * Parametreler:     void										 *
 *																 *
 * Dönen Deðer:      void										 *
 *																 *
 * Açýklama:         Bu fonksiyon IN paketi alýndýktan sonra bel-*
 *                   lek bölgesine yerleþtirilen (RAM-ROM) bil-  *
 *                   gileri gönderir.                            *
 *****************************************************************/
void ControlTransferWriteService( void )
{
	//Kontrol Read Transfer
	//PC veri basamaðýnda alým yapar.
	
	unsigned int iSendCount;
	
	//Gönderilecek veri adedi uçnokta0'dan büyükse uçnokta0 büyüklüðü
	//kadar veri gönderilir.Aksi halde tüm veriler gönderilir.
	if(iDataCount < ENDPOINT0_BUFFER_SIZE)
	   iSendCount = iDataCount;
	else
	   iSendCount = ENDPOINT0_BUFFER_SIZE;
	   
	//Gönderilecek verilerin adedi buffer descriptor table'a yükleniyor.
	EndPoint0BdtIn.BDnSTAT.BC9 = false;                   //Üst bitleri ilk olarak sýfýrla
	EndPoint0BdtIn.BDnSTAT.BC8 = false;
	EndPoint0BdtIn.BDnSTAT.BdsArea |= MSB(iSendCount);    //Þimdi BC9 ve BC8'e üst bitleri yükle
	EndPoint0BdtIn.BDnCNT = LSB(iSendCount);              //Son olarak alt byte'ý CNT'ye yükle
	
	//Geriye gönderilecek veri adedini iDataCount'a yükle
	iDataCount -= iSendCount;
	
	//Verilerin yazýlacaðý hedef'in handle'ýný al
	hDestination.bHandle = (unsigned char*)&CntrlDataPacket;
	
	//Künye bilgileri gibi bazý bilgiler rom bellekte tanýmlý olduðu için
	//transfer türü ROM_TYPE seçilir.Diðer transfer'lerde RAM_TYPE kullanýlýr.
	if(UsbDeviceStatus.HandleType == ROM_TYPE)
	{
		while(iSendCount) {
		    *hDestination.bHandle = *hSource.brHandle;
			hSource.brHandle++;
			hDestination.bHandle++;
			iSendCount--;
		}
	}// RAM_TYPE
	else
	{
		while(iSendCount) {
		    *hDestination.bHandle = *hSource.bHandle;
			hSource.bHandle++;
			hDestination.bHandle++;
			iSendCount--;
		}
	}
}

/*****************************************************************
 * Fonksiyon Ismi:   ControlTransferReadService()                *
 *																 *
 * Parametreler:	 void										 *
 *																 *
 * Dönen Deðer:      void										 *
 *																 *
 * Açýklama:         Bu fonksiyon OUT paketi alýndýktan sonra    *
 *                   bellek bölgesine yerleþtirilen verileri han-*
 *                   dle deðeri verilen tampona okur.            *
 *****************************************************************/
void ControlTransferReadService( void )
{
	//Kontrol Write Transfer
	//Cihaz veri basamaðýnda veri alýr.
	
	unsigned char iRecCount;

	//Son iki biti temizleyerek gelen veri adedini iRecCount'un 
	//alt ve üst byte'larýna yükle
	HighLoad(iRecCount, (0x03 & EndPoint0BdtOut.BDnSTAT.BdsArea));
	LowLoad(iRecCount, EndPoint0BdtOut.BDnCNT);
	
	//Geriye kalan okunacak veri adedini iDataCount'a yükle
	iDataCount += iRecCount;
	
	//Gelen verilerin okunacaðý bellek bölgesine bir handle al
	hSource.bHandle = (unsigned char*)&CntrlDataPacket;
	
	//Gelen veriler handle deðeri verilmiþ bellek bölgesine okunur
	while(iRecCount)
	{
		*hDestination.bHandle = *hSource.bHandle;
		hSource.bHandle++;
		hDestination.bHandle++;
		iRecCount--;
	}
}

/*****************************************************************
 * Fonksiyon Ismi:   ControlTransferComplateService()            *
 *																 *
 * Parametreler:	 void										 *
 *																 *
 * Dönen Deðer:      void										 *
 *																 *
 * Açýklama:         Bu fonksiyon SETUP basamaðýnda bmRequestType*
 *                   ýn 7.biti "1" olmasý halindeki IN iþlemi    *
 *                   sonrasý tampona yerleþtirilen verileri gön- *
 *                   derir, "0" olmasý halindeki OUT iþlemi için *
 *                   uçnoktayý hazýrlar yada istek desteklenmi-  *
 *                   yorsa STALL gönderir.                       *
 *****************************************************************/
void ControlTransferComplateService( void )
{
	//Windows'un istekleri cihaz tarafýndan desteklenmiyorsa bSessionOwner
	//SESSION_NULL olarak kalýr.Bu durumda uçnoktanýn STALL ile yanýt vermesi gerekir.
	if(bSessionOwner == SESSION_NULL)
	{
		//Uçnoktayý bir sonraki setup iþlemi için hazýrla
		EndPoint0BdtOut.BDnCNT = ENDPOINT0_BUFFER_SIZE;
		EndPoint0BdtOut.ADDR = (unsigned char*)&SetupPacket;
		
		EndPoint0BdtOut.BDnSTAT.BdsArea = _USIE|_BSTALL;
		EndPoint0BdtIn.BDnSTAT.BdsArea  = _USIE|_BSTALL;
	}
	else
	{
		if(SetupPacket.Direction == DEVICE_TO_HOST)
		{
			//Veriler cihaz'dan PC'ye doðru gidecekse tampondaki bilgileri
			//göndermek için ControlTransferWriteService()'ý çaðýr.(Çift iþlem)
			//Daha sonra uçnokta in-out'u sonraki iþlemler için hazýrla.
			if(SetupPacket.wLength < iDataCount) iDataCount = SetupPacket.wLength;    //PC'nin istediði kadar veriyi gönder
			ControlTransferWriteService();
			
			bTransferState = CTRL_SEND;
			
			EndPoint0BdtOut.BDnCNT = ENDPOINT0_BUFFER_SIZE;
			EndPoint0BdtOut.ADDR = (unsigned char*)&SetupPacket;
			EndPoint0BdtOut.BDnSTAT.BdsArea = _USIE;                  //SIE mod
			
			EndPoint0BdtIn.ADDR = (unsigned char*)&CntrlDataPacket;
			EndPoint0BdtIn.BDnSTAT.BdsArea = _USIE|_DAT1|_DTSEN;
		}
		else
		{
			bTransferState = CTRL_REC;
			
			//Eðer iþlem erken sonlandýrýlýrsa cihaz PC'ye statü basamaðýnda
			//0 uzunluklu bir veri paketi yollayarak transferin baþarýsýný bildirir.
			//Bu iþlem için uçnokta0 IN'i yapýlandýr.
			EndPoint0BdtIn.BDnCNT = 0;
			EndPoint0BdtIn.BDnSTAT.BdsArea = _USIE|_DAT1|_DTSEN;
			
			//Bir sonraki OUT iþlemi için uçnokta0'ý hazýrla
			EndPoint0BdtOut.BDnCNT = ENDPOINT0_BUFFER_SIZE;
			EndPoint0BdtOut.ADDR = (unsigned char*)&CntrlDataPacket;
			EndPoint0BdtOut.BDnSTAT.BdsArea = _USIE|_DAT1|_DTSEN;
		}
	}
	UCONbits.PKTDIS = false;    //Paket alýmý aktif
}

/*****************************************************************
 * Fonksiyon Ismi:   USBInitNextSetupTrf()                       *
 *															     *
 * Parametreler:     void										 *
 *																 *
 * Dönen Deðer:      void										 *
 *																 *
 * Açýklama:         Bu fonksiyon uçnoktayý bir sonraki SETUP iþ-*
 *                   lemi için hazýrlar.						 *
 *****************************************************************/
void USBInitNextSetupTrf( void )
{
	bTransferState = WAIT;
	EndPoint0BdtOut.BDnCNT = ENDPOINT0_BUFFER_SIZE;
	EndPoint0BdtOut.ADDR   = (unsigned char*)&SetupPacket;   
	EndPoint0BdtOut.BDnSTAT.BdsArea = _USIE|_DAT0|_DTSEN;    //Data Sync aktif, ilk durum DTS0, ve SIE mod
	EndPoint0BdtIn.BDnSTAT.BdsArea  = _UCPU;                 //Uçnokta0 In CPU'da
}