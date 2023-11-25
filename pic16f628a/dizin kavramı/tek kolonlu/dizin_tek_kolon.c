//dizi kavramý uygulamasý
//12.02.2007
//yazan = Remzi ÞAHÝNOÐLU
#if defined (__PCM__)
#include <16F628A.h>
#use delay(clock=4000000)
#fuses NOWDT,PUT,NOLVP,NOBROWNOUT,MCLR,INTRC_IO,NOPROTECT
#endif

int a[]={1,2,4,8};         // dizin olusturuldu

// a için verilen elemanlarýn bellekteki görünümü aþaðýdaki gibi olacaktýr
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
   {                         // x nin degeri a dizinindeki degere karsýlýk gelip 
      dizi = a[x];           // deger dizin saklayýcýsýna atandý
      output_b (dizi);       // dizin saklayýcýsýndaki deðer portb ye gönderildi
      delay_ms(200);   
   }
   goto tekrar;
}   
