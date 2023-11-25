//////////////////////////////////////////////////////////////
//                    benim_LCD.c                           // 
// Bu LCD s�r�c� dosyas� 2x16 HD44780 uyumlu LCD'ler        //
// i�in yaz�lm��t�r. LCD'den sadece okuma yap�laca�� i�in   //
// R/W ucu �aseye ba�lanmal�d�r. LCD ba�lant�s� a�a��dad�r. //
//////////////////////////////////////////////////////////////
// RB0 D4
// RB1 D5
// RB2 D6
// RB3 D7
// RB4 RS
// RB5 E
// R/W ucu direkt �aseye ba�lanacak

#define e   pin_b4 // LCD'nin E ucu RB5 pinine ba�l�
#define rs  pin_b5 // LCD'nin RS ucu RB4 pinine ba�l�

//****** LCD'ye Komut G�nderme Fonksiyonu **********
void lcd_komut(byte komut)
{
   output_b(komut>>4);  	// Komutun y�ksek de�erli 4 bitini g�nder
   output_low(rs);       	// LCD komut almak i�in ayarland�
   output_high(e);       	// E ucu lojik-1'den lojik-0'a �ekiliyor
   output_low(e);
   delay_ms(2);         	// 2 msn gecikme veriliyor

   output_b(komut&0x0F); 	// Komutun d���k de�erli 4 bitini g�nder
   output_low(rs);       	// LCD komut almak i�in ayarland�
   output_high(e);        	// E ucu lojik-1'den lojik-0'a �ekiliyor
   output_low(e);
   delay_ms(2);        		// 2 msn gecikme veriliyor
}

//******* LCD'ye Veri G�nderme Fonksiyonu **********
void lcd_veri(byte veri)
{
   output_b(veri>>4);    	// Verinin y�ksek de�erli 4 bitini g�nder
   output_high(rs);      	// LCD veri almak i�in ayarland�
   output_high(e);       	// E ucu lojik-1'den lojik-0'a �ekiliyor
   output_low(e);
   delay_ms(1);          	// 1 msn gecikme veriliyor

   output_b(veri&0x0F); 	// Verinin d���k de�erli 4 bitini g�nder
   output_high(rs);     	// LCD veri almak i�in ayarland�
   output_high(e);      	// E ucu lojik-1'den lojik-0'a �ekiliyor
   output_low(e);
   delay_ms(1);         	// 1 msn gecikme veriliyor
}

//******* LCD'de �mlec Konumland�rma Fonksiyonu ********
void imlec(byte satir, byte sutun)
{
   if (satir==1)      		// E�er sat�r de�i�keni "1" ise
      lcd_komut(0x80|(sutun-1));

   if (satir==2)      		// E�er sat�r de�i�keni "2" ise
      lcd_komut(0x80|(0x40+(sutun-1)));
}

//********* LCD Ba�lang�� Ayarlar� Fonksiyonu ******
void lcd_hazirla()
{
   lcd_komut(0x02);		// LCD'yi kullan�ma haz�r hale getir, imle�                // 1.sat�r 1.s�tunda komutu
   lcd_komut(0x28);		//2 sat�r, 4 bit ileti�im, 5x8 dot matris se�ildi 
   lcd_komut(0x08);		//Display kapal�,alt �izgi ve yan�p s�nme yok
   lcd_komut(0x0C);		//Display a��k,imle� alt �izgi ve yan�p s�nme yok
   lcd_komut(0x06);		// Her veri yaz�ld���nda imle� bir sa�a gitsin
   lcd_komut(0x01);		// Display sil. �mle� 1.sat�r 1.s�tunda
   delay_ms(1);    		// 1 msn bekle 
}

