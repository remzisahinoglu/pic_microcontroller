//Pic kalavye iletiþim uygulamasý
//28.04.2007
//yazan = Remzi ÞAHÝNOÐLU
//
// klavyedeki karakterlerin binary karþýþýklarýný portb ye yazdýran program
// hýzlý yazýldýgý zaman hata veriyor :)
// RA3 = veri, RA4 = clock hattý

#if defined (__PCM__)
#include <16F628A.h>
#use delay(clock=4000000)
#fuses NOWDT,PUT,NOLVP,NOBROWNOUT,MCLR,INTRC_IO,NOPROTECT
#endif

#byte PORTA = 0x05
// veri adresini tanýmladým
#byte veri = 0x25
#bit b_0 = 0x25.0
#bit b_1 = 0x25.1
#bit b_2 = 0x25.2
#bit b_3 = 0x25.3
#bit b_4 = 0x25.4
#bit b_5 = 0x25.5
#bit b_6 = 0x25.6
#bit b_7 = 0x25.7

char clk=0, mev_veri=0;

void main()
{
   veri=0;                     
   output_b(0);
      
   while(TRUE)
   {            
      tekrar:
      if(bit_test(PORTA,4)==0)    // clock palsi -start biti-
      {     
         bekle:                         
         if(bit_test(PORTA,4)==0){goto bekle;}   // ilk yükselen kenar biti gelene kadar bekle 
                           
         clk=clk+1;               // ilk clock pals geldi
         bekle1:
         switch (clk)             // clock palse göre veriyi algýlayýp kaydet 
         {            
            case 1:   if(bit_test(PORTA,3)==1){b_0=1;} else{b_0=0;}   break;
            case 2:   if(bit_test(PORTA,3)==1){b_1=1;} else{b_1=0;}   break;
            case 3:   if(bit_test(PORTA,3)==1){b_2=1;} else{b_2=0;}   break;
            case 4:   if(bit_test(PORTA,3)==1){b_3=1;} else{b_3=0;}   break;
            case 5:   if(bit_test(PORTA,3)==1){b_4=1;} else{b_4=0;}   break;
            case 6:   if(bit_test(PORTA,3)==1){b_5=1;} else{b_5=0;}   break;
            case 7:   if(bit_test(PORTA,3)==1){b_6=1;} else{b_6=0;}   break;
            case 8:   if(bit_test(PORTA,3)==1){b_7=1;} else{b_7=0;}   break;            
         }                                       
         mev_veri = veri;         // veriyi mevcut veriye kaydet ardýndan veriyi sil ki 
                                  // sonraki veriler üst üste binmesin                  
         if(bit_test(PORTA,4)==1){goto bekle1;}   // diger clk gelene kadar sürekli algýla ve yaz
                           
         if(clk == 11)            // son clk mý?
         {clk=0; delay_ms(50);}   // evet clkyý sil ve -uzun çizgi (stop) için bekletme yaptým-
         else
         {goto tekrar;}           // hayýr tekrar veriyi almaya git                
      }      
      veri = 0;                   // sonraki veriler üst üste binmesin diye siliniyor
      output_b(mev_veri);         // veriyi portb ye çak      
   }
}
