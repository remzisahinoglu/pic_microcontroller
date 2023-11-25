//Pic kalavye iletiþim uygulamasý
//26.04.2007
//yazan = Remzi ÞAHÝNOÐLU
//
// TMR0 zamanlayýcýsýný kullnarak harici kaynaktan girilen klavye sinyalini okumak 
// ra4 ucundaki düþen kenarda olusan data sinyali, ra3 deki clock sinyali 
//
#if defined (__PCM__)
#include <16F628A.h>
#use delay(clock=4000000)
#fuses NOWDT,PUT,NOLVP,NOBROWNOUT,MCLR,INTRC_IO,NOPROTECT
#endif

#byte PORTA = 0x05
#byte OPTION = 0x81
#byte TMR0 = 0x01

#byte veri = 0x25
#bit b_0 = 0x25.0
#bit b_1 = 0x25.1
#bit b_2 = 0x25.2
#bit b_3 = 0x25.3
#bit b_4 = 0x25.4
#bit b_5 = 0x25.5
#bit b_6 = 0x25.6
#bit b_7 = 0x25.7


void main()
{
   OPTION=0b00111000;          // TOCS=1   zamanlayýcý-0 için saat sinyal secme biti 
                               // TOSE=1   zamanlayýcý-0 iþaret kaynaðý kenar seçme biti
                               // PSA=1    ön bölücü atama biti
   veri=0;                     
   output_b(0);
   TMR0=0;
   
   while(TRUE)
   {      
      
      
               
         switch (TMR0)
         {
            case 1:   break;   // start biti
            case 4:   if(bit_test(PORTA.3)==1){b_0=1;} else{b_0=0;}   break;
            case 8:   if(bit_test(PORTA.3)==1){b_1=1;} else{b_1=0;}   break;
            case 12:   if(bit_test(PORTA.3)==1){b_2=1;} else{b_2=0;}   break;
            case 16:   if(bit_test(PORTA.3)==1){b_3=1;} else{b_3=0;}   break;
            case 20:   if(bit_test(PORTA.3)==1){b_4=1;} else{b_4=0;}   break;
            case 24:   if(bit_test(PORTA.3)==1){b_5=1;} else{b_5=0;}   break;
            case 28:   if(bit_test(PORTA.3)==1){b_6=1;} else{b_6=0;}   break;
            case 32:   if(bit_test(PORTA.3)==1){b_7=1;} else{b_7=0;}   break;
            case 10:  break;   // parity biti
            case 11:  TMR0=0;   break;   // stop biti
         }                  
      
      output_b(veri);
      
   }
}
