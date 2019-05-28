enum DetectMetalStates {DetectMetalStart, WaitForMetal, MetalSignal} DetectMetalState;

unsigned char MetalDetected; //Global variable

int DetectMetalTick(int state){
	static unsigned char i;
	const unsigned short threshHold = 156;
	switch(state){	
		case DetectMetalStart:
			state = WaitForMetal;
			i = 0;
			break;

		case WaitForMetal:
			state = ADC <= threshHold ? MetalSignal : WaitForMetal;
			break;
		
		case MetalSignal:	
			if (i < 5 || ADC <= threshHold ){    // Once a metal is detected, rise the metalDetected flag for 1/2 second so that the LEDSM can read it 
				state = MetalSignal;
			}
			else if (i >= 5 && ADC > threshHold){
				state = WaitForMetal;
				i = 0;
			}
			break;
			
		default:
			state = DetectMetalStart;
			break;
	}
	switch(state){
		case DetectMetalStart:
			break;
			
		case WaitForMetal:
			MetalDetected = 0;
			break;
			
		case MetalSignal:
			MetalDetected = 1;
			i++;
			break;
		
		default:
			break;	
	}
	return state;
}
