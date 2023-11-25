//Zamanlay�c�-0 (TMR0) dahili kaynak ile uygulamas�
//19.02.2007
//yazan = Remzi �AH�NO�LU
//
//TMR0 zamanlay�c�s�n� kullanarak 1 sn lik bekleme yapmak
//
#if defined (__PCM__)
#include <16F628A.h>
#use delay(clock=4000000)
#fuses NOWDT,PUT,NOLVP,NOBROWNOUT,MCLR,INTRC_IO,NOPROTECT
#endif

#byte OPTION = 0x81
#byte INTCON = 0x0B

char i=0, a=0;

#INT_TIMER0
void timer0()
{      
   bit_clear(INTCON,2);      // TMR0 kesmesini tekrar kullan�lmas� i�in s�f�rland�
   set_timer0(217);          // TMRO saklay�c�s�na 217 degeri y�klendi
                             // bu deger �n b�l�c� degeri ayarland�g�nda 10ms denk gelir 
   i=i+1;                  
   if(i==100)                // bu d�ng�de 10ms 100 kere say�l�p 1sn elde edilir.
   {      
      i=0;      
      if(a==0)               // burada led 1 ise 0 yap�l�r, 0 ise 1 yap�l�r
      {a=1;}
      else
      {a=0;}               
   }
}

void main()
{   
   set_timer0(217);                                 // TMR0 registerine parantez i�indeki degeri y�kler
   setup_counters( RTCC_INTERNAL, RTCC_DIV_256 );   // �n b�l�c� oran� 1:256 ve harici kaynak kullan�m� a��k
   enable_interrupts(INT_TIMER0);                   // tmr0 kesmesine izin ver
   enable_interrupts(GLOBAL);                       // kesmelere izin ver
   
   
//   set_timer0(217);           yukar�daki tan�mlama bu �ekilde de yap�labilir
//   OPTION = 87;               
//   INTCON = 0xA0;              
   
bas:   
   switch(a)
   {   
   case 0: output_b(0); break;
   case 1: output_b(1); break;    
   }
   goto bas;
}


