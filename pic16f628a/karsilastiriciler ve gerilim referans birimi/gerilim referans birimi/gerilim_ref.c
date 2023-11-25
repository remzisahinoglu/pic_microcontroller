//Gerilim referans birimi uygulamas�
//16.02.2007
//yazan = Remzi �AH�NO�LU
#if defined (__PCM__)
#include <16F628A.h>
#use delay(clock=4000000)
#fuses NOWDT,PUT,NOLVP,NOBROWNOUT,MCLR,INTRC_IO,NOPROTECT
#endif

#byte CMCON = 0x1F

#INT_COMP                              // C2out 1 oldugunda program ak�s� buradan devam eder   
void comp_isr() 
{                                      // RA1 portundan gelen voltaj vref den    
   output_low(PIN_B0);                 // k���k oldugunda ��k�s 0 olur 
}                                      // C2out 0 oldugunda kalad�g� yerden devam edilir          

void main()
{
   setup_comparator(A0_VR_A1_VR);      // 010 nolu comp aktif edildi
   setup_vref(VREF_LOW|5);             // vref=5 secildi bu 1v ta kars�l�k gelir
   enable_interrupts(INT_COMP);        // kars�last�r�c� kesmesi aktif
   enable_interrupts(GLOBAL);          // �evresel kesmeler aktif
   
tekrar:                  
   output_high(PIN_B0);                // RA1 portundan gelen voltaj vref den
   goto tekrar;                        // k���k oldugunda ��k�s 1 olur
}  

