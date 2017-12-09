/*
//SERENA Alarm System: Security Header
//
//Author: Michael Marinis
//
//©2017 Michael Marinis
//
//Website: http://www.youtube.com/SauROnmiKE
//
//This header file is a part of the SERENA Alarm System program and is used to handle the passwords of the alarm system
//
//Security.h MUST be in the Documents/Arduino/libraries e.g.: Documents/Arduino/libraries/Security/Security.h & .cpp
*/

#ifndef _SECURITY_h
#define _SECURITY_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class Security
{
public:
	Security(); // constructor

	int * GetPass(int passtype) const;
	int * GetTempPass(int passtype);
	int GetAutoTime(char ttype) const;
	void SetPass(int passtype);

	bool CheckSD();
	bool LoadSDPass(int passtype);
	void SetSDPass(int passtype);

	void SaveAutoLoad(bool autoload);
	bool LoadAutoLoad();
	void SaveAskArm(bool askarmpass);
	bool LoadAskArm();

	void SetAutoTime();
	
	void ResetInputs();

	//*****PASSWORD INPUTS*****//
	int ManualUpdate(int number, int buf);
	int ArmUpdate(int number, int buf);
	int AutoArmUpdate(int number, int buf);
	
	
	//*****TIME RELATED*****//
	int AutoTimeUpdate(int input, int timelength);

private:
	int armpass[4] = { 1, 1, 1, 1 };
	int autopass[5] = { 1, 1, 1, 1, 1 };
	int mpass[6] = { 1, 1, 1, 1, 1, 1 };

	int arminput[4];
	int autoinput[5];
	int manualinput[6];
	
	
	int timeinput = 0;
	int autoarmtime = 40;
};

#endif

