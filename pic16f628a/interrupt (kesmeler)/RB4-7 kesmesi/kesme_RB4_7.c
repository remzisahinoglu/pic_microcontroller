//RB4-7 harici kesme uygulamasý
//12.02.2007
//yazan = Remzi ÞAHÝNOÐLU
#if defined (__PCM__)
#include <16F628A.h>
#use delay(clock=4000000)
#fuses NOWDT,PUT,NOLVP,NOBROWNOUT,MCLR,INTRC_IO,NOPROTECT
#endif

#byte STATUS = 0x03
#byte OPTION = 0x81
#byte INTCON = 0x0B

#INT_RB                            // kesme geldiðinde program akýsý
void RB_isr()                      // buradan devam eder
{     
   output_a(1);                    // 1 degeri a portuna gönderilir.                                                                                        
}                                  // kesme iþlemi biter ve program 
                                   // kaldýðý yerden devam eder
void main()
{  
   set_tris_b(16);                 // RB4-7 uclarý giriþ yapýlýp boþta kalan      
                                   // uçlar direnç ile 1'e cekilmelidir
   enable_interrupts(INT_RB);      // RB4-7 harici kesmesi aktif yapýlýr
   enable_interrupts(GLOBAL);      // kesmelere izin verilir
   bit_clear(OPTION,6);            // kesme düþen kenarda oluþsun
                
tekrar:                            // pice ilk enerji verildiðinde 
   output_a(1);                    // 1-2-4-8 olarak 1 sn aralýkla      
   delay_ms(1000);                 // saymaya baslar    
   output_a(2);                    // bu deðerler portaya çýkýþ olarak verilir
   delay_ms(1000);
   output_a(4);
   delay_ms(1000);
   output_a(8);
   delay_ms(1000);
   goto tekrar;   
}
