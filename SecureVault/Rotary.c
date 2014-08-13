// Functions and data used to initialize rotary knob
// and also read its current state
// Change History:
//
// Name					Date		Changes
//
// Alazar Hailemariam	11/15/11		Final version.

#include "Rotary.h"
#include "HardwareProfile.h"
#include <plib.h>


static volatile BOOL q1;
static volatile BOOL q2;
static volatile BOOL prev_q1;

void initRotaryKnob()
{
	//-Disable-the-JTAG-port-in-order-to-use-RA0-
	DDPCONbits.JTAGEN = 0;
	//-Turn-off-all-analog-inputs
	AD1PCFG = 0xFFFF;
	mPORTFSetPinsDigitalIn(BIT_12);		//JF1
	mPORTFSetPinsDigitalIn(BIT_5);		//JF2
	q1 = TRUE;
	q2 = TRUE;
}

int readRotaryKnob()
{
	BOOL a;
	BOOL b;
	prev_q1 = q1;
	a=(0<mPORTFReadBits(BIT_12));
	b=(0<mPORTFReadBits(BIT_5));
	
	if(a==b)
		q1 = a;
	else 
		q2 = b;

	if(!prev_q1 && q1){
		if(q2)
			return 1;
		return -1;
	}
	return 0;	 	
}
