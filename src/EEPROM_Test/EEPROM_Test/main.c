/*
 * EEPROM_Test.c
 *
 * Created: 6/5/2019 2:04:49 AM
 * Author : joan1
 */ 

#include <avr/io.h>

void EEPROM_write(unsigned int uiAddress, unsigned char ucData)
{
	/* Wait for completion of previous write */
	while(EECR & (1<<EEPE))
	;
	/* Set up address and Data Registers */
	EEAR = uiAddress;
	EEDR = ucData;
	/* Write logical one to EEMPE */
	EECR |= (1<<EEMPE);
	/* Start eeprom write by setting EEPE */
	EECR |= (1<<EEPE);
}

unsigned char EEPROM_read(unsigned int uiAddress)
{
	/* Wait for completion of previous write */
	while(EECR & (1<<EEPE))
	;
	/* Set up address register */
	EEAR = uiAddress;
	/* Start eeprom read by writing EERE */
	EECR |= (1<<EERE);
	/* Return data from Data Register */
	return EEDR;
}

int main(void)
{
    DDRB = 0xFF; PORTB = 0x00;
	EEPROM_write(0x00, 5);
    while (1) 
    {
		if (EEPROM_read(0x00) == 4){
			PORTB = 0xFF;
		}
		else {
			PORTB = 0x00;
		}
    }
}

