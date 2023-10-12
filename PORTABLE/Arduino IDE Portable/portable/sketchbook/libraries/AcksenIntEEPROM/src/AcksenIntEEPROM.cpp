/***********************************************************/
/*!

@file AcksenIntEEPROM.cpp

@mainpage Arduino Library for storing data in 8-bit AVR internal EEPROM memory.

@section intro_sec Introduction

This is the documentation for Acksen Ltd's AcksenIntEEPROM library for the
Arduino platform.

This library allows storage of data in 8-bit AVR internal EEPROM memory in 
a convenient wrapper, allowing simple read/write of sequentially stored, 
common variable types, with validation options.

AcksenIntEEPROM calls the EEPROMEx library by Thijs Elenbaas 
(https://github.com/thijse/Arduino-EEPROMEx). 

@section dependencies Dependencies

Requires the EEPROMEx library by Thijs Elenbaas
(https://github.com/thijse/Arduino-EEPROMEx).

Arduino Library rev.2.2 - requires Arduino IDE v1.8.10 or greater.

@section author Author

Written by Richard Phillips for Acksen Ltd.

@section license License

This source file is licenced using the 3-Clause BSD License.

Copyright (c) 2022 Acksen Ltd, All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission. 

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/***********************************************************/

// Acksen Internal EEPROM Library v1.1.0


#include "Arduino.h"
#include "AcksenIntEEPROM.h"

AcksenIntEEPROM::AcksenIntEEPROM(int iStartAddress)
{
	
	this->iEEPROMStartAddress = iStartAddress;
	this->iEEPROMPresentAddress = iStartAddress;
	
}

bool AcksenIntEEPROM::writeEEPROMValueBit(bool bNewValue)
{

	if (bNewValue != EEPROM.readBit(this->iEEPROMPresentAddress, 0))
	{
		EEPROM.writeBit(this->iEEPROMPresentAddress, 0, bNewValue);
		this->iEEPROMPresentAddress = this->iEEPROMPresentAddress + EEPROM_BYTE_SIZE;
		return true;
	}
	else
	{
		this->iEEPROMPresentAddress = this->iEEPROMPresentAddress + EEPROM_BYTE_SIZE;
		return false;
	}

}


bool AcksenIntEEPROM::writeEEPROMValueInt(int iNewValue)
{

	if (iNewValue != EEPROM.readInt(this->iEEPROMPresentAddress))
	{
		EEPROM.writeInt(this->iEEPROMPresentAddress, iNewValue);
		this->iEEPROMPresentAddress = this->iEEPROMPresentAddress + EEPROM_INT_SIZE;
		return true;
	}
	else
	{
		this->iEEPROMPresentAddress = this->iEEPROMPresentAddress + EEPROM_INT_SIZE;
		return false;
	}


}

bool AcksenIntEEPROM::writeEEPROMValueFloat( float fNewValue)
{

	if (fNewValue != EEPROM.readFloat(this->iEEPROMPresentAddress))
	{
		EEPROM.writeFloat(this->iEEPROMPresentAddress, fNewValue);
		this->iEEPROMPresentAddress = this->iEEPROMPresentAddress + EEPROM_FLOAT_SIZE;
		return true;
	}
	else
	{
		this->iEEPROMPresentAddress = this->iEEPROMPresentAddress + EEPROM_FLOAT_SIZE;
		return false;
	}

}

bool AcksenIntEEPROM::writeEEPROMValueLong(long lNewValue)
{

	if (lNewValue != EEPROM.readLong(this->iEEPROMPresentAddress))
	{
		EEPROM.writeLong(this->iEEPROMPresentAddress, lNewValue);
		this->iEEPROMPresentAddress = this->iEEPROMPresentAddress + EEPROM_LONG_SIZE;
		return true;
	}
	else
	{
		this->iEEPROMPresentAddress = this->iEEPROMPresentAddress + EEPROM_LONG_SIZE;
		return false;
	}

}

float AcksenIntEEPROM::readEEPROMValueFloat()
{
	float fTemp;
	
	// Read value from EEPROM
	fTemp = EEPROM.readFloat(this->iEEPROMPresentAddress);
	
	// Increment Internal EEPROM Address Counter
	this->iEEPROMPresentAddress = this->iEEPROMPresentAddress + EEPROM_FLOAT_SIZE;
	
	// Return read value
	return fTemp;
}

int AcksenIntEEPROM::readEEPROMValueInt()
{
	int iTemp;
	
	// Read value from EEPROM
	iTemp = EEPROM.readInt(this->iEEPROMPresentAddress);
	
	// Increment Internal EEPROM Address Counter
	this->iEEPROMPresentAddress = this->iEEPROMPresentAddress + EEPROM_INT_SIZE;
	
	// Return read value
	return iTemp;
}

bool AcksenIntEEPROM::readEEPROMValueBit()
{
	bool bTemp;
	
	// Read value from EEPROM
	bTemp = EEPROM.readBit(this->iEEPROMPresentAddress, 0);
	
	// Increment Internal EEPROM Address Counter
	this->iEEPROMPresentAddress = this->iEEPROMPresentAddress + EEPROM_BYTE_SIZE;
	
	// Return read value
	return bTemp;
}

long AcksenIntEEPROM::readEEPROMValueLong()
{
	long lTemp;
	
	// Read value from EEPROM
	lTemp = EEPROM.readLong(this->iEEPROMPresentAddress);
	
	// Increment Internal EEPROM Address Counter
	this->iEEPROMPresentAddress = this->iEEPROMPresentAddress + EEPROM_LONG_SIZE;
	
	// Return read value
	return lTemp;
}

bool AcksenIntEEPROM::writeEEPROMValueBitToAddress(int *iEEPROMAddress, bool bNewValue)
{

	if (bNewValue != EEPROM.readBit(*iEEPROMAddress, 0))
	{
		EEPROM.writeBit(*iEEPROMAddress, 0, bNewValue);
		*iEEPROMAddress = *iEEPROMAddress + EEPROM_BYTE_SIZE;
		return true;
	}
	else
	{
		*iEEPROMAddress = *iEEPROMAddress + EEPROM_BYTE_SIZE;
		return false;
	}

}


bool AcksenIntEEPROM::writeEEPROMValueIntToAddress(int *iEEPROMAddress, int iNewValue)
{

	if (iNewValue != EEPROM.readInt(*iEEPROMAddress))
	{
		EEPROM.writeInt(*iEEPROMAddress, iNewValue);
		*iEEPROMAddress = *iEEPROMAddress + EEPROM_INT_SIZE;
		return true;
	}
	else
	{
		*iEEPROMAddress = *iEEPROMAddress + EEPROM_INT_SIZE;
		return false;
	}


}

bool AcksenIntEEPROM::writeEEPROMValueFloatToAddress(int *iEEPROMAddress, float fNewValue)
{

	if (fNewValue != EEPROM.readFloat(*iEEPROMAddress))
	{
		EEPROM.writeFloat(*iEEPROMAddress, fNewValue);
		*iEEPROMAddress = *iEEPROMAddress + EEPROM_FLOAT_SIZE;
		return true;
	}
	else
	{
		*iEEPROMAddress = *iEEPROMAddress + EEPROM_FLOAT_SIZE;
		return false;
	}

}

bool AcksenIntEEPROM::writeEEPROMValueLongToAddress(int *iEEPROMAddress, long lNewValue)
{

	if (lNewValue != EEPROM.readLong(*iEEPROMAddress))
	{
		EEPROM.writeLong(*iEEPROMAddress, lNewValue);
		*iEEPROMAddress = *iEEPROMAddress + EEPROM_LONG_SIZE;
		return true;
	}
	else
	{
		*iEEPROMAddress = *iEEPROMAddress + EEPROM_LONG_SIZE;
		return false;
	}

}

void AcksenIntEEPROM::setStartAddress(int iStartAddress)
{
	this->iEEPROMStartAddress = iStartAddress;
}

void AcksenIntEEPROM::resetPresentAddress()
{
	this->iEEPROMPresentAddress = this->iEEPROMStartAddress;
}

int AcksenIntEEPROM::getPresentAddress()
{
	return this->iEEPROMPresentAddress;
}

void AcksenIntEEPROM::setPresentAddress(int iPresentAddress)
{
	this->iEEPROMPresentAddress = iPresentAddress;
}

bool AcksenIntEEPROM::validateInt(int iMinValue, int iMaxValue, int iValue)
{
	if ((iMinValue <= iValue) && (iValue <= iMaxValue))
	{
		// OK
		return true;
	}
	else
	{
		// Validation Error
		return false;
	}	
}

bool AcksenIntEEPROM::validateUnsignedInt(unsigned int uiMinValue, unsigned int uiMaxValue, unsigned int uiValue)
{
	if ((uiMinValue <= uiValue) && (uiValue <= uiMaxValue))
	{
		// OK
		return true;
	}
	else
	{
		// Validation Error
		return false;
	}	
}

bool AcksenIntEEPROM::validateLong(long lMinValue, long lMaxValue, long lValue)
{
	if ((lMinValue <= lValue) && (lValue <= lMaxValue))
	{
		// OK
		return true;
	}
	else
	{
		// Validation Error
		return false;
	}	
}

bool AcksenIntEEPROM::validateUnsignedLong(unsigned long ulMinValue, unsigned long ulMaxValue, unsigned long ulValue)
{
	if ((ulMinValue <= ulValue) && (ulValue <= ulMaxValue))
	{
		// OK
		return true;
	}
	else
	{
		// Validation Error
		return false;
	}	
}

bool AcksenIntEEPROM::validateFloat(float fMinValue, float fMaxValue, float fValue)
{
	// Allow slightly under and over the stated values, in order to cope with float precision errors.
	if (((fMinValue-0.01) <= fValue) && (fValue <= (fMaxValue+0.01)))
	{
		// OK
		return true;
	}
	else
	{
		// Validation Error
		return false;
	}		
}