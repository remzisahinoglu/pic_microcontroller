// analog dijital dönüþtürücü ile step motoru ileri geri kontrol uygulamasý
// 22.04.2007
// yazan = Remzi ÞAHÝNOÐLU
#if defined (__PCM__)
#include <16F877A.h>
#use delay(clock=4000000)
#fuses XT,NOWDT,NOPROTECT,NOLVP,NOBROWNOUT
#endif

#byte PORTC = 0x07

// analog verisi için son 4 biti tanýmla
#bit d_1 = 0x20.4
#bit d_2 = 0x20.5
#bit d_3 = 0x20.6
#bit d_4 = 0x20.7
char deger=0, veri = 0, sol=0, sag=0;

void motor_sola()
{
   output_b(1);  delay_ms(15);
   output_b(2);  delay_ms(15);
   output_b(4);  delay_ms(15);
   output_b(8);  delay_ms(15);
   output_b(1);  delay_ms(15);
   output_b(2);  delay_ms(15);
   output_b(4);  delay_ms(15);
   output_b(8);  delay_ms(15);
}
void motor_saga()
{
   output_b(8);  delay_ms(15);
   output_b(4);  delay_ms(15);
   output_b(2);  delay_ms(15);
   output_b(1);  delay_ms(15);
   output_b(8);  delay_ms(15);
   output_b(4);  delay_ms(15);
   output_b(2);  delay_ms(15);
   output_b(1);  delay_ms(15);
}

void main()
{
   setup_adc(ADC_CLOCK_INTERNAL);   // Sets up the a/d mode like off, the adc clock etc.
   setup_adc_ports(AN0);            // Sets the available adc pins to be analog or digital.
   set_adc_channel(0);              // Specifies the channel to be use for the a/d call.
   delay_us(10);                    // a small delay is required after setting the channel 
       
   while(TRUE)
   {  
      veri = read_adc();                                  // analog bilgiyi veriye yaz                                                          // deger i sýfýrla
      if(bit_test(veri,4) == 1){d_1=1;} else{d_1=0;}      // son 4 bitini oku ve degere yaz
      if(bit_test(veri,5) == 1){d_2=1;} else{d_2=0;}
      if(bit_test(veri,6) == 1){d_3=1;} else{d_3=0;}
      if(bit_test(veri,7) == 1){d_4=1;} else{d_4=0;}
   
      switch(deger)       // **** DÝREKSÝYONUN KONUMU ****
      {
         case 0   :       // direksiyon "3.SAÐ" konumda
            if(sol == 2)
            {
               motor_sola();
               sag=2;
               sol=6;
            }break;           
         case 32  :       // direksiyon "2.SAÐ" konumda
            if(sol == 1)
            {
               motor_sola();
               sol=2;
               sag=3;
            }
                     if(sag ==2)
                     {
                        motor_saga();
                        sol=2;
                        sag=3;
                     }break;
         case 80  :       // direksiyon "1.SAÐ" konumda
            if(sol == 0)
            {
               motor_sola();
               sol=1;
               sag=1;
            }
                     if(sag == 3)
                     {
                        motor_saga();
                        sol=1;
                        sag=1;
                     }break;
         case 112 :       // direksiyon "DÜZ" konumda
            if(sag ==1)
            {
               motor_saga();
               sol=0;
               sag=0;
            }
                     if(sol ==4)
                     {
                        motor_sola();
                        sol=0;
                        sag=0;
                     }break;           
         case 144 :       // direksiyon "1.SOL" konumda 
            if(sag ==0)
            {
               motor_saga();
               sol=4;
               sag=2;
            }
                     if(sol ==5)
                     {
                        motor_sola();
                        sol=4;
                        sag=2;
                     }break;                             
         case 192 :       // direksiyon "2.SOL" konumda 
            if(sag ==2)
            {
               motor_saga();
               sol=5;
               sag=3;
            }
                     if(sol ==6)
                     {
                        motor_sola();
                        sol=5;
                        sag=3;
                     }break;                         
         case 224 :       // direksiyon "3.SOL" konumda 
            if(sag ==3)
            {
               motor_saga();
               sol=6;
               sag=6;
            }break;                          
      }
   }
}
