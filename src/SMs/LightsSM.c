enum LightsStates { LightsOff, LightsOn, BlinkLights } LightsState;

//Global variables
unsigned char buttonPressed;
unsigned char MetalDetected;
unsigned char LightsOutput;

int LightsTick(int state){
	static unsigned char i;
	switch(state){
		case LightsOff:
			if (buttonPressed){
				state = LightsOn;
				i = 0;
			}
			else {
				state = LightsOff;
			}
			break;
		
		case LightsOn:
			if (MetalDetected){
				state = BlinkLights;
				LightsOutput = 0x00;
				i = 0;
			}
			else {
				state = LightsOn;
			}
			break;
		
		case BlinkLights:
			if (i < 10 || MetalDetected){
				state = BlinkLights;
			}
			else if (i >= 10 && !MetalDetected){
				state = LightsOff;
			}
			break;
		
		default:
			state = LightsOff;
			break;
	}
	
	switch (state){
		case MotorsOff:
			LightsOutput = 0x00;
			break;
		
		case MotorsOn:
			if (i == 0){
				LightsOutput = 0x00;
			}
			else if (i == 1){
				LightsOutput = 0x01;
			}
			else if (i == 2){
				LightsOutput = 0x03;
			}
			else if (i == 3){
				LightsOutput = 0x07;
			}
			else {
				LightsOutput = 0x0F;
			}
			
			if (i < 4){
				i++;
			}
			else {
				i = 0;
			}
			break;
		
		case BlinkLights:
			LightsOutput = ~LightsOutput;
			i++;
			break;
		
		default:
			break;
	}
	return state;
}