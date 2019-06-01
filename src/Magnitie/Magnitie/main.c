/*
 * Magnitie.c
 *
 * Created: 5/28/2019 12:18:02 PM
 * Author : Johan Guzman Avalos
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <bit.h>
#include <timer.h>
#include <stdio.h>
#include <scheduler.h>
#include <ADC.h>
#include "io.c"
#include "ButtonSM.c"
#include "DetectMetalSM.c"
#include "LCDSM.c"
#include "MotorsSM.c"
#include "CounterSM.c"
#include "LightsSM.c"
#include "PORTBOutputSM.c"

//Global Variables
unsigned char MetalDetected;
unsigned char buttonPressed;
unsigned int  MetalsFound;
unsigned char MotorsOutput;
unsigned char LightsOutput;

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Set up sensor input and button input
	DDRB = 0xFF; PORTB = 0x00; // Output for the motors
	DDRC = 0xFF; PORTC = 0x00; // LCD data lines
	DDRD = 0xFF; PORTD = 0x00; // LCD control lines
	
	static task task1; //Detect Metal
	static task task2; //LCD
	static task task3; //Button
	static task task4; //Motors
	static task task5; //Counter
	static task task6; //Lights
	static task task7; //PORTBOutput
	
	// Period for the tasks
	unsigned long int DetectMetalSMPeriod = 100;
	unsigned long int LCDPeriod     = 500;
	unsigned long int ButtonPeriod  = 250;
	unsigned long int MotorsPeriod  = 500;
	unsigned long int CounterPeriod = 500;
	unsigned long int LightsPeriod  = 500;
	unsigned long int OutputPeriod  = 500;

	//Calculating GCD
	unsigned long int tempGCD = findGCD(ButtonPeriod, LCDPeriod);
	unsigned long int taskPeriodGCD = findGCD(tempGCD, DetectMetalSMPeriod);
	
	//Set up of the array of tasks
	task *tasks[] = {&task1, &task2, &task3, &task4, &task5, &task6, &task7};
	unsigned char numTasks = 7;
	
	//task 1
	task1.state = DetectMetalStart;
	task1.period = DetectMetalSMPeriod;
	task1.elapsedTime = task1.period;
	task1.TickFct = &DetectMetalTick;
	
	//task 2
	task2.state = LCDStart;
	task2.period = LCDPeriod;
	task2.elapsedTime = task2.period;
	task2.TickFct = &LCDTick;
	
	//task 3
	task3.state = WaitForButton;
	task3.period = ButtonPeriod;
	task3.elapsedTime = task3.period;
	task3.TickFct = &ButtonTick;
	
	//task 4
	task4.state = MotorsOff;
	task4.period = MotorsPeriod;
	task4.elapsedTime = task4.period;
	task4.TickFct = &MotorsTick;
	
	//task5
	task5.state = CounterStart;
	task5.period = CounterPeriod;
	task5.elapsedTime = task5.period;
	task5.TickFct = &CounterTick;
	
	//task6
	task6.state = LightsOff;
	task6.period = LightsPeriod;
	task6.elapsedTime = task6.period;
	task6.TickFct = &LightsTick;
	
	//task7
	task7.state = Output;
	task7.period = OutputPeriod;
	task7.elapsedTime = task7.period;
	task7.TickFct = &PORTBOutputTick;
	
	LCD_init();
	ADC_init();
	TimerSet(taskPeriodGCD);
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
			tasks[i]->elapsedTime += taskPeriodGCD; 
		}
		while(!TimerFlag);
		TimerFlag = 0;
	}
}