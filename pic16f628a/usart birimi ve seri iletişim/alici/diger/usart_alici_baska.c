//usart birimi seri iletiþim alýcý uygulamasý
//20.02.2007
//yazan = Remzi ÞAHÝNOÐLU
#if defined (__PCM__)
#include <16F628A.h>
#fuses NOWDT,PUT,NOLVP,NOBROWNOUT,MCLR,INTRC_IO,NOPROTECT 
#use delay(clock=4000000) 
#use rs232(baud=9600, rcv=PIN_B1, xmit=PIN_B2) 
#endif

#byte porta = 5 
#byte portb = 6 
#bit  led = portb.1 

char gelen; 

void main() { 
     SET_TRIS_A(0x01); 
     SET_TRIS_B(0x02); 
     porta=portb=0; 
     led=1; 
     printf("Start\n"); 
     for(;;) { 
        gelen=getc(); 
        printf("%c",gelen); 
        if (gelen=='1') { 
           led=0; 
        } 
        else if (gelen=='2') { 
           led=1; 
        } 
     } 
} 
