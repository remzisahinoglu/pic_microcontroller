//dizi kavram� uygulamas�
//12.02.2007
//yazan = Remzi �AH�NO�LU
#if defined (__PCM__)
#include <16F628A.h>
#use delay(clock=4000000)
#fuses NOWDT,PUT,NOLVP,NOBROWNOUT,MCLR,INTRC_IO,NOPROTECT
#endif

int a[]={1,2,4,8};         // dizin olusturuldu

// a i�in verilen elemanlar�n bellekteki g�r�n�m� a�a��daki gibi olacakt�r
//     x 
//   a[0]=1
//   a[1]=2      
//   a[2]=4 
//   a[3]=8 
//

void main()
{
int dizi;
int x=0;

tekrar:
x=0;
   for (x=0; x<3; x=x+1)
   {                         // x nin degeri a dizinindeki degere kars�l�k gelip 
      dizi = a[x];           // deger dizin saklay�c�s�na atand�
      output_b (dizi);       // dizin saklay�c�s�ndaki de�er portb ye g�nderildi
      delay_ms(200);   
   }
   goto tekrar;
}   
