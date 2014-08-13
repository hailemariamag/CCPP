/******************************************************************************

Software License Agreement

The software supplied herewith by Microchip Technology Incorporated
(the “Company”) for its PICmicro® Microcontroller is intended and
supplied to you, the Company’s customer, for use solely and
exclusively on Microchip PICmicro Microcontroller products. The
software is owned by the Company and/or its supplier, and is
protected under applicable copyright laws. All rights are reserved.
Any use in violation of the foregoing restrictions may subject the
user to criminal sanctions under applicable laws, as well as to
civil liability for the breach of the terms and conditions of this
license.

THIS SOFTWARE IS PROVIDED IN AN “AS IS” CONDITION. NO WARRANTIES,
WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.

*******************************************************************************/

// Change History:
//
// Name					Date		Changes
//
// Alazar Hailemariam	11/15/11		Final version.


// Clock constants

#define SYS_CLOCK_FREQ        80000000
#define GetSystemClock()      (SYS_CLOCK_FREQ)
#define GetPeripheralClock()  (SYS_CLOCK_FREQ / 8)  // FPBDIV = DIV_8
#define GetInstructionClock() (SYS_CLOCK_FREQ)
#define I2C_CLOCK_FREQ        100000


// I2C constants

#define LCD_I2C_BUS           I2C1             // Channel 1
#define LCD_ADDRESS           0x48             // LCD address
#define EEPROM_ADDRESS		  0x50			   // EEPROM address

