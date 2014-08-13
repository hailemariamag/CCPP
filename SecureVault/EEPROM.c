// Functions and data used to initialize EEPROM,
// and also read and write to EEPROM
// Change History:
//
// Name					Date		Changes
//
// Alazar Hailemariam	11/15/11		Final version.

#include "EEPROM.h"
#include "HardwareProfile.h"
#include <plib.h>
#include <string.h>
#include "i2c.h"
void initEEPROM()
{
}
void eepromWrite(UINT16 address, int length, UINT8* buffer)
{
	UINT8 localbyte[66];
	localbyte[0] = (address>>8) & 0xFF;
	localbyte[1] = address & 0xFF;
	int x;
	for(x = 2; x<length+2; x++)
	{	
		localbyte[x] = buffer[x-2];
	}		
	I2CSend(EEPROM_ADDRESS, length+2, localbyte);
}

void eepromRead(UINT16 address, int length, UINT8* buffer)
{
	UINT8 localbyte[2];	
	localbyte[0] = (address>>8) & 0xFF;
	localbyte[1] = address & 0xFF;
	I2CSend(EEPROM_ADDRESS, 2, localbyte);
	I2CRecv(EEPROM_ADDRESS, length, buffer);	
}

