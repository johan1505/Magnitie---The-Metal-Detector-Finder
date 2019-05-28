enum MotorsStates {WaitM, MotorsOn} MotorsState;
	
//Global variable
unsigned char buttonPressed; //Global variables
unsigned char MetalDetected;

int MotorsTick(int state){
	switch(state){
		case WaitM:
			state = buttonPressed ? MotorsOn : WaitM;
			break;
			
		case MotorsOn:
			state = MetalDetected ? WaitM : MotorsOn;
			break;
			
		default:
			state = WaitM;
			break;
	}
	switch(state){
		case WaitM:
			PORTB = 0x00;
			break;
			
		case  MotorsOn:
			PORTB = 0xFF;
			break;
			
		default:
			break;
	}
	return state;
}