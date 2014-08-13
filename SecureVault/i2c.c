// Functions and data used to initialize I2C, 
// send and recieve to data using eeprom
// and aslo other functions which make this possible
// Change History:
//
// Name					Date		Changes
//
// Alazar Hailemariam	11/15/11		Final version.
#include "i2c.h"
#include "HardwareProfile.h"
#include <plib.h>
#include <string.h>


/*******************************************************************************
  Function:
    BOOL StartTransfer( BOOL restart )

  Summary:
    Starts (or restarts) a transfer to/from the EEPROM.

  Description:
    This routine starts (or restarts) a transfer to/from the LCD, waiting (in
    a blocking loop) until the start (or re-start) condition has completed.

  Precondition:
    The I2C module must have been initialized.

  Parameters:
    restart - If FALSE, send a "Start" condition
            - If TRUE, send a "Restart" condition
    
  Returns:
    TRUE    - If successful
    FALSE   - If a collision occured during Start signaling
    
  Example:
    <code>
    StartTransfer(FALSE);
    </code>

  Remarks:
    This is a blocking routine that waits for the bus to be idle and the Start
    (or Restart) signal to complete.
  *****************************************************************************/

BOOL StartTransfer( BOOL restart )
{
    I2C_STATUS  status;

    // Send the Start (or Restart) signal
    if(restart)
    {
        I2CRepeatStart(LCD_I2C_BUS);
    }
    else
    {
        // Wait for the bus to be idle, then start the transfer
        while( !I2CBusIsIdle(LCD_I2C_BUS) );

        if(I2CStart(LCD_I2C_BUS) != I2C_SUCCESS)
        {
            DBPRINTF("Error: Bus collision during transfer Start\n");
            return FALSE;
        }
    }

    // Wait for the signal to complete
    do
    {
        status = I2CGetStatus(LCD_I2C_BUS);

    } while ( !(status & I2C_START) );

    return TRUE;
}

/*******************************************************************************
  Function:
    void StopTransfer( void )

  Summary:
    Stops a transfer to the LCD.

  Description:
    This routine Stops a transfer to the LCD, waiting (in a 
    blocking loop) until the Stop condition has completed.

  Precondition:
    The I2C module must have been initialized & a transfer started.

  Parameters:
    None.
    
  Returns:
    None.
    
  Example:
    <code>
    StopTransfer();
    </code>

  Remarks:
    This is a blocking routine that waits for the Stop signal to complete.
  *****************************************************************************/

void StopTransfer( void )
{
    I2C_STATUS  status;

    // Send the Stop signal
    I2CStop(LCD_I2C_BUS);

    // Wait for the signal to complete
    do
    {
        status = I2CGetStatus(LCD_I2C_BUS);

    } while ( !(status & I2C_STOP) );
}

/*******************************************************************************
  Function:
    BOOL TransmitOneByte( UINT8 data )

  Summary:
    This transmits one byte to the LCD.

  Description:
    This transmits one byte to the LCD, and reports errors for any bus
    collisions.

  Precondition:
    The transfer must have been previously started.

  Parameters:
    data    - Data byte to transmit

  Returns:
    TRUE    - Data was sent successfully
    FALSE   - A bus collision occured

  Example:
    <code>
    TransmitOneByte(0xAA);
    </code>

  Remarks:
    This is a blocking routine that waits for the transmission to complete.
  *****************************************************************************/

BOOL TransmitOneByte( UINT8 data )
{
    // Wait for the transmitter to be ready
    while(!I2CTransmitterIsReady(LCD_I2C_BUS));

    // Transmit the byte
    if(I2CSendByte(LCD_I2C_BUS, data) == I2C_MASTER_BUS_COLLISION)
    {
        DBPRINTF("Error: I2C Master Bus Collision\n");
        return FALSE;
    }

    // Wait for the transmission to finish
    while(!I2CTransmissionHasCompleted(LCD_I2C_BUS));

    return TRUE;
}

BOOL ReceiveOneByte(UINT8* data, BOOL ack)
{
	if(I2CReceiverEnable(I2C1, TRUE)==I2C_SUCCESS)
	{
		while(!I2CReceivedDataIsAvailable(I2C1));
		*data = I2CGetByte(I2C1);
		I2CAcknowledgeByte(I2C1, ack);
		while(!I2CAcknowledgeHasCompleted(I2C1));
		return TRUE;
	}
	return FALSE;
}

void initI2C()
{
    // Set the I2C baudrate.
    I2CSetFrequency(LCD_I2C_BUS, GetPeripheralClock(), I2C_CLOCK_FREQ);
	// Enable the I2C bus.
    I2CEnable(LCD_I2C_BUS, TRUE);
}
BOOL I2CSend(UINT8 address, int length, const UINT8* buffer)
{
	BOOL success = TRUE;
	if(StartTransfer(FALSE)){
		I2C_7_BIT_ADDRESS SlaveAddress;
		I2C_FORMAT_7_BIT_ADDRESS(SlaveAddress, address, I2C_WRITE);
		if(TransmitOneByte(SlaveAddress.byte)){
			int x = 0;
			while(x<length){	//possibly 
				if(TransmitOneByte(buffer[x]))
					x++;
				else{
					success = FALSE;
					break;
				}
			}
			if(success)
				StopTransfer();
		}
		else
			success = FALSE;
	}
	else
		success = FALSE;
	return success;
}
	
BOOL I2CRecv(UINT8 address, int length, UINT8* buffer)
{
	BOOL ack = TRUE;
	BOOL success = TRUE;
	if(StartTransfer(FALSE)){
		I2C_7_BIT_ADDRESS SlaveAddress;
		I2C_FORMAT_7_BIT_ADDRESS(SlaveAddress, address, I2C_READ);
		if(TransmitOneByte(SlaveAddress.byte)){
			int x = 0;
			while(x<length){	//possibly 
				if(x == length-1)
					ack = FALSE;
				if(ReceiveOneByte(buffer+x, ack)){
					x++;
				}
				else{
					success = FALSE;
					break;
				}
			}
			if(success)
				StopTransfer();
		}
		else
			success = FALSE;
	}
	else
		success = FALSE;
	return success;
}

