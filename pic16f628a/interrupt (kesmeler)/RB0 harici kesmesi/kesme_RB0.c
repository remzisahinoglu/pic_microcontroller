//RB0/INT harici kesme uygulamas�
//12.02.2007
//yazan = Remzi �AH�NO�LU
#if defined (__PCM__)
#include <16F628A.h>
#use delay(clock=4000000)
#fuses NOWDT,PUT,NOLVP,NOBROWNOUT,MCLR,INTRC_IO,NOPROTECT
#endif

#byte STATUS = 0x03
#byte OPTION = 0x81
#byte INTCON = 0x0B

#INT_EXT                           // kesme geldi�inde program ak�s�
void RB_isr()                      // buradan devam eder
{                     
bas:     
   output_a(1);                    // 1 degeri a portuna g�nderilir.     
   bit_clear(INTCON,1);            // RB0/INT harici kesme bayra��  
   delay_ms(100);                  // tekrar kullan�lmak i�in s�f�rlan�r
                                   // butona bas�lmak i�in beklenir 
   if(input(PIN_B0)==0)            // kesme sinyali var m� yok mu?
   {                                  
      goto bas;                    // var ise bas a d�n i�lemkeri tekrar uygula 
   }                               // yok ise kesmeyi bitir
}                                  // kald���n yerden devam et  

void main()
{  
   set_tris_b(1);                  // RB0/INT ucu giri� yap�l�r
   enable_interrupts(INT_EXT);     // RB0 harici kesmesi aktif yap�l�r
   enable_interrupts(GLOBAL);      // kesmelere izin verilir
   bit_clear(OPTION,6);            // kesme d��en kenarda olussun
                
tekrar:                            // pice ilk enerji verildi�inde 
   output_a(1);                    // 1-2-4-8 olarak 1 sn aral�kla      
   delay_ms(1000);                 // saymaya baslar    
   output_a(2);                    // bu de�erler portaya ��k�� olarak verilir
   delay_ms(1000);
   output_a(4);
   delay_ms(1000);
   output_a(8);
   delay_ms(1000);
   goto tekrar;   
}

