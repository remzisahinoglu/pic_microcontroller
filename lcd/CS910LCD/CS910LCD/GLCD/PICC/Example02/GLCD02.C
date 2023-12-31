#include <18F452.h>
#fuses HS,NOWDT,NOPROTECT,NOLVP
#use delay(clock=20000000)
//#define FAST_GLCD    // Try commenting this out to see the differences
#define GLCD_WIDTH	128

#include "HDM64GS12.c"
#include "graphics.c"
#include "math.h"

void displayVoltage(int adc) {
   char voltage[9];
   sprintf(voltage, "%f", (float)adc * .01960784); // Converts adc to text
   voltage[4] = '\0';                              // Limit shown digits to 3
   glcd_rect(45, 18, 69, 25, YES, OFF);            // Clear the old voltage
   glcd_text57(45, 18, voltage, 1, ON);            // Write the new voltage
}

void main() {
   int1  warn = FALSE;
   int8  adc = 0, adc_old = 0;
   char  voltText[] = "Volts", warning[] = "Warning";
   float theta = 0;

   setup_adc_ports(RA0_ANALOG);
   setup_adc(ADC_CLOCK_INTERNAL);
   set_adc_channel(0);

   glcd_init(ON);                               // Must initialize the LCD
   glcd_rect(1, 5, 126, 15, NO, ON);            // Outline the bar
   glcd_text57(70, 18, voltText, 1, ON);        // Display "Volts"
   glcd_circle(30, 47, 10, NO, ON);             // Draw the clock circle

   for(;;) {
      adc = read_adc();                         // Read a value from the ADC
      displayVoltage(adc);                      // Display the reading
      adc = (adc > 249) ? 249 : adc;            // Keep the value 249 or less

      if(adc != adc_old) {
         glcd_rect(adc/2+1, 6, adc_old/2+1, 14, YES, OFF);  // Clears the old bar
         glcd_rect(1, 6, adc/2+1, 14, YES, ON);             // Draws a new bar
         adc_old = adc;                                     // Set old value to new

         if(adc > 200 && !warn) {                  // Check if over 4 volts
            glcd_rect(45, 38, 124, 55, YES, ON);   // Draw a filled black rectangle
            glcd_text57(47, 40, warning, 2, OFF);  // Write "Warning" on the LCD
            warn = TRUE; }
         else if(adc <=200 && warn) {
            glcd_rect(45, 37, 125, 55, YES, OFF);  // Draw a filled white rectangle
            warn = FALSE; }
      }

      // The following 3 lines make the clock hand spin around
      glcd_line(30, 47, 30+(int)(8*sin(theta)+.5), 47-(int)(8*cos(theta)+.5), OFF);
      theta = (theta > 5.9) ? 0 : (theta += .3);
      glcd_line(30, 47, 30+(int)(8*sin(theta)+.5), 47-(int)(8*cos(theta)+.5), ON);

      #ifdef FAST_GLCD
      glcd_update();
      #else
      delay_ms(100);    // Reduces flicker by allowing pixels to be on
                        // much longer than off
      #endif
   }
}
