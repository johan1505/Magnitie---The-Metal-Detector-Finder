enum CompareDistanceStates {Compare} CompareDistanceState;
//Global variables
unsigned char BREAK;
uint16_t ObstacleDistance;

int CompareDistancesTick(int state){
    switch (state){
        case Compare:
            state = Compare;
            if (ObstacleDistance == 0xFFFF){ // Error signal from the pulse function, object is around 13 in. from Magnitie so BREAK!
	            BREAK = 1;
            }
            else {
                BREAK = 0;
            }
            break;
    
        default:
            state = Compare;
            break;
    }
    return state;
}