
enum buzzerStates {buzzerOff, buzzerOn} buzzerState;
unsigned char BREAK;

int buzzerTick(int state){
    switch (state){
		case buzzerOff:
			if (BREAK){
				state = buzzerOn;
			}
			else {
				state = buzzerOff;
			}
			break;
		
		case buzzerOn:
			if (BREAK){
				state = buzzerOn;
			}
			else {
				state = buzzerOff;
			}
			break;
		
		default:
			break;
    }
	
	switch(state){
		case buzzerOff:
			set_PWM(0); // buzzer off
			break;
		
		case  buzzerOn:
			set_PWM(261.63); //Buzzer on
			break;
		
		default:
			break;
	}
	return state;
}