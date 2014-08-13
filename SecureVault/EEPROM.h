// Functions prototypes used to initialize EEPROM,
// and also read and write to EEPROM
// Change History:
//
// Name					Date		Changes
//
// Alazar Hailemariam	11/15/11		Final version.
#include<plib.h>

#ifndef EEPROM_H
#define EEPROM_H


void initEEPROM();

void eepromWrite(UINT16 address, int length, UINT8* buffer);

void eepromRead(UINT16 address, int length, UINT8* buffer);


#endif
