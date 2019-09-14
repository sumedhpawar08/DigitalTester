#include "util/delay.h"
#define F_CPU 1000000UL
#define RS 5
#define RW 6
#define EN 4
#define lcd_port PORTC		//D4 to PC3........D7 to PC0

#define sbit(reg,bit)	reg |= (1<<bit)			// Macro defined for Setting a bit of any register.
#define cbit(reg,bit)	reg &= ~(1<<bit)		// Macro defined for Clearing a bit of any register.

void init_ports();
void lcd_reset();
void lcd_init();
void lcd_wr_command(unsigned char);
void lcd_wr_char(char);
void lcd_line1();
void lcd_line2();
void lcd_string(char*);
void lcd_clear();

unsigned int temp;
unsigned int unit;
unsigned int tens;
unsigned int hundred;
unsigned int thousand;
unsigned int million;

//Function to configure LCD port
void lcd_port_config (void)
{
 //DDRC = DDRC | 0xF7; //all the LCD pin's direction set as output
 DDRC |= (1<<PC0)|(1<<PC1)|(1<<PC2)|(1<<PC3)|(1<<PC4)|(1<<PC5)|(1<<PC6);
 //PORTC = PORTC & 0x80; // all the LCD pins are set to logic 0 except PORTC 7
PORTC = PORTC & 0x01;
}



//Function to Reset LCD
void lcd_set_4bit()
{
	_delay_ms(1);

	cbit(lcd_port,RS);				//RS=0 --- Command Input
	cbit(lcd_port,RW);				//RW=0 --- Writing to LCD
	lcd_port = 0x0C;				//Sending 3
	sbit(lcd_port,EN);				//Set Enable Pin
	_delay_ms(5);					//Delay
	cbit(lcd_port,EN);				//Clear Enable Pin

	_delay_ms(1);

	cbit(lcd_port,RS);				//RS=0 --- Command Input
	cbit(lcd_port,RW);				//RW=0 --- Writing to LCD
	lcd_port = 0x0C;				//Sending 3
	sbit(lcd_port,EN);				//Set Enable Pin
	_delay_ms(5);					//Delay
	cbit(lcd_port,EN);				//Clear Enable Pin

	_delay_ms(1);

	cbit(lcd_port,RS);				//RS=0 --- Command Input
	cbit(lcd_port,RW);				//RW=0 --- Writing to LCD
	lcd_port = 0x0C;				//Sending 3
	sbit(lcd_port,EN);				//Set Enable Pin
	_delay_ms(5);					//Delay
	cbit(lcd_port,EN);				//Clear Enable Pin

	_delay_ms(1);

	cbit(lcd_port,RS);				//RS=0 --- Command Input
	cbit(lcd_port,RW);				//RW=0 --- Writing to LCD
	lcd_port = 0x04;				//Sending 2 to initialise LCD 4-bit mode
	sbit(lcd_port,EN);				//Set Enable Pin
	_delay_ms(5);					//Delay
	cbit(lcd_port,EN);				//Clear Enable Pin

	
}

//Function to Initialize LCD
void lcd_init()
{
	lcd_port_config();
	_delay_ms(30);
	lcd_set_4bit();
	_delay_ms(1);

	lcd_wr_command(0x28);			//LCD 4-bit mode and 2 lines.
	lcd_wr_command(0x01);
	lcd_wr_command(0x06);
	lcd_wr_command(0x0E);
	lcd_wr_command(0x80);
		
}

	 
//Function to Write Command on LCD
void lcd_wr_command(unsigned char cmd)
{
	unsigned char temp;
	unsigned char var3;
	unsigned char var2;
	unsigned char var1;
	unsigned char var0;
	
	temp = cmd;
	temp = temp & 0xF0;
	lcd_port &= 0xF0;
	temp = temp >> 4;
	
	var3 = temp & 0b00001000;
	var2 = temp & 0b00000100;
	var1 = temp & 0b00000010;
	var0 = temp & 0b00000001;
	
	var3 = var3 >> 3;
	var2 = var2 >> 1;
	var1 = var1 << 1;
	var0 = var0 << 3;
	
	temp = 0x00;
	temp |= var3 | var2 | var1 | var0;
	
	lcd_port |= temp;
	cbit(lcd_port,RS);
	cbit(lcd_port,RW);
	sbit(lcd_port,EN);
	_delay_ms(5);
	cbit(lcd_port,EN);
	
	cmd = cmd & 0x0F;
	//cmd = cmd<<4;
	lcd_port &= 0xF0;
	
	var3 = cmd & 0b00001000;
	var2 = cmd & 0b00000100;
	var1 = cmd & 0b00000010;
	var0 = cmd & 0b00000001;
	
	var3 = var3 >> 3;
	var2 = var2 >> 1;
	var1 = var1 << 1;
	var0 = var0 << 3;
	
	cmd = 0x00;
	cmd |= var3 | var2 | var1 | var0;
	
	lcd_port |= cmd;
	cbit(lcd_port,RS);
	cbit(lcd_port,RW);
	sbit(lcd_port,EN);
	_delay_ms(5);
	cbit(lcd_port,EN);
}

//Function to Write Data on LCD
void lcd_wr_char(char letter)
{
	char temp;
	unsigned char var3;
	unsigned char var2;
	unsigned char var1;
	unsigned char var0;
	
	
	temp = letter;
	temp = (temp & 0xF0);
	temp = temp >> 4;
	
	lcd_port &= 0xF0;
	
	var3 = temp & 0b00001000;
	var2 = temp & 0b00000100;
	var1 = temp & 0b00000010;
	var0 = temp & 0b00000001;
	
	var3 = var3 >> 3;
	var2 = var2 >> 1;
	var1 = var1 << 1;
	var0 = var0 << 3;
	
	temp = 0x00;
	temp |= var3 | var2 | var1 | var0;
	
	lcd_port |= temp;
	sbit(lcd_port,RS);
	cbit(lcd_port,RW);
	sbit(lcd_port,EN);
	_delay_ms(5);
	cbit(lcd_port,EN);

	letter = letter & 0x0F;
	//letter = letter<<4;
	lcd_port &= 0xF0;
	
	var3 = letter & 0b00001000;
	var2 = letter & 0b00000100;
	var1 = letter & 0b00000010;
	var0 = letter & 0b00000001;
	
	var3 = var3 >> 3;
	var2 = var2 >> 1;
	var1 = var1 << 1;
	var0 = var0 << 3;
	
	letter = 0x00;
	letter |= var3 | var2 | var1 | var0;
	
	lcd_port |= letter;
	sbit(lcd_port,RS);
	cbit(lcd_port,RW);
	sbit(lcd_port,EN);
	_delay_ms(5);
	cbit(lcd_port,EN);
}

void lcd_clear()
{
	lcd_wr_command(0x01);
	
}
//Function to bring cursor at home position
void lcd_home()
{
	lcd_wr_command(0x80);
}


//Function to Print String on LCD
void lcd_string(char *str)
{
	while(*str != '\0')
	{
		lcd_wr_char(*str);
		str++;
	}
	
}

//Position the LCD cursor at "row", "column".

void lcd_cursor (char row, char column)
{
	switch (row) {
		case 1: lcd_wr_command (0x80 + column - 1); break;
		case 2: lcd_wr_command (0xc0 + column - 1); break;
		case 3: lcd_wr_command (0x94 + column - 1); break;
		case 4: lcd_wr_command (0xd4 + column - 1); break;
		default: break;
	}
}

//Function To Print Any input value upto the desired digit on LCD
void lcd_print (char row, char coloumn, unsigned int value, int digits)
{
	unsigned char flag=0;
	if(row==0||coloumn==0)
	{
		lcd_home();
	}
	else
	{
		lcd_cursor(row,coloumn);
	}
	if(digits==5 || flag==1)
	{
		million=value/10000+48;
		lcd_wr_char(million);
		flag=1;
	}
	if(digits==4 || flag==1)
	{
		temp = value/1000;
		thousand = temp%10 + 48;
		lcd_wr_char(thousand);
		flag=1;
	}
	if(digits==3 || flag==1)
	{
		temp = value/100;
		hundred = temp%10 + 48;
		lcd_wr_char(hundred);
		flag=1;
	}
	if(digits==2 || flag==1)
	{
		temp = value/10;
		tens = temp%10 + 48;
		lcd_wr_char(tens);
		flag=1;
	}
	if(digits==1 || flag==1)
	{
		unit = value%10 + 48;
		lcd_wr_char(unit);
	}
	if(digits>5)
	{
		lcd_wr_char('E');
	}
	
}


