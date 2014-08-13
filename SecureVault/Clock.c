// Functions and data used to provide the elapsed time measured
// in seconds, minutes, and hours.
// The targeted board is a Digilent Cerebot 32MX7.
// The RTCC peripheral is not used because of a missing watch
// crystal on the Cerebot board.
// A timer 2 interrupt updates the time every second.
//
// Change History:
//
// Name					Date		Changes
//
// Alazar Hailemariam	11/15/11		Final version.


#include <plib.h>
#include "Clock.h"
#include "Button.h"


extern int sprintf(char *, const char *, ...);


// A structure capturing time expressed in seconds, minutes and hours,
// and a flag indicating that the seconds field has been incremented.

typedef struct __timeStruct {
	unsigned int seconds;
	unsigned int minutes;
	unsigned int hours;
	BOOL       updateSecond;
} timeStruct;


// An instance of the time structure.

static volatile timeStruct currentTime;
unsigned int update;

/*******************************************************************************
  Function:
    void _Timer2Handler(void)

  Summary:
    An ISR for Timer 2 interrupts.

  Description:
    The ISR increments a time structure containing the elapsed seconds, minutes,
    and hours since the timer was initialized and interrupts were enabled.

  Precondition:
    It is assumed that Timer 2 has been initialized to generate an interrupt
    every second, and that interrupts have been enabled.

  Parameters:
    None.
    
  Returns:
    None.
    
  Example:
    Not directly callable by other functions.

  Remarks:
    A local updateSecond flag is asserted when the ISR is called.
  *****************************************************************************/

static void __ISR(_TIMER_2_VECTOR, ipl7) _Timer2Handler(void)
{
	// Clear the interrupt flag.
	mT2ClearIntFlag();
	sampleButtons();
	// Increment seconds, and possibly roll over to minutes and hours.
    //??
	if(currentTime.seconds<59999)
	{
		currentTime.seconds++;
	}
	else
	{
		currentTime.seconds=0;
		if(currentTime.minutes<59)
		{
			currentTime.minutes++;
		}
		else
		{
			currentTime.minutes=0;
			if(currentTime.hours<99)
			{
				currentTime.hours++;
			}
			else
			{
				currentTime.hours=0;
			}
		}
	}
	update++;
	if(update == 1000)
	{
		currentTime.updateSecond = TRUE;
		update = 0;
	}
}


/*******************************************************************************
  Function:
    void initClock(void)

  Summary:
    Sets up Timer 2 to maintain the elapsed time measured in seconds, minutes,
    and hours.

  Description:
    Timer 2 will update a structure containing the elapsed seconds, minutes,
    and hours since the timer was initialized and interrupts were enabled.

  Precondition:
    It is assumed that the peripheral bus runs at 10 MHz.

  Parameters:
    None.
    
  Returns:
    None.
    
  Example:
    <code>
    initClock()
	</code>

  Remarks:
    This function does not enable interrupts.
  *****************************************************************************/

void initClock()
{
	// Reset the curent time fields.
	//??
	currentTime.seconds = 0;
	currentTime.minutes = 0;
	currentTime.hours = 0;
	update = 0;

	// Setup timer 2 to interrupt once per second at interrupt priority 7.
	// Assuming a processor frequency of 80 MHz, peripheral bus frequency set
	// to 1/8 of the processor frequency, and a timer prescale of 256, we get 
	// 80,000,000 / (8 * 256) = 39062 ticks.
	// interrupts can be increased from one per second up to ten per second
	// Using function speedUpdate() in button.h which return an int between 1
	// and 10
//	OpenTimer2(T2_ON | T2_SOURCE_INT | T2_PS_1_256, (39062/speedUpdate()));
	OpenTimer2(T2_ON | T2_SOURCE_INT | T2_PS_1_256, (39062/1000));
	ConfigIntTimer2(T2_INT_ON | T2_INT_PRIOR_7);
}



	

/*******************************************************************************
  Function:
    BOOL secondTick(void)

  Summary:
    Indicates whether the current time has changed since the last query.

  Description:
    This function is polled to check whether a second has elapsed as measured by
    Timer 2.

  Precondition:
    It is assumed that initClock() has been called and interrupts are enabled.

  Parameters:
    None.
    
  Returns:
    TRUE  if the second count has been updated since the last successful check.
    FALSE if the second count has not been updated since the last successful check.
    
  Example:
    <code>
    if secondTick() {...}
	</code>

  Remarks:
    This function provides an alternative to using a global variable.
    If a second has elapsed, the updateSecond variable is reset.
  *****************************************************************************/

BOOL secondTick()
{
	if (currentTime.updateSecond) 
	{
		currentTime.updateSecond = FALSE;
		return TRUE;
	}

	return FALSE;
}


/*******************************************************************************
  Function:
    void getCurrentClockString(char string[])

  Summary:
    Writes the current time to the string argument.

  Description:
    The current time is formatted as "Timer: HH:MM:SS".

  Precondition:
    It is assumed that initClock() has been called and interrupts are enabled.

  Parameters:
    A character array having a length of at least 18.
    
  Returns:
    None.
    
  Example:
    <code>
    char clockString[20];
    getCurrentClockString(clockString);
	</code>

  Remarks:
    The sprintf() library function is used to format the string.
  *****************************************************************************/

void getCurrentClockString(char string[])
{
	//??
	//string = "fes;alksdjfa;slkdjf";
	sprintf(string,"Timer: %2.2u:%2.2u:%2.2u",currentTime.hours,currentTime.minutes,currentTime.seconds);
}
