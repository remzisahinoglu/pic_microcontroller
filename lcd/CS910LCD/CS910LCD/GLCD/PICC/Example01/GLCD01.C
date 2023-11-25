#include <18F452.h>
#fuses HS,NOWDT,NOPROTECT,NOLVP
#use delay(clock=20000000)
#use rs232(baud=9600,xmit=PIN_C6,rcv=PIN_C7)
//#define FAST_GLCD    // Try commenting this out to see the differences
#define GLCD_WIDTH	128

#include "HDM64GS12.c"
#include "graphics.c"
#include "math.h"
char str[20]="GLCD Example";
void main() {
   
   setup_adc_ports(RA0_ANALOG);
   setup_adc(ADC_CLOCK_INTERNAL);
   glcd_init(ON);
   
   puts("glcd_pixel()  \n\r");
   puts("Press any Key\n\r");
   getc();
  
   glcd_pixel(1,1,1);
   glcd_pixel(1,2,1);	
   glcd_pixel(1,3,1);
   glcd_pixel(1,4,1);	
   glcd_pixel(1,5,1);
   glcd_pixel(1,6,1);	
   
   glcd_pixel(1,1,1);
   glcd_pixel(2,1,1);	
   glcd_pixel(3,1,1);
   glcd_pixel(4,1,1);	
   glcd_pixel(5,1,1);
   glcd_pixel(6,1,1);	
   puts("glcd_fillScreen() Black color  \n\r");
   puts("Press any Key\n\r");
   getc();
   glcd_fillScreen(1);
   puts("glcd_fillScreen() White Color \n\r");
   puts("Press any Key\n\r");
   getc();
   glcd_fillScreen(0);
                                           
   puts("glcd_rect(),glcd_circle()\n\r");
   puts("Press any Key\n\r");
   getc();
                              
   glcd_rect(28,30 , 115, 50, NO, ON);            
   glcd_circle(64, 13, 10, YES, ON);             
   puts("glcd_text57\n\r");
   puts("Press any Key\n\r");
   getc();
   glcd_text57(40, 35,str,1, ON);        
  
      #ifdef FAST_GLCD
      glcd_update();
      #else
      delay_ms(100);    // Reduces flicker by allowing pixels to be on
                        // much longer than off
      #endif
   puts("End");
   
   while(1);
   
}
