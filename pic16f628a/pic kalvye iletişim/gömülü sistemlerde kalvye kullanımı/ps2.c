/*********************************************************/
/*	PS2.c  - FlashLite186 için PS/2 Klavye Fonksiyonlarý  */
/* Lima Endustriyel Bilgisayar - www.lima.com.tr			*/
/* Haziran 2004 - Istanbul, Turkiye								*/
/*																			*/
/*	Konsol, INT2 ve PORTC'nin  0 + 1. pinlerini kullanýr	*/
/*                                                       */
/* Klavyenin CLOCK sinyali INT2 ve PORTC'nin 0. pinine	*/
/* DATA sinyali ise PORTC'nin 1. pinine baðlýdýr         */
/*				           												*/
/* Her iki açýk kolektör sinyal 10k'luk dirençlerle 		*/
/*	5V'a baðlanmýþtýr												   */
/*-------------------------------------------------------*/
/*	PS2.c  - PS/2 Keyboard Input Functions for FL186 by   */
/* Lima Endustriyel Bilgisayar - www.lima.com.tr			*/
/* June 2004 -  Istanbul, Turkey									*/
/*																			*/
/*	Uses Console, INT2 and Pin0 and 1 of Port C.		 		*/
/*                                                       */
/* CLOCK signal of the keyboard is connected to INT2 &   */
/*				  Pin 0 of PortC.									   */
/* DATA signal is connected to  Pin 1 of PortC           */
/*				           												*/
/* Both signals are "pulled-up" to 5V via 10k resistor	*/
/*	Keyboard's "Ground" needs to be common with the FL186 */
/*																			*/
/*********************************************************/

#define DEBUG

#define TRUE  1
#define FALSE 0

#include <bios.h>
#include <dos.h>
#include <stdlib.h>
#include <string.h>

#include "r8822.h"


#ifdef DEBUG
	#include <stdio.h>
	unsigned char bitarr[256];
#endif


// Variables & Functions Global to all
void ps2_startint(void);
void interrupt ps2_int(void);
int  ps2_getkey(short* key, short* shift);
void ps2_stopint( void );



// Variables & Functions Only Global to PS/2
void ps2_show(void);
void ps2_decode(unsigned char);


#define PS2_MAX 128
#define PS2_WAIT 30

// Bit/Bayt Dönüþüm Çarpanlarý
//---------------------------------------------
// Bit/Byte Conversion Multipliers
unsigned char bits[]= {1,2,4,8,16,32,64,128};

// Son Tuþ Çözümleme Zamaný (ms)
//---------------------------------------------
// Last time a key was decoded (ms)
unsigned long ps2_clock;

// Klavye ve 'Shift' kodlarý Tampon Bellekleri
//---------------------------------------------
// Key and Shift code buffers
unsigned char keycode[PS2_MAX + 1], shiftcode[PS2_MAX + 1];

#ifdef DEBUG
	unsigned char  bitCount =   0;
#endif
static short charcount 	=   0;



/*********************************************************/
/*	ps2_int: PS/2 klavye kodlayýcýsýnýn gönderdiði 10 biti*/
/* toplayan kesme hizmet yordamý	  								*/
/* Parite  ve Stop bitlerinin doðruluðunu kontrol etmez.	*/
/*********************************************************/
/*	ps2_int: INT driven service routine that collects 10 	*/
/* bits that make up a scan code sent by a PS2 encoder.	*/
/* It does not check the validity of parity or stop bits	*/
/*********************************************************/
void interrupt ps2_int(void)
{
	unsigned char  retry = 0, binp;
	unsigned static char bkey  = 0, bcount = 0;

	if (bcount < 10)
	{
			// CLOCK sinyali aþaðý (0V) inene kadar bekle
			//--------------------------------------------
			// Wait until CLOCK signal is down (0V)
			while ( ((binp = inportb(0x602)) & 1) &&  (retry++ < PS2_WAIT) );

			 if ( retry < PS2_WAIT )
			 {
#ifdef DEBUG
				bitarr[bitCount] = (binp &  2) >> 1;
#endif
				if ( (bcount < 8) && (binp & 2)) bkey = bkey + bits[bcount];
				bcount++;

			 } else {
			// CLOCK sinyali aþaðý inmedi - HATA
			//--------------------------------------------
			// CLOCK signal did not go down - ERROR
#ifdef DEBUG
					bitarr[bitCount] = 9;
#endif
					bcount = 0;
					bkey   = 0;
			 }

#ifdef DEBUG
		bitCount++;
#endif

	} else {
				// 10. bit geldi - Tuþu çözümler
				//--------------------------------------------
				// 10. bit is received  - Decode the key
				ps2_decode(bkey);
				bcount = 0;
				bkey   = 0;
	}
	// INT2 için kesme sonu
	//--------------------------------------------
	// Selective End of Interrupt for INT2
	outport(EOI, 0x000E );
}

/*********************************************************/
/*	ps2_decode: Kesme yordamýnýn topladýðý deðerleri		*/
/* 'key' (klavye) ve 'shift' kodlarýna dönüþtürür. Son 	*/
/* iki dönüþüm arasýnda geçen zaman farkýna baðlý olarak	*/
/* karakter sayýsýný (charcount) arttýrýr						*/
/*		shiftcode bitlerinin kullaným þekli aþaðýdadýr:		*/
/*				0 : Sol / Sað Shift									*/
/*				1 : Sol / Sað Control								*/
/*				2 : Sol / Sað Alt										*/
/*				3 : 'Extended' Tuþ (kodu 'E0' ile baþlýyor)	*/
/*********************************************************/
/*	ps2_decode: Decodes characters collect by ps2_int into*/
/* key and shift code  buffers.  It increments character	*/
/* count (charcount) by checking the time interval			*/
/* between the last two calls (stored in ps2_clock)      */
/*		shiftcode bits are set as below:							*/
/*				0 : Left / Right Shift								*/
/*				1 : Left / Right Control							*/
/*				2 : Left / Right Alt									*/
/*				3 : Extended Key Flag (key starts with E0)	*/
/*********************************************************/
void ps2_decode(unsigned char keyin)
{
#define PS2_RATE 3
	//Decode key or shift code
	switch (keyin)
	{
		case 0x0:
		//NULL
			break;

		case 0xF0:
		//Key UP identifier
			break;

		case 0xE0:
		//Extended key
			shiftcode[charcount] = shiftcode[charcount] | 8;
			break;

		case 0x11:
		//Left or Right Alt
			shiftcode[charcount] = shiftcode[charcount] | 4;
			break;

		case 0x12:
		//Left SHIFT
			shiftcode[charcount] = shiftcode[charcount] | 1;
			break;

		case 0x14:
		//Left or Right CONTROL
			shiftcode[charcount] = shiftcode[charcount] | 2;
			break;

		case 0x59:
		//Right SHIFT
			shiftcode[charcount] = shiftcode[charcount] | 1;
			break;

		default :
			keycode[charcount] = keyin;
			break;
		}

		if ( (keycode[charcount] != 0) && \
				(labs(biostime(0, 0L) - ps2_clock ) >= PS2_RATE ))
		{
				if (charcount < PS2_MAX)
				{
					charcount++;
					shiftcode[charcount] = 0;
					keycode[charcount] = 0;
				}
		}
		// Son çözümleme zamanýný güncelle
		//-----------------------------------
		// Update last decoding time
		ps2_clock = biostime(0, 0L);

}


void ps2_startint()
{
		ps2_clock = biostime(0, 0L);
		outport(IMASK,    (inport(IMASK)    & 0x7FBF)); //Enable Int2
		// INT2 is the bit 31 in PIOMODE1 & PDIR1
		// PIOMODE1=0 & PIODIR1=0 couple sets INT2 for "NORMAL OPERATION"
		outport(PIOMODE1, (inport(PIOMODE1) & 0x7FFF));
		outport(PDIR1,    (inport(PDIR1)    & 0x7FFF));

		return;
}

void ps2_stopint( void )
{
	// Disable  Int2
	outport(IMASK, (inport(IMASK) | 0x0040));
	return;
}

void ps2_show( void )
{

#ifdef DEBUG
	int bitx, charx, charmax;

	fprintf(stderr, "\n%3d bits received.\n", bitCount);
	charmax = bitCount / 10;
	if (bitCount < 10) return;
	for (bitx = 0; bitx < 10; bitx++)
	{     if (bitx < 8)
			{
				fprintf(stderr, "%2.2d :", bitx);
			} else {
				if (bitx==8)
				{
					fprintf(stderr, "PAR:");
				} else {
					fprintf(stderr, "STP:");
				}
			}
			for (charx = 0; charx < charmax; charx++)
			{
				fprintf(stderr, " %1.1d :", bitarr[bitx + 10 * charx]);
			}
			fprintf(stderr, "\n");

	}
	// Toplam bit sayýsýný sýfýrla
	//-----------------------------
	// Reset total bitcount
	bitCount = 0;
#endif
	return;
}


int ps2_getkey(short* key, short* shift)
{
		if ((keycode[0]== 0) || (labs(biostime(0, 0L) - ps2_clock ) < PS2_RATE ))
		{
			return 0;
		}
	disable();
#ifdef DEBUG
		ps2_show();
#endif
		*key   = (short) keycode[0];
		*shift = (short) shiftcode[0];

		keycode[charcount]  = 0;
		shiftcode[charcount]= 0;
		memmove(&keycode[0],    &keycode[1], charcount);
		memmove(&shiftcode[0],&shiftcode[1], charcount);
		if (charcount > 0) charcount--;
	enable();
	return 1;
}

