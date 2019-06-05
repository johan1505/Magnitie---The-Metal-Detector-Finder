enum ButtonStates {ButtonStart, WaitForButton, SendButtonSignal} ButtonState;

unsigned char buttonPressed; //Global variable

int ButtonTick(int state){
	unsigned char b = ~PINA & 0x02;
	static unsigned char i;
	switch(state){	
		case WaitForButton:
			if (b){
				state = SendButtonSignal;
				i = 0;
			}
			else {
				state = WaitForButton;
			}
			break;
		
		case SendButtonSignal: // Rise the button signal for 1/2 second
			if (i < 2  || b){
				state  = SendButtonSignal;
			}
			else if (i >= 2 && !b){
				state = WaitForButton;
			}
			break;
		
		default:
			state = WaitForButton;
			break;
	}
	
	switch(state){
		case WaitForButton:
			buttonPressed = 0;
			break;
		
		case SendButtonSignal:
			buttonPressed = 1;
			i++;
			break;
			
		default:
			break;
		
	}
	return state;
}