//dizi kavram� �ift kolon uygulamas�
//12.02.2007
//yazan = Remzi �AH�NO�LU
#if defined (__PCM__)
#include <16F628A.h>
#use delay(clock=4000000)
#fuses NOWDT,PUT,NOLVP,NOBROWNOUT,MCLR,INTRC_IO,NOPROTECT
#endif

int a[3][5]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};      // dizin olusturuldu
                                                                                                         
// a i�in verilen elemanlar�n bellekteki g�r�n�m� a�a��daki gibi olacakt�r
//     x  y  
//   a[0][0]=1   a[0][1]=2   a[0][2]=3   a[0][3]=4   a[0][4]=5  
//   a[1][0]=6   a[1][1]=7   a[1][2]=8   a[1][3]=9   a[1][4]=10      
//   a[2][0]=11  a[2][1]=12  a[2][2]=13  a[2][3]=14  a[2][4]=15     

void main()
{
int dizi;
int x=0, y=0;

tekrar:                      // dizin elamanlar� s�rayla syd�r���p portb ye g�nderilir 
x=0;                         // s�rekli g�ng� i�in degerler s�f�rlan�r
y=0;
   for (x=0; x<3; x=x+1)     // ilk �nce x kontrol edilir. 
   {                          
      for(y=0; y<4; y=y+1)   // sonra y kontrol edilip
      {
      
      dizi = a[x][y];        // de�er dizine atan�r ve portb ye g�nderilir
      output_b (dizi);       // ilk d�ng�de x=1 bu d�ng�de y 1'den 4'e kadar say�l�r
      delay_ms(200);         // 4 oldu�unda d�ng�den c�k�l�r   
      }
      y=0;                   // ilk d�ng�de x=2 yap�larak i�lem deveam edilir 
   }                         // x=2 demek dizinde ikinci sat�ra ge�ilmi� demektir
   goto tekrar;              // y=1 demek dizinde x in oldu�u sat�rdaki 1.s�tuna ge�ilmi� demektir 
}  

