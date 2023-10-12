/*!
@file AcksenIntEEPROM.h
 
*/
 
/***********************************************************
This source file is licenced using the 3-Clause BSD License.

Copyright (c) 2022 Acksen Ltd, All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission. 

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************/
// Acksen Internal EEPROM Library v1.1.0

// v1.1.0	25 Jul 2022
// - Add Button Mode to select between Normal, LongPress or Repeat modes at initialisation
//
// v1.0.0
// - Initial Version
//

#ifndef AcksenIntEEPROM_h
#define AcksenIntEEPROM_h

#define AcksenIntEEPROM_ver   110	///< Constant used to set the present library version. Can be used to ensure any code using this library, is correctly updated with necessary changes in subsequent versions, before compilation.

#include <EEPROMex.h>

// Constants
#define EEPROM_LONG_SIZE				4	///< Size of Long variables required in EEPROM memory, in bytes.
#define EEPROM_FLOAT_SIZE				4	///< Size of Float variables required in EEPROM memory, in bytes.
#define EEPROM_INT_SIZE					2	///< Size of Int variables required in EEPROM memory, in bytes.
#define EEPROM_BYTE_SIZE				1	///< Size of Byte variables required in EEPROM memory, in bytes.

/**************************************************************************/
/*! 
    @brief  Class that defines the AcksenIntEEPROM state and functions
*/
/**************************************************************************/
class AcksenIntEEPROM
{

public:
	
/**************************************************************************/
/*!
    @brief  Class initialisation.
            Sets up the Internal EEPROM interface object using defined start address.
    @param  iStartAddress
            EEPROM start address to use (in bytes).
    @return No return value.
*/
/**************************************************************************/
	AcksenIntEEPROM(int iStartAddress); 
	
/**************************************************************************/
/*!
    @brief  Write a Float value to EEPROM, using the current Memory Address.  The Memory Address will be incremented after writing.
    @param  fNewValue
            The value to be written to the Present Memory Address.
    @return No return value.
*/
/**************************************************************************/
	bool writeEEPROMValueFloat(float fNewValue);
	
/**************************************************************************/
/*!
    @brief  Write an Int value to EEPROM, using the current Memory Address.  The Memory Address will be incremented after writing.
    @param  iNewValue
            The value to be written to the Present Memory Address.
    @return No return value.
*/
/**************************************************************************/
	bool writeEEPROMValueInt(int iNewValue);
	
/**************************************************************************/
/*!
    @brief  Write a Bit/Bool value to EEPROM, using the current Memory Address.  The Memory Address will be incremented after writing.
    @param  bNewValue
            The value to be written to the Present Memory Address.
    @return No return value.
*/
/**************************************************************************/
	bool writeEEPROMValueBit(bool bNewValue);
	
/**************************************************************************/
/*!
    @brief  Write a Long value to EEPROM, using the current Memory Address.  The Memory Address will be incremented after writing.
    @param  lNewValue
            The value to be written to the Present Memory Address.
    @return No return value.
*/
/**************************************************************************/
	bool writeEEPROMValueLong(long lNewValue);
	
/**************************************************************************/
/*!
    @brief  Read a Float value from EEPROM, using the Present Memory Address.  The Memory Address will be incremented after writing.
    @return The value to be read from the Present Memory Address.
*/
/**************************************************************************/
	float readEEPROMValueFloat();
	
/**************************************************************************/
/*!
    @brief  Read an Int value from EEPROM, using the Present Memory Address.  The Memory Address will be incremented after writing.
    @return The value to be read from the Present Memory Address.
*/
/**************************************************************************/
	int readEEPROMValueInt();
	
/**************************************************************************/
/*!
    @brief  Read a Bit/Bool value from EEPROM, using the Present Memory Address.  The Memory Address will be incremented after writing.
    @return The value to be read from the Present Memory Address.
*/
/**************************************************************************/
	bool readEEPROMValueBit();
	
/**************************************************************************/
/*!
    @brief  Read a Long value from EEPROM, using the Present Memory Address.  The Memory Address will be incremented after writing.
    @return The value to be read from the Present Memory Address.
*/
/**************************************************************************/
	long readEEPROMValueLong();

/**************************************************************************/
/*!
    @brief  Write a Float value to EEPROM to a specific Memory Address.  It will be incremented to the new Memory Address after writing.
    @param  *iEEPROMAddress
            Pointer to the Memory Address to write to.  It will be incremented to the new Memory Address after writing.
    @param  fNewValue
            The value to be written.
    @return No return value.
*/
/**************************************************************************/
	bool writeEEPROMValueFloatToAddress(int *iEEPROMAddress, float fNewValue);
	
/**************************************************************************/
/*!
    @brief  Write an Int value to EEPROM to a specific Memory Address.  It will be incremented to the new Memory Address after writing.
    @param  *iEEPROMAddress
            Pointer to the Memory Address to write to.  It will be incremented to the new Memory Address after writing.
    @param  iNewValue
            The value to be written.
    @return No return value.
*/
/**************************************************************************/
	bool writeEEPROMValueIntToAddress(int *iEEPROMAddress, int iNewValue);
	
/**************************************************************************/
/*!
    @brief  Write a Bit/Bool value to EEPROM to a specific Memory Address.  It will be incremented to the new Memory Address after writing.
    @param  *iEEPROMAddress
            Pointer to the Memory Address to write to.  It will be incremented to the new Memory Address after writing.
    @param  bNewValue
            The value to be written.
    @return No return value.
*/
/**************************************************************************/
	bool writeEEPROMValueBitToAddress(int *iEEPROMAddress, bool bNewValue);
	
/**************************************************************************/
/*!
    @brief  Write a Long value to EEPROM to a specific Memory Address.  It will be incremented to the new Memory Address after writing.
    @param  *iEEPROMAddress
            Pointer to the Memory Address to write to.  It will be incremented to the new Memory Address after writing.
    @param  lNewValue
            The value to be written.
    @return No return value.
*/
/**************************************************************************/
	bool writeEEPROMValueLongToAddress(int *iEEPROMAddress, long lNewValue);
	
/**************************************************************************/
/*!
    @brief  Set the Starting Memory Address for your EEPROM data.
    @param  iStartAddress
            Starting Memory Address for EEPROM read/writes in bytes.
    @return No return value.
*/
/**************************************************************************/
	void setStartAddress(int iStartAddress);
	
/**************************************************************************/
/*!
    @brief  Reset the Present Memory Address to the Starting Memory Address.
    @return No return value.
*/
/**************************************************************************/
	void resetPresentAddress();

/**************************************************************************/
/*!
    @brief  Get the Present Memory Address.
    @return Present Memory Address (in bytes).
*/
/**************************************************************************/
	int getPresentAddress();
	
/**************************************************************************/
/*!
    @brief  Set the Starting Memory Address for your EEPROM data.
    @param  iPresentAddress
            Present Memory Address for EEPROM read/writes in bytes.
    @return No return value.
*/
/**************************************************************************/
	void setPresentAddress(int iPresentAddress);
	
/**************************************************************************/
/*!
    @brief  Validate an Int against a defined Maximum and Minimum Range.
    @param  iMinValue
            Minimum range for validation against supplied Value.
    @param  iMaxValue
            Maximum range for validation against supplied Value.
    @param  iValue
            Value to validate against Min/Max range.
    @return True if validation has passed (Min <= Value <= Max)
			False if validation has failed.
*/
/**************************************************************************/
	bool validateInt(int iMinValue, int iMaxValue, int iValue);
	
/**************************************************************************/
/*!
    @brief  Validate an Unsigned Int against a defined Maximum and Minimum Range.
    @param  uiMinValue
            Minimum range for validation against supplied Value.
    @param  uiMaxValue
            Maximum range for validation against supplied Value.
    @param  uiValue
            Value to validate against Min/Max range.
    @return True if validation has passed (Min <= Value <= Max)
			False if validation has failed.
*/
/**************************************************************************/
	bool validateUnsignedInt(unsigned int uiMinValue, unsigned int uiMaxValue, unsigned int uiValue);
	
/**************************************************************************/
/*!
    @brief  Validate a Long against a defined Maximum and Minimum Range.
    @param  lMinValue
            Minimum range for validation against supplied Value.
    @param  lMaxValue
            Maximum range for validation against supplied Value.
    @param  lValue
            Value to validate against Min/Max range.
    @return True if validation has passed (Min <= Value <= Max)
			False if validation has failed.
*/
/**************************************************************************/	
	bool validateLong(long lMinValue, long lMaxValue, long lValue);
	
/**************************************************************************/
/*!
    @brief  Validate an Unsigned Long against a defined Maximum and Minimum Range.
    @param  ulMinValue
            Minimum range for validation against supplied Value.
    @param  ulMaxValue
            Maximum range for validation against supplied Value.
    @param  ulValue
            Value to validate against Min/Max range.
    @return True if validation has passed (Min <= Value <= Max)
			False if validation has failed.
*/
/**************************************************************************/	
	bool validateUnsignedLong(unsigned long ulMinValue, unsigned long ulMaxValue, unsigned long ulValue);
	
/**************************************************************************/
/*!
    @brief  Validate a Float against a defined Maximum and Minimum Range.
    @param  iMinValue
            Minimum range for validation against supplied Value.
    @param  iMaxValue
            Maximum range for validation against supplied Value.
    @param  iValue
            Value to validate against Min/Max range.
    @return True if validation has passed (Min <= Value <= Max)
			False if validation has failed.
*/
/**************************************************************************/	
	bool validateFloat(float fMinValue, float fMaxValue, float fValue);
  
protected:
  
	int iEEPROMStartAddress;	///< Starting Memory Address for EEPROM data
	int iEEPROMPresentAddress;	///< Present Memory Address used for reading/writing EEPROM data
};

#endif


