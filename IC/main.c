/*
 * IC.c
 *
 * Created: 09-03-2016 11:41:23 PM
 * Author : Montya
 */ 
#include <avr/io.h>
#include "lcd.h"
#include "util/delay.h"
#include "avr/interrupt.h"
#include "connections.h"
#define TOTAL_MODES 7	//total number of modes
static uint8_t mode=-1;

void IC_7408();
void IC_7432();
void IC_7486();
void IC_7400();
void IC_7404();
void IC_7402();
void IC_Result(uint8_t,uint8_t,uint8_t,uint8_t);
void findIC();

ISR(INT0_vect)
{
	mode++;
	lcd_clear();
	lcd_cursor(1,2);
	switch(mode%TOTAL_MODES)
	{
		case 0:lcd_string("7400 NAND IC");break;
		case 1:lcd_string("7402 NOR IC");break;
		case 2:lcd_string("7404 NOT IC");break;
		case 3:lcd_string("7408 AND IC");break;
		case 4:lcd_string("7432 OR IC");break;
		case 5:lcd_string("7486 EXOR IC");break;
		case 6:lcd_string("Find IC");break;
		
	}
}
ISR(INT1_vect)
{
	lcd_clear();
	lcd_cursor(1,2);
	switch(mode%TOTAL_MODES)
	{
		case 0: IC_7400();break;
		case 1: IC_7402();break;
		case 2: IC_7404();break;
		case 3: IC_7408();break;
		case 4: IC_7432();break;
		case 5: IC_7486();break;
		case 6: findIC();break;
	}
	
}

void Init_Type1()		// 1 & 2 Inputs and 3 Output pattern IC
{
	//Gate 1
	CON1DDR |= (1<<CON1);	//output from controller and input of ic
	CON2DDR |= (1<<CON2);	//output from controller and input of ic
	CON3DDR &= ~(1<<CON3);	//input  to controller and output of ic
	CON3PORT &=~(1<<CON3);
	//DDRD &= ~(1 << PD2);        // see comment #1
	//Gate 2	
	CON4DDR |= (1<<CON4);	//output from controller and input of ic
	CON5DDR |= (1<<CON5);	//output from controller and input of ic
	CON6DDR &= ~(1<<CON6);	//input  to controller and output of ic
	CON6PORT &=~(1<<CON6);
	//Gate 3
	CON10DDR |= (1<<CON10);	//output from controller and input of ic
	CON9DDR |= (1<<CON9);	//output from controller and input of ic
	CON8DDR &= ~(1<<CON8);	//input  to controller and output of ic
	CON8PORT &=~(1<<CON8);
	//Gate 4
	CON13DDR |= (1<<CON13);	//output from controller and input of ic
	CON12DDR |= (1<<CON12);	//output from controller and input of ic
	CON11DDR &= ~(1<<CON11);	//input  to controller and output of ic
	CON11PORT &=~(1<<CON11);
	
}
void Init_Type2()		// 2 & 3 Inputs and 1 Output pattern IC
{
	//Gate 1
	CON1DDR &= ~(1<<CON1);	////input  to controller and output of ic
	CON2DDR |= (1<<CON2);	//output from controller and input of ic
	CON3DDR |= (1<<CON3);	//output from controller and input of ic
	CON1PORT &=~(1<<CON1);
	//DDRD &= ~(1 << PD2);        // see comment #1
	//Gate 2
	CON4DDR &= ~(1<<CON4);	//input  to controller and output of ic
	CON5DDR |= (1<<CON5);	//output from controller and input of ic
	CON6DDR |= (1<<CON6);	//output from controller and input of ic
	CON4PORT &=~(1<<CON4);
	//Gate 3
	CON10DDR &= ~(1<<CON10);	//input  to controller and output of ic
	CON9DDR |= (1<<CON9);	//output from controller and input of ic
	CON8DDR |= (1<<CON8);	//output from controller and input of ic
	CON10PORT &=~(1<<CON10);
	//Gate 4
	CON13DDR &= ~(1<<CON13);	//input  to controller and output of ic
	CON12DDR |= (1<<CON12);	//output from controller and input of ic
	CON11DDR |= (1<<CON11);	//output from controller and input of ic
	CON13PORT &=~(1<<CON13);
	
}
void Init_Type3()	//1 Input 2 Output of pattern IC
{
	//Gate1
	CON1DDR |= (1<<CON1);		//output from controller and input to ic
	CON2DDR &= ~(1<<CON2);		//input to controller and output from ic
	CON2PORT &= ~(1<<CON2PORT);
	//Gate2
	CON3DDR |= (1<<CON3);		//output from controller and input to ic
	CON4DDR &= ~(1<<CON4);		//input to controller and output from ic
	CON4PORT &= ~(1<<CON4PORT);
	//Gate3
	CON5DDR |= (1<<CON5);		//output from controller and input to ic
	CON6DDR &= ~(1<<CON6);		//input to controller and output from ic
	CON6PORT &= ~(1<<CON6PORT);
	//Gate4
	CON9DDR |= (1<<CON9);		//output from controller and input to ic
	CON8DDR &= ~(1<<CON8);		//input to controller and output from ic
	CON8PORT &= ~(1<<CON8PORT);
	//Gate5
	CON11DDR |= (1<<CON11);		//output from controller and input to ic
	CON10DDR &= ~(1<<CON10);		//input to controller and output from ic
	CON10PORT &= ~(1<<CON10PORT);
	//Gate6
	CON13DDR |= (1<<CON13);		//output from controller and input to ic
	CON12DDR &= ~(1<<CON12);		//input to controller and output from ic
	CON12PORT &= ~(1<<CON12PORT);
}

void put_signal_type1(uint8_t o1,uint8_t o2,uint8_t gate)	//output signals from controller and inputs to ic
{
	switch(gate)
	{
	case 1:	
			if(o1==1)
			CON1PORT |= (1<<CON1);
			else
			CON1PORT &= ~(1<<CON1);
			
			if(o2==1)
			CON2PORT |= (1<<CON2);	
			else
			CON2PORT &= ~(1<<CON2);
			
			break;
	
	case 2:	
			if(o1==1)
			CON4PORT |= (1<<CON4);
			else
			CON4PORT &= ~(1<<CON4);
			
			if(o2==1)
			CON5PORT |= (1<<CON5);
			else
			CON5PORT &= ~(1<<CON5);
		
			break;
	
	case 3:	
			if(o1==1)
			CON10PORT |= (1<<CON10);
			else
			CON10PORT &= ~(1<<CON10);
			
			if(o2==1)
			CON9PORT |= (1<<CON9);
			else
			CON9PORT &= ~(1<<CON9);
	
			break;
	
	case 4:	
			if(o1==1)
			CON13PORT |= (1<<CON13);
			else
			CON13PORT &= ~(1<<CON13);
			
			if(o2==1)
			CON12PORT |= (1<<CON12);
			else
			CON12PORT &= ~(1<<CON12);
			
			break;
	}
}
void put_signal_type2(uint8_t o1,uint8_t o2,uint8_t gate)	//output signals from controller and inputs to ic
{
	switch(gate)
	{
		case 1:
		if(o1==1)
		CON2PORT |= (1<<CON2);
		else
		CON2PORT &= ~(1<<CON2);
		
		if(o2==1)
		CON3PORT |= (1<<CON3);
		else
		CON3PORT &= ~(1<<CON3);
		
		break;
		
		case 2:
		if(o1==1)
		CON5PORT |= (1<<CON5);
		else
		CON5PORT &= ~(1<<CON5);
		
		if(o2==1)
		CON6PORT |= (1<<CON6);
		else
		CON6PORT &= ~(1<<CON6);
		
		break;
		
		case 3:
		if(o1==1)
		CON8PORT |= (1<<CON8);
		else
		CON8PORT &= ~(1<<CON8);
		
		if(o2==1)
		CON9PORT |= (1<<CON9);
		else
		CON9PORT &= ~(1<<CON9);
		
		break;
		
		case 4:
		if(o1==1)
		CON11PORT |= (1<<CON11);
		else
		CON11PORT &= ~(1<<CON11);
		
		if(o2==1)
		CON12PORT |= (1<<CON12);
		else
		CON12PORT &= ~(1<<CON12);
		
		break;
	}
}
void put_signal_type3(uint8_t o1,uint8_t gate)
{
	switch(gate)
	{
		case 1:if(o1 == 1)
			CON1PORT |= (1<<CON1);
			else
			CON1PORT &= ~(1<<CON1);
			break;
	case 2:if(o1 == 1)
			CON3PORT |= (1<<CON3);
			else
			CON3PORT &= ~(1<<CON3);
			break;
	case 3:if(o1 == 1)
			CON5PORT |= (1<<CON5);
			else
			CON5PORT &= ~(1<<CON5);
			break;
	case 4:if(o1 == 1)
			CON9PORT |= (1<<CON9);
			else
			CON9PORT &= ~(1<<CON9);
			break;
	case 5:if(o1 == 1)
			CON11PORT |= (1<<CON11);
			else
			CON11PORT &= ~(1<<CON11);
			break;
	case 6:if(o1 == 1)
			CON13PORT |= (1<<CON13);
			else
			CON13PORT &= ~(1<<CON13);
			break;
	}
}

uint8_t get_signal_type1(uint8_t gate)		// get outputs from ic
{
	switch(gate)
	{
		case 1: if(CON3PIN & (1<<CON3)) return 1;else return 0;break;
		case 2: if(CON6PIN & (1<<CON6)) return 1;else return 0;break;
		case 3: if(CON8PIN & (1<<CON8)) return 1;else return 0;break;
		case 4: if(CON11PIN & (1<<CON11)) return 1;else return 0;break;
	}
	return 0;
}
uint8_t get_signal_type2(uint8_t gate)		// get outputs from ic
{
	switch(gate)
	{
		case 1: if(CON1PIN & (1<<CON1)) return 1;else return 0;break;
		case 2: if(CON4PIN & (1<<CON4)) return 1;else return 0;break;
		case 3: if(CON10PIN & (1<<CON10)) return 1;else return 0;break;
		case 4: if(CON13PIN & (1<<CON13)) return 1;else return 0;break;
	}
	return 0;
}
uint8_t get_signal_type3(uint8_t gate)
{
	switch(gate)
	{
		case 1:if(CON2PIN & (1<<CON2)) return 1;else return 0;break;
		case 2:if(CON4PIN & (1<<CON4)) return 1;else return 0;break;
		case 3:if(CON6PIN & (1<<CON6)) return 1;else return 0;break;
		case 4:if(CON8PIN & (1<<CON8)) return 1;else return 0;break;
		case 5:if(CON10PIN & (1<<CON10)) return 1;else return 0;break;
		case 6:if(CON12PIN & (1<<CON12)) return 1;else return 0;break;
	}
}

uint8_t Check_gate_type1(uint8_t t1,uint8_t t2,uint8_t t3,uint8_t t4,uint8_t gate) //t1,t2,t3,t4 are valid outputs from ic
{
	uint8_t ticks=0;
	
	put_signal_type1(0,0,gate);
	_delay_ms(3);
	if(get_signal_type1(gate) == t1)
	ticks++;
	//lcd_print(1,2,get_signal_type1(gate),1);
	
	put_signal_type1(0,1,gate);
	_delay_ms(3);
	if(get_signal_type1(gate) == t2)
	ticks++;
	//lcd_print(1,3,get_signal_type1(gate),1);
	
	put_signal_type1(1,0,gate);
	_delay_ms(3);
	if(get_signal_type1(gate) == t3)
	ticks++;
	//lcd_print(1,4,get_signal_type1(gate),1);
	
	put_signal_type1(1,1,gate);
	_delay_ms(3);
	if(get_signal_type1(gate) == t4)
	ticks++;
	//lcd_print(1,5,get_signal_type1(gate),1);
	
	put_signal_type1(0,0,gate);
	_delay_ms(3);
	
	return ticks;
}
uint8_t Check_gate_type2(uint8_t t1,uint8_t t2,uint8_t t3,uint8_t t4,uint8_t gate) //t1,t2,t3,t4 are valid outputs from ic
{
	uint8_t ticks=0;
	
	put_signal_type2(0,0,gate);
	_delay_ms(3);
	if(get_signal_type2(gate) == t1)
	ticks++;
	//lcd_print(1,2,get_signal_type1(gate),1);
	
	put_signal_type2(0,1,gate);
	_delay_ms(3);
	if(get_signal_type2(gate) == t2)
	ticks++;
	//lcd_print(1,3,get_signal_type1(gate),1);
	
	put_signal_type2(1,0,gate);
	_delay_ms(3);
	if(get_signal_type2(gate) == t3)
	ticks++;
	//lcd_print(1,4,get_signal_type1(gate),1);
	
	put_signal_type2(1,1,gate);
	_delay_ms(3);
	if(get_signal_type2(gate) == t4)
	ticks++;
	//lcd_print(1,5,get_signal_type1(gate),1);
	
	put_signal_type2(0,0,gate);
	_delay_ms(3);
	
	return ticks;
}
uint8_t Check_gate_type3(uint8_t t1,uint8_t t2,uint8_t gate)
{
	uint8_t ticks=0;
	put_signal_type3(0,gate);
	_delay_ms(3);
	if(get_signal_type3(gate) == t1)
	ticks++;
	
	
	put_signal_type3(1,gate);
	_delay_ms(3);
	if(get_signal_type3(gate) == t2)
	ticks++;
	
	put_signal_type3(0,gate);
	_delay_ms(3);
	
	return ticks;
}

void IC_7408()
{
	uint8_t g1=0,g2=0,g3=0,g4=0; //1 means gates are working fine and 0 means not fine	
	lcd_string("IC 7408 AND");
	lcd_cursor(2,2);
	//send output from controller as inputs to ic
	Init_Type1();
	
	if(Check_gate_type1(0,0,0,1,1) == 4)
	g1 = 1;
	if(Check_gate_type1(0,0,0,1,2) == 4)
	g2 = 1;
	if(Check_gate_type1(0,0,0,1,3) == 4)
	g3 = 1;
	if(Check_gate_type1(0,0,0,1,4) == 4)
	g4 = 1;
	IC_Result(g1,g2,g3,g4);
	
}
void IC_7432()
{
	uint8_t g1=0,g2=0,g3=0,g4=0; //1 means gates are working fine and 0 means not fine
		
		lcd_string("IC 7432 OR");
		lcd_cursor(2,2);
		//send output from controller as inputs to ic
		Init_Type1();
		
		if(Check_gate_type1(0,1,1,1,1) == 4)
			g1 = 1;
		if(Check_gate_type1(0,1,1,1,2) == 4)
			g2 = 1;
		if(Check_gate_type1(0,1,1,1,3) == 4)
			g3 = 1;
		if(Check_gate_type1(0,1,1,1,4) == 4)
			g4 = 1;
		IC_Result(g1,g2,g3,g4);
	
}
void IC_7486()
{
	uint8_t g1=0,g2=0,g3=0,g4=0; //1 means gates are working fine and 0 means not fine
		
		//send output from controller as inputs to ic
		Init_Type1();
		lcd_string("IC 7486 XOR");
		lcd_cursor(2,2);
		if(Check_gate_type1(0,1,1,0,1) == 4)
			g1 = 1;
		if(Check_gate_type1(0,1,1,0,2) == 4)
			g2 = 1;
		if(Check_gate_type1(0,1,1,0,3) == 4)
			g3 = 1;
		if(Check_gate_type1(0,1,1,0,4) == 4)
			g4 = 1;
		IC_Result(g1,g2,g3,g4);
}
void IC_7400()
{
	uint8_t g1=0,g2=0,g3=0,g4=0; //1 means gates are working fine and 0 means not fine
		//send output from controller as inputs to ic
		Init_Type1();
		lcd_string("IC 7400 NAND");
		lcd_cursor(2,2);
		if(Check_gate_type1(1,1,1,0,1) == 4)
			g1 = 1;
		if(Check_gate_type1(1,1,1,0,2) == 4)
			g2 = 1;
		if(Check_gate_type1(1,1,1,0,3) == 4)
			g3 = 1;
		if(Check_gate_type1(1,1,1,0,4) == 4)
			g4 = 1;
		IC_Result(g1,g2,g3,g4);
}
void IC_7404()
{
	uint8_t g1=0,g2=0,g3=0,g4=0,g5=0,g6=0; //1 means gates are working fine and 0 means not fine
	
		lcd_string("IC 7404 NOT");
		lcd_cursor(2,2);
		//send output from controller as inputs to ic
		Init_Type3();
		
		if(Check_gate_type3(1,0,1) == 2)
			g1 = 1;
		if(Check_gate_type3(1,0,2) == 2)
			g2 = 1;
		if(Check_gate_type3(1,0,3) == 2)
			g3 = 1;
		if(Check_gate_type3(1,0,4) == 2)
			g4 = 1;
		if(Check_gate_type3(1,0,5) == 2)
			g5 = 1;
		if(Check_gate_type3(1,0,6) == 2)
			g6 = 1;
		
		if(g1 == 1 && g2 == 1 && g3 == 1 && g4 == 1 && g5 == 1 && g6 == 1)
			lcd_string("All Gates OK");
		else
		{
				lcd_string("Gate ");
				if(g1 == 1)
				lcd_string("1");
				if(g2 == 1)
				lcd_string("2");
				if(g3 == 1)
				lcd_string("3");
				if(g4 == 1)
				lcd_string("4");
				if(g5 == 1)
				lcd_string("5");
				if(g6 == 1)
				lcd_string("6");
				lcd_string(" OK");
		}
		
	
}
void IC_7402()
{
	uint8_t g1=0,g2=0,g3=0,g4=0; //1 means gates are working fine and 0 means not fine
	//send output from controller as inputs to ic
	Init_Type2();
	lcd_string("IC 7402 NOR");
	lcd_cursor(2,2);
	if(Check_gate_type2(1,0,0,0,1) == 4)
	g1 = 1;
	if(Check_gate_type2(1,0,0,0,2) == 4)
	g2 = 1;
	if(Check_gate_type2(1,0,0,0,3) == 4)
	g3 = 1;
	if(Check_gate_type2(1,0,0,0,4) == 4)
	g4 = 1;
	IC_Result(g1,g2,g3,g4);
}

void IC_Result(uint8_t g1,uint8_t g2,uint8_t g3,uint8_t g4)
{

	if(g1 == 1 && g2 == 1 && g3 == 1 && g4 == 1)
		lcd_string("All Gates OK");
	else if(g1 == 0 && g2 == 0 && g3 == 0 && g4 == 0)
		lcd_string("Wrong IC");
	else
	{
		lcd_string("Gate ");
		if(g1 == 1)
		lcd_string("1");
		if(g2 == 1)
		lcd_string("2");
		if(g3 == 1)
		lcd_string("3");
		if(g4 == 1)
		lcd_string("4");
		lcd_string(" OK");
	}	
}
void result1()
{
	lcd_clear();
	lcd_cursor(1,2);
	lcd_string("Gt 1:1,2-I/P");
	lcd_cursor(2,2);
	lcd_string("       3-O/P");
}
void findIC()
{
	Init_Type1();
	if(Check_gate_type1(0,0,0,1,1) == 4)
	{
		lcd_string("IC 7408 AND");
		lcd_cursor(2,2);
		lcd_string("Two I/p And");
		_delay_ms(5000);
		result1();
	}
	else if(Check_gate_type1(0,1,1,1,1) == 4)
	{
		lcd_string("IC 7432 OR");
		lcd_cursor(2,2);
		lcd_string("Two I/p OR");
		_delay_ms(5000);
		result1();
	}
	else if(Check_gate_type1(0,1,1,0,1) == 4)
	{
		lcd_string("IC 7486 XOR");
		lcd_cursor(2,2);
		lcd_string("Two I/p XOR");
		_delay_ms(5000);
		result1();
	}
	else if(Check_gate_type1(1,1,1,0,1) == 4)
	{
		lcd_string("IC 7400 NAND");
		lcd_cursor(2,2);
		lcd_string("Two I/p NAND");
		_delay_ms(5000);
		result1();
	}
	else 
	{
		Init_Type2();
		_delay_ms(50);
		if(Check_gate_type2(1,0,0,0,1) == 4)
		{
		lcd_string("IC 7402 NOR");
		lcd_cursor(2,2);
		lcd_string("Two I/p NOR");
		}
		else
		{
			lcd_string("IC 7404 NOT");
		}
	}
	
	
}

int main(void)
{
	GICR |= (1<<INT0)|(1<<INT1);
	MCUCR |= (0<<ISC01) | (1<<ISC00) | (0<<ISC11) | (1<<ISC10);
	sei();
	lcd_init();
	lcd_home();
	lcd_string("  Digital");
	lcd_cursor(2,2);
	lcd_string(" IC Tester");

	while (1) 
    {
		
    }
}

