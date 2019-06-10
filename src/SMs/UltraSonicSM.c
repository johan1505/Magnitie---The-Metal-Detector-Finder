/********************************************************************************************************************/
// HELPERS FUNCTIONS WERE DERIVED FROM THIS GITHUB REPOSITORY: https://github.com/baileyherms/CS-120B-Final-Project.git
/********************************************************************************************************************/
#define F_CPU 8000000UL
#include <util/delay.h>
#include <stdlib.h>

//Global variable
uint16_t  ObstacleDistance;
#define N_BER 64000 // Used to in the pulse helper function
enum SonarStates {UltraSonicStart} SonarState;
	
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
	
	for(i = 0; i < N_BER; i++)
	{
		if(GetBit(PINA, 2)){
			break;
		}
		else{
			continue;
		}
		i++;
	}
	
	if(i == N_BER)
	{
		return 0xFFFF;
	}

	callTimer();
	i = 0;
	for(i = 0; i < N_BER; i++)
	{
		if(GetBit(PINA, 2))
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
	PORTD |= 1<<0x00;//set trig high
	_delay_us(10);
	PORTD &= ~(1<<0x00);//set trig low
	_delay_ms(5);
}