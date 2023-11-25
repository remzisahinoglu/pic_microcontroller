//dizi kavramý çift kolon uygulamasý
//12.02.2007
//yazan = Remzi ÞAHÝNOÐLU
#if defined (__PCM__)
#include <16F628A.h>
#use delay(clock=4000000)
#fuses NOWDT,PUT,NOLVP,NOBROWNOUT,MCLR,INTRC_IO,NOPROTECT
#endif

int a[3][5]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};      // dizin olusturuldu
                                                                                                         
// a için verilen elemanlarýn bellekteki görünümü aþaðýdaki gibi olacaktýr
//     x  y  
//   a[0][0]=1   a[0][1]=2   a[0][2]=3   a[0][3]=4   a[0][4]=5  
//   a[1][0]=6   a[1][1]=7   a[1][2]=8   a[1][3]=9   a[1][4]=10      
//   a[2][0]=11  a[2][1]=12  a[2][2]=13  a[2][3]=14  a[2][4]=15     

void main()
{
int dizi;
int x=0, y=0;

tekrar:                      // dizin elamanlarý sýrayla sydýrýþýp portb ye gönderilir 
x=0;                         // sürekli göngü için degerler sýfýrlanýr
y=0;
   for (x=0; x<3; x=x+1)     // ilk önce x kontrol edilir. 
   {                          
      for(y=0; y<4; y=y+1)   // sonra y kontrol edilip
      {
      
      dizi = a[x][y];        // deðer dizine atanýr ve portb ye gönderilir
      output_b (dizi);       // ilk döngüde x=1 bu döngüde y 1'den 4'e kadar sayýlýr
      delay_ms(200);         // 4 olduðunda döngüden cýkýlýr   
      }
      y=0;                   // ilk döngüde x=2 yapýlarak iþlem deveam edilir 
   }                         // x=2 demek dizinde ikinci satýra geçilmiþ demektir
   goto tekrar;              // y=1 demek dizinde x in olduðu satýrdaki 1.sütuna geçilmiþ demektir 
}  

