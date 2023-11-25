/*****************************************************************
 *             U S B   H I D   U Y G U L A M A S I               * 
 *****************************************************************
 * Dosya Ismi:      usbdefs.h - USB tanýmlamalarý                *
 *                                                               *
 * Version:         1.00                                         *
 *																 *
 * Tarih:           06.05.2007                                   *
 *																 *
 * Author:          Ahmet ATAR								     *
 *****************************************************************
 * Açýklama:        Bu dosyada USB haberleþmesi süresince kulla- *
 *                  nýlan tanýmlamalar yer almaktadýr.           *
 *****************************************************************/
#ifndef USB_DEFS_H
#define USB_DEFS_H

//Bool yapýsý
typedef enum Boolean { FALSE = 0, TRUE } bool;

#define true                       1
#define false                      0

#define SET						   1
#define CLEAR					   0

//BUS Takma/Çýkarma algýlama pin
#define USB_ATTACH_REMOVE_PIN      PORTCbits.RC0
#define USB_ATTACH_REMOVE_TRIS	   TRISCbits.TRISC0

//Power Led
#define TEST_LED				   PORTCbits.RC1
#define TEST_LED_TRIS			   TRISCbits.TRISC1

#define BUS_ATTACHED			   1
#define BUS_DETACHED			   0

/*
 * Cihaz'in Windows tarafýndan listelenmesi sýrasýnda
 * geçtiði durumlarýn tanýmlamalarý
 */
#define DETACHED		           0
#define ATTACHED		           1
#define POWERED		           	   2
#define DEFAULT			           3
#define ADR_PENDIG		           4
#define ADDRESS			           5
#define CONFIGURED		           6

//Buffer Descriptor Register'ý kurmak için gerekli parametreler
#define _BSTALL                 0x04
#define _DTSEN                  0x08
#define _INCDIS                 0x10
#define _KEN                    0x20
#define _DAT0                   0x00
#define _DAT1                   0x40
#define _DTSMASK                0x40
#define _USIE                   0x80
#define _UCPU                   0x00

//USB Configuration Register'ý kurmak için gerekli parametreler
#define _PPBM0                  0x00
#define _PPBM1                  0x01
#define _PPBM2                  0x02
#define _LS                     0x00
#define _FS                     0x04
#define _TRINT                  0x00
#define _TREXT                  0x08
#define _PUEN                   0x10
#define _OEMON                  0x40
#define _UTEYE                  0x80

//UEPn Register'ýný kurmak için gerekli parametreler
#define EP_CTRL                 0x06
#define EP_OUT                  0x0C
#define EP_IN                   0x0A	
#define EP_OUT_IN               0x0E
#define HSHK_EN                 0x10

//UEI Register'ýný kurmak için gerekli parametreler
#define _URSTIE				    0x01
#define _UERRIE					0x02
#define _ACTVIE					0x04
#define _TRNIE					0x08
#define _IDLEIE					0x10
#define _STALLIE				0x20
#define _SOFIE					0x40

//UEIE Register'ýný kurmak için gerekli parametreler
#define _PIDEE					0x01
#define _CRC5EE				    0x02
#define _CRC16EE				0x03
#define _DFN8EE					0x04
#define _BTOEE					0x10
#define _BTSEE					0x80

#define IN			               1
#define OUT				           0

//Uçnokta IN-OUT tanýmlamalarý
#define EP0_OUT     (0x00<<3)|(OUT<<2)
#define EP0_IN      (0x00<<3)|(IN<<2)
#define EP1_OUT     (0x01<<3)|(OUT<<2)
#define EP1_IN      (0x01<<3)|(IN<<2)

#define _EP01_OUT               0x01
#define _EP01_IN                0x81
#define _EP02_OUT			    0x02
#define _EP02_IN				0x82

//Künye tipleri
#define DEVICE                     1
#define CONFIGURATION              2
#define STRING                     3
#define INTERFACE                  4
#define ENDPOINT                   5

/*
 * Konfigrasyon künyesi'nin ve Uçnokta künyesinin bmAttributes
 * alanýndaki tanýmlamalar
 */
#define _DEFAULT             0x01<<7
#define _SELF                0x01<<6
#define _RWU                 0x01<<5

#define CONTROL                 0x00
#define ISOCHRONOUS             0x01
#define BULK				    0x02
#define INTERRUPT	            0x03

#define NO_SYNC              0x00<<2
#define ASYNCHRONOUS         0x01<<2
#define ADAPTIV	             0x02<<2
#define SYNCHRONOUS	         0x03<<2

#define DATA_ENDPOINT                  0x00<<4
#define FEEDBACK_ENDPOINT              0x01<<4
#define IMPLICIT_FEEDBACK_ENDPOINT     0x02<<4

//Kontrol transfer basamaklarý
#define WAIT		               0
#define CTRL_REC	               1
#define CTRL_SEND	               2

//Jeton tanýmlamalarý - PID
#define SETUP_TOKEN	          0b1101
#define OUT_TOKEN		      0b0001
#define IN_TOKEN		      0b1001

//bmRequestType için tanýmlamalar
#define HOST_TO_DEVICE	           0
#define DEVICE_TO_HOST	           1

#define STANDART		           0
#define CLASS			           1
#define VENDOR			           2

#define RCPT_DEVICE		           0
#define RCPT_INTERFACE	           1
#define RCPT_ENDPOINT	           2
#define RCPT_OTHER		           3

//Arabirim sayýsý ve UCFG register kurma deðeri
#define MAX_INTERFACE_NUM          1
#define PRM_UCFG		          _PUEN|_TRINT|_FS|_PPBM0

//Ýþlem sahipleri
#define SESSION_NULL	           0   //Ýþlem desteklenmiyor
#define SESSION_CNTRL	           1   //Ýþlem kontrol transferi
#define SESSION_HID		           2   //Ýþlem sýnýfa özgü HID transferi

//HID için tanýmlamalar
#define HID_INTERFACE_ID           0
#define HID_DESC_NUM	           1
#define HID_REPORT_SIZE			  37

//Standart istekler
#define GET_STATUS                 0
#define CLR_FEATURE                1
#define SET_FEATURE                3
#define SET_ADDRESS                5
#define GET_DESCRIPTOR	           6
#define SET_DESCRIPTOR	           7
#define GET_CONFIGURATION          8
#define SET_CONFIGURATION          9
#define GET_INTERFACE             10
#define SET_INTERFACE             11
#define SYNCH_FRAME               12

//Standart cihaz özellikleri
#define DEVICE_REMOTE_WAKEUP    0x01
#define ENDPOINT_HALT           0x00

//Standart HID istekleri
#define GET_REPORT              0x01
#define GET_IDLE                0x02
#define GET_PROTOCOL            0x03
#define SET_REPORT              0x09
#define SET_IDLE                0x0A
#define SET_PROTOCOL            0x0B

//HID Künye tipleri
#define HID_DESC	            0x21
#define REPORT_DESC             0x22
#define PHYSCAL_DESC		    0x23

//HID Sýnýf kodu
#define HID_INTERFACE_CODE		0x03

//HID Rapor tipleri
#define INPUT_REPORT               1
#define OUTPUT_REPORT			   2
#define FEATURE_REPORT             3

//Boot arabirimi alt sýnýf kodlarý
#define BOOT_INTERFACE_SUBCLASS 0x01

//HandleType deðerleri
#define ROM_TYPE                   0
#define RAM_TYPE                   1

#define INTERRUPT_IN			   1
#define INTERRUPT_OUT			   0

#endif