// 
//

#include <Graphics.h>
#include <UTFT.h>
#include <URTouch.h>
#include <UTFT_Geometry.h>
#include <stdint.h>
#include <DS3231.h>


UTFT myGLCD(ITDB32WC, 38, 39, 40, 41); // In brackets: Screen Model and all the required pins (may differ with each screen)
URTouch  myTouch(6, 5, 4, 3, 2); // In brackets: Pin connections
UTFT_Geometry geo(&myGLCD);
DS3231  rtc(SDA, SCL);


//*******RTC-RELATED VARIABLES*******//
String currentClock, currentHours, currentMinutes, currentSeconds, currentDate, currentDay, currentMonth, currentYear;
String timeString, hoursString, minutesString, secondsString, hoursS, minutesS, secondsS, dateS;
float currentTemperature, temperature;
//***********************************//

extern unsigned int energy[0x400];
extern unsigned int snow[0x100];
extern unsigned int snow2[0x100];
extern unsigned int snow3[0x100];
extern unsigned int snow4[0x100];

int x1 = random(2, 15);
int y1 = random(30, 145);
int x2 = random(75, 140);
int y2 = random(2, 40);
int x3 = random(40, 140);
int y3 = random(123, 160);
int x4 = random(160, 220);
int y4 = random(26, 40);
int x5 = random(160, 220);
int y5 = random(123, 160);
int x6 = random(250, 300);
int y6 = random(26, 40);
int x7 = random(250, 300);
int y7 = random(123, 160);

//*******************TFT FONTS*******************//
extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t SevenSegNumFont[];
extern uint8_t Various_Symbols_16x32[6084];
extern uint8_t GroteskBold24x48[13684];
extern uint8_t UbuntuBold[9124];
//***********************************************//

// Constructor
Graphics::Graphics()
{
	
}


// Initializes the screen when the board powers or resets
void Graphics::InitializeScreen() const
{
	myGLCD.InitLCD();
	myGLCD.clrScr();
	myTouch.InitTouch();
	myTouch.setPrecision(PREC_EXTREME); //PREC_LOW //PREC_MEDIUM //PREC_HI //PREC_EXTREME
}

void Graphics::InitializeRTC() const
{
   rtc.begin();
	
   currentTemperature = rtc.getTemp();
   currentDate = rtc.getDateStr();
   currentClock = rtc.getTimeStr();
   timeString = rtc.getTimeStr();
   currentHours = timeString.substring(0, 2);
   currentMinutes = timeString.substring(3, 5);
   currentSeconds = timeString.substring(6, 8);
}

// Mostly checking if everything's OK, before moving to the main menu
void Graphics::LoadingScreen()
{
	delay(850);
	myGLCD.setFont(SmallFont);
	myGLCD.setColor(255, 255, 255);
	myGLCD.setBackColor(0, 0, 0);

	myGLCD.drawBitmap(278, 3, 36, 32, energy);
	myGLCD.print("SERENA Alarm Control, version 2.8.1", 0, 0);
	myGLCD.print("Made by Michael Marinis, Airgeorge", 0, 14);

	delay(1000);
	myGLCD.print("Making necessary checks. . .", 0, 42);
	delay(1200);
	myGLCD.print("Checking clock and temperature functions. . .", 0, 56);
	myGLCD.print("Temperature: ", 0, 70);
	myGLCD.printNumI(rtc.getTemp(), 112, 70);
	myGLCD.print("C", 136, 70);
	delay(500);

	myGLCD.print("Date: ", 0, 84);
	myGLCD.print(rtc.getDateStr(), 56, 84);
	delay(300);

	myGLCD.print("Time: ", 0, 98);
	myGLCD.print(currentHours, 56, 98);
	myGLCD.print(":", 75, 98);
	myGLCD.print(currentMinutes, 88, 98);
	myGLCD.print(":", 107, 98);
	myGLCD.print(currentSeconds, 120, 98);
	myGLCD.print("TIME OK! Proceeding to SD Card check...", 0, 112);
	delay(1500);

	myGLCD.print("Checking SD Card.", 0, 138);
	delay(500);
	myGLCD.print(".", 136, 138);
	delay(500);
	myGLCD.print(".", 144, 138);
}

void Graphics::BootProgram()
{
	myGLCD.setBackColor(0, 0, 0);
	myGLCD.setColor(255, 255, 0);
	myGLCD.print("Preparing to boot program!", 0, 221);
	delay(3500);
}

void Graphics::DrawArc() const
{
	myGLCD.clrScr();
	myGLCD.setColor(255, 0, 0);
	myGLCD.setBackColor(0, 0, 0);
	geo.drawArc(192, 132, 120, 0, 100, 3);
}

// Intro screen
void Graphics::Intro() const
{
	myGLCD.clrScr();
	Graphics::DrawArc();

	myGLCD.setFont(UbuntuBold);
	myGLCD.setColor(255, 165, 0);
	myGLCD.setBackColor(0, 0, 0);
	myGLCD.print("SERENA", 135, 65);
	myGLCD.setFont(SmallFont);
	myGLCD.print("A COMPLETE ALARM CONTROL PANEL", 64, 90);
	myGLCD.print("VERSION 2.8.1", CENTER, 170);

	delay(1500);

	myGLCD.fillRect(0, 226, 319, 239);
	myGLCD.print("(c) 2017, Michael & George Marinis", CENTER, 227);

	for (int i = 0; i < 30; i++)
	{
		myGLCD.fillRect(4 + (i * 10), 190, 25 + (i * 10), 210);
		delay(150);
	}

	delay(5500);
}

void Graphics::SDFound(bool sdfound) const
{
	if (sdfound)
	{
		myGLCD.setColor(255, 0, 0);
		myGLCD.print("An SD Card was found. Do you want", 0, 152);
		myGLCD.print("to check for existing passwords?", 0, 166);
		myGLCD.setColor(255, 255, 255);
		myGLCD.drawRect(5, 185, 50, 215);
		myGLCD.print("Y", 25, 195);
		myGLCD.drawRect(60, 185, 105, 215);
		myGLCD.print("N", 80, 195);
	}
	else
	{
		myGLCD.print("No SD Card was found. Do you want", 0, 152);
		myGLCD.print("to check again?", 0, 166);
		myGLCD.drawRect(5, 185, 50, 215);
		myGLCD.print("Y", 25, 195);
		myGLCD.drawRect(60, 185, 105, 215);
		myGLCD.print("N", 80, 195);
	}
}

// Prints message on password load
void Graphics::LoadingPass(bool load, int passtype) const
{
	if (load)
	{
		if (passtype == 6)
		{
			myGLCD.print("Searching for 6-Digit Pass...", 0, 0);
			delay(1000);
		}
		else if (passtype == 5)
		{
			myGLCD.print("Searching for 5-Digit Pass...", 0, 42);
			delay(1000);
		}
		else
		{
			myGLCD.print("Searching for 4-Digit Pass...", 0, 84);
			delay(1000);
		}
	}
	else
	{
		myGLCD.setColor(255, 255, 0);
		myGLCD.print("Preparing to boot program!", 0, 221);
		delay(3500);
	}
	
}

// Prints message on if the pass loading was successful or not
void Graphics::PassLoadSuccess(bool success, int passtype) const
{
	delay(2500);

	if (passtype == 6)
	{
		if (success)
		{
			myGLCD.print("Password was loaded successfully!", 0, 14);
			delay(1000);
		}
		else
		{
			myGLCD.print("Loading failed. File not found.", 0, 14);
			delay(850);
		}
	}

	if (passtype == 5)
	{
		if (success)
		{
			myGLCD.print("Password was loaded successfully!", 0, 56);
			delay(1000);
		}
		else
		{
			myGLCD.print("Loading failed. File not found.", 0, 56);
			delay(850);
		}
	}

	if (passtype == 4)
	{
		if (success)
		{
			myGLCD.print("Password was loaded successfully!", 0, 98);
			delay(1000);

			myGLCD.setColor(255, 255, 0);
			myGLCD.print("Preparing to boot program!", 0, 124);
			delay(3500);
		}
		else
		{
			myGLCD.print("Loading failed. File not found.", 0, 98);
			delay(850);

			myGLCD.setColor(255, 255, 0);
			myGLCD.print("Preparing to boot program!", 0, 124);
			delay(3500);
		}
	}
	

}

void Graphics::ClearScreen() const
{
	myGLCD.clrScr();
}

///**********************TITLES*******************************///
// Prints the Alarm Title without "BY MICHAEL MARINIS"
void Graphics::AlarmTitle() const
{
	myGLCD.setFont(SmallFont);
	myGLCD.clrScr();
	myGLCD.setColor(255, 255, 255);
	myGLCD.setBackColor(255, 0, 0);
	myGLCD.drawLine(0, 14, 319, 14);
	myGLCD.print("SERENA ALARM CONTROL PANEL v.2.8.1", CENTER, 1);
}

// Prints the Alarm Title with "BY MICHAEL MARINIS"
void Graphics::Title() const
{
	Graphics::AlarmTitle();
	myGLCD.print("BY MICHAEL MARINIS", CENTER, 30);
}
///***********************************************************///



///***************BUTTONS********************///
// Top button
void Graphics::DrawButtonT() const
{
	myGLCD.setColor(192, 192, 192);
	myGLCD.fillRoundRect(20, 60, 300, 100);
}

// Bottom button
void Graphics::DrawButtonB() const
{
	myGLCD.setColor(192, 192, 192);
	myGLCD.fillRoundRect(20, 120, 300, 160);
}

void Graphics::DrawDisarm() const
{
	myGLCD.setColor(192, 192, 192);
	myGLCD.fillRoundRect(20, 180, 300, 220);
	myGLCD.setColor(0, 0, 0);
	myGLCD.setBackColor(192, 192, 192);
	myGLCD.setFont(BigFont);
	myGLCD.print("DISARM", CENTER, 193);
}

void Graphics::DrawNo() const
{
	myGLCD.setColor(192, 192, 192);
	myGLCD.fillRoundRect(20, 180, 150, 210);
	myGLCD.setBackColor(192, 192, 192);
	myGLCD.setColor(0, 0, 0);
	myGLCD.setFont(SmallFont);
	myGLCD.print("NO", 80, 190);
}

void Graphics::DrawYes() const
{
	myGLCD.setColor(192, 192, 192);
	myGLCD.fillRoundRect(170, 180, 300, 210);
	myGLCD.setBackColor(192, 192, 192);
	myGLCD.setColor(0, 0, 0);
	myGLCD.setFont(SmallFont);
	myGLCD.print("YES", 225, 190);
}

void Graphics::DrawNext() const
{
	myGLCD.setColor(192, 192, 192);
	myGLCD.fillRoundRect(170, 180, 300, 210);
	myGLCD.setBackColor(192, 192, 192);
	myGLCD.setColor(0, 0, 0);
	myGLCD.setFont(SmallFont);
	myGLCD.print("NEXT", 220, 190);
}

void Graphics::DrawNextBig() const
{
	myGLCD.setColor(192, 192, 192);
	myGLCD.fillRoundRect(170, 165, 300, 210);
	myGLCD.setBackColor(192, 192, 192);
	myGLCD.setColor(0, 0, 0);
	myGLCD.setFont(SmallFont);
	myGLCD.print("NEXT", 220, 183);
}


void Graphics::DrawBack() const
{
	myGLCD.setColor(192, 192, 192);
	myGLCD.fillRoundRect(20, 180, 150, 210);
	myGLCD.setBackColor(192, 192, 192);
	myGLCD.setColor(0, 0, 0);
	myGLCD.setFont(SmallFont);
	myGLCD.print("BACK", 70, 190);
}

void Graphics::DrawBackBig() const
{
	myGLCD.setColor(192, 192, 192);
	myGLCD.fillRoundRect(20, 165, 150, 210);
	myGLCD.setBackColor(192, 192, 192);
	myGLCD.setColor(0, 0, 0);
	myGLCD.setFont(SmallFont);
	myGLCD.print("BACK", 70, 183);
}


void Graphics::DrawOptions() const
{
	myGLCD.setFont(BigFont);
	myGLCD.setColor(255, 0, 0);
	myGLCD.fillRoundRect(20, 70, 300, 120);
	myGLCD.setBackColor(255, 0, 0);
	myGLCD.setColor(255, 255, 255);
	myGLCD.print("OPTIONS", CENTER, 90);
}

void Graphics::DrawArm() const
{
	myGLCD.setFont(BigFont);
	myGLCD.setColor(255, 0, 0);
	myGLCD.fillRoundRect(20, 140, 300, 190);
	myGLCD.setBackColor(255, 0, 0);
	myGLCD.setColor(255, 255, 255);
	myGLCD.print("ARM", CENTER, 160);
}

///******************************************///


// Draws all the buttons when the user enters a password or time (for the auto-arm time)
void Graphics::DrawKeyboard() const
{
	myGLCD.clrScr();
	int z;
	myGLCD.setFont(SmallFont);
	for (z = 0; z < 3; z++)
	{
		myGLCD.setColor(255, 0, 0);
		myGLCD.fillRoundRect(20 + (z * 60), 15, 60 + (z * 60), 55);
		myGLCD.setColor(255, 255, 255);
		myGLCD.setBackColor(255, 0, 0);
		myGLCD.printNumI(z + 1, 37 + (z * 60), 30);
	}

	for (z = 0; z < 3; z++)
	{
		myGLCD.setColor(255, 0, 0);
		myGLCD.fillRoundRect(20 + (z * 60), 70, 60 + (z * 60), 110);
		myGLCD.setColor(255, 255, 255);
		myGLCD.setBackColor(255, 0, 0);
		myGLCD.printNumI(z + 4, 37 + (z * 60), 85);
	}

	for (z = 0; z < 3; z++)
	{
		myGLCD.setColor(255, 0, 0);
		myGLCD.fillRoundRect(20 + (z * 60), 125, 60 + (z * 60), 165);
		myGLCD.setColor(255, 255, 255);
		myGLCD.setBackColor(255, 0, 0);
		myGLCD.printNumI(z + 7, 37 + (z * 60), 140);
	}

	for (z = 0; z < 3; z++)
	{
		myGLCD.setColor(255, 0, 0);
		myGLCD.fillRoundRect(190, 15 + (z * 55), 290, 55 + (z * 55));
	}


	myGLCD.setColor(255, 0, 0);
	myGLCD.fillRoundRect(20, 175, 80, 205);
	myGLCD.setColor(255, 255, 255);
	myGLCD.setBackColor(255, 0, 0);
	myGLCD.printNumI(0, 48, 185);

	myGLCD.setColor(255, 255, 255);
	myGLCD.setBackColor(255, 0, 0);
	myGLCD.print("CLEAR", 221, 30);
	myGLCD.print("OK", 232, 85);
	myGLCD.print("BACK", 225, 140);

	myGLCD.setFont(SevenSegNumFont);
	myGLCD.setBackColor(0, 0, 0);
}



///***************MENUS*************///
void Graphics::MainMenu() const
{
	Graphics::Title();
	Graphics::DrawArm();
	Graphics::DrawOptions();

	myGLCD.setFont(SmallFont);
	myGLCD.setColor(255, 165, 0);
	myGLCD.setBackColor(0, 0, 0);
	myGLCD.fillRect(0, 226, 319, 239);
	myGLCD.print("(c) 2017, Michael & George Marinis", CENTER, 227);
}

void Graphics::OptionsMenu(int page) const
{
	Graphics::AlarmTitle();
	myGLCD.setColor(255, 0, 0);
	myGLCD.setBackColor(0, 0, 0);
	myGLCD.print("OPTIONS MENU", CENTER, 20);
	Graphics::DrawButtonT();
	Graphics::DrawButtonB();
	Graphics::DrawBack();
	Graphics::DrawNext();

	myGLCD.setFont(SmallFont);
	myGLCD.setColor(255, 165, 0);
	myGLCD.setBackColor(0, 0, 0);
	myGLCD.fillRect(0, 226, 319, 239);
	myGLCD.print("(c) 2017, Michael & George Marinis", CENTER, 227);

	if (page == 1)
	{
		myGLCD.setBackColor(192, 192, 192);
		myGLCD.setColor(0, 0, 255);
		myGLCD.print("SET MANUAL-ARM PASSWORD", CENTER, 73);
		myGLCD.print("SET ARM PASSWORD", CENTER, 133);
	}
	else if (page == 2)
	{
		myGLCD.setBackColor(192, 192, 192);
		myGLCD.setColor(0, 0, 255);
		myGLCD.print("SET AUTO-ARM PASSWORD", CENTER, 73);
		myGLCD.print("SET AUTO-ARM TIME", CENTER, 133);
	}
	else if (page == 3)
	{
		myGLCD.setBackColor(192, 192, 192);
		myGLCD.setColor(0, 0, 255);
		myGLCD.print("TIME SETTINGS", CENTER, 73);
		myGLCD.print("DATE SETTINGS", CENTER, 133);
	}
}

// Last page of the options menu (does not have the "NEXT" button)
void Graphics::OptionsMenuEnd() const
{
	Graphics::AlarmTitle();
	myGLCD.setColor(255, 0, 0);
	myGLCD.setBackColor(0, 0, 0);
	myGLCD.print("OPTIONS MENU", CENTER, 20);
	Graphics::DrawButtonT();
	Graphics::DrawBack();
	myGLCD.setFont(SmallFont);
	myGLCD.setBackColor(192, 192, 192);
	myGLCD.setColor(0, 0, 255);
	myGLCD.print("OTHER SETTINGS", CENTER, 73);

	myGLCD.setFont(SmallFont);
	myGLCD.setColor(255, 165, 0);
	myGLCD.setBackColor(0, 0, 0);
	myGLCD.fillRect(0, 226, 319, 239);
	myGLCD.print("(c) 2017, Michael & George Marinis", CENTER, 227);
}
///**************************************///


///****SCREEN UPDATE METHODS****///
// Checks if the screen sis touched
bool Graphics::Available() const
{
	return myTouch.dataAvailable();
}

// Reads the touch's X point
int16_t Graphics::GetX() const
{
	myTouch.read();
	return myTouch.getX();
}

// Reads the touch's Y point
int16_t Graphics::GetY() const
{
	myTouch.read();
	return myTouch.getY();
}
///*****************************///


///********PASSWORD RELATED********///
void Graphics::PassInputMessage(int passtype, char newoldpass) const
{
	Graphics::Title();
	myGLCD.setBackColor(0, 0, 0);
	myGLCD.setColor(255, 0, 0);

	if (newoldpass = 'o')
	{
		if (passtype == 6)
		{
			myGLCD.print("ENTER 6-DIGIT PASSWORD", CENTER, 119);
		}
		else if (passtype == 5)
		{
			myGLCD.print("ENTER 5-DIGIT PASSWORD", CENTER, 119);
		}
		else if (passtype = 4)
		{
			myGLCD.print("ENTER 4-DIGIT PASSWORD", CENTER, 119);
		}
	}
	else
	{
		myGLCD.setBackColor(0, 0, 0);
		myGLCD.setColor(255, 0, 0);
		myGLCD.print("ENTER NEW PASSWORD", CENTER, 119);
	}


	Graphics::DrawBackBig();
	Graphics::DrawNextBig();
}


void Graphics::PassOk() const
{
	myGLCD.clrScr();
	myGLCD.setFont(BigFont);
	myGLCD.print("PASS OK!", CENTER, 100);
}


void Graphics::ShowPass(int passtype, int * temppass) const
{
	myGLCD.setFont(SevenSegNumFont);
	myGLCD.setColor(255, 0, 0);
	myGLCD.setBackColor(0, 0, 0);

	if (passtype == 6)
	{
		for (int i = 0; i < 6; i++)
		{
			myGLCD.printNumI(*(temppass + i), 20 + (i * 50), 120);
		}
	}
	else if (passtype == 5)
	{
		for (int i = 0; i < 5; i++)
		{
			myGLCD.printNumI(*(temppass + i), 40 + (i * 50), 120);
		}
	}
	else if (passtype == 4)
	{
		for (int i = 0; i < 4; i++)
		{
			myGLCD.printNumI(*(temppass + i), 60 + (i * 50), 120);
		}
	}
}


void Graphics::PrintNumber(int input, int buf, int passtype) const
{
	if (passtype == 6)
	{
		if (buf <= 5)
		{
			myGLCD.setColor(255, 255, 255);
			myGLCD.setBackColor(0, 0, 0);
			myGLCD.printNumI(input, 100 + (buf * 35), 185);
		}
	}
	else if (passtype == 5)
	{
		if (buf <= 4)
		{
			myGLCD.setColor(255, 255, 255);
			myGLCD.setBackColor(0, 0, 0);
			myGLCD.printNumI(input, 100 + (buf * 35), 185);
		}
	}
	else if (passtype == 4)
	{
		if (buf <= 3)
		{
			myGLCD.setColor(255, 255, 255);
			myGLCD.setBackColor(0, 0, 0);
			myGLCD.printNumI(input, 100 + (buf * 35), 185);
		}
	}
}


void Graphics::ConfirmPass() const
{
	myGLCD.setColor(255, 0, 0);
	myGLCD.setBackColor(0, 0, 0);
	myGLCD.print("CONFIRM PASS CHANGE?", CENTER, 85);
}


void Graphics::PassChangeSuccessful() const
{
	myGLCD.clrScr();
	myGLCD.setColor(255, 0, 0);
	myGLCD.setBackColor(0, 0, 0);
	myGLCD.setFont(BigFont);
	myGLCD.print("PASSWORD CHANGE", CENTER, 100);
	myGLCD.print("SUCCESSFUL", CENTER, 150);
}


void Graphics::WrongPass() const
{
	myGLCD.clrScr();
	myGLCD.setFont(BigFont);
	myGLCD.print("WRONG PASSWORD!", CENTER, 100);
}
///********************************///


///******TIME RELATED******///
void Graphics::AutoTimeScreen(int autotime, int page)
{
	if (page == 1)
	{
		myGLCD.clrScr();
		myGLCD.setColor(255, 0, 0);
		myGLCD.setBackColor(0, 0, 0);
		myGLCD.print("SET NEW AUTO-ARM TIME", CENTER, 40);
		myGLCD.print("THE CURRENT ONE IS", CENTER, 60);
		myGLCD.setFont(SevenSegNumFont);
		myGLCD.printNumI(autotime, CENTER, 90);
		myGLCD.setFont(BigFont);
		myGLCD.print("SECONDS", CENTER, 150);

		Graphics::DrawBack();
		Graphics::DrawNext();
	}
	else if (page == 2)
	{              
		myGLCD.clrScr();
		Graphics::AlarmTitle();
		myGLCD.setColor(255, 0, 0);
		myGLCD.setBackColor(0, 0, 0);
		myGLCD.setFont(BigFont);
		myGLCD.print("ACCEPT CHANGES?", CENTER, 50);

		myGLCD.setFont(SevenSegNumFont);
		myGLCD.printNumI(autotime, CENTER, 70);

		myGLCD.setFont(BigFont);
		myGLCD.print("SECONDS", CENTER, 135);

		Graphics::DrawNo();
		Graphics::DrawYes();
	}
}

void Graphics::PrintAutoArmTime(int input, int timelength)
{
	if (timelength <= 4)
	{
		myGLCD.setColor(255, 255, 255);
		myGLCD.setBackColor(0, 0, 0);
		myGLCD.printNumI(input, 140, 185);
	}
}

void Graphics::AutoTimeChangeSuccess()
{
	myGLCD.clrScr();
	myGLCD.setColor(255, 0, 0);
	myGLCD.setBackColor(0, 0, 0);
	myGLCD.setFont(BigFont);
	myGLCD.print("AUTO-ARM TIME WAS", CENTER, 110);
	myGLCD.print("SUCCESSFULLY CHANGED", CENTER, 130);
}


void Graphics::DrawTimeDate() const
{
	myGLCD.clrScr();
	myGLCD.setBackColor(0, 0, 0);
	myGLCD.setColor(255, 255, 255);
	myGLCD.setFont(BigFont);
	myGLCD.print(rtc.getDateStr(), 153, 7);
	myGLCD.printNumI(rtc.getTemp(), 1, 7);
	myGLCD.print("C", 44, 7);
	myGLCD.setFont(SmallFont);
	myGLCD.print("o", 38, 5);

	Graphics::DrawTime();
}

void Graphics::DrawColon() const
{
	myGLCD.setColor(255, 165, 0);
	myGLCD.fillCircle(112, 85, 4);
	myGLCD.setColor(255, 165, 0);
	myGLCD.fillCircle(112, 105, 4);

	myGLCD.setColor(255, 165, 0);
	myGLCD.fillCircle(208, 85, 4);
	myGLCD.setColor(255, 165, 0);
	myGLCD.fillCircle(208, 105, 4);
}

void Graphics::DrawTime() const
{
	timeString = rtc.getTimeStr();
	currentHours = timeString.substring(0, 2);
	currentMinutes = timeString.substring(3, 5);
	currentSeconds = timeString.substring(6, 8);
	myGLCD.setFont(SevenSegNumFont);
	myGLCD.setColor(255, 165, 0);
	myGLCD.print(currentSeconds, 224, 70);
	myGLCD.print(currentMinutes, 128, 70);
	myGLCD.print(currentHours, 32, 70);

	Graphics::DrawColon();
}



///*******ARM/DISARM RELATED*******///
void Graphics::SystemArmed() const
{
	myGLCD.clrScr();
	myGLCD.setColor(255, 0, 0);
	myGLCD.setBackColor(0, 0, 0);
	myGLCD.setFont(BigFont);
	myGLCD.print("SYSTEM ARMED!", CENTER, 110);
}

void Graphics::SystemDisarmed() const
{
	myGLCD.clrScr();
	myGLCD.setColor(255, 0, 0);
	myGLCD.setBackColor(0, 0, 0);
	myGLCD.setFont(BigFont);
	myGLCD.print("SYSTEM DISARMED!", CENTER, 110);
}

void Graphics::UpdateTime() const
{	
	if (currentClock != rtc.getTimeStr()) 
	{
		timeString = rtc.getTimeStr();
		hoursS = timeString.substring(0, 2);
		minutesS = timeString.substring(3, 5);
		secondsS = timeString.substring(6, 8);

		myGLCD.setFont(SevenSegNumFont);
		myGLCD.setColor(255, 165, 0);
		myGLCD.setBackColor(0, 0, 0);


		myGLCD.print(secondsS, 224, 70);

		if (currentMinutes != minutesS) 
		{
			myGLCD.print(minutesS, 128, 70);
			currentMinutes = minutesS;
		}
		if (currentHours != hoursS) 
		{
			myGLCD.print(hoursS, 32, 70);
			currentHours = hoursS;
		}

		// Checks for change of the date
		dateS = rtc.getDateStr();
		delay(10);
		if (currentDate != dateS) 
		{
			myGLCD.setColor(255, 255, 255);
			myGLCD.setFont(BigFont);
			myGLCD.print(rtc.getDateStr(), 153, 7);
		}

		// Checks for change of the temperature
		temperature = rtc.getTemp();
		delay(10);
		if (currentTemperature != temperature) 
		{
			myGLCD.setColor(255, 255, 255);
			myGLCD.setFont(BigFont);
			myGLCD.printNumI(temperature, 1, 7);
			currentTemperature = temperature;
		}
		delay(10);
		currentClock = rtc.getTimeStr();

		Graphics::DrawSeasonGraphics();
	}
}

void Graphics::DrawSeasonGraphics() const
{
	// default y <= 54 (over time)
	// default y = 120 (under time)
	// default x < 32 (left side of time)
	// default x > 288 (right side of time)
	currentMonth = dateS.substring(3, 5);

	myGLCD.setBackColor(0, 0, 0);
	myGLCD.setColor(0, 0, 0);

	myGLCD.fillRect(x1, y1, x1 + 16, y1 + 16);
	myGLCD.fillRect(x2, y2, x2 + 16, y2 + 16);
	myGLCD.fillRect(x3, y3, x3 + 16, y3 + 16);
	myGLCD.fillRect(x4, y4, x4 + 16, y4 + 16);
	myGLCD.fillRect(x5, y5, x5 + 16, y5 + 16);
	myGLCD.fillRect(x6, y6, x6 + 16, y6 + 16);
	myGLCD.fillRect(x7, y7, x7 + 16, y7 + 16);

	x1 = random(2, 15);
	y1 = random(30, 145);
	x2 = random(80, 140);
	y2 = random(2, 40);
	x3 = random(80, 140);
	y3 = random(123, 160);
	x4 = random(160, 220);
	y4 = random(26, 40);
	x5 = random(160, 220);
	y5 = random(123, 160);
	x6 = random(250, 300);
	y6 = random(26, 40);
	x7 = random(250, 300);
	y7 = random(123, 160);

	if (currentMonth.toInt() > 11 || currentMonth.toInt() < 3)
	{
		myGLCD.drawBitmap(x1, y1, 16, 16, snow2, random(0, 90), 8, 8);
		myGLCD.drawBitmap(x2, y2, 16, 16, snow3, random(0, 90), 8, 8);
		myGLCD.drawBitmap(x3, y3, 16, 16, snow3, random(0, 90), 8, 8);
		myGLCD.drawBitmap(x4, y4, 16, 16, snow, random(0, 90), 8, 8);
		myGLCD.drawBitmap(x5, y5, 16, 16, snow, random(0, 90), 8, 8);
		myGLCD.drawBitmap(x6, y6, 16, 16, snow4, random(0, 90), 8, 8);
		myGLCD.drawBitmap(x7, y7, 16, 16, snow4, random(0, 90), 8, 8);
	}
}
void Graphics::AutoCountDown(int autoarmtime) const
{
	myGLCD.setFont(BigFont);
	myGLCD.setColor(255, 255, 255);
	myGLCD.setBackColor(0, 0, 0);
	myGLCD.print("THE ALARM WILL", CENTER, 40);
	myGLCD.print("BE ARMED IN", CENTER, 60);
	myGLCD.setFont(SevenSegNumFont);
	myGLCD.printNumI(autoarmtime, CENTER, 110);
	myGLCD.setFont(BigFont);
	myGLCD.print("SECONDS", CENTER, 170);
}

void Graphics::SystemAutoArmed() const
{
	myGLCD.clrScr();
	myGLCD.setColor(255, 0, 0);
	myGLCD.setBackColor(0, 0, 0);
	myGLCD.setFont(BigFont);
	myGLCD.print("SYSTEM IS AUTO-ARMED", CENTER, 110);
}

void Graphics::SystemFullyDisarmed() const
{
	myGLCD.clrScr();
	myGLCD.setFont(BigFont);
	myGLCD.print("SYSTEM", CENTER, 100);
	myGLCD.print("FULLY DISARMED!", CENTER, 130);
}


///******TIME SETTINGS RELATED******///
void Graphics::PrintTime() const
{
	myGLCD.clrScr();
	myGLCD.setFont(SmallFont);
	myGLCD.setColor(255, 255, 255);
	myGLCD.setBackColor(255, 0, 0);
	myGLCD.drawLine(0, 14, 319, 14);
	myGLCD.print("ADJUST TIME", CENTER, 1);
	
	myGLCD.setBackColor(0, 0, 0);
	Graphics::DrawTime();
	myGLCD.setFont(Various_Symbols_16x32);
	for (int i = 0; i < 3; i++)
	{
		myGLCD.print("c", 54 + (i*96), 33); // x = 54, 150, 246  y = 33, 125		x = 70, 166, 262  y = 65, 157
		myGLCD.print("d", 54 + (i * 96), 125);
	}

	Graphics::DrawBack();
	Graphics::DrawNext();
}

void Graphics::AdjustTime(char type, int incdec) const // Where: incdec can either be 1 or 0 (increase/decrease) and type can be 'h', 'm' or 's'.
{
	int hours = currentHours.toInt();
	int minutes = currentMinutes.toInt();
	int seconds = currentSeconds.toInt();

	if (type == 'h') // Changing hours
	{
		if (incdec == 1)
		{
			if (hours == 23)
			{
				currentHours = "00";
				return;
			}
			else
			{
				hours++;
			}
		}
		else if(incdec == 0)
		{
			if (hours == 0)
			{
				hours = 23;
			}
			else
			{
				hours--;
			}
		}

		currentHours = String(hours);
	}
	else if (type == 'm') // Changing minutes
	{
		if (incdec == 1)
		{
			if (minutes == 59)
			{
				currentMinutes = "00";
				return;
			}
			else
			{
				minutes++;
			}
		}
		else if (incdec == 0)
		{
			if (minutes == 0)
			{
				minutes = 59;
			}
			else
			{
				minutes--;
			}
		}

		currentMinutes = String(minutes);
	}
	else if (type == 's') // Changing seconds
	{
		if (incdec == 1)
		{
			if (seconds == 59)
			{
				currentSeconds = "00";
				return;
			}
			else
			{
				seconds++;
			}
		}
		else if(incdec == 0)
		{
			if (seconds == 0)
			{
				seconds = 59;
			}
			else
			{
				seconds--;
			}
		}

		currentSeconds = String(seconds);
	}
}

void Graphics::HoursCorrection(bool dec) const
{
	if (currentHours.toInt() > 0 && currentHours.toInt() < 10)
	{
		currentHours = "0" + currentHours;
	}
	if (dec)
	{
		if (currentHours.toInt() == 0)
		{
			currentHours = "00";
		}
	}
}

void Graphics::MinutesCorrection(bool dec) const
{
	if (currentMinutes.toInt() > 0 && currentMinutes.toInt() < 10)
	{
		currentMinutes = "0" + currentMinutes;
	}
	if (dec)
	{
		if (currentMinutes.toInt() == 0)
		{
			currentMinutes = "00";
		}
	}
}

void Graphics::SecondsCorrection(bool dec) const
{
	if (currentSeconds.toInt() > 0 && currentSeconds.toInt() < 10)
	{
		currentSeconds = "0" + currentSeconds;
	}
	if (dec)
	{
		if (currentSeconds.toInt() == 0)
		{
			currentSeconds = "00";
		}
	}
}

void Graphics::PrintTempTime() const
{
	myGLCD.setFont(SevenSegNumFont);
	myGLCD.setColor(0, 255, 0);
	myGLCD.setBackColor(0, 0, 0);

	myGLCD.print(currentHours, 32, 70);
	myGLCD.print(currentSeconds, 224, 70);
	myGLCD.print(currentMinutes, 128, 70);

	Graphics::DrawColon();
}

void Graphics::SetTime() const
{
	rtc.setTime(currentHours.toInt(), currentMinutes.toInt(), currentSeconds.toInt());
}

void Graphics::AdjustTimeSuccess() const
{
	myGLCD.clrScr();
	myGLCD.setColor(255, 0, 0);
	myGLCD.setBackColor(0, 0, 0);
	myGLCD.setFont(BigFont);
	myGLCD.print("TIME WAS ADJUSTED", CENTER, 100);
	myGLCD.print("SUCCESSFULLY!", CENTER, 150);
}


///******DATE SETTINGS RELATED******//
void Graphics::DrawDate() const
{
	dateS = rtc.getDateStr();
	currentDay = dateS.substring(0, 2);
	currentMonth = dateS.substring(3, 5);
	currentYear = dateS.substring(6, 10);
	myGLCD.setFont(GroteskBold24x48);
	myGLCD.setColor(255, 165, 0);
	myGLCD.print(currentYear, 210, 70);
	myGLCD.print(currentMonth, 130, 70);
	myGLCD.print(currentDay, 40, 70);
}


void Graphics::PrintDate() const
{
	myGLCD.clrScr();
	myGLCD.setFont(SmallFont);
	myGLCD.setColor(255, 255, 255);
	myGLCD.setBackColor(255, 0, 0);
	myGLCD.drawLine(0, 14, 319, 14);
	myGLCD.print("ADJUST DATE", CENTER, 1);

	myGLCD.setBackColor(0, 0, 0);
	Graphics::DrawDate();
	myGLCD.setFont(Various_Symbols_16x32);
	for (int i = 0; i < 3; i++)
	{
		myGLCD.print("c", 54 + (i * 96), 33); // x = 54, 150, 246  y = 33, 125		x = 70, 166, 262  y = 65, 157
		myGLCD.print("d", 54 + (i * 96), 125);
	}

	Graphics::DrawBack();
	Graphics::DrawNext();
}

void Graphics::AdjustDate(char type, int incdec) const
{
	int day = currentDay.toInt();
	int month = currentMonth.toInt();
	int year = currentYear.toInt();

	if (type == 'd') // Changing hours
	{
		if (incdec == 1)
		{
			if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)
			{
				if (day == 31)
				{
					currentDay = "01";
					return;
				}
				else
				{
					day++;
				}
			}
			else if (month == 2)
			{
				if (day == 28)
				{
					if (year % 4 == 0)
					{
						if (day > 29) // "Safety-net", day could be bigger than 29 from another month
						{
							currentDay = "01";
							return;
						}
						currentDay = "29";
						return;
					}
					else
					{
						if (day > 28) // "Safety-net", day could be bigger than 28 from another month
						{
							currentDay = "01";
							return;
						}
						currentDay = "01";
						return;
					}
				}
				else if (day == 29)
				{
					currentDay = "01";
					return;
				}
				else
				{
					day++;
				}
			}
			else
			{
				if (day == 30)
				{
					currentDay = "01";
					return;
				}
				else
				{
					day++;
				}
			}
		}
		else if (incdec == 0)
		{
			if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)
			{
				if (day == 1)
				{
					currentDay = "31";
					return;
				}
				else
				{
					day--;
				}
			}
			else if (month == 2)
			{
				if (day == 1)
				{
					if (year % 4 == 0)
					{
						currentDay = "29";
						return;
					}
					else
					{
						currentDay = "28";
						return;
					}
				}
				else
				{
					day--;
				}
			}
			else
			{
				if (day == 1)
				{
					currentDay = "30";
					return;
				}
				else
				{
					day--;
				}
			}
		}

		currentDay = String(day);
	}
	else if (type == 'm') // Changing minutes
	{
		if (incdec == 1)
		{
			if (month == 12)
			{
				currentMonth = "01";
				return;
			}
			else
			{
				month++;
			}
		}
		else if (incdec == 0)
		{
			if (month == 1)
			{
				currentMonth = "12";
				return;
			}
			else
			{
				month--;
			}
		}

		currentMonth = String(month);
	}
	else if (type == 'y') // Changing seconds
	{
		if (incdec == 1)
		{
			year++;
		}
		else if (incdec == 0)
		{
			year--;
		}

		currentYear = String(year);
	}
}

void Graphics::DaysCorrection(bool dec) const
{
	if (currentDay.toInt() > 1 && currentDay.toInt() < 10)
	{
		currentDay = "0" + currentDay;
	}
	if (dec)
	{
		if (currentDay.toInt() == 1)
		{
			currentDay = "01";
		}
	}
}

void Graphics::MonthsCorrection(bool dec) const
{
	if (currentMonth.toInt() > 1 && currentMonth.toInt() < 10)
	{
		currentMonth = "0" + currentMonth;
	}
	if (dec)
	{
		if (currentMonth.toInt() == 1)
		{
			currentMonth = "01";
		}
	}
}

void Graphics::YearsCorrection() const
{
	if (currentYear.toInt() > 1 && currentYear.toInt() < 10)
	{
		currentYear = "0" + currentYear;
	}
}

void Graphics::PrintTempDate() const
{
	myGLCD.setFont(GroteskBold24x48);
	myGLCD.setColor(0, 255, 0);
	myGLCD.setBackColor(0, 0, 0);

	myGLCD.print(currentYear, 210, 70);
	myGLCD.print(currentMonth, 130, 70);
	myGLCD.print(currentDay, 40, 70);
}

void Graphics::SetDate() const
{
	rtc.setDate(currentDay.toInt(), currentMonth.toInt(), currentYear.toInt());
}

void Graphics::AdjustDateSuccess() const
{
	myGLCD.clrScr();
	myGLCD.setColor(255, 0, 0);
	myGLCD.setBackColor(0, 0, 0);
	myGLCD.setFont(BigFont);
	myGLCD.print("DATE WAS ADJUSTED", CENTER, 100);
	myGLCD.print("SUCCESSFULLY!", CENTER, 150);
}


///******SD CARD RELATED******///
void Graphics::SelectStorage() const
{
	myGLCD.clrScr();

	myGLCD.setFont(SmallFont);
	myGLCD.setColor(255, 0, 0);
	myGLCD.setBackColor(0, 0, 0);
	myGLCD.print("SELECT STORAGE", CENTER, 10);

	myGLCD.setColor(0, 0, 255);
	myGLCD.drawRect(0, 0, 319, 239);

	Graphics::DrawButtonT();
	Graphics::DrawButtonB();

	myGLCD.setColor(0, 0, 0);
	myGLCD.setBackColor(192, 192, 192);
	myGLCD.print("INTERNAL STORAGE", CENTER, 75);
	myGLCD.print("INTERNAL + SD CARD", CENTER, 135);

	myGLCD.setColor(255, 255, 255);
	myGLCD.setBackColor(0, 0, 0);
	myGLCD.print("Don't forget to insert the SD card in ", CENTER, 170);
	myGLCD.print("case you want to save your passwords", CENTER, 184);
	myGLCD.print(" in the SD card as well. . .", CENTER, 198);
}

void Graphics::SDNotFound() const
{
	myGLCD.clrScr();
	myGLCD.setFont(BigFont);
	myGLCD.setColor(255, 0, 0);
	myGLCD.setBackColor(0, 0, 0);
	myGLCD.print("SD CARD NOT FOUND!", CENTER, 50);

	myGLCD.setFont(SmallFont);
	myGLCD.print("Make sure that the SD card", CENTER, 90);
	myGLCD.print("is inserted correctly and try again", CENTER, 104);
	delay(3000);
}

//******MISCELLANEOUS******//
void Graphics::OtherSettings(bool autoload, bool askarmpass) const
{
	Graphics::AlarmTitle();
	Graphics::DrawBack();
	
	myGLCD.setColor(255, 255, 255);
	myGLCD.setBackColor(255, 0, 0);
	myGLCD.print("AUTO PASSWORD LOADING", 5, 30);
	myGLCD.drawLine(5, 43, 173, 43);

	myGLCD.print("ARM PASSWORD", 45, 110);
	myGLCD.drawLine(45, 123, 138, 123);

	myGLCD.setBackColor(0, 0, 0);
	myGLCD.print("*SD REQUIRED!", 190, 60);

	

	myGLCD.setFont(BigFont);
	myGLCD.setBackColor(0, 0, 0);
	myGLCD.print("ON", 5, 47);
	myGLCD.print("OFF", 125, 47);

	myGLCD.setFont(BigFont);
	myGLCD.setBackColor(0, 0, 0);
	myGLCD.print("ON", 5, 127);
	myGLCD.print("OFF", 125, 127);

	myGLCD.setFont(Various_Symbols_16x32);
	if (autoload)
	{
		myGLCD.print("B", 15, 65);
		myGLCD.print("A", 140, 65);
	}
	else
	{
		myGLCD.print("A", 15, 65);
		myGLCD.print("B", 140, 65);
	}

	if (askarmpass)
	{
		myGLCD.print("B", 15, 145);
		myGLCD.print("A", 140, 145);
	}
	else
	{
		myGLCD.print("A", 15, 145);
		myGLCD.print("B", 140, 145);
	}
}

void Graphics::AutoLoadTrue(bool autoload) const
{
	myGLCD.setColor(255, 255, 255);
	myGLCD.setBackColor(0, 0, 0);

	if (autoload)
	{
		myGLCD.print("B", 15, 65);
		myGLCD.print("A", 140, 65);
	}
	else
	{
		myGLCD.print("A", 15, 65);
		myGLCD.print("B", 140, 65);
	}
}

void Graphics::AskArmPass(bool askarmpass) const
{
	myGLCD.setColor(255, 255, 255);
	myGLCD.setBackColor(0, 0, 0);

	if (askarmpass)
	{
		myGLCD.print("B", 15, 145);
		myGLCD.print("A", 140, 145);
	}
	else
	{
		myGLCD.print("A", 15, 145);
		myGLCD.print("B", 140, 145);
	}
}
//*************************//
