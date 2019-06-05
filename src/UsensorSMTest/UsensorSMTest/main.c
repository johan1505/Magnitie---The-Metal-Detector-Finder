
#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/wdt.h> //
#include <bit.h>
#include <timer.h>
#include <stdio.h> 
#include <scheduler.h>
#include "io.c"
#define N_BER 64000

//Global variable
uint16_t distance = 0;

enum SonarStates {getDist} SonarState;
int SonarTick(int state);
//Helpers for sonar SM
uint16_t pulse();
void callTimer();
void setup();

enum DistCompStates {Compare} DistCompState;
int DistCompTick(int state);

int main(void)
{
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	DDRB = 0xFF; PORTB = 0x00;
	DDRA = 0x00; PORTA = 0xFF;
	
	static task task1;
	static task task2;
	
	unsigned long int Period = 50;
	
	task *tasks[] = {&task1, &task2};
	unsigned char numTasks = 2;
	//task 1
	task1.state = getDist;
	task1.period = Period;
	task1.elapsedTime = task1.period;
	task1.TickFct = &SonarTick;
	
	//task 2
	task2.state = Compare;
	task2.period = 1;
	task2.elapsedTime = task2.period;
	task2.TickFct = &DistCompTick;	
	
	sei();
	LCD_init();
	TimerSet(Period);
	TimerOn();
	
   unsigned char i;

   while (1)
   {
	   for ( i = 0; i < numTasks; i++ ) {
		   // Task is ready to tick
		   if ( tasks[i]->elapsedTime >= tasks[i]->period ) {
			   // Setting next state for task
			   tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
			   // Reset the elapsed time for next tick.
			   tasks[i]->elapsedTime = 0;
		   }
		   tasks[i]->elapsedTime += Period;
	   }
	   while(!TimerFlag);
	   TimerFlag = 0;
   }
}

int SonarTick(int state){	
	switch(state){
		case getDist:
			setup();
			distance = pulse();
			state = getDist;
			break;
		
		default:
			state = getDist;
			break;	
	}	
	return state;
}

int DistCompTick(int state){
	switch(state){
		case Compare:
			state = Compare;
			if (distance == 0xFFFF){
				LCD_DisplayString(1, "ERROR");
				//PORTB = 0x00;
			}
			else {
				LCD_DisplayString(1, "No Obstacle");
				//PORTB = 0xFF;
			}
			break;
		
		default:
			state = Compare;
			break;
	}
	return state;
}

//Helpers for sonar
uint16_t pulse()
{
	//Use trig to get the signal duration
	uint32_t i = 0;//uint32 initially
	//going = 1;
	
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
//	TimerOff();
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