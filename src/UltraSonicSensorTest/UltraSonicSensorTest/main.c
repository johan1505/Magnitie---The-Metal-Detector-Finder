
// IF AN OBJECT IS DETECTED 9.5 INCH FROM THE SENSOR THEN IT SEND an error signal else it does not
#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <timer.h> // Different timer than mine. Would it work with my code???
#include <bit.h>
#include "io.c"

#define N_BER 64000 // This is use to check if there has been enough iterations in the for loops of pulse
unsigned char going = 1; // what is this need for?

uint16_t pulse();
void callTimer();
void setup();

int main(){
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	DDRA = 0x00; PORTA = 0xFF;
	TimerSet(1000);
	//TimerOn();
	sei();
	LCD_init();
	uint16_t dist;
	int convert;
	int digit1;
	int digit2;
	int digit3;
	int digit4;
	int convert1 = 0;	
	
	while(1) {
			setup();
			dist = pulse();
			if(dist == 0xFFFF)//Error
			{
				convert1 = 0;
				LCD_DisplayString(1, "Error");
				//Wait();
			}
			else if(dist == 0xFFFE)//No Obstacle
			{
				convert1 = 0;
				LCD_DisplayString(1, "No Obstacle");
				//Wait();
			}
			else if(dist == 0xFFFD)
			{
				convert1 = 0;
				LCD_DisplayString(1, "Error1");
				//Wait();
			}
			else
			{
				convert = (dist/58.0);
				convert1 = 1;
				if(convert >= 1000)
				{
					digit4 = convert % 10;
					digit3 = (convert / 10) % 10;
					digit2 = (convert / 100) % 10;
					digit1 = (convert / 1000);
				}
				else if(convert >= 100)
				{
					digit4 = convert % 10;
					digit3 = (convert / 10) % 10;
					digit2 = (convert / 100);
					digit1 = 0;
				}
				else if(convert >= 10)
				{
					digit4 = convert % 10;
					digit3 = (convert / 10);
					digit2 = 0;
					digit1 = 0;
				}
				else
				{
					digit4 = convert;
					digit3 = 0;
					digit2 = 0;
					digit1 = 0;
				}
				LCD_Cursor(1);
				LCD_WriteData('0' + digit1);
				LCD_Cursor(2);
				LCD_WriteData('0' + digit2);
				LCD_Cursor(3);
				LCD_WriteData('0' + digit3);
				LCD_Cursor(4);
				LCD_WriteData('0' + digit4);
				LCD_Cursor(5);
				LCD_WriteData('i');
				LCD_Cursor(6);
				LCD_WriteData('n');
				LCD_Cursor(0);
			}
			_delay_ms(500);
	}
}

uint16_t pulse()
{
	//Use trig to get the signal duration
	uint32_t i = 0;//uint32 initially
	going = 1;
	
	//while(i < N_BER)// && going)//wait for high
	for(i = 0; i < N_BER; i++)
	{
		if(GetBit(PINA, 2)){//(PINA & (1 << echo)))
			break;
		}
		else{
			continue;
		}
		i++;
	}
	
	if(i == N_BER)
	{
		return 0xFFFF;//Ends Here Somehow (change to 0xFFFF when fixed)
	}
	//Start Timer
	//TimerOn();
	callTimer();
	
	//wait for falling edge
	i = 0;
	//going = 1;
	//while(i < N_BER)// && going)//wait for low
	for(i = 0; i < N_BER; i++)
	{
		if(GetBit(PINA, 2))//(PINA & (1 << echo)))//(!GetBit(PINA, 4))//
		{
			if(TCNT1 > N_BER){
				break;
			}
			else {
				continue;
			}
		}
		else{
			break;
		}
		i++;
	}
	
	if(i == N_BER)
	{
		return 0xFFFF;
	}
	//falling edge found
	
	//Stop Timer
	uint32_t amount = TCNT1;
	TimerOff();
	if(amount > N_BER){
		return 0xFFFE;
	}
	else {
		return (amount >> 1);
	}
}

void callTimer()
{
	TimerOn();
	TCCR1A = 0x00;
}

void setup()
{
	//PORTA &= ~(1 << trig);
	PORTD |= 1<<0x00;//set trig high
	_delay_us(10);
	PORTD &= ~(1<<0x00);//set trig low
	_delay_ms(5);
}

/*#include <avr/io.h>
#include "sonar.c"
#include <util/delay.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <timer.h>
#include <scheduler.h>
#include "io.c"

void dispNum(int dist);

int main(){
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFD; PORTD = 0x02;
	DDRB = 0xFF; PORTB = 0x00;
	
	TimerSet(1);
	TimerOn();
	
	int distance_in_cm=0;
	// initialize display, cursor off
	LCD_init();
	LCD_ClearScreen();
	//lcd_command(LCD_DISP_ON);       // turn on cursor
	
	while(1)
	{
		distance_in_cm = read_sonar();
		if (distance_in_cm == TRIG_ERROR)
		{
			LCD_DisplayString(1, "ERROR!!!");
			_delay_ms(DELAY_BETWEEN_TESTS/2);
			LCD_ClearScreen();
			_delay_ms(DELAY_BETWEEN_TESTS/2);
			
		}
		else if (distance_in_cm == ECHO_ERROR)
		{
			
			LCD_DisplayString(1, "CLEAR!!!");
			_delay_ms(DELAY_BETWEEN_TESTS);
			LCD_ClearScreen();
		}
		else
		{
			if (distance_in_cm < 15){
				PORTB = 0xFF;
			}
			else {
				PORTB = 0x00;
			}
			
			LCD_DisplayString(1, "Distance(cm):");
			//lcd_put_int(distance_in_cm);
			dispNum(distance_in_cm);
			_delay_ms(DELAY_BETWEEN_TESTS);
			LCD_ClearScreen();
		}
	}
	return 0;
}

void dispNum(int dist){
	// Transforming the number of MetalsFound into an array of chars
	char NumToDisplay[4] = {'0', '0', '0', '0'};
	if (dist >= 0 && dist < 10){ // We know the integer is 1 digit
		char t1[1];
		sprintf(t1, "%d", dist);
		NumToDisplay[3] = t1[0];
	}
	else if (dist >= 10 && dist < 100){ // We know the integer is 2 digits
		char t2[2];
		sprintf(t2, "%d", dist);
		NumToDisplay[2] = t2[0];
		NumToDisplay[3] = t2[1];
	}
	else if (dist >= 100 && dist < 1000){ // We know the integer is 3 digits
		char t3[3];
		sprintf(t3, "%d", dist);
		NumToDisplay[1] = t3[0];
		NumToDisplay[2] = t3[1];
		NumToDisplay[3] = t3[2];
	}
	else if (dist >= 1000 && dist < 10000){ // We know the integer is 4 digits
		sprintf(NumToDisplay, "%d", dist);
	}
	
	//Displaying the number of MetalsFound in the LCD
	for (unsigned char i = 0; i < 4; i++){
		LCD_Cursor(i + 17);
		LCD_WriteData(NumToDisplay[i]);
	}
	
}
*/