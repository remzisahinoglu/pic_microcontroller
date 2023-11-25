/*****************************************************************
 *             U S B   H I D   U Y G U L A M A S I               * 
 *****************************************************************
 * Dosya Ismi:      interrupt.h - Donaným Kesmeleri              *
 * 															     *
 * Version:         1.00                                         *
 *																 *
 * Tarih:           06.05.2007                                   *
 *																 *
 * Author:          Ahmet ATAR								     *
 *																 *
 * Açýklama:        Bu dosyada interrupt.c dosyasý için gerekli  *
 *                  tanýmlamalar yer almatadýr.                  *
 *****************************************************************/
#ifndef INTERRUPT_H
#define INTERRUPT_H

//I N C L U D E S 
#include <p18cxxx.h>
#include "usbdefs.h"
#include "mem_alloc.h"

//Ö Z E L  F O N K S I Y O N   P R O T O T I P,
void LowPriorityInterrupts( void );
void HighPriorityInterrupts( void );
void ResolveLowInterruptSource( void );
void ResolveHighInterruptSource( void );

//T A N I M L A M A L A R

#endif