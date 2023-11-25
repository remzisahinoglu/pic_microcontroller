//Gerilim referans birimi uygulamasý
//16.02.2007
//yazan = Remzi ÞAHÝNOÐLU
#if defined (__PCM__)
#include <16F628A.h>
#use delay(clock=4000000)
#fuses NOWDT,PUT,NOLVP,NOBROWNOUT,MCLR,INTRC_IO,NOPROTECT
#endif

#byte CMCON = 0x1F

#INT_COMP                              // C2out 1 oldugunda program akýsý buradan devam eder   
void comp_isr() 
{                                      // RA1 portundan gelen voltaj vref den    
   output_low(PIN_B0);                 // küçük oldugunda çýkýs 0 olur 
}                                      // C2out 0 oldugunda kaladýgý yerden devam edilir          

void main()
{
   setup_comparator(A0_VR_A1_VR);      // 010 nolu comp aktif edildi
   setup_vref(VREF_LOW|5);             // vref=5 secildi bu 1v ta karsýlýk gelir
   enable_interrupts(INT_COMP);        // karsýlastýrýcý kesmesi aktif
   enable_interrupts(GLOBAL);          // çevresel kesmeler aktif
   
tekrar:                  
   output_high(PIN_B0);                // RA1 portundan gelen voltaj vref den
   goto tekrar;                        // küçük oldugunda çýkýs 1 olur
}  

