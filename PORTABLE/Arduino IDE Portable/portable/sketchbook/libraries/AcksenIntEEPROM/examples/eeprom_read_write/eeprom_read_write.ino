/***********************************************************
This source file is licenced using the 3-Clause BSD License.

Copyright (c) 2022 Acksen Ltd, All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission. 

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************/

/*
Example: 		eeprom_read_write.ino
Library:		AcksenIntEEPROM
Author: 		Richard Phillips for Acksen Ltd

Created:		    25 Jul 2022
Last Modified:	26 Jul 2022

Description:
Demonstrate via debug serial port, writing and reading of sequential values to Internal EEPROM on 8-bit AVR devices.

*/

#include <AcksenIntEEPROM.h>

// ***********************************
// Serial Debug
// ***********************************
#define DEBUG_BAUD_RATE			115200

// ***********************************
// I/O  
// ***********************************


// ***********************************
// Constants
// ***********************************
#define EEPROM_SETTINGS							360	// Start Address for EEPROM data
#define EEPROM_MAX_WRITES						300	// Maximum number of writes to EEPROM in one session

// ***********************************
// Variables
// ***********************************
AcksenIntEEPROM IntEEPROM(EEPROM_SETTINGS);	// Initialise EEPROM library with defined start address

// ************************************************
// Setup 
// ************************************************
void setup()
{

	// Initialise Serial Port
	Serial.begin(DEBUG_BAUD_RATE);
	
	// Initialise EEPROM variables for EEPROMEx
	EEPROM.setMemPool(0, EEPROMSizeATmega328);	// See EEPROMex.h for other options
	EEPROM.setMaxAllowedWrites(EEPROM_MAX_WRITES);
	
	Serial.println("Startup Complete!");
	Serial.println("");
 
}

// ************************************************
// Main Control Loop
// ************************************************
void loop()
{

	// Declare Variables to Write
	bool bTest = true;
	int iTest = 34;
	long lTest = 3423423;
	float fTest = 3.14159;
	
	// Output the write test variables via debug serial
	Serial.println("EEPROM Write Test:");
	Serial.println("******************");
	
	Serial.print("Bool = ");
	Serial.println(bTest);
	
	Serial.print("Int = ");
	Serial.println(iTest);

	Serial.print("Long = ");
	Serial.println(lTest);

	Serial.print("Float = ");
	Serial.println(fTest);
	
	Serial.println("");

	// Write data out to EEPROM
	IntEEPROM.resetPresentAddress();	// Ensure that EEPROM Present Memory Address is reset to Starting Address

	IntEEPROM.writeEEPROMValueBit(bTest);
	IntEEPROM.writeEEPROMValueInt(iTest);
	IntEEPROM.writeEEPROMValueLong(lTest);
	IntEEPROM.writeEEPROMValueFloat(fTest);
	
	// Declare Variables to Write
	bool bReadTest;
	int iReadTest;
	long lReadTest;
	float fReadTest;
	
	
	
	// Read Data back from EEPROM
	IntEEPROM.resetPresentAddress();	// Ensure that EEPROM Present Memory Address is reset to Starting Address

	// Read EEPROM Values
	bReadTest = IntEEPROM.readEEPROMValueBit();
	iReadTest = IntEEPROM.readEEPROMValueInt();
	lReadTest = IntEEPROM.readEEPROMValueLong();
	fReadTest = IntEEPROM.readEEPROMValueFloat();
	
	// Output the read test variables via debug serial
	Serial.println("EEPROM Read Test:");
	Serial.println("******************");

	Serial.print("Bool = ");
	Serial.println(bReadTest);
	
	Serial.print("Int = ");
	Serial.println(iReadTest);

	Serial.print("Long = ");
	Serial.println(lReadTest);

	Serial.print("Float = ");
	Serial.println(fReadTest);
	
	Serial.println("");



	// Validate the returned values
	bool bValidateInt;
	bool bValidateLong;
	bool bValidateFloat;
		
	bValidateInt = IntEEPROM.validateInt(0, 1000, iReadTest);
	bValidateLong = IntEEPROM.validateLong(0, 9999999, lReadTest);
	bValidateFloat = IntEEPROM.validateFloat(10, 20, fReadTest);	// Expected to fail
	
	// Output the validation test variables via debug serial
	Serial.println("EEPROM Validation Test:");
	Serial.println("******************");

	Serial.print("Int Validation (0 <= x <= 1000) = ");
  if (bValidateInt == true)
  {
    Serial.println("Passed");
  }
  else
  {
    Serial.println("Failed");
  }

	Serial.print("Long Validation (0 <= x <= 9999999) = ");
  if (bValidateLong == true)
  {
    Serial.println("Passed");
  }
  else
  {
    Serial.println("Failed");
  }
	
	Serial.print("Float Validation (10 <= x <= 20) = ");
  if (bValidateFloat == true)
  {
    Serial.println("Passed");
  }
  else
  {
    Serial.println("Failed");
  }
	
	Serial.println("");
	
	
	
	// Pause after single execution
	while(1);
	
}
