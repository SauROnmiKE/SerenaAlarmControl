/*
//SERENA Alarm System: Graphics Header
//
//Author: Michael Marinis
//
//©2017 Michael Marinis
//
//Website: http://www.youtube.com/SauROnmiKE
//
//This header file is a part of the SERENA Alarm System program and is used to draw all the graphics on the TFT screen
//
//Graphics.h MUST be in the Documents/Arduino/libraries e.g.: Documents/Arduino/libraries/Graphics/Graphics.h & .cpp
*/


#ifndef _GRAPHICS_h
#define _GRAPHICS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class Graphics
{
public:
	Graphics(); // Constructor

	void InitializeScreen() const;
	void InitializeRTC();
	void CheckRTC();

	void SDFound(bool sdfound) const;
	void LoadingPass(bool load, int passtype) const;
	void PassLoadSuccess(bool success, int passtype) const;
	void LoadingScreen(bool alarmarmed);
	void BootProgram();

	void Intro() const;
	void DrawArc() const;

	void AlarmTitle() const;
	void Title() const;
	
	void ClearScreen() const;

	void DrawButtonT() const;
	void DrawButtonB() const;
	void DrawDisarm() const;
	void DrawNo() const;
	void DrawYes() const;
	void DrawNext() const;
	void DrawNextBig() const;
	void DrawBack() const;
	void DrawBackBig() const;
	void DrawKeyboard() const;
	void DrawOptions() const;
	void DrawArm() const;

	
	void MainMenu() const;
	void OptionsMenu(int page) const;
	void OptionsMenuEnd() const;

	int16_t GetX() const;
	int16_t GetY() const;


	bool Available() const;
	bool clockfound;

	//********PASSWORD RELATED********//
	void PassInputMessage(int passtype, char newoldpass) const;
	void PassOk() const;
	void ShowPass(int passtype, int * temppass) const;
	void PrintNumber(int input, int buf, int passtype) const;
	void ConfirmPass() const;
	void PassChangeSuccessful() const;
	void WrongPass() const;
	
	//******TIME RELATED******//
	bool GetClockStatus();
	void ClockNotFound(int menu) const;
	void AutoTimeScreen(int autotime, int page);
	void PrintAutoArmTime(int input, int timelength);
	void AutoTimeChangeSuccess();
	void DrawTimeDate() const;
	void DrawColon() const;
	void DrawTime() const;

	//*******ARM/DISARM RELATED*******//
	void SystemArmed() const;
	void SystemDisarmed() const;
	void UpdateTime() const;
	void DrawSeasonGraphics() const;
	void AutoCountDown(int autoarmtime) const;
	void SystemAutoArmed() const;
	void SystemFullyDisarmed() const;

	//******TIME SETTINGS RELATED******//
	void PrintTime() const;
	void AdjustTime(char type, int incdec) const;
	void HoursCorrection(bool dec) const;
	void MinutesCorrection(bool dec) const;
	void SecondsCorrection(bool dec) const;
	void PrintTempTime() const;
	void SetTime() const;
	void AdjustTimeSuccess() const;

	//******DATE SETTINGS RELATED******//
	void DrawDate() const;
	void PrintDate() const;
	void AdjustDate(char type, int incdec) const;
	void DaysCorrection(bool dec) const;
	void MonthsCorrection(bool dec) const;
	void YearsCorrection() const;
	void PrintTempDate() const;
	void SetDate() const;
	void AdjustDateSuccess() const;

	//******SD CARD RELATED******//
	void SelectStorage() const;
	void SDNotFound() const;
	void SDCheckSkipped() const;
	//***************************//

	//******MISCELLANEOUS******//
	void OtherSettings(int page, bool autoload, bool askarmpass, bool sdchecking) const;
	void AutoLoadTrue(bool autoload) const;
	void AskArmPass(bool askarmpass) const;
	void SDChecking(bool sdchecking) const;
	void EEPROMWarning() const;
	//*************************//
	
	void Debug(long var);
};
#endif

