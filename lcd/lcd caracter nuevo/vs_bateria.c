////////////////////////////////////////////////////////////////////////////////////
//                     VsZeNeR'05      
//                   30/Septiembre/05
//                  vszener@gmail.com
//   Programa:   Bateria
//   Version:   0.0
//
//   Dispositivo: PIC 16F648A         Compilador:    CCS vs3.227
//   Entorno IDE: MPLAB IDE v7.21      Simulador:    Proteus 6.7sp3
//
//   Notas: Se muestra por pantalla de lcd(LM016L) la animacion de carga de 
//         una bateria, para ello se han dise�ado nuevos caracteres y almacenados
//         en la memoria GCRAM de la lcd, solo se ha usado una posicion de memoria
//         de dicha GCRAM. Tener en cuenta que hay que poner la directiva NOLVP para 
//         que el pin B4 sea de salida. 
//
//   Conexiones:      B0 -> E            
//               B1 -> RS         
//               B2 -> RW      
//               B4 -> D4        
//               B5 -> D5            
//               B6 -> D6   
//               B7 -> D7                   
//////////////////////////////////////////////////////////////////////////////////

#include <16f648a.h>                
#fuses XT,NOWDT,NOPROTECT,PUT,NOLVP      
#use delay (clock=4000000)         
#define use_portb_lcd TRUE
#include <lcd.c>              
void anim(void);      
char cont=0;      //kartakter tablosu
   int8    bat0[]={0b00001110,0b00001010,0b00010001,0b00010001,0b00010001,0b000010001,0b00010001,0b00011111},
bat20[]={0b00001110,0b00001010,0b00010001,0b00010001,0b00010001,0b00010001,0b00011111,0b00011111},
bat40[]={0b00001110,0b00001010,0b00010001,0b00010001,0b00010001,0b00011111,0b00011111,0b00011111},
bat60[]={0b00001110,0b00001010,0b00010001,0b00010001,0b00011111,0b00011111,0b00011111,0b00011111},
bat80[]={0b00001110,0b00001010,0b00010001,0b00011111,0b00011111,0b00011111,0b00011111,0b00011111},
bat100[]={0b00001110,0b00001010,0b00011111,0b00011111,0b00011111,0b00011111,0b00011111,0b00011111};      
void main(void)
{ lcd_init();      
  anim();        
   lcd_gotoxy(5,1);   
   lcd_putc("BATARYA\n   ");    
   for(;;){   
      lcd_gotoxy(2,1);      
      lcd_send_byte(1,0);    
      delay_ms(250);         
      cont++;              
      anim();          
         }}
//Animasyon fonksiyonu
void anim(void){
   char i;       
   lcd_send_byte(0,0x40);    
   switch(cont){  
      case 0: for(i=0;i<8;i++)   //batarya  0%
               lcd_send_byte(1,bat0[i]); 
            break;
      case 1: for(i=0;i<8;i++)   //batarya  20%
               lcd_send_byte(1,bat20[i]); 
            break;
      case 2: for(i=0;i<8;i++)   //batarya  40%
               lcd_send_byte(1,bat40[i]); 
            break;
      case 3: for(i=0;i<8;i++)   //batarya  60%
               lcd_send_byte(1,bat60[i]); 
            break;
      case 4: for(i=0;i<8;i++)   //batarya  80%
               lcd_send_byte(1,bat80[i]); 
            break;
      case 5: for(i=0;i<8;i++)   //batarya  100%
               lcd_send_byte(1,bat100[i]); 
               cont=0;         
         }
}         

