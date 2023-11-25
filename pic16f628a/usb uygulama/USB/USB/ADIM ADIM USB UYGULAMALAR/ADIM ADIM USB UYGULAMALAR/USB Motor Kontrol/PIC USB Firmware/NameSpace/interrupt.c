/*****************************************************************
 *             U S B   H I D   U Y G U L A M A S I               * 
 *****************************************************************
 * Dosya Ismi:      interrupt.c - Donan�m Kesmeleri              *
 * 															     *
 * Version:         1.00                                         *
 *																 *
 * Tarih:           06.05.2007                                   *
 *																 *
 * Author:          Ahmet ATAR								     *
 *																 *
 * A��klama:        Bu dosyada donan�m kesmeleri olu�tu�unda,    *
 *                  rekli kesmelerin tespiti ve ilgili fonksi-   *
 *                  yonlar�n i�letilmesi ile ilgili i�lemler ta- *
 *                  n�ml�d�r.									 *
 *****************************************************************/
//I N C L U D E 
#include "interrupt.h"

//G L O B A L   D E � � � K E N L E R

/*****************************************************************
 * Fonksiyon Ismi:
 *
 * Parametreler:
 *
 * D�nen De�er:
 *
 * A��klama:
 *****************************************************************/
#pragma code HighInterruptVector = 0x08
void HighPriorityInterrupts( void ) {
	_asm goto ResolveHighInterruptSource _endasm
}
#pragma code

/*****************************************************************
 * Fonksiyon Ismi:
 *
 * Parametreler:
 *
 * D�nen De�er:
 *
 * A��klama:
 *****************************************************************/
#pragma code LowInterruptVector = 0x18
void LowPriorityInterrupts( void ) {
	_asm goto ResolveLowInterruptSource _endasm
}
#pragma code

/*****************************************************************
 * Fonksiyon Ismi:
 *
 * Parametreler:
 *
 * D�nen De�er:
 *
 * A��klama:
 *****************************************************************/
#pragma interrupt ResolveHighInterruptSource
void ResolveHighInterruptSource( void )
{
}

/*****************************************************************
 * Fonksiyon Ismi:
 *
 * Parametreler:
 *
 * D�nen De�er:
 *
 * A��klama:
 *****************************************************************/
#pragma interruptlow ResolveLowInterruptSource
void ResolveLowInterruptSource( void )
{
}

