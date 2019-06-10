
enum CounterStates {CounterStart, WaitCounter, CheckForMetals} CounterState;
//Global variables
unsigned char MetalDetected;
unsigned char buttonPressed;

//Helper function
void DisplayMetalsFound(int MetalsFound);

int CounterTick(int state){
	static unsigned int MetalsFound;
    switch (state){
        case CounterStart:
			if (!buttonPressed){
				state = CounterStart;
			}
			else {
				state = WaitCounter;
				MetalsFound = 0;
			}
            break;
			
		case WaitCounter:
			if (MetalDetected){
				state = CheckForMetals;
				if (MetalsFound >= 9999){ // If the number of metalsfound is 9999 than set it to 0
					MetalsFound = 0;
				}
				else {                    // Else increase it
					++MetalsFound;
	
				}
				DisplayMetalsFound(MetalsFound);
			}
			else {
				state = WaitCounter;
			}
			break;
				
        case CheckForMetals:
            if (MetalDetected){ // if a metal objects is detected then increase the number of metals found otherwise, else keep the current number
                state = CheckForMetals;
            }
			else {
				state = WaitCounter;	
			}
            break;                

        default:
            state = CounterStart;
            break;
    }
    return state;
}

void DisplayMetalsFound(int MetalsFound){
	// Transforming the number of MetalsFound into an array of chars
	char NumToDisplay[4] = {'0', '0', '0', '0'};
	if (MetalsFound >= 0 && MetalsFound < 10){ // We know the integer is 1 digit
		char t1[1];
		sprintf(t1, "%d", MetalsFound);
		NumToDisplay[3] = t1[0];
	}
	else if (MetalsFound >= 10 && MetalsFound < 100){ // We know the integer is 2 digits
		char t2[2];
		sprintf(t2, "%d", MetalsFound);
		NumToDisplay[2] = t2[0];
		NumToDisplay[3] = t2[1];
	}
	else if (MetalsFound >= 100 && MetalsFound < 1000){ // We know the integer is 3 digits
		char t3[3];
		sprintf(t3, "%d", MetalsFound);
		NumToDisplay[1] = t3[0];
		NumToDisplay[2] = t3[1];
		NumToDisplay[3] = t3[2];
	}
	else if (MetalsFound >= 1000 && MetalsFound < 10000){ // We know the integer is 4 digits
		sprintf(NumToDisplay, "%d", MetalsFound);
	}
	
	//Displaying the number of MetalsFound in the LCD
	for (unsigned char i = 0; i < 4; i++){
		LCD_Cursor(i + 3);
		LCD_WriteData(NumToDisplay[i]);
	}
	
}