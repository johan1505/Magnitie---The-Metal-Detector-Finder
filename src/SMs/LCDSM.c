#include "EEPROM.h"
enum LCDStates {LCDStart, LCDWaitForButon, NormalFace, SadFace, HappyFace, ObstacleWarning} LCDstate;

//Global variables
unsigned char buttonPressed;
unsigned char MetalDetected;
unsigned char BREAK;
unsigned int MetalsFound; // Number of metals found as Magnitie travels

//Helper functions
void InitCustomCharacters();
void DisplayMetalsFound(int MetalsFound);
void DisplayRecordOfMetals(unsigned int record);
void DisplayClear();
void DisplayObstacleWarning();

int LCDTick(int state){
	static unsigned char i;
	switch(state){		
		case LCDStart:
			LCD_DisplayString(1, "Press the button to start! :D"); //Display the start message
			state = LCDWaitForButon;
			break;
		
		case LCDWaitForButon: // Wait for button. Need to consider the BREAK signal here before starting
			if (buttonPressed && !BREAK){
				state = NormalFace;
				i = 0;
				LCD_DisplayString(1, "                              ");
				InitCustomCharacters(); // Initialize 
				DisplayClear();
			}
			else if (BREAK){ // if there is a break than go to the ObstacleDetected state regardless
				state = ObstacleWarning;
				LCD_ClearScreen();
				LCD_DisplayString(1, "    OBSTACLE        DETECTED    ");// Display obstacle detected warning
			}
			else {
				state = LCDWaitForButon;
			}
			break;
		
		case NormalFace:
			if (MetalDetected){
				state = HappyFace;
				// Display happy mouth
				LCD_Cursor(29);
				LCD_WriteData(1);
				LCD_Cursor(30);
				LCD_WriteData(2);
			}
			else if (BREAK){
				state = SadFace;
				DisplayObstacleWarning();
				//Display sad mouth
				LCD_Cursor(29);
				LCD_WriteData(3);
				LCD_Cursor(30);
				LCD_WriteData(4);
			}
			else {
				state = NormalFace;
			}
			break;
		
		case HappyFace:
			if (i < 6 || MetalDetected){ // Once a metal is detected, keep the Displaying Happy face for 3 seconds
				state = HappyFace;
			}
			else if (i >= 6 && !MetalDetected){
				state = NormalFace;
				i = 0;
				LCD_Cursor(29);   // Display the sad mouth
				LCD_WriteData(5);
				LCD_Cursor(30);
				LCD_WriteData(6);
			}
			break;
		
		case SadFace:
			if (BREAK){
				state = SadFace;
			}
			else {
				state = NormalFace;
				DisplayClear();
				LCD_Cursor(29);   // Displays the normal mouth
				LCD_WriteData(5);
				LCD_Cursor(30);
				LCD_WriteData(6);				
			}
			break;
			
		case ObstacleWarning:
			if (BREAK){
				state = BREAK;
			}
			else {
				state = LCDWaitForButon;
				LCD_ClearScreen();
				LCD_DisplayString(1, "Press the button to start! :D");
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
		
		case NormalFace:
			break;
			
		case HappyFace:
			i++;
			break;
		
		case SadFace:
			break;
		
		case ObstacleWarning:
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

void DisplayRecordOfMetals(unsigned int record){
	char NumToDisplay[3] = {' ', ' ', ' '};
	if (record >= 0 && record < 10){ // We know the integer is 1 digit
		char t1[1];
		sprintf(t1, "%d", record);
		NumToDisplay[2] = t1[0];
	}
	else if (record >= 10 && record < 100){ // We know the integer is 2 digits
		char t2[2];
		sprintf(t2, "%d", record);
		NumToDisplay[1] = t2[0];
		NumToDisplay[2] = t2[1];
	}
	else if (record >= 100 && MetalsFound < 256){ // We know the integer is 3 digits
		sprintf(NumToDisplay, "%d", record);
	}
	
	//Displaying the number of MetalsFound in the LCD
	for (unsigned char i = 0; i < 3; i++){
		LCD_Cursor(i + 25);
		LCD_WriteData(NumToDisplay[i]);
	}
}

void DisplayClear(){
	LCD_Cursor(17);
	LCD_WriteData(0x43); //C
	LCD_Cursor(18);
	LCD_WriteData(0x7C); //l
	LCD_Cursor(19);
	LCD_WriteData(0x65); //e
	LCD_Cursor(20);
	LCD_WriteData(0x61); //a
	LCD_Cursor(21);
	LCD_WriteData(0x72); //r
	LCD_Cursor(22);
	LCD_WriteData(0x21); //!
	LCD_Cursor(23);
	LCD_WriteData(0x20); //" "
	LCD_Cursor(24);
	LCD_WriteData(0x20); //" "
	LCD_Cursor(25);
	LCD_WriteData(0x20); //" "
	LCD_Cursor(26);
	LCD_WriteData(0x20); //" "
}

void DisplayObstacleWarning(){
	LCD_Cursor(17);
	LCD_WriteData(0x4F); //O
	LCD_Cursor(18);
	LCD_WriteData(0x42); //B
	LCD_Cursor(19);
	LCD_WriteData(0x53); //S
	LCD_Cursor(20);
	LCD_WriteData(0x54); //T
	LCD_Cursor(21);
	LCD_WriteData(0x41); //A
	LCD_Cursor(22);
	LCD_WriteData(0x43); //C
	LCD_Cursor(23);
	LCD_WriteData(0x4C); //L
	LCD_Cursor(24);
	LCD_WriteData(0x45); //E
	LCD_Cursor(25);
	LCD_WriteData(0x21); //!
	LCD_Cursor(26);
	LCD_WriteData(0x21); //!
}