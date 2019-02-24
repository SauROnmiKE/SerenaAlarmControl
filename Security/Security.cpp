//
//

#include <Security.h>
#include <SPI.h>
#include <SD.h>
#include <avr/eeprom.h>

File OpenedFile;

// Constructor
Security::Security()
{
	
}

bool Security::SaveArming(bool alarmarmed)
{
	eeprom_update_byte(3, alarmarmed);

	String counterStr;
	char Number;
	long counter;

	if (SD.begin(53))
	{
		if (SD.exists("eeprom.txt"))
		{
			OpenedFile = SD.open("eeprom.txt", FILE_READ);

			while (OpenedFile.available())
			{
				Number = OpenedFile.read();
				counterStr += Number;
			}

			OpenedFile.close();
			// Removing for easy edit
			SD.remove("eeprom.txt");
		}
		else
		{
			// Setting counterStr to "0" if it writes for the first time
			counterStr = "0";
		}

		counter = counterStr.toInt();
		counter++;

		OpenedFile = SD.open("eeprom.txt", FILE_WRITE);
		OpenedFile.print(counter);
		OpenedFile.close();
	}

	// returns a true value if the byte in EEPROM has been written over 90000 times. ATMEL's specs for ATmeaga2560 is at least 100000 write/erase cycles
	if (counter > 90000)
	{
		return true;
	}

	return false;
}

bool Security::CheckArming()
{
	return eeprom_read_byte(3);
}

int * Security::GetPass(int passtype) const
{
	int const * gp;

	if (passtype == 4)
	{
		gp = armpass;
	}
	else if (passtype == 5)
	{
		gp = autopass;
	}
	else if (passtype == 6)
	{
		gp = mpass;
	}

	return gp;
}

int * Security::GetTempPass(int passtype)
{
	int const * gtp;

	if (passtype == 4)
	{
		gtp = arminput;
	}
	else if (passtype == 5)
	{
		gtp = autoinput;
	}
	else if (passtype == 6)
	{
		gtp = manualinput;
	}

	return gtp;
}

// Sets new passwords
void Security::SetPass(int passtype)
{
	int i;

	if (passtype == 4)
	{
		for (i = 0; i < 4; i++)
		{
			armpass[i] = arminput[i];
		}
	}
	else if (passtype == 5)
	{
		for (i = 0; i < 5; i++)
		{
			autopass[i] = autoinput[i];
		}
	}
	else if (passtype == 6)
	{
		for (i = 0; i < 6; i++)
		{
			mpass[i] = manualinput[i];
		}
	}
}

// Checks if the SD card is inserted and returns true or false
bool Security::CheckSD()
{
	if (SD.begin(53))
	{
		return true;
	}
	return false;
}

// Checks the SD card for password files. Returns "true" if the specified file is found and "false" if it's not.
bool Security::LoadSDPass(int passtype)
{

	if (passtype == 6)
	{
		String Password;
		char Number;
		long temppass;

		if (SD.exists("manual.txt"))
		{
			OpenedFile = SD.open("manual.txt", FILE_READ);
			
			while (OpenedFile.available())
			{
				Number = OpenedFile.read();
				Password += Number;
			}

			OpenedFile.close();
			temppass = Password.toInt();

			for (int i = 0; i < 6; i++)
			{
				if (i == 0)
				{
					mpass[0] = temppass / 100000;
				}
				else if (i == 1)
				{
					temppass = temppass % 100000;
					mpass[1] = temppass / 10000;
				}
				else if (i == 2)
				{
					temppass = temppass % 10000;
					mpass[2] = temppass / 1000;
				}
				else if (i == 3)
				{
					temppass = temppass % 1000;
					mpass[3] = temppass / 100;
				}
				else if (i == 4)
				{
					temppass = temppass % 100;
					mpass[4] = temppass / 10;
				}
				else if (i == 5)
				{
					temppass = temppass % 10;
					mpass[5] = temppass;
				}
			}
			return true;
		}

		return false;
	}

	if (passtype == 5)
	{
		String Password;
		char Number;
		long temppass;

		if (SD.exists("auto.txt"))
		{
			OpenedFile = SD.open("auto.txt", FILE_READ);

			while (OpenedFile.available())
			{
				Number = OpenedFile.read();
				Password += Number;
			}

			OpenedFile.close();
			temppass = Password.toInt();

			for (int i = 0; i < 5; i++)
			{
				if (i == 0)
				{
					autopass[0] = temppass / 10000;
				}
				else if (i == 1)
				{
					temppass = temppass % 10000;
					autopass[1] = temppass / 1000;
				}
				else if (i == 2)
				{
					temppass = temppass % 1000;
					autopass[2] = temppass / 100;
				}
				else if (i == 3)
				{
					temppass = temppass % 100;
					autopass[3] = temppass / 10;
				}
				else if (i == 4)
				{
					temppass = temppass % 10;
					autopass[4] = temppass;
				}
			}

			return true;
		}

		return false;
	}

	if (passtype == 4)
	{
		String Password;
		char Number;
		long temppass;

		if (SD.exists("arm.txt"))
		{
			OpenedFile = SD.open("arm.txt", FILE_READ);

			while (OpenedFile.available())
			{
				Number = OpenedFile.read();
				Password += Number;
			}

			OpenedFile.close();
			temppass = Password.toInt();

			for (int i = 0; i < 4; i++)
			{
				if (i == 0)
				{
					armpass[0] = temppass / 1000;
				}
				else if (i == 1)
				{
					temppass = temppass % 1000;
					armpass[1] = temppass / 100;
				}
				else if (i == 2)
				{
					temppass = temppass % 100;
					armpass[2] = temppass / 10;
				}
				else if (i == 3)
				{
					temppass = temppass % 10;
					armpass[3] = temppass;
				}
			}

			return true;
		}

		return false;
	}
}
// Saves the new pass in the SD card
void Security::SetSDPass(int passtype)
{
	int i;

	if (passtype == 4)
	{
		if (SD.exists("arm.txt"))
		{
			SD.remove("arm.txt");
		}

		OpenedFile = SD.open("arm.txt", FILE_WRITE);

		for (i = 0; i < 4; i++)
		{
			OpenedFile.print(armpass[i]);
		}

		OpenedFile.close();
	}
	else if (passtype == 5)
	{
		if (SD.exists("auto.txt"))
		{
			SD.remove("auto.txt");
		}

		OpenedFile = SD.open("auto.txt", FILE_WRITE);

		for (i = 0; i < 5; i++)
		{
			OpenedFile.print(autopass[i]);
		}

		OpenedFile.close();
	}
	else if (passtype == 6)
	{
		if (SD.exists("manual.txt"))
		{
			SD.remove("manual.txt");
		}

		OpenedFile = SD.open("manual.txt", FILE_WRITE);

		for (i = 0; i < 6; i++)
		{
			OpenedFile.print(mpass[i]);
		}

		OpenedFile.close();
	}
}

void Security::SaveAutoLoad(bool autoload)
{
	eeprom_update_byte(0, autoload);
}

bool Security::LoadAutoLoad()
{
	return eeprom_read_byte(0);
}

void Security::SaveAskArm(bool askarmpass)
{
	eeprom_update_byte(1, askarmpass);
}

bool Security::LoadAskArm()
{
	return eeprom_read_byte(1);
}

void Security::SaveSDChecking(bool sdchecking)
{
	eeprom_update_byte(2, sdchecking);
}

bool Security::LoadSDChecking()
{
	return eeprom_read_byte(2);
}

// Resets user inputs whenever a new or a current password is about to be entered
void Security::ResetInputs()
{
	int i;

	for (i = 0; i < 4; i++)
	{
		arminput[i] = 0;
	}

	for (i = 0; i < 5; i++)
	{
		autoinput[i] = 0;
	}

	for (i = 0; i < 6; i++)
	{
		manualinput[i] = 0;
	}

	timeinput = 0;
}

// Updates the user's input for the 6-digit password
int Security::ManualUpdate(int number, int buf)
{
	if (buf < 6)
	{
		if (buf == 0)
		{
			manualinput[0] = number;
			return manualinput[0];
		}

		if (buf == 1)
		{
			manualinput[1] = number;
			return manualinput[1];
		}

		if (buf == 2)
		{
			manualinput[2] = number;
			return manualinput[2];
		}

		if (buf == 3)
		{
			manualinput[3] = number;
			return manualinput[3];
		}

		if (buf == 4)
		{
			manualinput[4] = number;
			return manualinput[4];
		}

		if (buf == 5)
		{
			manualinput[5] = number;
			return manualinput[5];
		}
	}
}

// Updates the user's input for the 4-digit password
int Security::ArmUpdate(int number, int buf)
{
	if (buf < 4)
	{
		if (buf == 0)
		{
			arminput[0] = number;
			return arminput[0];
		}

		if (buf == 1)
		{
			arminput[1] = number;
			return arminput[1];
		}

		if (buf == 2)
		{
			arminput[2] = number;
			return arminput[2];
		}

		if (buf == 3)
		{
			arminput[3] = number;
			return arminput[3];
		}
	}
}

// Updates the user's input for the 5-digit password
int Security::AutoArmUpdate(int number, int buf)
{
	if (buf < 5)
	{
		if (buf == 0)
		{
			autoinput[0] = number;
			return autoinput[0];
		}

		if (buf == 1)
		{
			autoinput[1] = number;
			return autoinput[1];
		}

		if (buf == 2)
		{
			autoinput[2] = number;
			return autoinput[2];
		}

		if (buf == 3)
		{
			autoinput[3] = number;
			return autoinput[3];
		}

		if (buf == 4)
		{
			autoinput[4] = number;
			return autoinput[4];
		}
	}
}


// Gets the auto-arm time and passes it to the main program, so that a Graphics object can use it. "o" = OLD, "n" = NEW
int Security::GetAutoTime(char ttype) const
{
	if (ttype == 'o')
	{
		return autoarmtime;
	}
	else if (ttype == 'n')
	{
		return timeinput;
	}
}


void Security::SetAutoTime()
{
	autoarmtime = timeinput;
}

// Modifies the timeinput value, which stores the user's input in the "SET AUTO-ARM TIME" function.
int Security::AutoTimeUpdate(int input, int timelength)
{
	if (timelength < 4)
	{
		if (timelength == 0)
		{
			timeinput = input;
			return timeinput;
		}
		else
		{
			timeinput = (timeinput * 10) + input;
			return timeinput;
		}
	}

}