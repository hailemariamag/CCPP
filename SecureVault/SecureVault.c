// Functions initializing vault
// Functions for entering and reseting password,
// and for editing data in the vault with reset or correct password
// Change History:
//
// Name					Date		Changes
//
// Alazar Hailemariam	11/15/11		Final version.

#include <plib.h>
#include <string.h>
#include <stdio.h>
#include "SecureVault.h"
#include "LCD.h"
#include "Button.h"
#include "EEPROM.h"
#include "Clock.h"
#include "i2c.h"
#include "rotary.h"

static volatile UINT8 pass[3];	//For storing user input password
static BOOL button1;			//Flag for printing welcome screen

//Initialize values
void initVault()
{
	button1 = TRUE;
}

//Prints Welcome screen
void welcomeScreen()
{	
	if(button1)
	{
		lcdInstruction("j");
		while(!secondTick());
		while(!secondTick());
		lcdString("Secure Vault");
		button1 = FALSE;
		
	}
}
//Function to enter password or reset password
void enterVault()
{
	char display[8];
	pass[0] = 0x00;
	pass[1] = 0x00;
	pass[2] = 0x00;
	if(buttonPressed(BTN_ROTARY))	//Section for entering password
	{	
		UINT8 mem[3];
		eepromRead(0x0000, 3, mem);	//Read stored password from eeprom
		int y;
		y = 0;
		lcdInstruction("j");
		lcdString("   Enter Key   "); 
		lcdInstruction("1;0H");
		lcdString("    ");
		sprintf(display, "%2.2X-%2.2X-%2.2X", pass[0], pass[1], pass[2]);
		lcdString(display);		
		while(TRUE)
		{
			int x;
			x = 0;
			if(buttonPressed(BTN_ROTARY))
				y++;
			x = readRotaryKnob();
			if(x != 0 || y == 3)
			{	
				if(switchState(SWITCH_ROTARY))
					x = x*8;
				pass[y] += x;
				lcdInstruction("j");
				lcdString("   Enter Key   "); 
				lcdInstruction("1;0H");
				lcdString("    ");
				sprintf(display, "%2.2X-%2.2X-%2.2X", pass[0], pass[1], pass[2]);
				lcdString(display); 
				if(y == 3)				
				{
					lcdInstruction("j");
					if(mem[0] == pass[0] && mem[1] == pass[1] && mem[2] == pass[2])		//Compare passwords
					{
						lcdString("  Key Correct! ");
						while(!secondTick());
						while(!secondTick());
						vault();				//If correct go to data editing function
						break;
					}
					else
					{
						lcdString(" Key Incorrect! ");		//If incorrect print message and break
						while(!secondTick());
						while(!secondTick());
						break;
					}
				}
			}
		}			
	}
	
	else if(buttonPressed(BTN3))		//Section for resetting password
	{
		char data[17] = "abcdefghijklmnop";		//Used to reset data on eeprom
		int y;
		y = 0;
		lcdInstruction("j");
		lcdString("   Reset Key   "); 
		lcdInstruction("1;0H");
		lcdString("    ");
		sprintf(display, "%2.2X-%2.2X-%2.2X", pass[0], pass[1], pass[2]);
		lcdString(display);
		while(TRUE)
		{
			int x;
			x = 0;
			if(buttonPressed(BTN_ROTARY))
				y++;
			x = readRotaryKnob();
			if(x != 0 || y == 3)
			{	
					if(switchState(SWITCH_ROTARY))	
						x = x*8;
					pass[y] += x;
					lcdInstruction("j");
					lcdString("   Reset Key   "); 
					lcdInstruction("1;0H");
					lcdString("    ");
					sprintf(display, "%2.2X-%2.2X-%2.2X", pass[0], pass[1], pass[2]);
					lcdString(display); 
					if(y == 3)
					{
						lcdInstruction("j");
						lcdString("   Key Reset!   ");
						eepromWrite(0x0000, 3, pass);		//store new password
						while(!secondTick());
						while(!secondTick());
						while(!secondTick());
						eepromWrite(0x0003, 16, data);		//reset data
						vault();
						break;
					}
				}
			}
	}
}

//Function to store and edit user data
void vault()
{
	char cursor[5];		//used to format cursor
	UINT8 data[17];		//used to store the user data
	while(!secondTick());
	while(!secondTick());
	eepromRead(0x0003, 16, data);
	lcdInstruction("j");
	lcdString("   Edit Data: "); 
	lcdInstruction("1;0H");
	lcdString(data);
	lcdInstruction("1;0H");
	lcdInstruction("1c");
	
	int y;
	y = 0;
	while(TRUE)
	{
		int x;
		x = 0;
		if(buttonPressed(BTN_ROTARY))
		{
			y++;
			if(y == 16)
				y = 0;
			sprintf(cursor, "1;%dH", y);
 			lcdInstruction(cursor);
		}
		x = readRotaryKnob();
		if(x != 0)					//To edit data
		{
			if(switchState(SWITCH_ROTARY))
				x = x*8;
			data[y] += x;
			if((int)data[y] >= 123)
			{	
				int k;
				k = (int)data[y] -123;// = k;  97;
				data[y] = 97 + k;
			}
			if((int)data[y] <= 96)
			{
				int k;
				k = 96 - (int)data[y];
				data[y] = 122 - k;
			}
			lcdInstruction("j");
			lcdString("   Edit Data: "); 
			lcdInstruction("1c");
			lcdInstruction("1;0H");			
			lcdString(data);
			sprintf(cursor, "1;%dH", y);
 			lcdInstruction(cursor);
		}
		if(buttonPressed(BTN1))		//To exit data entry 
		{
			lcdInstruction("j");
			lcdInstruction("0c");
			lcdString("   Data Saved   ");
			eepromWrite(0x0003, 16, data);
			button1 = TRUE;
			while(!secondTick());
			while(!secondTick());
			break;
		}
	}

}

