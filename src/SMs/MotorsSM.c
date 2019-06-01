enum MotorsStates { MotorsOff, MotorsOn, WaitFiveSec } MotorsState;
	
//Global variables
unsigned char buttonPressed; 
unsigned char MetalDetected;
unsigned char MotorsOutput;

int MotorsTick(int state){
	static unsigned char i;
	switch(state){
		case MotorsOff:
			state = buttonPressed ? MotorsOn : MotorsOff;
			break;
		
		case MotorsOn:
			if (MetalDetected){
				state = WaitFiveSec;
				i = 0;
			}
			else {
				state = MotorsOn;
			}
			break;
		
		case WaitFiveSec:
			if (i < 10 || MetalDetected){
				state = WaitFiveSec;
			}
			else if (i >= 10 && !MetalDetected){
				state = MotorsOff;
			}
			break;
		
		default:
			state = MotorsOff;
			break;
	}
	
	switch (state){
		case MotorsOff:
			MotorsOutput = 0x00;
			break;
		
		case MotorsOn:
			MotorsOutput = 0x03;
			break;
			
		case WaitFiveSec:
			MotorsOutput = 0x00;
			i++;
			break;	
		
		default:
			break;
	}
	return state;
}