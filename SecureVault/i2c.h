// Functions and data used to initialize I2C, 
// send and recieve to data using eeprom
// and aslo other functions which make this possible
// Change History:
//
// Name					Date		Changes
//
// Alazar Hailemariam	11/15/11		Final version.
#include <plib.h>

#ifndef i2c_H
#define i2c_H


void initI2C();

BOOL I2CSend(UINT8 address, int length, const UINT8* buffer);

BOOL I2CRecv(UINT8 address, int length, UINT8* buffer);

#endif
