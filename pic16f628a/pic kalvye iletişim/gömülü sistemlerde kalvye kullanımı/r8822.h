// Some R8822 Definitions - Only a few are used
#define EOI 		0xFF22   // EOI
#define IMASK 		0xFF28   // interrupt mask register

#define T1CON		0xFF5E	// timer 1 control register
#define T1CMPA		0xFF5A	// timer 1 compare register A
#define T1CMPB		0xFF5C	// timer 1 compare register B

#define PIOMODE0	0xFF70 	// pio mode 0 register
#define PDIR0		0xFF72 	// pio direction 0 register
#define PDATA0		0xFF74 	// pio data 0 register

#define PIOMODE1	0xFF76 	// pio mode 1 register
#define PDIR1	 	0xFF78 	// pio direction 1 register
#define PDATA1	 	0xFF7A 	// pio data 1 register

#define INT0CON	0xFF38	// int0 control register
#define INT1CON	0xFF3A	// int1 control register
#define INT2CON	0xFF3C	// int2 control register
#define INT3CON	0xFF3E	// int3 control register
#define INT4CON	0xFF40	// int4 control register

#define TMR0NDX   0x08     //Timer 0 Index In Vector Table
#define INT0NDX   0x0C     //Int0  Index In Vector Table
#define INT1NDX   0x0D     //Int1  Index In Vector Table
#define INT2NDX   0x0E     //Int2  Index In Vector Table
#define INT3NDX   0x0F     //Int3  Index In Vector Table
#define INT4NDX   0x10     //Int4  Index In Vector Table
#define TMR1NDX   0x12     //Timer 1 Index In Vector Table
#define TMR2NDX   0x13     //Timer 2 Index In Vector Table

