// dizin i�ersindeki veriyi g�ndermek - basar�l�
//21.09.2009
//yazan = Remzi �AH�NO�LU
#if defined (__PCM__)
#include <16F877A.h>
#fuses XT,NOWDT,NOPROTECT,NOBROWNOUT,NOLVP,NOPUT,NOWRT,NODEBUG,NOCPD
#use delay (clock=4000000) // Gecikme fonksiyonu i�in kullan�lacak osilat�r frekans� belirtiliyor.
#use rs232 (baud=9600, xmit=pin_C6, rcv=pin_C7, parity=N, stop=1) // RS232 protokol�n�n 9600 bit/sn baud h�z�nda olaca��n� ve
                                                                  // TX,RX u�lar�n�n hangi pinler olaca��n� tan�ml�yor
                                                                  // parity bitinin olmad���n�, stop bitinin 1 bit olaca�� belirtiliyor
char data[30];  // 80 bytel�k data saklay�c�s� olustur
#byte reg = 0x72
#byte x = 0x71
#byte porta = 0x05    

#int_rda   // RX ucuna veri gelince meydane gelen kesme alt program� buradan devam eder
// MESAJ GELD� UYARI +CMTI: "MT",1  �EKL�NDE
void serihaberlesme_kesmesi ()
{
   disable_interrupts(int_rda); // int_rda kesmesini pasif yap
   output_high(pin_c5);  // RC5 ��k��� lojik-1
      
   data[0]="A";          // A karakterini kayedet dizinin 0. sat�r�na
   data[1]="b";
   
   output_low(pin_c5);   // RC5 ��k��� lojik-0
   
}

/********* ANA PROGRAM FONKS�YONU********/

void main ( )
{
   setup_psp(PSP_DISABLED);        // PSP birimi devre d���
   setup_timer_1(T1_DISABLED);     // T1 zamanlay�c�s� devre d���
   setup_timer_2(T2_DISABLED,0,1); // T2 zamanlay�c�s� devre d���
   setup_adc_ports(NO_ANALOGS);    // ANALOG giri� yok
   setup_adc(ADC_OFF);             // ADC birimi devre d���
   setup_CCP1(CCP_OFF);            // CCP1 birimi devre d���
   setup_CCP2(CCP_OFF);            // CCP2 birimi devre d���

   output_low(pin_c5); // RC5 ��k��� ilk anda s�f�rlan�yor

   enable_interrupts(GLOBAL);      // Aktif edilen t�m kesmelere izin ver

   while(TRUE) // Sonsuz d�ng�
   {  
      
      enable_interrupts(int_rda);   // int_rda kesmesi aktif
      if(bit_test(porta,0)==1)
      {
         putc(data[0]);                     // dizin i�ersindeki veriyi gonder
         putc(data[1]);
         printf("atd 05358574412\r");       // \r=0D ENTER-SATIR BA�I, \n=0A SATIR ATLA  
         delay_ms(1000);                    // butondaki ark� s�nd�rmek i�in bekle
         for (x=0; x<31; x=x+1)
         {                          // x nin degeri 80 den k���k oldugu s�rece  
            data[x]=0;              // d�ng�n�n i�ini yap
         }
      }
   }
}
