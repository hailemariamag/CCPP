// Function prototypes for initializing and writing to the Digilent PmodCLS LCD.
//
// Change History:
//
// Name					Date		Changes
//
// Alazar Hailemariam	9/13/11		Final version.


#ifndef LCD_H
#define LCD_H

void initLCD();

BOOL lcdString(const char string[]);

BOOL lcdInstruction(const char string[]);

#endif
