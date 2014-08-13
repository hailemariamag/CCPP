// Function prototypes for initializing and reading a timer providing
// elapsed hours, minutes and seconds since the last reset.
// Also function prototypes for increasing and decreasing clock speed
// and a function to call initClock again if clock speed is changed
// Change History:
//
// Name					Date		Changes
//
// Alazar Hailemariam	11/15/11		Final version.

#ifndef CLOCK_H
#define CLOCK_H

void initClock();

BOOL secondTick();

void getCurrentClockString(char string[]);



#endif
