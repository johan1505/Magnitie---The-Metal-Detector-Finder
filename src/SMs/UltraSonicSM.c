#define F_CPU 8000000UL
#include <util/delay.h>
#include <stdlib.h>

//Global variable
uint16_t  ObstacleDistance;

#define N_BER 64000 // Used to in the pulse helper function
enum SonarStates {UltraSonicStart} SonarState;
int UltrasonicSensorTick(int state);
//Helpers for UltrasonicSensor SM
uint16_t pulse();
void callTimer();
void setup();

int UltrasonicSensorTick(int state){
	switch(state){
        case UltraSonicStart:
			setup();
			ObstacleDistance = pulse();
			state = UltraSonicStart;
            break;

        default:
            state = UltraSonicStart;
            break;
    }
	return state;
}

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