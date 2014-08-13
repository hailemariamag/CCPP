// A simple application targeting the Digilent Cerebot 32MX7 board connected
// to a Digilent PmodCLS LCD display using I2C Channel 1.
// An hours/minutes/second timer is displayed on the first line of the LCD.
// The elaped time is maintained using the Timer 2 peripheral, which generates
// an interrupt every second.
// Although the PIC32's Real Time Clock and Calendar (RTCC) would be simpler
// to use, the RTCC is unavailable because the Cerebot board does not have the
// 32.768 kHz secondary oscillator needed by the RTCC.
//
// This example illustrates encapsulating an application into separate tasks,
// files and scopes.
// Function and variable scope is minimized, any global variables are accessed
// through functions, and delays are implemented with timers rather than loops.
// The end result is a succinct main() function with fewer than 10 statements.
//
// Change History:
//
// Name					Date		Changes
//
// Alazar Hailemariam	11/15/11		Final version.


#include <plib.h>
#include "Clock.h"
#include "LCD.h"
#include "Button.h"
#include "Rotary.h"
#include "stdio.h"
#include "EEPROM.h"
#include "i2c.h"
#include "SecureVault.h"

// Configuration Bit settings
//
// SYSCLK = 80 MHz (8MHz Crystal/ FPLLIDIV * FPLLMUL / FPLLODIV)
// PBCLK = 10 MHz
// WDT OFF

#ifndef OVERRIDE_CONFIG_BITS
        
    #pragma config FPLLMUL  = MUL_20        // PLL Multiplier
    #pragma config FPLLIDIV = DIV_2         // PLL Input Divider
    #pragma config FPLLODIV = DIV_1         // PLL Output Divider
    #pragma config FPBDIV   = DIV_8         // Peripheral Clock divisor
    #pragma config FWDTEN   = OFF           // Watchdog Timer 
    #pragma config WDTPS    = PS1           // Watchdog Timer Postscale
    #pragma config FCKSM    = CSDCMD        // Clock Switching & Fail Safe Clock Monitor
    #pragma config OSCIOFNC = OFF           // CLKO Enable
    #pragma config POSCMOD  = HS            // Primary Oscillator
    #pragma config IESO     = OFF           // Internal/External Switch-over
    #pragma config FSOSCEN  = OFF           // Secondary Oscillator Enable
    #pragma config FNOSC    = PRIPLL        // Oscillator Selection
    #pragma config CP       = OFF           // Code Protect
    #pragma config BWP      = OFF           // Boot Flash Write Protect
    #pragma config PWP      = OFF           // Program Flash Write Protect
    #pragma config ICESEL   = ICS_PGx1      // ICE/ICD Comm Channel Select
    #pragma config DEBUG    = OFF           // Debugger Disabled for Starter Kit
            
#endif // OVERRIDE_CONFIG_BITS


// ****************************************************************************
// ****************************************************************************
// Application Main Entry Point
// ****************************************************************************
// ****************************************************************************

int main()
{
	// Enable interrupts after setting up the LCD and timer peripherals.
	initI2C();
	initLCD();
	initClock();
	initRotaryKnob();
	INTEnableSystemMultiVectoredInt();
	initButtons();
	initVault();

	// Loop until a reset.
	
	while(1) 
	{
		welcomeScreen();
		enterVault();
	}
	
	// Unreachable, but main() requires a return code.
	return 0;
}
