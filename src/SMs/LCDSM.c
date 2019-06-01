enum LCDStates {LCDStart, LCDWaitForButon, SadFace, HappyFace} LCDstate;

//Global variables
unsigned char buttonPressed;
unsigned char MetalDetected;
unsigned int MetalsFound; // Number of metals found as Magnitie travels

//Helper functions
void InitCustomCharacters();
void DisplayMetalsFound(int MetalsFound);

int LCDTick(int state){
	static unsigned char i;
	switch(state){
		case LCDStart:
			LCD_DisplayString(1, "Press the button to start! :D"); //Display the start message
			state = LCDWaitForButon;
			break;
		
		case LCDWaitForButon:
			if (buttonPressed){
				state = SadFace;
				i = 0;
				LCD_DisplayString(1, "               ");// Clear the screen
				InitCustomCharacters(); // Initialize the sad face
			}
			else {
				state = LCDWaitForButon;
			}
			break;
		
		case SadFace:
			if (MetalDetected){
				LCD_Cursor(30);
				LCD_WriteData(2);
				state = HappyFace;
				LCD_Cursor(29);   // Display the happy mouth
				LCD_WriteData(1);
			}
			else {
				state = SadFace;
			}
			break;
		
		case HappyFace:
			if (i < 10 || MetalDetected){ // Once a metal is detected, keep the Displaying Happy face for 5 second
				state = HappyFace;
			}
			else if (i >= 10 && !MetalDetected){
				state = SadFace;
				i = 0;
				LCD_Cursor(29);   // Display the sad mouth
				LCD_WriteData(5);
				LCD_Cursor(30);
				LCD_WriteData(6);
			}
			break;
		
		default:
			state = LCDStart;
			break;
	}
	
	switch(state){
		case LCDStart:
			break;
		
		case LCDWaitForButon:
			break;
		
		case SadFace:
			break;

		case HappyFace:
			i++;
			break;
		
		default:
			break;
	}
	return state;
}


void InitCustomCharacters(){                    //================EYE=========================/=============Happy Left Mouth=================/=============Happy Right Mouth==================/============Sad Left Mouth===================/===============Sad Right Mouth====================/================Normal Left Mouth===============/================Normal Right Mouth============/
	const  unsigned char customCharacters[56] = {0x1B, 0x15, 0x0E, 0x1F, 0x11, 0x11, 0x11, 0x1F, 0x1E, 0x1E, 0x0E, 0x0F, 0x07, 0x00, 0x10, 0x18, 0x0F, 0x0F, 0x0E, 0x1E, 0x1C, 0x00, 0x01, 0x03, 0x1E, 0x1E, 0x1F, 0x18, 0x10, 0x00, 0x07, 0x0F, 0x0F, 0x0F, 0x1F, 0x03, 0x01, 0x00, 0x1C, 0x1E, 0x1E, 0x1E, 0x1F, 0x10, 0x10, 0x10, 0x1C, 0x1E, 0x0F, 0x0F, 0x1F, 0x01, 0x01, 0x01, 0x07, 0x0F};
	unsigned char j;
	LCD_WriteCommand(0x40); // With this command we can save custom characters in the memory of the LCD
	//Loading custom characters
	for (j = 0; j < 56; j++){
		LCD_WriteData(customCharacters[j]);
	}

	// Characters' locations in LCD Memory
	///0   -> Eye
	//1,2 -> Happy Face
	//3,4 -> Sad Face
	//5,6 -> Normal Face
	
	// Number of Metals and Objects
	LCD_Cursor(1);   //Displays 'M' for metals found
	LCD_WriteData(0x4D);
	LCD_Cursor(2);   //Displays Semicolon
	LCD_WriteData(0x3A);
	
	//This displays the initial zeros
	LCD_Cursor(3);
	LCD_WriteData('0');
	LCD_Cursor(4);
	LCD_WriteData('0');
	LCD_Cursor(5);
	LCD_WriteData('0');
	LCD_Cursor(6);
	LCD_WriteData('0');

	LCD_Cursor(17);  //Displays 'O' for how far is an obstacle at
	LCD_WriteData(0x4F);
	LCD_Cursor(18);   //Displays Semicolon
	LCD_WriteData(0x3A);
	
	LCD_Cursor(19);
	LCD_WriteData('0');
	LCD_Cursor(20);
	LCD_WriteData('0');
	LCD_Cursor(21);
	LCD_WriteData('0');
	LCD_Cursor(22);
	LCD_WriteData('0');
	
	//Displaying White spaces of the face
	LCD_Cursor(28);
	LCD_WriteData(0xFF);
	
	LCD_Cursor(31);
	LCD_WriteData(0xFF);
	
	LCD_Cursor(13);
	LCD_WriteData(0xFF);
	
	LCD_Cursor(14);
	LCD_WriteData(0xFF);
	
	//Displaying ears
	LCD_Cursor(11);
	LCD_WriteData(0xF1);
	
	LCD_Cursor(16);
	LCD_WriteData(0xF0);
	
	//Displaying eyes
	LCD_Cursor(12);
	LCD_WriteData(0);
	
	LCD_Cursor(15);
	LCD_WriteData(0);
	
	LCD_Cursor(29);   // Displays the normal mouth
	LCD_WriteData(5);
	LCD_Cursor(30);
	LCD_WriteData(6);
}
