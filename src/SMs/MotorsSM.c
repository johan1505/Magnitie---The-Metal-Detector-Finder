enum MotorsStates { MotorsOff, MotorsOn, WaitFiveSec, BreakMotors } MotorsState;
	
//Global variables
unsigned char buttonPressed; 
unsigned char MetalDetected;
unsigned char MotorsOutput;
unsigned char BREAK; 

int MotorsTick(int state){
	static unsigned char i;
	switch(state){
		case MotorsOff:
			if (buttonPressed && !BREAK){ // if there is a buttonPressed and no break signal then start motors
				state = MotorsOn;
			}
			else {
				state = MotorsOff;
			}
			break;
		
		case MotorsOn:
			if (!MetalDetected && !BREAK){
				state = MotorsOn;
			}
			else if (MetalDetected && !BREAK){
				state = WaitFiveSec;
				i = 0;
			}
			else if (MetalDetected && BREAK){ // Give priority to the breaks, note the number of metals should being recorded and shown on the screen no matter what
				state = BreakMotors;
			}
			else {
				state = BreakMotors;
			}
			break;
		
		case WaitFiveSec:  //Double check this state later when testing
			if (i < 6 || MetalDetected){
				state = WaitFiveSec;
			}
			else if (i >= 6 && !MetalDetected){
				state = MotorsOff;
			}
			break;
			
		case BreakMotors:
			state = BREAK ? BreakMotors : MotorsOff; // if BREAK signal is high stay here else go to MotorsOff and wait for user to press the buttton to start Magnitie again
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
			MotorsOutput = 0x03; // Turns on the motors
			break;
			
		case WaitFiveSec:
			MotorsOutput = 0x00; // Turns off the motors
			i++;
			break;	
		
		case BreakMotors:
			MotorsOutput = 0x00; // Turns off the motors
			break;
			
		default:
			break;
	}
	return state;
}