// Function prototypes for initializing and reading a timer providing
// Functions prototypes for initializing buttons, sensing input from
// buttons, increase/decrease clock speed using buttons, and 
// inititializing clock again after a change in clock speed
// Change History:
//
// Name					Date		Changes
//
// Alazar Hailemariam	11/15/11		Final version.


#define BTN1	1
#define BTN2	2
#define BTN3	3
#define BTN_ROTARY	4
#define SWITCH_ROTARY	5

#ifndef Button_H
#define Button_H


void initButtons();

BOOL buttonPressed(unsigned int button);

unsigned int speedUpdate();

void speedChange();

BOOL switchState(unsigned int whichSwitch);

void sampleButtons();

#endif
