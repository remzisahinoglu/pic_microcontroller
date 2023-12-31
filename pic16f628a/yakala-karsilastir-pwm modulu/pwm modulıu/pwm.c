// CCP birimini kullnamadan pwm kontrol uygulmas� 
// 12.03.2011
// yazan = Remzi �AH�NO�LU

// 20 ms periyod ve 1.5 ms duty cycle i�in pwm modu uygulams�
//
// pwm periyodu hesaplama formul�
// Tpwm = Tosc x 4 x (PR2 + 1) x (TMR2 b�lme oran�)
// PR2 periyod s�resini belirleyen de�er
// TMR2 b�lme oran� ise T2_DIV_BY_16 ifadesindeki 16 de�eri
//
// pwm frekans� hesaplama form�l�
// fpwm = 1 / Tpwm
//
// 20 ms periyod i�in PR2 ye girilmesi gereken de�er 1249
//
//

#if defined (__PCM__)
#include <16F628A.h>
#use delay(clock=4000000)
#fuses NOWDT,PUT,NOLVP,NOBROWNOUT,NOMCLR,INTRC_IO,NOPROTECT
#endif

#byte PORTA = 0x05
int16 i=10;

void main()
{  
   setup_timer_1(T1_DISABLED);            // timer1 i kapat
   
   set_tris_a(0x03);                      // a0 ve a1 giri� digerleri c�k��
   set_tris_b(0x00);                      // b nin hepsi c�k�s
   
   setup_ccp1(CCP_PWM);                   // ccp birimini pwm moduna al
   
   setup_timer_2(T2_DIV_BY_16,1249,1);    // timer2 yi kur
   
   //set_pwm1_duty(i);                    // duty cycle s�resi 10
   
   while(TRUE)
   {
      i = 10;
      set_pwm1_duty(i);
      
   
   }

}

