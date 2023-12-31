///////////////////////////////////////////////////////////////////////////////
////                    xyz_kontrol_18F4620.C                             ////
////                                                                       ////
////  Bu program 3-dof delata robotu, x,y,z eksenleri do�rultusunda        ////
////  kullan�c� taraf�ndan 10 micron ile 10 cm hassasiyetle kumandadan     ////
////  kontrol edilmesini sa�lar.                                           ////
////                                                                       ////
////  Sisteme enerji verildi�inde lcd ekranda ilk olarak 3sn boyunca       ////
////  giri� ekran� g�r�nt�lenir. Ard�ndan "Position" ekran�nda robotun     ////
////  o anki pozisonu g�r�nt�lenir. lcd butonuna ardarda bas�larak         ////
////  "Angle", "Step Count" ve di�er ekranlar g�r�nt�lenebilir.            ////
////                                                                       ////
////  Robotun ilk kurulumdan sonra kalibrasyonundan ayar� yap�labilmesi    ////
////  i�in "Kalibre" butonu bir kez kontrol edilir. Ard�ndan eksen         ////
////  butonlar� i�in tarama k�s�r d�ng�ye girer. Bu esnada eksen           ////
////  butonlar�ndan girilen koordinat bilgisinin a�� de�erleri robotun     ////
////  s�f�r posizyonuna g�re hesaplan�r. Bu de�erlere g�re step motorun    ////
////  hareketi i�in ad�m de�erleri hesaplan�r. Ad�m de�erleri baz al�narak ////
////  �� motorunda ayn� zamanda �al��maya ba�lay�p sonlanmas� i�in ad�mlar ////
////  aras� zaman de�i�kenleri hesaplan�p motor ad�m bilgisi ile motor     ////
////  s�r�c� microdenetleyicilerine (PIC16F628A, xyz_kontrol_16F628A.C)    ////
////  g�nderilir.                                                          ////
////                                                                       ////
////  Motor s�r�c� microdenetleyicileri taraf�ndan al�nan bu de�erler      ////
////  pozitif ise motoru o de�er kadar ve hesaplanan gecikme s�releri      ////
////  ile motora hareket verir. De�er negatif ise ayn� �ekilde ters        ////  
////  y�nde hereket verir. E�er kumandadan girilen koordinat bilgisi       ////
////  robotun �al��ma uzay� d���nda ise lcd ekranda hata mesaj� g�r�t�-    ////
////  lenir ve kullan�c�dan �al��ma uzay� i�inde bir nokta girilmesi       ////
////  beklenir.                                                            ////
////                                                                       ////
////  Robot geometry:                                                      ////
////     e = 109.12;     // end effector                                   ////
////     f = 346.41;     // base                                           ////
////     re = 323.59;    // rod                                            ////
////     rf = 133.09;    // arm                                            ////
////                                                                       ////
////  Kullan�lan kontrolor 18F46K20 (32Kb, 4MHz), 16F628A(2Kb, 4MHz)       ////
////  LCD ekran HD44780 tabal� 4x16 HY-1604A06 alphanumeric numeric        ////
////                                                                       ////
////  LCD ekran pinleri: PORTB                                             ////
////     Vss 1: GND                  Data ba�lant�lar�                     ////
////     Vdd 2: +5V                                                        ////
////     Vee 3: 10k POT                                                    ////
////     RS  4: RB1                                                        ////
////     RW  5: RB2                                                        ////
////     E   6: RB0                                                        ////
////     D0-D3 7-10: No connection                                         ////
////     D4 11: RB4                                                        ////
////     D5 12: RB5                                                        ////
////     D6 13: RB6                                                        ////
////     D7 14: RB7                                                        ////
////                                                                       ////
////  XYZ eksen butonlar�: PORTD                                           ////
////     +X: RD0                                                           ////
////     -X: RD1                                                           ////
////     +Y: RD2                                                           ////
////     -Y: RD3                                                           ////
////     +Z: RD4                                                           ////
////     -Z: RD5                                                           ////
////                                                                       ////
////  Di�er butonlar: PORTD                                                ////
////     LCD         : RD6                                                 ////
////     Hassasiyet  : RD7                                                 ////
////        : RC2                                                          ////
////        : RC3                                                          ////
////        : RC4                                                          ////
////        : RC5                                                          ////
////                                                                       ////
///////////////////////////////////////////////////////////////////////////////
////  YAZAN: Remzi �AH�NO�LU                                               ////
////  TAR�H: 05.05.2011                                                    ////
////                                                                       ////
///////////////////////////////////////////////////////////////////////////////

//******************** yap�lacak i�lemler
// motorlar ad�m i�lemlerini tamamlad�ktan sonra geri bildirim yap�lmal�
// tx ucundan pc ye veri g�nderilmeli
// 3 pic i�in a de�i�kenini eproma y�klenmeli - eproma veri yazma i�lemi 10ms gecikme ile oldugundan devre yap�ld�ktan sonra denemeli

#if defined(__PCH__)
#include <18F4620.h>
#include <MATH.h> 
#use delay(clock=4000000)
#fuses XT, NOWDT, NOPROTECT, NOLVP, NOBROWNOUT, NOPUT, NOWRT, NODEBUG, NOCPD
#use rs232 (baud=9600, xmit=PIN_C6, rcv=PIN_c7, parity=N, stop=1)
#include <stdlib.h>                    // bu s�ralama dogru rs232 den altta olmal�
#include <input.c>
#define use_portb_lcd TRUE             // lcd bilgisi i�in port b�yi kullan�yoruz
#include <LCD416.c>                    // lcd i�in gerekecek fonksiyonlar�n bulundu�u dosya lcd416.c
#endif

//#device PIC18F4620

int degisim=1, degisim_motor=0, lcd_ekran=1, i=1;
// motor ad�m ve zaman de�i�kenler
signed long long mA_step=0, mB_step=0, mC_step=0, mA_step_old=0, mB_step_old=0, mC_step_old=0;
signed long long ma=0, mb=0, mc=0;
long long ta=0,tb=0,tc=0;
// inverse kinematic de�i�kenleri
float e,f,re,rf,x0,y0,z0,x02,x03,y1,y01,y02,y03,z02,z03,theta1,theta2,theta3,a,b,d,yj,zj;
float hassasiyet=1,tahvil_orani=0;

//********** write eeprom - float ********************************************//
void ee_write_float(unsigned int addr , float *floatptr)
{
unsigned char edata;
unsigned char I;
    for(I=0;I<4;I++){
      edata = *((unsigned char *)floatptr+I);
      write_eeprom(addr+I,edata);
   }
}
//********** read eeprom - float *********************************************//
void ee_read_float(unsigned int addr , float *floatptr)
{
unsigned char edata;
unsigned char I;
   for(I=0;I<4;I++){
      edata=read_eeprom(I+addr);   
        *((unsigned char *)floatptr+I) = edata;
   }
}
//********** �al��ma uzay� d���nda bir nokta girildi�inde ********************//
void no_point()
{
   lcd_send_byte(0,0x01);     // lcd ekran� temzile
   lcd_gotoxy(1,1);
   printf(lcd_putc, "hata: calisma");
   lcd_gotoxy(1,2);
   printf(lcd_putc, "uzayi icinde bir");
   lcd_gotoxy(1,3);
   printf(lcd_putc, "nokta giriniz");
   delay_ms(2000);
}
//******* kollar�n a�� de�erlerini hesaplama: theta1, theta2, theta3 *********//
void hesapla_aci()
{
   //********** theta 1 ******************************************************//
   y1 = -0.5 * 0.57735 * f;                  // f/2 * tg 30
   y01 = y0 -0.5 * 0.57735 * e;              // shift center to edge

   // z = a + b*y
   a = (x0*x0 + y01*y01 + z0*z0 +rf*rf - re*re - y1*y1)/(2*z0);
   b = (y1-y01)/z0;

   // discriminant
   d = -(a+b*y1)*(a+b*y1)+rf*(b*b*rf+rf);
   if (d > 0)
   {
      yj = (y1 - a*b - sqrt(d))/(b*b + 1);  // choosing outer point
      zj = a + b*yj;
      theta1 = 180.0*atan(-zj/(y1 - yj))/pi + ((yj>y1)?180.0:0.0);   // yj, y1 den k���kse 180; degilse 0 de�erini ��kar�r
   }
   else{no_point();}

   //********** theta 2 ******************************************************//
   x02 = x0*cos(120*pi/180) + y0*sin(120*pi/180);
   y02 = y0*cos(120*pi/180) - x0*sin(120*pi/180);
   z02 = z0;

   //y1 = cos(120)*(-0.5 * 0.57735 * f);
   y1 = -0.5 * 0.57735 * f;     // f/2 * tg 30
   y02 = y02 -0.5 * 0.57735 * e;    // shift center to edge
   
   // z = a + b*y
   a = (x02*x02 + y02*y02 + z02*z02 +rf*rf - re*re - y1*y1)/(2*z02);
   b = (y1-y02)/z02;
   
   // discriminant
   d = -(a+b*y1)*(a+b*y1)+rf*(b*b*rf+rf);
   if (d > 0)
   {
       yj = (y1 - a*b - sqrt(d))/(b*b + 1);  // choosing outer point
       zj = a + b*yj;
       theta2 = 180.0*atan(-zj/(y1 - yj))/pi + ((yj>y1)?180.0:0.0);    //yj, y1 den k���kse 180; degilse 0 de�erini ��kar�r
   }
   else{no_point();}
                 
   //********** theta 3 ******************************************************//
   
   x03 = x0*cos(120*pi/180) - y0*sin(120*pi/180);
   y03 = y0*cos(120*pi/180) + x0*sin(120*pi/180);
   z03 = z0;
   
   y1 = -0.5 * 0.57735 * f;     // f/2 * tg 30
   y03 = y03 -0.5 * 0.57735 * e;    // shift center to edge
   
   // z = a + b*y
   a = (x03*x03 + y03*y03 + z03*z03 +rf*rf - re*re - y1*y1)/(2*z03);
   b = (y1-y03)/z03;
   
   // discriminant
   d = -(a+b*y1)*(a+b*y1)+rf*(b*b*rf+rf);
   if (d > 0)
   {
       yj = (y1 - a*b - sqrt(d))/(b*b + 1);  // choosing outer point
       zj = a + b*yj;
       theta3 = 180.0*atan(-zj/(y1 - yj))/pi + ((yj>y1)?180.0:0.0);    //yj, y1 den k���kse 180; degilse 0 de�erini ��kar�r
   }
   else{no_point();}
}
//********** Motor ad�m ve zaman bilgilerinin hesaplan�p eproma kayd� ********//
void motor_hareket()
{
   mA_step = theta1 / tahvil_orani;         // ad�m de�erleri tam say� c�km�yor.
   mB_step = theta2 / tahvil_orani;         // c�kmas� i�in int olarak tan�mlanmas� gerekiyor.
   mC_step = theta3 / tahvil_orani;         // e�er tan�mlan�rsa 10.85 c�kan ad�m de�eri 10 olarak al�n�yor
              
   //********** motora g�nderilecek ad�m de�erlerinin hesaplanmas� ***********//
   
   // motor A
   if(mA_step >= 0)
   {
      if(mA_step > mA_step_old)           // motor ileri
      {
         ma = mA_step - mA_step_old;         // ma positive
         mA_step_old = mA_step;
      }
      else                                // motor geri
      {
         ma = mA_step - mA_step_old;         // ma negative
         mA_step_old = mA_step;  
      }
   }
   else  // mA_step < 0
   {
      if(mA_step < mA_step_old)           // motor geri
      {
         ma = mA_step - mA_step_old;
         mA_step_old = mA_step;              // ma negative
      }
      else                                // motor ileri
      {
         ma = mA_step - mA_step_old;         // ma positive
         mA_step_old = mA_step;
      }
   }
   
   // motor B
   if(mB_step >= 0)
   {
      if(mB_step > mB_step_old)           // motor ileri
      {
         mb = mB_step - mB_step_old;         // mb positive
         mB_step_old = mB_step;
      }
      else                                // motor geri
      {
         mb = mB_step - mB_step_old;         // mb negative
         mB_step_old = mB_step;  
      }
   }
   else  // mB_step < 0
   {
      if(mB_step < mB_step_old)           // motor geri
      {
         mb = mB_step - mB_step_old;
         mB_step_old = mB_step;              // mb negative
      }
      else                                // motor ileri
      {
         mb = mB_step - mB_step_old;         // mb positive
         mB_step_old = mB_step;
      }
   }
   
   // motor C
   if(mC_step >= 0)
   {
      if(mC_step > mC_step_old)           // motor ileri
      {
         mc = mC_step - mC_step_old;         // mc positive
         mC_step_old = mC_step;
      }
      else                                // motor geri
      {
         mc = mC_step - mC_step_old;         // mc negative
         mC_step_old = mC_step;  
      }
   }
   else  // mC_step < 0
   {
      if(mC_step < mC_step_old)           // motor geri
      {
         mc = mC_step - mC_step_old;
         mC_step_old = mC_step;              // mc negative
      }
      else                                // motor ileri
      {
         mc = mC_step - mC_step_old;         // mc positive
         mC_step_old = mC_step;
      }
   }

   // en b�y�k ad�m� bulma ve ona g�re zaman de�i�kenlerini hesaplama//
   if(labs(mb) > labs(mc))        // labs(): long t�r�nden bir say�s�n�n mutlak de�erini al
   {
      if(labs(mb) > labs(ma))      
      {
         // b en b�y�k
         tb = 15;                                     // tb = 15ms
         ta = (labs(mb)/labs(ma))*tb;
         tc = (labs(mb)/labs(mc))*tb;                                            
      }
      else
      {
         // a en b�y�k
         ta = 15;                                     // ta = 15ms
         tb = (labs(ma)/labs(mb))*ta;
         tc = (labs(ma)/labs(mc))*ta;
      }
   }
   else
   {
      if(labs(mc) > labs(ma))
      {
         // c en b�y�k
         tc = 15;                                     // tc = 15ms
         ta = (labs(mc)/labs(ma))*tc;
         tb = (labs(mc)/labs(mb))*tc;
      }
      else
      {
         // a en b�y�k
         ta = 15;                                     // ta = 15ms
         tb = (labs(ma)/labs(mb))*ta;
         tc = (labs(ma)/labs(mc))*ta;
      }           
   }
   
   //****** eeroma ad�m bilgilerini ve a�� de�erlerini kaydet *******//
   
   ee_write_float(0x00, &x0); delay_ms(10);           // konum degerlerini eproma yaz
   ee_write_float(0x04, &y0); delay_ms(10);  
   ee_write_float(0x08, &z0); delay_ms(10);
   
   //***** ad�m bilgileri ve zaman de�i�kenlerini s�rayla g�nder ****//
   
   // A motoruna g�nder
   output_low(PIN_C0);     // S0=0
   output_low(PIN_C1);     // S1=0
   delay_us(100);
   printf("%ld\r",ma);
   printf("%ld\r",ta);
   delay_ms(5);
   
   // B motoruna g�nder
   output_low(PIN_C0);     // S0=0
   output_high(PIN_C1);    // S1=1
   delay_us(100);
   printf("%ld\r",mb);
   printf("%ld\r",tb);
   delay_ms(5);
   
   // C motoruna g�nder
   output_high(PIN_C0);    // S0=1
   output_low(PIN_C1);     // S1=0
   delay_us(100);
   printf("%ld\r",mc);
   printf("%ld\r",tc);
   delay_ms(5);
   
   
   // PC ye g�nder
   output_high(PIN_C0);    // S0=1
   output_high(PIN_C1);    // S1=0
   delay_us(100);
   printf("data gonderildi: mA:%ld, ta:%ld, mB:%ld, tb:%ld, mC:%ld, tc:%ld\n\r", ma, ta, mb, tb, mc, tc);
   delay_ms(5);
}


//************************* ANA PROGRAM **************************************//
void main()
{
   setup_PSP(PSP_DISABLED);
   setup_spi(SPI_SS_DISABLED);
   setup_timer_1(T1_DISABLED);
   setup_timer_2(T2_DISABLED,0,1);
   setup_adc_ports(NO_ANALOGS);
   setup_adc(ADC_OFF);
   setup_CCP1(CCP_OFF);
   setup_CCP2(CCP_OFF);
   
   set_tris_a(0b00001111);       // port a giri�
   set_tris_c(0b10000000);       // port c ��k�� RC7-RX giri�
   set_tris_d(0b11111111);       // port d giri�
   set_tris_e(0b00001000);       // port d ��k�� mclr giri�
   
   output_c(0);
   output_d(0);
   output_e(0);
   
   lcd_init();    // lcdyi haz�rla
   
//**************** proje bilgilerini ekranda yazd�r***************************//
   lcd_gotoxy(1,1);                                      // 1.sutun 1.sat�r
   printf(lcd_putc, "MARMARA UNI");
   delay_ms(500);
   lcd_gotoxy(1,2);                                      // 1.sutun 2.sat�r
   printf(lcd_putc, "MAKINE MUH");   
   delay_ms(500);
   lcd_gotoxy(1,3);                                      // 1.sutun 3.sat�r
   printf(lcd_putc, "DELTA ROBOT PROJ");
   delay_ms(500);
   lcd_gotoxy(1,4);                                      // 1.sutun 4.sat�r
   printf(lcd_putc, "04.05.2011");
   delay_ms(1500);
   
   lcd_send_byte(0,0x01);                                // lcd ekran� temzile
   
//************************** robot geometri **********************************//
   e = 109.12;     // end effector
   f = 346.41;     // base
   re = 323.59;    // rod
   rf = 133.09;    // arm
   
   tahvil_orani = 0.008948849;

//***************** robotu kalibre et ****************************************//

   if(input(PIN_A2)==0)                         // kalibre butonu RA2 bas�l�m�
   {
      theta1=theta2=theta3=0;
      ma=mb=mc=0;
      ta=tb=tc=0;
      x0=0;
      y0=0;
      z0=-253.1246;
      
      ee_write_float(0x00, &x0); delay_ms(10);           // rototun s�f�r pozisyonunda 
      ee_write_float(0x04, &y0); delay_ms(10);  
      ee_write_float(0x08, &z0); delay_ms(10);
      
      lcd_gotoxy(1,1);
      printf(lcd_putc, "Kalibre edildi.");
      delay_ms(200);
      lcd_gotoxy(1,2);
      printf(lcd_putc, "Robot suanda");
      lcd_gotoxy(1,3);
      printf(lcd_putc, "sifir pozisyo-");
      lcd_gotoxy(1,4);
      printf(lcd_putc, "nunda olmali.");
      delay_ms(3000);
      
      lcd_send_byte(0, 0x01);
   }

//***************** robotun konumunu eepromdan oku ***************************//
   
   ee_read_float(0x00, &x0);
   delay_ms(5);
   ee_read_float(0x04, &y0);
   delay_ms(5);
   ee_read_float(0x08, &z0);
   delay_ms(5);
   
   hesapla_aci();
   degisim=1;

//******************************* Ana D�ng� **********************************//
   while(TRUE)
   {
   //************************* buton taramas� ********************************//     
      if(input(PIN_A3)==1)
      {
      
         if(input(PIN_D0)==0)                         // +x do�rultusunda 1mm git
         {
            delay_ms(100);         
            x0+=hassasiyet; 
            degisim=1;
            degisim_motor=1;
            hesapla_aci();
         }                   
         if(input(PIN_D1)==0)                         // -x do�rultusunda 1mm git
         {
            delay_ms(100);
            x0-=hassasiyet;
            degisim=1;
            degisim_motor=1;
            hesapla_aci();         
         }
         if(input(PIN_D2)==0)                         // +y do�rultusunda 1mm git
         {
            delay_ms(100);
            y0+=hassasiyet;
            degisim=1;
            degisim_motor=1;
            hesapla_aci();
         }
         if(input(PIN_D3)==0)                         // -y do�rultusunda 1mm git
         {
            delay_ms(100);
            y0-=hassasiyet;
            degisim=1;
            degisim_motor=1;
            hesapla_aci();    
         }
         if(input(PIN_D4)==0)                         // +z do�rultusunda 1mm git
         {
            delay_ms(100);
            z0+=hassasiyet;
            degisim=1;
            degisim_motor=1;
            hesapla_aci();      
         }
         if(input(PIN_D5)==0)                         // -z do�rultusunda 1mm git
         {
            delay_ms(100);
            z0-=hassasiyet;
            degisim=1;
            degisim_motor=1;
            hesapla_aci(); 
         }
      }
      
      if(input(PIN_D6)==0)                         // ekran� de�i�tir
      {
         delay_ms(500);                            // butondaki ark� s�nd�r
         lcd_ekran = lcd_ekran + 1;
         degisim=1;
         if(lcd_ekran == 5){lcd_ekran=1;}         
      }
      
      if(input(PIN_D7)==0)                         // hassasiyet artt�r butonu
      {
         delay_ms(500);                            // butondaki ark� s�nd�r aksi halde art�s muazzam boyutta olur
         i+=1;                                     // 4 kademeli hassasiyet art�m�
         if(i >= 6){i=1;}                          
         switch(i)
         {
            case 1: hassasiyet = 0.01; break;   // 10 micron
            case 2: hassasiyet = 0.1;  break;
            case 3: hassasiyet = 1;    break;
            case 4: hassasiyet = 10;   break;
            case 5: hassasiyet = 100;  break;   // 10 cm
         }
         degisim=1;
         lcd_ekran=4;
      }

      //***** de�i�im oldugunda ekran� yenile ve bilgileri eproma kaydet *****//
      if(degisim == 1)
      {
         degisim = 0;                                         
         //******** step motor kontrol ***************************************//
         if(degisim_motor == 1)     // sadece motor verileri de�i�tiginde
         {
            degisim_motor = 0;
            motor_hareket();        // motoru hareket ettir
         }           
         //******************** Ekran G�r�n�m� *******************************//         
         switch(lcd_ekran)                            // istenilen ekranlar� g�ster 
         {
            case 1:  // posiyon bilgilerini g�ster
                     degisim = 0;
                     lcd_send_byte(0,0x01);                    // ekran� temizle
                     lcd_gotoxy(1,1);                          // SUTUN , SATIR
                     printf(lcd_putc, "POSITION (mm)");     
            
                     lcd_gotoxy(1,2);
                     printf(lcd_putc, "X: %4.4f", x0);         // %4.4f= float t�r�nden bir say�y� xxxx.xxxx �eklinde ekrana yans�t�r
                     
                     lcd_gotoxy(1,3);
                     printf(lcd_putc, "Y: %4.4f", y0);
                     
                     lcd_gotoxy(1,4);
                     printf(lcd_putc, "Z: %4.4f", z0);  break;
                     
            case 2:  // ac� bilgilerini g�ster
                     degisim=0;
                     lcd_send_byte(0,0x01);                    // ekran� temizle
                     lcd_gotoxy(1,1);                          // SUTUN , SATIR
                     printf(lcd_putc, "ANGLE (derece)");
                     
                     lcd_gotoxy(1,2);
                     printf(lcd_putc, "A: %4.6f", theta1);        // %f : float t�r�nden
                     
                     lcd_gotoxy(1,3);
                     printf(lcd_putc, "B: %4.6f", theta2);        // %E : virg�lden sonra 6 hane ve E00 seklinde g�sterimi
                     
                     lcd_gotoxy(1,4);
                     printf(lcd_putc, "B: %4.6f", theta3);  break;
                     
            case 3:  // step motor ad�m bilgilerini g�ster
                     degisim=0;
                     lcd_send_byte(0,0x01);
                     lcd_gotoxy(1,1);
                     printf(lcd_putc, "STEP COUNT(step)");
            
                     lcd_gotoxy(1,2);
                     printf(lcd_putc, "MA: %ld", mA_step);      // %ld long ifade decimal t�r�nden
                     
                     lcd_gotoxy(1,3);
                     printf(lcd_putc, "MB: %ld", mB_step);
                     
                     lcd_gotoxy(1,4);
                     printf(lcd_putc, "MC: %ld", mC_step);  break;
                     
            case 4:  // hassasiyet bilgisini g�ster
                     degisim=0;
                     lcd_send_byte(0,0x01);
                     lcd_gotoxy(1,1);
                     printf(lcd_putc, "HASSASIYET");
                     
                     lcd_gotoxy(1,2);
                     printf(lcd_putc, "0.01 < H < 100");
                     
                     lcd_gotoxy(1,3);
                     printf(lcd_putc, "arasinda olmali");
                     
                     lcd_gotoxy(1,4);
                     printf(lcd_putc, "H: %f mm", hassasiyet); break;
         
         }
      }
   }
}

