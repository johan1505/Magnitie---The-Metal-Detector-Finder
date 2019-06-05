enum LightsStates { LightsOff, LightsOn, BlinkLights, BreakLights } LightsState;

//Global variables
unsigned char buttonPressed;
unsigned char MetalDetected;
unsigned char LightsOutput;
unsigned char BREAK;

int LightsTick(int state){
	static unsigned char i;
	switch(state){
		case LightsOff:
			if (buttonPressed && !BREAK){ // if there is a button pressed and no break signal then turn on lights
				state = LightsOn;
				i = 0;
			}  
			else if(buttonPressed && BREAK) {                  
				state = BreakLights;
			}
			else if (!buttonPressed && BREAK){
				state = BreakLights;
			}
			else {
				state = LightsOff;
			}
			
			break;
		
		case LightsOn:
			if (MetalDetected && BREAK){
				state = BreakLights;
				//might need to add stuff 
			}
			else if (MetalDetected && !BREAK){
				state = BlinkLights;
				LightsOutput = 0x00;
				i = 0;
			}
			else if (!MetalDetected && BREAK){
				state = BreakLights;
				//might need to add stuff 
			}
			else {
				state = LightsOn;
			}
			break;
		
		case BlinkLights:  //Double check this state later when testing
			if (i < 6 || MetalDetected){
				state = BlinkLights;
			}
			else if (i >= 6 && !MetalDetected){
				state = LightsOff;
			}
			break;
		
		case BreakLights:
			if (BREAK) { // as long as there is Break signal, stay on this state
				state = BreakLights;
			}
			else {       // Else turn off the lights and wait for user to turn them on by pressing the button
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
			else {
				LightsOutput = 0x03;
			}
			
			if (i < 3){
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
		
		case BreakLights:
			LightsOutput = 0xFF; //LIGHTS ON
			break;
			
		default:
			break;
	}
	return state;
}