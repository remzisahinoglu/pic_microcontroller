////////////////////////////////////////////////////////////////////////////////////
//                     VsZeNeR'05      
//                     26/Agosto/05
//                  vszener@gmail.com
//   Programa:   Abecedario
//   Version:   0.0
//
//   Dispositivo: PIC 16F648A         Compilador:    CCS vs3.227
//   Entorno IDE: MPLAB IDE v7.21      Simulador:    Proteus 6.7sp3
//
//   Notas: Se muestra por pantalla de lcd(LM016L) el abecedario y vuelta a empezar 
//         Tener en cuenta que hay que poner la directiva NOLVP para que el pin B4 sea de 
//         salida. 
//            ·abecedario[] -> vector donde se almacena abecedario
//            ·x -> indice para vector abecedario e indice para columna lcd
//             ·y -> indice para fila lcd: y=1 -> Fila 1
//                                 y=2 -> Fila 2
//   Conexiones:      B0 -> E            
//               B1 -> RS         
//               B2 -> RW      
//               B4 -> D4        
//               B5 -> D5            
//               B6 -> D6   
//               B7 -> D7                   
//////////////////////////////////////////////////////////////////////////////////

#include <16f648a.h>            //pic a utilizar          
#fuses XT,NOWDT,NOPROTECT,PUT,NOLVP      //ordenes para el programador 
#use delay (clock=4000000)         //Fosc=4Mhz
#define use_portb_lcd TRUE         //definir portb lcd
#include<lcd.c>                  //libreria manejo lcd
      
///PROGRAMA
void main(void)
{
   char alfabe[27]={' ','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'},x,y=1;   //tabla y variables definidas
      
   lcd_init();         //inicializa lcd
   
   do{                        //bucle...
   for(x=1;x<27;x++){            //bucle para mostrar digito
      if(y==1)               //1º fila del lcd
         lcd_gotoxy(x,y);
      else                  //2º fila del lcd
         lcd_gotoxy((x-16),y);
      
      if(x>15)               //¿ya esta completa la 1º fila del lcd?
         y=2;               //SI -> escribe en 2º fila
         
      printf(lcd_putc,"%c",alfabe[x]);   //muestra por pantalla el caracter  
      delay_ms(300);               
      }
   printf(lcd_putc,"\f");   //borra pantalla del lcd
   y=1;               //restablece indice
   }while(TRUE);               //...infinito
}
