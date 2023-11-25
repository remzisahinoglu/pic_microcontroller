// dizin içersindeki veriyi göndermek - basarýlý
//21.09.2009
//yazan = Remzi ÞAHÝNOÐLU
#if defined (__PCM__)
#include <16F877A.h>
#fuses XT,NOWDT,NOPROTECT,NOBROWNOUT,NOLVP,NOPUT,NOWRT,NODEBUG,NOCPD
#use delay (clock=4000000) // Gecikme fonksiyonu için kullanýlacak osilatör frekansý belirtiliyor.
#use rs232 (baud=9600, xmit=pin_C6, rcv=pin_C7, parity=N, stop=1) // RS232 protokolünün 9600 bit/sn baud hýzýnda olacaðýný ve
                                                                  // TX,RX uçlarýnýn hangi pinler olacaðýný tanýmlýyor
                                                                  // parity bitinin olmadýðýný, stop bitinin 1 bit olacaðý belirtiliyor
char data[30];  // 80 bytelýk data saklayýcýsý olustur
#byte reg = 0x72
#byte x = 0x71
#byte porta = 0x05    

#int_rda   // RX ucuna veri gelince meydane gelen kesme alt programý buradan devam eder
// MESAJ GELDÝ UYARI +CMTI: "MT",1  ÞEKLÝNDE
void serihaberlesme_kesmesi ()
{
   disable_interrupts(int_rda); // int_rda kesmesini pasif yap
   output_high(pin_c5);  // RC5 çýkýþý lojik-1
      
   data[0]="A";          // A karakterini kayedet dizinin 0. satýrýna
   data[1]="b";
   
   output_low(pin_c5);   // RC5 çýkýþý lojik-0
   
}

/********* ANA PROGRAM FONKSÝYONU********/

void main ( )
{
   setup_psp(PSP_DISABLED);        // PSP birimi devre dýþý
   setup_timer_1(T1_DISABLED);     // T1 zamanlayýcýsý devre dýþý
   setup_timer_2(T2_DISABLED,0,1); // T2 zamanlayýcýsý devre dýþý
   setup_adc_ports(NO_ANALOGS);    // ANALOG giriþ yok
   setup_adc(ADC_OFF);             // ADC birimi devre dýþý
   setup_CCP1(CCP_OFF);            // CCP1 birimi devre dýþý
   setup_CCP2(CCP_OFF);            // CCP2 birimi devre dýþý

   output_low(pin_c5); // RC5 çýkýþý ilk anda sýfýrlanýyor

   enable_interrupts(GLOBAL);      // Aktif edilen tüm kesmelere izin ver

   while(TRUE) // Sonsuz döngü
   {  
      
      enable_interrupts(int_rda);   // int_rda kesmesi aktif
      if(bit_test(porta,0)==1)
      {
         putc(data[0]);                     // dizin içersindeki veriyi gonder
         putc(data[1]);
         printf("atd 05358574412\r");       // \r=0D ENTER-SATIR BAÞI, \n=0A SATIR ATLA  
         delay_ms(1000);                    // butondaki arký söndürmek için bekle
         for (x=0; x<31; x=x+1)
         {                          // x nin degeri 80 den küçük oldugu sürece  
            data[x]=0;              // döngünün içini yap
         }
      }
   }
}
