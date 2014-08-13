// Functions used for writing to the Digilent PmodCLS LCD display over the
// Cerebot 32MX7 I2C channel 1.
// The I2C functions are copied from MicroChip's I2C peripheral library
// example code (i2c_master.c).
//
// Change History:
//
// Name					Date		Changes
//
// Alazar Hailemariam	11/15/11		Final version.


#include <plib.h>
#include <string.h>
#include "HardwareProfile.h"
#include "LCD.h"
#include "i2c.h"

/*******************************************************************************
  Function:
    void initLCD(void)

  Summary:
    Enables the LCD's I2C bus, resets and clears the display.

  Description:
    This routine sets the I2C bus speed, enables the I2C bus, 
    and sends LCD commands to reset and clear the display.

  Precondition:
    None.

  Parameters:
    None.
    
  Returns:
    None.
    
  Example:
    <code>
    initLCD();
    </code>

  Remarks:
    Since the LCD is a write-only device, no error diagnostics are provided.
  *****************************************************************************/

void initLCD()
{
	const char resetDisplay[]  = "*";   // Equivalent to a cycling the power.
	const char enableDisplay[] = "1e";  // Enables display with the backlight off.
	// Reset and clear the display.
	lcdInstruction(resetDisplay);
	lcdInstruction(enableDisplay);	
}

/*******************************************************************************
  Function:
    BOOL lcdString(const char string[])

  Summary:
    Displays a character string on the PmodCLS LCD.

  Description:
    Each non-null byte in the character string is sent over the I2C bus to the
    LCD.  There is no length check on the string.

  Precondition:
    The LCD's I2C bus must have been initialized and enabled.

  Parameters:
    None.
    
  Returns:
    TRUE if the write succeeded, and FALSE otherwise.
    
  Example:
    <code>
    lcdString("Hello");
    </code>

  Remarks:
    All string characters are sent in a single I2C bus transaction,
    and there are no retries.
  *****************************************************************************/

BOOL lcdString(const char string[])
{
	return I2CSend(LCD_ADDRESS, strlen(string), string);
}


/*******************************************************************************
  Function:
    BOOL lcdInstruction(const char[] string)

  Summary:
    Writes an instruction (command) to the PmodCLS LCD.

  Description:
    Each non-null byte in the character string is sent over the I2C bus to the
    LCD.  At most 28 bytes from the instruction string are sent.

  Precondition:
    The LCD's I2C bus must have been initialized and enabled.

  Parameters:
    A pointer to a null-terminated string of at most 28 characters.
    
  Returns:
    TRUE if the write succeeded, and FALSE otherwise.
    
  Example:
    <code>
    lcdInstruction("*");  // performs a reset
    </code>

  Remarks:
    The instruction escape sequence is prepended to the string.
  *****************************************************************************/

BOOL lcdInstruction(const char string[])
{
	char instruction[32] = "\x1B[";  // Instruction escape preamble.

	return lcdString(strncat(instruction,string,28));
}
