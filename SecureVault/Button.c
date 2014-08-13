// Functions and data used to initialize buttons, sense input from
// buttons, increase/decrease clock speed using buttons, and 
// inititialize clock again after a change in clock speed
// Change History:
//
// Name					Date		Changes
//
// Alazar Hailemariam	11/15/11		Final version.


#include <plib.h>
#include "Button.h"
#include "Clock.h"

//An unsigned in to manipulate clock speed
static volatile unsigned int speeds;
static volatile UINT8 samples[4];
static int waitingFor[4];

//Function to initialize buttons BTN1, BTN2, and BTN3
void initButtons()
{
	speeds = 1;
	int x;
	for(x = 0; x<4; x++)
	{
		samples[x] = 0x00;
		waitingFor[x] = 1;
	}
	mPORTGSetPinsDigitalIn(BIT_6 | BIT_7);
	mPORTDSetPinsDigitalIn(BIT_13 );
	mPORTFSetPinsDigitalIn(BIT_4 | BIT_13);
}

//Function to check if a button has been pressed
BOOL buttonPressed(unsigned int button)
{
	if(button == BTN1)
	{
		if((waitingFor[0] == 1) && (samples[0] == 0xff))
		{
			waitingFor[0] = 0;
			return TRUE;
		}
		else if((waitingFor[0] == 0) && (samples[0] == 0x00))
			waitingFor[0] = 1;
		return FALSE;
	} 
	else if(button == BTN2)
	{
		if((waitingFor[1] == 1) && (samples[1] == 0xff))
		{
			waitingFor[1] = 0;
			return TRUE;
		}
		else if((waitingFor[1] == 0) && (samples[1] == 0x00))
			waitingFor[1] = 1;
		return FALSE;
	}
	else if(button == BTN3)
	{
		if((waitingFor[2] == 1) && (samples[2] == 0xff))
		{
			waitingFor[2] = 0;
			return TRUE;
		}
		else if((waitingFor[2] == 0) && (samples[2] == 0x00))
			waitingFor[2] = 1;
		return FALSE;
	}
	else if(button == BTN_ROTARY)
	{
		if((waitingFor[3] == 1) && (samples[3] == 0xff))
		{
			waitingFor[3] = 0;
			return TRUE;
		}
		else if((waitingFor[3] == 0) && (samples[3] == 0x00))
			waitingFor[3] = 1;
		return FALSE;
	}	
	return FALSE;
}

//This function speeds up the clock if BTN2 is pressed
//it slows down the clock if BTN3 is pressed
//the number of ticks in the interrupt section of clock.c
//was divided by speedUpdate() to achieve this
unsigned int speedUpdate()
{
	if(buttonPressed(BTN2))
	{
		if(speeds > 10)
			speeds = 10;
		else
			speeds++;
	}
	else if(buttonPressed(BTN3))
	{
		if(speeds>1)
			speeds--;
		else
			speeds =1;	
	}
	return speeds;
}

//This function checks to see if the clock speed needs to be changed
//if the current speed does not match the desired speed, it calls
//initClock function to update the clock speed
//For this function to work properly, button.h was included
//in clock.c and the number of ticks in the interrupt section was 
//divided by speedUpdate() function
void speedChange()
{
	int x = speeds;
	if(x != speedUpdate())
		initClock();
}

BOOL switchState(unsigned int whichSwitch)
{
	if(whichSwitch == SWITCH_ROTARY)
	{
		return mPORTFReadBits(BIT_13);
	}
}

void sampleButtons()
{
	//BTN1
	samples[0] <<= 1;
	if(mPORTGReadBits(BIT_6))
		samples[0] |= 0x01;
	
	//BTN2
	samples[1] <<= 1;
	if(mPORTGReadBits(BIT_7))
		samples[1] |= 0x01;

	//BTN3	
	samples[2] <<= 1;
	if(mPORTDReadBits(BIT_13))
		samples[2] |= 0x01;
	
	//BTN_ROTARY		bit 4
	samples[3] <<= 1;
	if(mPORTFReadBits(BIT_4))
		samples[3] |= 0x01;
}	
