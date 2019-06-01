enum {Output};

//Global Variables
unsigned char MotorsOutput;
unsigned char LightsOutput;

int PORTBOutputTick(int state){
	switch(state){
		case Output:
			state = Output;
			PORTB = (LightsOutput << 2) | MotorsOutput;
			break;
	}
	return state;
}

