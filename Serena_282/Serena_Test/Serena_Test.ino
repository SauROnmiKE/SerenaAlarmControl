/*
  //SERENA Alarm System
  //
  //Author: Michael Marinis
  //
  // �2017 Michael Marinis. All Rights Reserved
  //
  //Website: http://www.youtube.com/SauROnmiKE
*/


#include <Graphics.h>
#include <Security.h>
#include <SoftwareSerial.h>

Security SerenaSecurity;
Graphics SerenaGraphics;
SoftwareSerial BTSerial(10, 11);

//******TFT VARIABLES******//
int16_t x;
int16_t y;
//*************************//

//**********COMPONENTS**********//
int buzzerpin = 8;
int pushbutton = 13;
void Beep();
void LongBeep();
void OKBeep();
//******************************//

//******MISCELLANEOUS******//
int input = 0;
bool autoload;
//*************************//


//*****ALARM RELATED*****//
bool alarmarmed = false;
bool askarmpass = false;
//***********************//


//*******TIME RELATED*******//

//**************************//

void setup()
{
  SerenaGraphics.InitializeScreen(); // When the board is powered or reset
  SerenaGraphics.InitializeRTC(); // Initialize Screen and other devices
  Serial.begin(9600);
  BTSerial.begin(38400);
  pinMode(buzzerpin, OUTPUT); // Initialize outputs
  OKBeep(); // Buzzer on for .2 seconds to indicate power's on. ---Loading screen, check for available SD
  SerenaGraphics.LoadingScreen();
 
SDCheck:  // Check SD Card

  if (SerenaSecurity.CheckSD())
  {
    autoload = SerenaSecurity.LoadAutoLoad();
	askarmpass = SerenaSecurity.LoadAskArm();
    if (!autoload)
    {
      SerenaGraphics.SDFound(true);
      while (true)
      {
        if (SerenaGraphics.Available())
        {
          x = SerenaGraphics.GetX();
          y = SerenaGraphics.GetY();

          if ((x >= 5) && (x <= 50) && (y >= 185) && (y <= 215)) // "Y" Button
          {
            SerenaGraphics.ClearScreen();
            SerenaGraphics.LoadingPass(true, 6);
            if (SerenaSecurity.LoadSDPass(6))
            {
              SerenaGraphics.PassLoadSuccess(true, 6);
            }
            else
            {
              SerenaGraphics.PassLoadSuccess(false, 6);
            }
            SerenaGraphics.LoadingPass(true, 5);
            if (SerenaSecurity.LoadSDPass(5))
            {
              SerenaGraphics.PassLoadSuccess(true, 5);
            }
            else
            {
              SerenaGraphics.PassLoadSuccess(false, 5);
            }

            SerenaGraphics.LoadingPass(true, 4);
            if (SerenaSecurity.LoadSDPass(4))
            {
              SerenaGraphics.PassLoadSuccess(true, 4);
              goto Intro;
            }
            else
            {
              SerenaGraphics.PassLoadSuccess(false, 4);
              goto Intro;
            }
          }

          if ((x >= 60) && (x <= 105) && (y >= 185) && (y <= 215)) // "N" Button
          {
            SerenaGraphics.LoadingPass(false, 0);
            goto Intro;
          }
        }
      }
    }
    else
    {
      SerenaGraphics.ClearScreen();
      SerenaGraphics.LoadingPass(true, 6);
      if (SerenaSecurity.LoadSDPass(6))
      {
        SerenaGraphics.PassLoadSuccess(true, 6);
      }
      else
      {
        SerenaGraphics.PassLoadSuccess(false, 6);
      }
      SerenaGraphics.LoadingPass(true, 5);
      if (SerenaSecurity.LoadSDPass(5))
      {
        SerenaGraphics.PassLoadSuccess(true, 5);
      }
      else
      {
        SerenaGraphics.PassLoadSuccess(false, 5);
      }

      SerenaGraphics.LoadingPass(true, 4);
      if (SerenaSecurity.LoadSDPass(4))
      {
        SerenaGraphics.PassLoadSuccess(true, 4);
        goto Intro;
      }
      else
      {
        SerenaGraphics.PassLoadSuccess(false, 4);
        goto Intro;
      }
    }

  }
  else
  {
    SerenaGraphics.SDFound(false);

    while (true)
    {
      if (SerenaGraphics.Available())
      {
        x = SerenaGraphics.GetX();
        y = SerenaGraphics.GetY();

        if ((x >= 5) && (x <= 50) && (y >= 185) && (y <= 215)) // "Y" Button
        {
          goto SDCheck;
        }

        if ((x >= 60) && (x <= 105) && (y >= 185) && (y <= 215)) // "N" Button
        {
          SerenaGraphics.BootProgram();
          goto Intro;
        }
      }
    }
  }
  // Print intro screen for 5 seconds

Intro:
  SerenaGraphics.Intro();
}



void loop()
{
mainmenu:

  // Show the main menu
  SerenaGraphics.MainMenu();
  while (true)
  {
    // If the screen is touched
    if (SerenaGraphics.Available())
    {
      // Get the touch's position
      x = SerenaGraphics.GetX();
      y = SerenaGraphics.GetY();

      if ((x >= 20) && (x <= 300) && (y >= 70) && (y <= 120)) // Options button
      {
        Beep();

options: // Options menu

        SerenaGraphics.OptionsMenu(1);

        while (true)
        {
          if (SerenaGraphics.Available())
          {
            x = SerenaGraphics.GetX();
            y = SerenaGraphics.GetY();



            if ((x >= 20) && (x <= 150) && (y >= 180) && (y <= 210)) // "BACK" button (goes to main menu)
            {
              Beep();
              goto mainmenu;
            }

            if ((x >= 20) && (x <= 300) && (y >= 60) && (y <= 100)) // "Set Manual Password" Button
            {
              SerenaGraphics.PassInputMessage(6, 'o');
              Beep();
              while (true)
              {
                if (SerenaGraphics.Available())
                {
                  x = SerenaGraphics.GetX();
                  y = SerenaGraphics.GetY();

                  // "Back" button
                  if ((x >= 20) && (x <= 150) && (y >= 165) && (y <= 210))
                  {
                    Beep();
                    goto options;
                  }

                  // Button menu for 6-digit password
                  if ((x >= 170) && (x <= 300) && (y >= 165) && (y <= 210))
                  {
                    SerenaSecurity.ResetInputs();

SDPC: //Six-Digit Pass Change

                    int buf = 0;

                    SerenaGraphics.DrawKeyboard(); // Draw keyboard

                    Beep();

                    while (true)
                    {
                      if (SerenaGraphics.Available())
                      {
                        x = SerenaGraphics.GetX();
                        y = SerenaGraphics.GetY();

                        // "Back" button
                        if ((x >= 190) && (x <= 290) && (y >= 125) && (y <= 165))
                        {
                          Beep();
                          goto options;
                        }

                        // "Clear" button
                        if ((x >= 190) && (x <= 290) && (y >= 15) && (y <= 55))
                        {
                          buf = 0;
                          SerenaSecurity.ResetInputs();
                          SerenaGraphics.DrawKeyboard();
                          Beep();
                        }

                        // Buttons 1-3
                        if ((y >= 15) && (y <= 55)) // Checking the first row
                        {
                          if ((x >= 20) && (x <= 60)) //#1
                          {
                            input = SerenaSecurity.ManualUpdate(1, buf);
                            SerenaGraphics.PrintNumber(input, buf, 6);
                            buf++;
                            delay(85);
                            Beep();
                          }

                          if ((x >= 80) && (x <= 120)) //#2
                          {
                            input = SerenaSecurity.ManualUpdate(2, buf);
                            SerenaGraphics.PrintNumber(input, buf, 6);
                            buf++;
                            delay(85);
                            Beep();
                          }

                          if ((x >= 140) && (x <= 180)) //#3
                          {
                            input = SerenaSecurity.ManualUpdate(3, buf);
                            SerenaGraphics.PrintNumber(input, buf, 6);
                            buf++;
                            delay(85);
                            Beep();
                          }
                        }

                        // Buttons 4-6
                        if ((y >= 70) && (y <= 110)) // Checking the second row
                        {
                          if ((x >= 20) && (x <= 60)) //#4
                          {
                            input = SerenaSecurity.ManualUpdate(4, buf);
                            SerenaGraphics.PrintNumber(input, buf, 6);
                            buf++;
                            delay(85);
                            Beep();
                          }

                          if ((x >= 80) && (x <= 120)) //#5
                          {
                            input = SerenaSecurity.ManualUpdate(5, buf);
                            SerenaGraphics.PrintNumber(input, buf, 6);
                            buf++;
                            delay(85);
                            Beep();
                          }

                          if ((x >= 140) && (x <= 180)) //#6
                          {
                            input = SerenaSecurity.ManualUpdate(6, buf);
                            SerenaGraphics.PrintNumber(input, buf, 6);
                            buf++;
                            delay(85);
                            Beep();
                          }
                        }

                        // Buttons 7-9
                        if ((y >= 125) && (y <= 165))
                        {
                          if ((x >= 20) && (x <= 60)) //#7
                          {
                            input = SerenaSecurity.ManualUpdate(7, buf);
                            SerenaGraphics.PrintNumber(input, buf, 6);
                            buf++;
                            delay(85);
                            Beep();
                          }

                          if ((x >= 80) && (x <= 120)) //#8
                          {
                            input = SerenaSecurity.ManualUpdate(8, buf);
                            SerenaGraphics.PrintNumber(input, buf, 6);
                            buf++;
                            delay(85);
                            Beep();
                          }

                          if ((x >= 140) && (x <= 180)) //#9
                          {
                            input = SerenaSecurity.ManualUpdate(9, buf);
                            SerenaGraphics.PrintNumber(input, buf, 6);
                            buf++;
                            delay(85);
                            Beep();
                          }
                        }

                        // #0 Button
                        if ((x >= 20) && (x <= 80) && (y >= 175) && (y <= 205))
                        {
                          input = SerenaSecurity.ManualUpdate(0, buf);
                          SerenaGraphics.PrintNumber(input, buf, 6);
                          buf++;
                          delay(85);
                          Beep();
                        }

                        // "OK" button
                        if ((x >= 190) && (x <= 290) && (y >= 70) && (y <= 110))
                        {
                          bool correctpass = true;
                          for (int t = 0; t < 6; t++)
                          {
                            // Checking validity
                            if (*(SerenaSecurity.GetTempPass(6) + t) != *(SerenaSecurity.GetPass(6) + t))
                            {
                              correctpass = false;
                            }
                          }

                          if (correctpass)
                          {
                            SerenaSecurity.ResetInputs();
                            SerenaGraphics.PassOk();
                            Beep();
                            delay(2000);
                            SerenaGraphics.PassInputMessage(6, 'n');

                            while (true)
                            {
                              if (SerenaGraphics.Available())
                              {
                                x = SerenaGraphics.GetX();
                                y = SerenaGraphics.GetY();

                                int buf = 0;

                                // Next button: prompts user to enter new password
                                if ((x >= 170) && (x <= 300) && (y >= 165) && (y <= 210))
                                {

NMPI: // New manual pass input

                                  SerenaGraphics.DrawKeyboard();
                                  Beep();

                                  while (true)
                                  {
                                    if (SerenaGraphics.Available())
                                    {
                                      x = SerenaGraphics.GetX();
                                      y = SerenaGraphics.GetY();

                                      // "Back" button
                                      if ((x >= 190) && (x <= 290) && (y >= 125) && (y <= 165))
                                      {
                                        Beep();
                                        goto options;
                                      }

                                      // "Clear" button
                                      if ((x >= 190) && (x <= 290) && (y >= 15) && (y <= 55))
                                      {
                                        buf = 0;
                                        SerenaSecurity.ResetInputs();
                                        SerenaGraphics.DrawKeyboard();
                                        Beep();
                                      }

                                      // Buttons 1-3
                                      if ((y >= 15) && (y <= 55)) // Checking the first row
                                      {
                                        if ((x >= 20) && (x <= 60)) //#1
                                        {
                                          input = SerenaSecurity.ManualUpdate(1, buf);
                                          SerenaGraphics.PrintNumber(input, buf, 6);
                                          buf++;
                                          delay(85);
                                          Beep();
                                        }

                                        if ((x >= 80) && (x <= 120)) //#2
                                        {
                                          input = SerenaSecurity.ManualUpdate(2, buf);
                                          SerenaGraphics.PrintNumber(input, buf, 6);
                                          buf++;
                                          delay(85);
                                          Beep();
                                        }

                                        if ((x >= 140) && (x <= 180)) //#3
                                        {
                                          input = SerenaSecurity.ManualUpdate(3, buf);
                                          SerenaGraphics.PrintNumber(input, buf, 6);
                                          buf++;
                                          delay(85);
                                          Beep();
                                        }
                                      }

                                      // Buttons 4-6
                                      if ((y >= 70) && (y <= 110)) // Checking the second row
                                      {
                                        if ((x >= 20) && (x <= 60)) //#4
                                        {
                                          input = SerenaSecurity.ManualUpdate(4, buf);
                                          SerenaGraphics.PrintNumber(input, buf, 6);
                                          buf++;
                                          delay(85);
                                          Beep();
                                        }

                                        if ((x >= 80) && (x <= 120)) //#5
                                        {
                                          input = SerenaSecurity.ManualUpdate(5, buf);
                                          SerenaGraphics.PrintNumber(input, buf, 6);
                                          buf++;
                                          delay(85);
                                          Beep();
                                        }

                                        if ((x >= 140) && (x <= 180)) //#6
                                        {
                                          input = SerenaSecurity.ManualUpdate(6, buf);
                                          SerenaGraphics.PrintNumber(input, buf, 6);
                                          buf++;
                                          delay(85);
                                          Beep();
                                        }
                                      }

                                      // Buttons 7-9
                                      if ((y >= 125) && (y <= 165))
                                      {
                                        if ((x >= 20) && (x <= 60)) //#7
                                        {
                                          input = SerenaSecurity.ManualUpdate(7, buf);
                                          SerenaGraphics.PrintNumber(input, buf, 6);
                                          buf++;
                                          delay(85);
                                          Beep();
                                        }

                                        if ((x >= 80) && (x <= 120)) //#8
                                        {
                                          input = SerenaSecurity.ManualUpdate(8, buf);
                                          SerenaGraphics.PrintNumber(input, buf, 6);
                                          buf++;
                                          delay(85);
                                          Beep();
                                        }

                                        if ((x >= 140) && (x <= 180)) //#9
                                        {
                                          input = SerenaSecurity.ManualUpdate(9, buf);
                                          SerenaGraphics.PrintNumber(input, buf, 6);
                                          buf++;
                                          delay(85);
                                          Beep();
                                        }
                                      }

                                      // #0 Button
                                      if ((x >= 20) && (x <= 80) && (y >= 175) && (y <= 205))
                                      {
                                        input = SerenaSecurity.ManualUpdate(0, buf);
                                        SerenaGraphics.PrintNumber(input, buf, 6);
                                        buf++;
                                        delay(85);
                                        Beep();
                                      }

                                      // "OK" button for new pass
                                      if ((x >= 190) && (x <= 290) && (y >= 70) && (y <= 110))
                                      {
                                        buf = 0;
                                        SerenaGraphics.Title();
                                        SerenaGraphics.ConfirmPass();
                                        SerenaGraphics.DrawYes();
                                        SerenaGraphics.DrawNo();

                                        int * showpass = SerenaSecurity.GetTempPass(6);
                                        SerenaGraphics.ShowPass(6, showpass);
                                        Beep();

                                        while (true)
                                        {
                                          if (SerenaGraphics.Available())
                                          {
                                            x = SerenaGraphics.GetX();
                                            y = SerenaGraphics.GetY();

                                            // "NO" button
                                            if ((x >= 20) && (x <= 150) && (y >= 180) && (y <= 210))
                                            {
                                              SerenaSecurity.ResetInputs();
                                              Beep();
                                              goto NMPI;
                                            }

                                            // "YES" button
                                            if ((x >= 170) && (x <= 300) && (y >= 180) && (y <= 210))
                                            {
SelectStorage:
                                              SerenaGraphics.SelectStorage();
                                         
                                              while (true)
                                              {
                                                if (SerenaGraphics.Available())
                                                {
                                                  x = SerenaGraphics.GetX();
                                                  y = SerenaGraphics.GetY();

                                                  if ((x >= 20) && (x <= 300) && (y >= 60) && (y <= 100)) // INTERNAL STORAGE
                                                  {
                                                    SerenaSecurity.SetPass(6);
                                                    SerenaSecurity.ResetInputs();
                                                    goto SDPS;
                                                  }

                                                  if ((x >= 20) && (x <= 300) && (y >= 120) && (y <= 160)) // SD + INTERNAL
                                                  {
                                                    if (SerenaSecurity.CheckSD())
                                                    {
                                                      SerenaSecurity.SetPass(6);
                                                      SerenaSecurity.SetSDPass(6);
                                                      SerenaSecurity.ResetInputs();
                                                      goto SDPS;
                                                    }
                                                    else
                                                    {
                                                      SerenaGraphics.SDNotFound();
                                                      goto SelectStorage;
                                                    }
                                                  }
                                                }
                                              }
SDPS:
                                              SerenaGraphics.PassChangeSuccessful();
                                              LongBeep();
                                              delay(3000);
                                              goto options;
                                            }
                                          }
                                        }
                                      }
                                    }
                                  }
                                } // "NEXT" button end

                                // "BACK" buton
                                if ((x >= 20) && (x <= 150) && (y >= 165) && (y <= 210))
                                {
                                  Beep();
                                  goto options;
                                }
                              }
                            }
                          }
                          else // Wrong password
                          {
                            SerenaSecurity.ResetInputs();
                            SerenaGraphics.WrongPass();
                            Beep();
                            delay(2000);
                            goto SDPC;
                          }
                        }
                      }
                    }
                  }
                }
              }
            } // Set Manual-Pass button end


            if ((x >= 20) && (x <= 300) && (y >= 120) && (y <= 160)) // Set Arm-Pass button
            {
              SerenaGraphics.PassInputMessage(4, 'o');
              SerenaGraphics.DrawBackBig();
              SerenaGraphics.DrawNextBig();
              Beep();

              while (true)
              {
                if (SerenaGraphics.Available())
                {
                  x = SerenaGraphics.GetX();
                  y = SerenaGraphics.GetY();

                  // "BACK" button
                  if ((x >= 20) && (x <= 150) && (y >= 165) && (y <= 210))
                  {
                    Beep();
                    goto options;
                  }


                  // "NEXT" button
                  if ((x >= 170) && (x <= 300) && (y >= 165) && (y <= 210))
                  {
                    SerenaSecurity.ResetInputs();

APC: // Arm-Pass Change

                    int buf = 0;
                    SerenaGraphics.DrawKeyboard();
                    Beep();

                    while (true)
                    {
                      if (SerenaGraphics.Available())
                      {
                        x = SerenaGraphics.GetX();
                        y = SerenaGraphics.GetY();

                        // "Back" button
                        if ((x >= 190) && (x <= 290) && (y >= 125) && (y <= 165))
                        {
                          Beep();
                          goto options;
                        }

                        // "Clear" button
                        if ((x >= 190) && (x <= 290) && (y >= 15) && (y <= 55))
                        {
                          buf = 0;
                          SerenaSecurity.ResetInputs();
                          SerenaGraphics.DrawKeyboard();
                          Beep();
                        }

                        // Buttons 1-3
                        if ((y >= 15) && (y <= 55)) // Checking the first row
                        {
                          if ((x >= 20) && (x <= 60)) //#1
                          {
                            input = SerenaSecurity.ArmUpdate(1, buf);
                            SerenaGraphics.PrintNumber(input, buf, 4);
                            buf++;
                            delay(85);
                            Beep();
                          }

                          if ((x >= 80) && (x <= 120)) //#2
                          {
                            input = SerenaSecurity.ArmUpdate(2, buf);
                            SerenaGraphics.PrintNumber(input, buf, 4);
                            buf++;
                            delay(85);
                            Beep();
                          }

                          if ((x >= 140) && (x <= 180)) //#3
                          {
                            input = SerenaSecurity.ArmUpdate(3, buf);
                            SerenaGraphics.PrintNumber(input, buf, 4);
                            buf++;
                            delay(85);
                            Beep();
                          }
                        }

                        // Buttons 4-6
                        if ((y >= 70) && (y <= 110)) // Checking the second row
                        {
                          if ((x >= 20) && (x <= 60)) //#4
                          {
                            input = SerenaSecurity.ArmUpdate(4, buf);
                            SerenaGraphics.PrintNumber(input, buf, 4);
                            buf++;
                            delay(85);
                            Beep();
                          }

                          if ((x >= 80) && (x <= 120)) //#5
                          {
                            input = SerenaSecurity.ArmUpdate(5, buf);
                            SerenaGraphics.PrintNumber(input, buf, 4);
                            buf++;
                            delay(85);
                            Beep();
                          }

                          if ((x >= 140) && (x <= 180)) //#6
                          {
                            input = SerenaSecurity.ArmUpdate(6, buf);
                            SerenaGraphics.PrintNumber(input, buf, 4);
                            buf++;
                            delay(85);
                            Beep();
                          }
                        }

                        // Buttons 7-9
                        if ((y >= 125) && (y <= 165))
                        {
                          if ((x >= 20) && (x <= 60)) //#7
                          {
                            input = SerenaSecurity.ArmUpdate(7, buf);
                            SerenaGraphics.PrintNumber(input, buf, 4);
                            buf++;
                            delay(85);
                            Beep();
                          }

                          if ((x >= 80) && (x <= 120)) //#8
                          {
                            input = SerenaSecurity.ArmUpdate(8, buf);
                            SerenaGraphics.PrintNumber(input, buf, 4);
                            buf++;
                            delay(85);
                            Beep();
                          }

                          if ((x >= 140) && (x <= 180)) //#9
                          {
                            input = SerenaSecurity.ArmUpdate(9, buf);
                            SerenaGraphics.PrintNumber(input, buf, 4);
                            buf++;
                            delay(85);
                            Beep();
                          }
                        }

                        // #0 Button
                        if ((x >= 20) && (x <= 80) && (y >= 175) && (y <= 205))
                        {
                          input = SerenaSecurity.ArmUpdate(0, buf);
                          SerenaGraphics.PrintNumber(input, buf, 4);
                          buf++;
                          delay(85);
                          Beep();
                        }

                        // "OK" button
                        if ((x >= 190) && (x <= 290) && (y >= 70) && (y <= 110))
                        {
                          bool correctpass = true;

                          for (int t = 0; t < 4; t++)
                          {
                            // Checking validity
                            if (*(SerenaSecurity.GetTempPass(4) + t) != *(SerenaSecurity.GetPass(4) + t))
                            {
                              correctpass = false;
                            }
                          }

                          if (correctpass == true)
                          {
                            SerenaSecurity.ResetInputs();
                            SerenaGraphics.PassOk();
                            Beep();
                            delay(2000);
                            SerenaGraphics.Title();
                            SerenaGraphics.PassInputMessage(4, 'n');
                            SerenaGraphics.DrawBackBig();
                            SerenaGraphics.DrawNextBig();

                            while (true)
                            {
                              if (SerenaGraphics.Available())
                              {
                                x = SerenaGraphics.GetX();
                                y = SerenaGraphics.GetY();

                                // "BACK" button
                                if ((x >= 20) && (x <= 150) && (y >= 165) && (y <= 210))
                                {
                                  Beep();
                                  goto options;
                                }

                                if ((x >= 170) && (x <= 300) && (y >= 165) && (y <= 210)) // "NEXT" button
                                {

NAPI: // New Arm-Pass Input

                                  int buf = 0;
                                  SerenaGraphics.DrawKeyboard();
                                  Beep();

                                  while (true)
                                  {
                                    if (SerenaGraphics.Available())
                                    {
                                      x = SerenaGraphics.GetX();
                                      y = SerenaGraphics.GetY();

                                      // "Back" button
                                      if ((x >= 190) && (x <= 290) && (y >= 125) && (y <= 165))
                                      {
                                        Beep();
                                        goto options;
                                      }

                                      // "Clear" button
                                      if ((x >= 190) && (x <= 290) && (y >= 15) && (y <= 55))
                                      {
                                        buf = 0;
                                        SerenaSecurity.ResetInputs();
                                        SerenaGraphics.DrawKeyboard();
                                        Beep();
                                      }

                                      // Buttons 1-3
                                      if ((y >= 15) && (y <= 55)) // Checking the first row
                                      {
                                        if ((x >= 20) && (x <= 60)) //#1
                                        {
                                          input = SerenaSecurity.ArmUpdate(1, buf);
                                          SerenaGraphics.PrintNumber(input, buf, 4);
                                          buf++;
                                          delay(85);
                                          Beep();
                                        }

                                        if ((x >= 80) && (x <= 120)) //#2
                                        {
                                          input = SerenaSecurity.ArmUpdate(2, buf);
                                          SerenaGraphics.PrintNumber(input, buf, 4);
                                          buf++;
                                          delay(85);
                                          Beep();
                                        }

                                        if ((x >= 140) && (x <= 180)) //#3
                                        {
                                          input = SerenaSecurity.ArmUpdate(3, buf);
                                          SerenaGraphics.PrintNumber(input, buf, 4);
                                          buf++;
                                          delay(85);
                                          Beep();
                                        }
                                      }

                                      // Buttons 4-6
                                      if ((y >= 70) && (y <= 110)) // Checking the second row
                                      {
                                        if ((x >= 20) && (x <= 60)) //#4
                                        {
                                          input = SerenaSecurity.ArmUpdate(4, buf);
                                          SerenaGraphics.PrintNumber(input, buf, 4);
                                          buf++;
                                          delay(85);
                                          Beep();
                                        }

                                        if ((x >= 80) && (x <= 120)) //#5
                                        {
                                          input = SerenaSecurity.ArmUpdate(5, buf);
                                          SerenaGraphics.PrintNumber(input, buf, 4);
                                          buf++;
                                          delay(85);
                                          Beep();
                                        }

                                        if ((x >= 140) && (x <= 180)) //#6
                                        {
                                          input = SerenaSecurity.ArmUpdate(6, buf);
                                          SerenaGraphics.PrintNumber(input, buf, 4);
                                          buf++;
                                          delay(85);
                                          Beep();
                                        }
                                      }

                                      // Buttons 7-9
                                      if ((y >= 125) && (y <= 165))
                                      {
                                        if ((x >= 20) && (x <= 60)) //#7
                                        {
                                          input = SerenaSecurity.ArmUpdate(7, buf);
                                          SerenaGraphics.PrintNumber(input, buf, 4);
                                          buf++;
                                          delay(85);
                                          Beep();
                                        }

                                        if ((x >= 80) && (x <= 120)) //#8
                                        {
                                          input = SerenaSecurity.ArmUpdate(8, buf);
                                          SerenaGraphics.PrintNumber(input, buf, 4);
                                          buf++;
                                          delay(85);
                                          Beep();
                                        }

                                        if ((x >= 140) && (x <= 180)) //#9
                                        {
                                          input = SerenaSecurity.ArmUpdate(9, buf);
                                          SerenaGraphics.PrintNumber(input, buf, 4);
                                          buf++;
                                          delay(85);
                                          Beep();
                                        }
                                      }

                                      // #0 Button
                                      if ((x >= 20) && (x <= 80) && (y >= 175) && (y <= 205))
                                      {
                                        input = SerenaSecurity.ArmUpdate(0, buf);
                                        SerenaGraphics.PrintNumber(input, buf, 4);
                                        buf++;
                                        delay(85);
                                        Beep();
                                      }

                                      // OK Button
                                      if ((x >= 190) && (x <= 290) && (y >= 70) && (y <= 110))
                                      {
                                        buf = 0;
                                        SerenaGraphics.Title();
                                        SerenaGraphics.ConfirmPass();
                                        SerenaGraphics.DrawYes();
                                        SerenaGraphics.DrawNo();

                                        int * showpass = SerenaSecurity.GetTempPass(4);
                                        SerenaGraphics.ShowPass(4, showpass);
                                        Beep();

                                        while (true)
                                        {
                                          if (SerenaGraphics.Available())
                                          {
                                            x = SerenaGraphics.GetX();
                                            y = SerenaGraphics.GetY();

                                            // "NO" button
                                            if ((x >= 20) && (x <= 150) && (y >= 180) && (y <= 210))
                                            {
                                              SerenaSecurity.ResetInputs();
                                              Beep();
                                              goto NAPI;
                                            }

                                            // "YES" button
                                            if ((x >= 170) && (x <= 300) && (y >= 180) && (y <= 210))
                                            {
SelectStorageArm:
                                              SerenaGraphics.SelectStorage();

                                              while (true)
                                              {
                                                if (SerenaGraphics.Available())
                                                {
                                                  x = SerenaGraphics.GetX();
                                                  y = SerenaGraphics.GetY();

                                                  if ((x >= 20) && (x <= 300) && (y >= 60) && (y <= 100)) // INTERNAL STORAGE
                                                  {
                                                    SerenaSecurity.SetPass(4);
                                                    SerenaSecurity.ResetInputs();
                                                    goto ARPC;
                                                  }

                                                  if ((x >= 20) && (x <= 300) && (y >= 120) && (y <= 160)) // SD + INTERNAL
                                                  {
                                                    if (SerenaSecurity.CheckSD())
                                                    {
                                                      SerenaSecurity.SetPass(4);
                                                      SerenaSecurity.SetSDPass(4);
                                                      SerenaSecurity.ResetInputs();
                                                      goto ARPC;
                                                    }
                                                    else
                                                    {
                                                      SerenaGraphics.SDNotFound();;
                                                      goto SelectStorageArm;
                                                    }
                                                  }
                                                }
                                              }
ARPC:
                                              SerenaGraphics.PassChangeSuccessful();
                                              LongBeep();
                                              delay(3000);
                                              goto options;
                                            }
                                          }
                                        }
                                      }
                                    }
                                  }
                                }

                                if ((x >= 20) && (x <= 150) && (y >= 180) && (y <= 210))
                                {
                                  Beep();
                                  goto options;
                                }

                              }
                            }
                          }
                          else //wrong password
                          {
                            SerenaSecurity.ResetInputs();
                            SerenaGraphics.WrongPass();
                            Beep();
                            delay(2000);
                            goto APC;
                          }

                        }
                      }
                    }
                  }
                }
              }
            } // "Set Arm-Pass" button


            if ((x >= 170) && (x <= 300) && (y >= 180) && (y <= 210)) // "NEXT" button (goes to options 2)
            {
              Beep();

options2:

              SerenaGraphics.OptionsMenu(2);

              while (true)
              {
                if (SerenaGraphics.Available())
                {
                  x = SerenaGraphics.GetX();
                  y = SerenaGraphics.GetY();

                  if ((x >= 20) && (x <= 150) && (y >= 180) && (y <= 210)) // "BACK" goes to first options page
                  {
                    Beep();
                    goto options;
                  }

                  if ((x >= 20) && (x <= 300) && (y >= 60) && (y <= 100)) // "SET AUTO-ARM PASSWORD" Button
                  {
                    SerenaGraphics.PassInputMessage(5, 'o');
                    Beep();
                    while (true)
                    {
                      if (SerenaGraphics.Available())
                      {
                        x = SerenaGraphics.GetX();
                        y = SerenaGraphics.GetY();
                      }

                      if ((x >= 20) && (x <= 150) && (y >= 165) && (y <= 210)) // "BACK" button, goes to options2
                      {
                        Beep();
                        goto options2;
                      }

                      // Button menu for 6-digit password
                      if ((x >= 170) && (x <= 300) && (y >= 165) && (y <= 210))
                      {
                        Beep();
                        SerenaSecurity.ResetInputs();

FDPC: //Five-Digit Pass Change

                        int buf = 0;

                        SerenaGraphics.DrawKeyboard(); // Draw keyboard

                        while (true)
                        {
                          if (SerenaGraphics.Available())
                          {
                            x = SerenaGraphics.GetX();
                            y = SerenaGraphics.GetY();

                            // "Back" button
                            if ((x >= 190) && (x <= 290) && (y >= 125) && (y <= 165))
                            {
                              Beep();
                              goto options2;
                            }

                            // "Clear" button
                            if ((x >= 190) && (x <= 290) && (y >= 15) && (y <= 55))
                            {
                              buf = 0;
                              SerenaSecurity.ResetInputs();
                              SerenaGraphics.DrawKeyboard();
                              Beep();
                            }

                            // Buttons 1-3
                            if ((y >= 15) && (y <= 55)) // Checking the first row
                            {
                              if ((x >= 20) && (x <= 60)) //#1
                              {
                                input = SerenaSecurity.AutoArmUpdate(1, buf);
                                SerenaGraphics.PrintNumber(input, buf, 5);
                                buf++;
                                delay(85);
                                Beep();
                              }

                              if ((x >= 80) && (x <= 120)) //#2
                              {
                                input = SerenaSecurity.AutoArmUpdate(2, buf);
                                SerenaGraphics.PrintNumber(input, buf, 5);
                                buf++;
                                delay(85);
                                Beep();
                              }

                              if ((x >= 140) && (x <= 180)) //#3
                              {
                                input = SerenaSecurity.AutoArmUpdate(3, buf);
                                SerenaGraphics.PrintNumber(input, buf, 5);
                                buf++;
                                delay(85);
                                Beep();
                              }
                            }

                            // Buttons 4-6
                            if ((y >= 70) && (y <= 110)) // Checking the second row
                            {
                              if ((x >= 20) && (x <= 60)) //#4
                              {
                                input = SerenaSecurity.AutoArmUpdate(4, buf);
                                SerenaGraphics.PrintNumber(input, buf, 5);
                                buf++;
                                delay(85);
                                Beep();
                              }

                              if ((x >= 80) && (x <= 120)) //#5
                              {
                                input = SerenaSecurity.AutoArmUpdate(5, buf);
                                SerenaGraphics.PrintNumber(input, buf, 5);
                                buf++;
                                delay(85);
                                Beep();
                              }

                              if ((x >= 140) && (x <= 180)) //#6
                              {
                                input = SerenaSecurity.AutoArmUpdate(6, buf);
                                SerenaGraphics.PrintNumber(input, buf, 5);
                                buf++;
                                delay(85);
                                Beep();
                              }
                            }

                            // Buttons 7-9
                            if ((y >= 125) && (y <= 165))
                            {
                              if ((x >= 20) && (x <= 60)) //#7
                              {
                                input = SerenaSecurity.AutoArmUpdate(7, buf);
                                SerenaGraphics.PrintNumber(input, buf, 5);
                                buf++;
                                delay(85);
                                Beep();
                              }

                              if ((x >= 80) && (x <= 120)) //#8
                              {
                                input = SerenaSecurity.AutoArmUpdate(8, buf);
                                SerenaGraphics.PrintNumber(input, buf, 5);
                                buf++;
                                delay(85);
                                Beep();
                              }

                              if ((x >= 140) && (x <= 180)) //#9
                              {
                                input = SerenaSecurity.AutoArmUpdate(9, buf);
                                SerenaGraphics.PrintNumber(input, buf, 5);
                                buf++;
                                delay(85);
                                Beep();
                              }
                            }

                            // #0 Button
                            if ((x >= 20) && (x <= 80) && (y >= 175) && (y <= 205))
                            {
                              input = SerenaSecurity.AutoArmUpdate(0, buf);
                              SerenaGraphics.PrintNumber(input, buf, 5);
                              buf++;
                              delay(85);
                              Beep();
                            }


                            if ((x >= 190) && (x <= 290) && (y >= 70) && (y <= 110)) // "OK" button
                            {
                              bool correctpass = true;
                              for (int t = 0; t < 5; t++)
                              {
                                // Checking validity
                                if (*(SerenaSecurity.GetTempPass(5) + t) != *(SerenaSecurity.GetPass(5) + t))
                                {
                                  correctpass = false;
                                }
                              }

                              if (correctpass)
                              {
                                SerenaSecurity.ResetInputs();
                                SerenaGraphics.PassOk();
                                Beep();
                                delay(2000);
                                SerenaGraphics.PassInputMessage(5, 'n');

                                while (true)
                                {
                                  if (SerenaGraphics.Available())
                                  {
                                    x = SerenaGraphics.GetX();
                                    y = SerenaGraphics.GetY();

                                    int buf = 0;

                                    // Next button: prompts user to enter new password
                                    if ((x >= 170) && (x <= 300) && (y >= 165) && (y <= 210))
                                    {

NAAPI: // New auto-arm pass input

                                      SerenaGraphics.DrawKeyboard();
                                      Beep();

                                      while (true)
                                      {
                                        if (SerenaGraphics.Available())
                                        {
                                          x = SerenaGraphics.GetX();
                                          y = SerenaGraphics.GetY();

                                          // "Back" button
                                          if ((x >= 190) && (x <= 290) && (y >= 125) && (y <= 165))
                                          {
                                            Beep();
                                            goto options2;
                                          }

                                          // "Clear" button
                                          if ((x >= 190) && (x <= 290) && (y >= 15) && (y <= 55))
                                          {
                                            buf = 0;
                                            SerenaSecurity.ResetInputs();
                                            SerenaGraphics.DrawKeyboard();
                                            Beep();
                                          }

                                          // Buttons 1-3
                                          if ((y >= 15) && (y <= 55)) // Checking the first row
                                          {
                                            if ((x >= 20) && (x <= 60)) //#1
                                            {
                                              input = SerenaSecurity.AutoArmUpdate(1, buf);
                                              SerenaGraphics.PrintNumber(input, buf, 5);
                                              buf++;
                                              delay(85);
                                              Beep();
                                            }

                                            if ((x >= 80) && (x <= 120)) //#2
                                            {
                                              input = SerenaSecurity.AutoArmUpdate(2, buf);
                                              SerenaGraphics.PrintNumber(input, buf, 5);
                                              buf++;
                                              delay(85);
                                              Beep();
                                            }

                                            if ((x >= 140) && (x <= 180)) //#3
                                            {
                                              input = SerenaSecurity.AutoArmUpdate(3, buf);
                                              SerenaGraphics.PrintNumber(input, buf, 5);
                                              buf++;
                                              delay(85);
                                              Beep();
                                            }
                                          }

                                          // Buttons 4-6
                                          if ((y >= 70) && (y <= 110)) // Checking the second row
                                          {
                                            if ((x >= 20) && (x <= 60)) //#4
                                            {
                                              input = SerenaSecurity.AutoArmUpdate(4, buf);
                                              SerenaGraphics.PrintNumber(input, buf, 5);
                                              buf++;
                                              delay(85);
                                              Beep();
                                            }

                                            if ((x >= 80) && (x <= 120)) //#5
                                            {
                                              input = SerenaSecurity.AutoArmUpdate(5, buf);
                                              SerenaGraphics.PrintNumber(input, buf, 5);
                                              buf++;
                                              delay(85);
                                              Beep();
                                            }

                                            if ((x >= 140) && (x <= 180)) //#6
                                            {
                                              input = SerenaSecurity.AutoArmUpdate(6, buf);
                                              SerenaGraphics.PrintNumber(input, buf, 5);
                                              buf++;
                                              delay(85);
                                              Beep();
                                            }
                                          }

                                          // Buttons 7-9
                                          if ((y >= 125) && (y <= 165))
                                          {
                                            if ((x >= 20) && (x <= 60)) //#7
                                            {
                                              input = SerenaSecurity.AutoArmUpdate(7, buf);
                                              SerenaGraphics.PrintNumber(input, buf, 5);
                                              buf++;
                                              delay(85);
                                              Beep();
                                            }

                                            if ((x >= 80) && (x <= 120)) //#8
                                            {
                                              input = SerenaSecurity.AutoArmUpdate(8, buf);
                                              SerenaGraphics.PrintNumber(input, buf, 5);
                                              buf++;
                                              delay(85);
                                              Beep();
                                            }

                                            if ((x >= 140) && (x <= 180)) //#9
                                            {
                                              input = SerenaSecurity.AutoArmUpdate(9, buf);
                                              SerenaGraphics.PrintNumber(input, buf, 5);
                                              buf++;
                                              delay(85);
                                              Beep();
                                            }
                                          }

                                          // #0 Button
                                          if ((x >= 20) && (x <= 80) && (y >= 175) && (y <= 205))
                                          {
                                            input = SerenaSecurity.AutoArmUpdate(0, buf);
                                            SerenaGraphics.PrintNumber(input, buf, 5);
                                            buf++;
                                            delay(85);
                                            Beep();
                                          }

                                          // "OK" button for new pass
                                          if ((x >= 190) && (x <= 290) && (y >= 70) && (y <= 110))
                                          {
                                            buf = 0;
                                            SerenaGraphics.Title();
                                            SerenaGraphics.ConfirmPass();
                                            SerenaGraphics.DrawYes();
                                            SerenaGraphics.DrawNo();

                                            int * showpass = SerenaSecurity.GetTempPass(5);
                                            SerenaGraphics.ShowPass(5, showpass);
                                            Beep();

                                            while (true)
                                            {
                                              if (SerenaGraphics.Available())
                                              {
                                                x = SerenaGraphics.GetX();
                                                y = SerenaGraphics.GetY();

                                                // "NO" button
                                                if ((x >= 20) && (x <= 150) && (y >= 180) && (y <= 210))
                                                {
                                                  SerenaSecurity.ResetInputs();
                                                  Beep();
                                                  goto NAAPI;
                                                }

                                                // "YES" button
                                                if ((x >= 170) && (x <= 300) && (y >= 180) && (y <= 210))
                                                {
SelectStorageAuto:
                                                  SerenaGraphics.SelectStorage();

                                                  while (true)
                                                  {
                                                    if (SerenaGraphics.Available())
                                                    {
                                                      x = SerenaGraphics.GetX();
                                                      y = SerenaGraphics.GetY();

                                                      if ((x >= 20) && (x <= 300) && (y >= 60) && (y <= 100)) // INTERNAL STORAGE
                                                      {
                                                        SerenaSecurity.SetPass(5);
                                                        SerenaSecurity.ResetInputs();
                                                        goto AUPS;
                                                      }

                                                      if ((x >= 20) && (x <= 300) && (y >= 120) && (y <= 160)) // SD + INTERNAL
                                                      {
                                                        if (SerenaSecurity.CheckSD())
                                                        {
                                                          SerenaSecurity.SetPass(5);
                                                          SerenaSecurity.SetSDPass(5);
                                                          SerenaSecurity.ResetInputs();
                                                          goto AUPS;
                                                        }
                                                        else
                                                        {
                                                          SerenaGraphics.SDNotFound();
                                                          goto SelectStorageAuto;
                                                        }
                                                      }
                                                    }
                                                  }
AUPS:
                                                  SerenaGraphics.PassChangeSuccessful();
                                                  LongBeep();
                                                  delay(3000);
                                                  goto options2;
                                                }
                                              }
                                            }
                                          }
                                        }
                                      }
                                    } // "NEXT" button end

                                    // "BACK" buton
                                    if ((x >= 20) && (x <= 150) && (y >= 165) && (y <= 210))
                                    {
                                      Beep();
                                      goto options2;
                                    }
                                  }
                                }
                              }
                              else // Wrong password
                              {
                                SerenaSecurity.ResetInputs();
                                SerenaGraphics.WrongPass();
                                Beep();
                                delay(2000);
                                goto FDPC;
                              }
                            }
                          }
                        }
                      }
                    }
                  } // "SET AUTO-ARM PASSWORD" Button end


                  if ((x >= 20) && (x <= 300) && (y >= 120) && (y <= 160)) // "SET AUTO-ARM TIME" Button
                  {
                    Beep();
                    SerenaGraphics.AutoTimeScreen(SerenaSecurity.GetAutoTime('o'), 1);


                    while (true)
                    {
                      if (SerenaGraphics.Available())
                      {
                        x = SerenaGraphics.GetX();
                        y = SerenaGraphics.GetY();

                        if ((x >= 20) && (x <= 150) && (y >= 180) && (y <= 210)) // "BACK" Button
                        {
                          Beep();
                          goto options2;
                        }//BACK BUTTON END

                        if ((x >= 170) && (x <= 300) && (y >= 180) && (y <= 210))// "NEXT" Button, will prompt user to enter new auto-arm time
                        {

AATC: // Auto-Arm Time Change

                          Beep();
                          int buf = 0;
                          SerenaGraphics.DrawKeyboard();

                          while (true)
                          {
                            if (SerenaGraphics.Available())
                            {
                              x = SerenaGraphics.GetX();
                              y = SerenaGraphics.GetY();

                              // "Back" button
                              if ((x >= 190) && (x <= 290) && (y >= 125) && (y <= 165))
                              {
                                Beep();
                                goto options2;
                              }

                              // "Clear" button
                              if ((x >= 190) && (x <= 290) && (y >= 15) && (y <= 55))
                              {
                                buf = 0;
                                SerenaSecurity.ResetInputs();
                                SerenaGraphics.DrawKeyboard();
                                Beep();
                              }

                              // Buttons 1-3
                              if ((y >= 15) && (y <= 55)) // Checking the first row
                              {
                                if ((x >= 20) && (x <= 60)) //#1
                                {
                                  input = SerenaSecurity.AutoTimeUpdate(1, buf);
                                  SerenaGraphics.PrintAutoArmTime(input, buf);
                                  buf++;
                                  delay(85);
                                  Beep();
                                }

                                if ((x >= 80) && (x <= 120)) //#2
                                {
                                  input = SerenaSecurity.AutoTimeUpdate(2, buf);
                                  SerenaGraphics.PrintAutoArmTime(input, buf);
                                  buf++;
                                  delay(85);
                                  Beep();
                                }

                                if ((x >= 140) && (x <= 180)) //#3
                                {
                                  input = SerenaSecurity.AutoTimeUpdate(3, buf);
                                  SerenaGraphics.PrintAutoArmTime(input, buf);
                                  buf++;
                                  delay(85);
                                  Beep();
                                }
                              }

                              // Buttons 4-6
                              if ((y >= 70) && (y <= 110)) // Checking the second row
                              {
                                if ((x >= 20) && (x <= 60)) //#4
                                {
                                  input = SerenaSecurity.AutoTimeUpdate(4, buf);
                                  SerenaGraphics.PrintAutoArmTime(input, buf);
                                  buf++;
                                  delay(85);
                                  Beep();
                                }

                                if ((x >= 80) && (x <= 120)) //#5
                                {
                                  input = SerenaSecurity.AutoTimeUpdate(5, buf);
                                  SerenaGraphics.PrintAutoArmTime(input, buf);
                                  buf++;
                                  delay(85);
                                  Beep();
                                }

                                if ((x >= 140) && (x <= 180)) //#6
                                {
                                  input = SerenaSecurity.AutoTimeUpdate(6, buf);
                                  SerenaGraphics.PrintAutoArmTime(input, buf);
                                  buf++;
                                  delay(85);
                                  Beep();
                                }
                              }

                              // Buttons 7-9
                              if ((y >= 125) && (y <= 165))
                              {
                                if ((x >= 20) && (x <= 60)) //#7
                                {
                                  input = SerenaSecurity.AutoTimeUpdate(7, buf);
                                  SerenaGraphics.PrintAutoArmTime(input, buf);
                                  buf++;
                                  delay(85);
                                  Beep();
                                }

                                if ((x >= 80) && (x <= 120)) //#8
                                {
                                  input = SerenaSecurity.AutoTimeUpdate(8, buf);
                                  SerenaGraphics.PrintAutoArmTime(input, buf);
                                  buf++;
                                  delay(85);
                                  Beep();
                                }

                                if ((x >= 140) && (x <= 180)) //#9
                                {
                                  input = SerenaSecurity.AutoTimeUpdate(9, buf);
                                  SerenaGraphics.PrintAutoArmTime(input, buf);
                                  buf++;
                                  delay(85);
                                  Beep();
                                }
                              }

                              // #0 Button
                              if ((x >= 20) && (x <= 80) && (y >= 175) && (y <= 205))
                              {
                                input = SerenaSecurity.AutoTimeUpdate(0, buf);
                                SerenaGraphics.PrintAutoArmTime(input, buf);
                                buf++;
                                delay(85);
                                Beep();
                              }


                              if ((x >= 190) && (x <= 290) && (y >= 70) && (y <= 110)) // "OK" Button, will prompt user to change the auto-arm time
                              {
                                Beep();
                                SerenaGraphics.AutoTimeScreen(SerenaSecurity.GetAutoTime('n'), 2);

                                while (true)
                                {
                                  if (SerenaGraphics.Available())
                                  {
                                    x = SerenaGraphics.GetX();
                                    y = SerenaGraphics.GetY();

                                    if ((x >= 20) && (x <= 150) && (y >= 180) && (y <= 210)) // "NO" Button
                                    {
                                      SerenaSecurity.ResetInputs();
                                      goto AATC;
                                    }

                                    if ((x >= 170) && (x <= 300) && (y >= 180) && (y <= 210)) // "YES" Button
                                    {
                                      LongBeep();
                                      SerenaSecurity.SetAutoTime();
                                      SerenaGraphics.AutoTimeChangeSuccess();
                                      delay(3000);
                                      goto options2;
                                    }
                                  }
                                }
                              }
                            }
                          }
                        } // "NEXT" Button end
                      }
                    }
                  } // "SET AUTO-ARM TIME" Button end


                  if ((x >= 170) && (x <= 300) && (y >= 180) && (y <= 210)) // Goes to the third options page
                  {
                    Beep();

options3:

                    SerenaGraphics.OptionsMenu(3);

                    while (true)
                    {
                      if (SerenaGraphics.Available())
                      {
                        x = SerenaGraphics.GetX();
                        y = SerenaGraphics.GetY();

                        if ((x >= 20) && (x <= 150) && (y >= 180) && (y <= 210)) // Go to the previous page
                        {
                          Beep();
                          goto options2;
                        }


                        if ((x >= 20) && (x <= 300) && (y >= 60) && (y <= 100)) // TIME SETTINGS
                        {
                          Beep();
                          SerenaGraphics.PrintTime();
                          while (true)
                          {
                            if (SerenaGraphics.Available())
                            {
                              x = SerenaGraphics.GetX();
                              y = SerenaGraphics.GetY();

                          
                              if ((x >= 20) && (x <= 150) && (y >= 180) && (y <= 210)) // Go to options 3
                              {
                                Beep();
                                goto options3;
                              }

                              if ((x >= 54) && (x <= 70) && (y >= 23) && (y <= 65)) // Increase hours
                              {
                                // Increase the corresponding number
                                // Print time again
                                SerenaGraphics.AdjustTime('h', 1);
                                SerenaGraphics.HoursCorrection(false);
                                SerenaGraphics.PrintTempTime();
                              }

                              if ((x >= 54) && (x <= 70) && (y >= 125) && (y <= 167)) // Decrease hours
                              {
                                SerenaGraphics.AdjustTime('h', 0);
                                SerenaGraphics.HoursCorrection(true);
                                SerenaGraphics.PrintTempTime();
                              }

                              if ((x >= 150) && (x <= 166) && (y >= 23) && (y <= 65)) // Increase minutes
                              {
                                SerenaGraphics.AdjustTime('m', 1);
                                SerenaGraphics.MinutesCorrection(false);
                                SerenaGraphics.PrintTempTime();
                              }

                              if ((x >= 150) && (x <= 166) && (y >= 125) && (y <= 167)) // Decrease minutes
                              {
                                SerenaGraphics.AdjustTime('m', 0);
                                SerenaGraphics.MinutesCorrection(true);
                                SerenaGraphics.PrintTempTime();
                              }

                              if ((x >= 246) && (x <= 262) && (y >= 23) && (y <= 65)) // Increase seconds
                              {
                                SerenaGraphics.AdjustTime('s', 1);
                                SerenaGraphics.SecondsCorrection(false);
                                SerenaGraphics.PrintTempTime();
                              }

                              if ((x >= 246) && (x <= 262) && (y >= 125) && (y <= 167)) // Decrease seconds
                              {
                                SerenaGraphics.AdjustTime('s', 0);
                                SerenaGraphics.SecondsCorrection(true);
                                SerenaGraphics.PrintTempTime();
                              }

                              if ((x >= 170) && (x <= 300) && (y >= 180) && (y <= 210)) // SET TIME
                              {
                                SerenaGraphics.SetTime();
                                SerenaGraphics.AdjustTimeSuccess();
                                LongBeep();
                                delay(1500);
                                goto options3;
                              }

                            }
                          }
                        } // TIME SETTINGS END


                        if ((x >= 20) && (x <= 300) && (y >= 120) && (y <= 160)) // SET DATE
                        {
                          Beep();
                          SerenaGraphics.PrintDate();

                          while (true)
                          {
                            if (SerenaGraphics.Available())
                            {
                              x = SerenaGraphics.GetX();
                              y = SerenaGraphics.GetY();

                              if ((x >= 20) && (x <= 150) && (y >= 180) && (y <= 210)) // Go to options 3
                              {
                                Beep();
                                goto options3;
                              }

                              if ((x >= 54) && (x <= 70) && (y >= 23) && (y <= 65)) // Increase hours
                              {
                                // Increase the corresponding number
                                SerenaGraphics.AdjustDate('d', 1);
                                SerenaGraphics.DaysCorrection(false);
                                SerenaGraphics.PrintTempDate();
                              }

                              if ((x >= 54) && (x <= 70) && (y >= 125) && (y <= 167)) // Decrease hours
                              {
                                SerenaGraphics.AdjustDate('d', 0);
                                SerenaGraphics.DaysCorrection(true);
                                SerenaGraphics.PrintTempDate();
                              }

                              if ((x >= 150) && (x <= 166) && (y >= 23) && (y <= 65)) // Increase minutes
                              {
                                SerenaGraphics.AdjustDate('m', 1);
                                SerenaGraphics.MonthsCorrection(false);
                                SerenaGraphics.PrintTempDate();
                              }

                              if ((x >= 150) && (x <= 166) && (y >= 125) && (y <= 167)) // Decrease minutes
                              {
                                SerenaGraphics.AdjustDate('m', 0);
                                SerenaGraphics.MonthsCorrection(true);
                                SerenaGraphics.PrintTempDate();
                              }

                              if ((x >= 246) && (x <= 262) && (y >= 23) && (y <= 65)) // Increase seconds
                              {
                                SerenaGraphics.AdjustDate('y', 1);
                                SerenaGraphics.YearsCorrection();
                                SerenaGraphics.PrintTempDate();
                              }

                              if ((x >= 246) && (x <= 262) && (y >= 125) && (y <= 167)) // Decrease seconds
                              {
                                SerenaGraphics.AdjustDate('y', 0);
                                SerenaGraphics.YearsCorrection();
                                SerenaGraphics.PrintTempDate();
                              }

                              if ((x >= 170) && (x <= 300) && (y >= 180) && (y <= 210)) // SET TIME
                              {
                                SerenaGraphics.SetDate();
                                SerenaGraphics.AdjustDateSuccess();
                                LongBeep();
                                delay(1500);
                                goto options3;
                              }
                            }
                          }
                        } // SET DATE END

                        if ((x >= 170) && (x <= 300) && (y >= 180) && (y <= 210)) // Goes to fourth options page
                        {
                          Beep();

options4:

                          SerenaGraphics.OptionsMenuEnd();

                          while (true)
                          {
                            if (SerenaGraphics.Available())
                            {
                              x = SerenaGraphics.GetX();
                              y = SerenaGraphics.GetY();

                              if ((x >= 20) && (x <= 150) && (y >= 180) && (y <= 210)) // Go to the previous page
                              {
                                Beep();
                                goto options3;
                              }

                              if ((x >= 20) && (x <= 300) && (y >= 60) && (y <= 100)) // "OTHER SETTINGS"
                              {
                                Beep();

                                SerenaGraphics.OtherSettings(autoload, askarmpass);

                                while (true)
                                {
                                  if (SerenaGraphics.Available())
                                  {
                                    x = SerenaGraphics.GetX();
                                    y = SerenaGraphics.GetY();


                                    if ((x >= 20) && (x <= 150) && (y >= 180) && (y <= 210)) // Go to options 4
                                    {
                                      Beep();
                                      goto options4;
                                    }

                                    if ((x >= 5) && (x <= 35) && (y >= 47) && (y <= 100)) // AUTO PASSWORD LOADING ON
                                    {
                                      if (SerenaSecurity.CheckSD())
                                      {
                                        autoload = true;
                                        SerenaGraphics.AutoLoadTrue(autoload);
                                        SerenaSecurity.SaveAutoLoad(autoload);
                                        Beep();
                                      }
                                      else
                                      {
                                        SerenaGraphics.SDNotFound();
                                        LongBeep();
                                        goto options4;
                                      }
                                    }

                                    if ((x >= 125) && (x <= 155) && (y >= 47) && (y <= 100)) // AUTO PASSWORD LOADING OFF
                                    {
                                      if (SerenaSecurity.CheckSD())
                                      {
                                        autoload = false;
                                        SerenaGraphics.AutoLoadTrue(autoload);
                                        SerenaSecurity.SaveAutoLoad(autoload);
                                        Beep();
                                      }
                                      else
                                      {
                                        SerenaGraphics.SDNotFound();
                                        LongBeep();
                                        goto options4;
                                      }

                                    }

                                    if ((x >= 5) && (x <= 35) && (y >= 127) && (y <= 172)) // ASK ARM PASS ON
                                    {
                                      askarmpass = true;
                                      SerenaGraphics.AskArmPass(askarmpass);
                                      SerenaSecurity.SaveAskArm(askarmpass);
                                      Beep();
                                    }

                                    if ((x >= 125) && (x <= 155) && (y >= 127) && (y <= 172)) // ASK ARM PASS OFF
                                    {
                                      askarmpass = false;
                                      SerenaGraphics.AskArmPass(askarmpass);
                                      SerenaSecurity.SaveAskArm(askarmpass);
                                      Beep();
                                    }
                                  }
                                }
                              } // "BOOT SETTINGS" END
                            }
                          }
                        }
                      }
                    }
                  } // "Options Third Page" end

                }
              }
            } // "Options Second Page" end
          }
        }
      } // "Options" button end


      if ((x >= 20) && (x <= 300) && (y >= 140) && (y <= 190)) // "ARM" Button
      {
        if (askarmpass)
        {
          Beep();
          SerenaGraphics.PassInputMessage(4, 'o');

          while (true)
          {
            if (SerenaGraphics.Available())
            {
              x = SerenaGraphics.GetX();
              y = SerenaGraphics.GetY();

              if ((x >= 20) && (x <= 150) && (y >= 180) && (y <= 210)) // Back to main menu
              {
                Beep();
                goto mainmenu;
              }

              if ((x >= 170) && (x <= 300) && (y >= 165) && (y <= 210)) // Goes to keyboard
              {
                Beep();
                SerenaSecurity.ResetInputs();

ArmPassInput:

                int buf = 0;
                SerenaGraphics.DrawKeyboard();

                while (true)
                {
                  if (SerenaGraphics.Available())
                  {
                    x = SerenaGraphics.GetX();
                    y = SerenaGraphics.GetY();

                    // "Back" button
                    if ((x >= 190) && (x <= 290) && (y >= 125) && (y <= 165))
                    {
                      Beep();
                      goto mainmenu;
                    }

                    // "Clear" button
                    if ((x >= 190) && (x <= 290) && (y >= 15) && (y <= 55))
                    {
                      buf = 0;
                      SerenaSecurity.ResetInputs();
                      SerenaGraphics.DrawKeyboard();
                      Beep();
                    }

                    // Buttons 1-3
                    if ((y >= 15) && (y <= 55)) // Checking the first row
                    {
                      if ((x >= 20) && (x <= 60)) //#1
                      {
                        input = SerenaSecurity.ArmUpdate(1, buf);
                        SerenaGraphics.PrintNumber(input, buf, 4);
                        buf++;
                        delay(85);
                        Beep();
                      }

                      if ((x >= 80) && (x <= 120)) //#2
                      {
                        input = SerenaSecurity.ArmUpdate(2, buf);
                        SerenaGraphics.PrintNumber(input, buf, 4);
                        buf++;
                        delay(85);
                        Beep();
                      }

                      if ((x >= 140) && (x <= 180)) //#3
                      {
                        input = SerenaSecurity.ArmUpdate(3, buf);
                        SerenaGraphics.PrintNumber(input, buf, 4);
                        buf++;
                        delay(85);
                        Beep();
                      }
                    }

                    // Buttons 4-6
                    if ((y >= 70) && (y <= 110)) // Checking the second row
                    {
                      if ((x >= 20) && (x <= 60)) //#4
                      {
                        input = SerenaSecurity.ArmUpdate(4, buf);
                        SerenaGraphics.PrintNumber(input, buf, 4);
                        buf++;
                        delay(85);
                        Beep();
                      }

                      if ((x >= 80) && (x <= 120)) //#5
                      {
                        input = SerenaSecurity.ArmUpdate(5, buf);
                        SerenaGraphics.PrintNumber(input, buf, 4);
                        buf++;
                        delay(85);
                        Beep();
                      }

                      if ((x >= 140) && (x <= 180)) //#6
                      {
                        input = SerenaSecurity.ArmUpdate(6, buf);
                        SerenaGraphics.PrintNumber(input, buf, 4);
                        buf++;
                        delay(85);
                        Beep();
                      }
                    }

                    // Buttons 7-9
                    if ((y >= 125) && (y <= 165))
                    {
                      if ((x >= 20) && (x <= 60)) //#7
                      {
                        input = SerenaSecurity.ArmUpdate(7, buf);
                        SerenaGraphics.PrintNumber(input, buf, 4);
                        buf++;
                        delay(85);
                        Beep();
                      }

                      if ((x >= 80) && (x <= 120)) //#8
                      {
                        input = SerenaSecurity.ArmUpdate(8, buf);
                        SerenaGraphics.PrintNumber(input, buf, 4);
                        buf++;
                        delay(85);
                        Beep();
                      }

                      if ((x >= 140) && (x <= 180)) //#9
                      {
                        input = SerenaSecurity.ArmUpdate(9, buf);
                        SerenaGraphics.PrintNumber(input, buf, 4);
                        buf++;
                        delay(85);
                        Beep();
                      }
                    }

                    // #0 Button
                    if ((x >= 20) && (x <= 80) && (y >= 175) && (y <= 205))
                    {
                      input = SerenaSecurity.ArmUpdate(0, buf);
                      SerenaGraphics.PrintNumber(input, buf, 4);
                      buf++;
                      delay(85);
                      Beep();
                    }

                    if ((x >= 190) && (x <= 290) && (y >= 70) && (y <= 110)) // "OK" Button, checks if the 4-digit pass is corrent and arms the system if it's true
                    {
                      bool correctpass = true;

                      for (int t = 0; t < 4; t++)
                      {
                        // Checking validity
                        if (*(SerenaSecurity.GetTempPass(4) + t) != *(SerenaSecurity.GetPass(4) + t))
                        {
                          correctpass = false;
                        }
                      }

                      if (correctpass)
                      {
                        goto Armed;
                      }
                    }
                  }
                }
              }
            }
          }
        }

        else
        {
          goto Armed;
        }

Armed:
        SerenaSecurity.ResetInputs();
        SerenaGraphics.SystemArmed();
        alarmarmed = true;
        BTSerial.write(alarmarmed);

        LongBeep();
        delay(2000);

DisarmMenu:

        SerenaGraphics.DrawTimeDate();
        SerenaGraphics.DrawDisarm();

        while (true)
        {
          SerenaGraphics.UpdateTime(); // TODO Complete Seasonal graphics

          if (SerenaGraphics.Available())
          {
            x = SerenaGraphics.GetX();
            y = SerenaGraphics.GetY();


            if ((x >= 20) && (x <= 300) && (y >= 180) && (y <= 220))
            {
              Beep();

DisarmInput:

              int buf = 0;
              SerenaGraphics.DrawKeyboard();

              while (true)
              {
                if (SerenaGraphics.Available())
                {
                  x = SerenaGraphics.GetX();
                  y = SerenaGraphics.GetY();

                  // "Back" button
                  if ((x >= 190) && (x <= 290) && (y >= 125) && (y <= 165))
                  {
                    Beep();
                    goto DisarmMenu;
                  }

                  // "Clear" button
                  if ((x >= 190) && (x <= 290) && (y >= 15) && (y <= 55))
                  {
                    buf = 0;
                    SerenaSecurity.ResetInputs();
                    SerenaGraphics.DrawKeyboard();
                    Beep();
                  }

                  // Buttons 1-3
                  if ((y >= 15) && (y <= 55)) // Checking the first row
                  {
                    if ((x >= 20) && (x <= 60)) //#1
                    {
                      input = SerenaSecurity.AutoArmUpdate(1, buf);
                      SerenaGraphics.PrintNumber(input, buf, 5);
                      buf++;
                      delay(85);
                      Beep();
                    }

                    if ((x >= 80) && (x <= 120)) //#2
                    {
                      input = SerenaSecurity.AutoArmUpdate(2, buf);
                      SerenaGraphics.PrintNumber(input, buf, 5);
                      buf++;
                      delay(85);
                      Beep();
                    }

                    if ((x >= 140) && (x <= 180)) //#3
                    {
                      input = SerenaSecurity.AutoArmUpdate(3, buf);
                      SerenaGraphics.PrintNumber(input, buf, 5);
                      buf++;
                      delay(85);
                      Beep();
                    }
                  }

                  // Buttons 4-6
                  if ((y >= 70) && (y <= 110)) // Checking the second row
                  {
                    if ((x >= 20) && (x <= 60)) //#4
                    {
                      input = SerenaSecurity.AutoArmUpdate(4, buf);
                      SerenaGraphics.PrintNumber(input, buf, 5);
                      buf++;
                      delay(85);
                      Beep();
                    }

                    if ((x >= 80) && (x <= 120)) //#5
                    {
                      input = SerenaSecurity.AutoArmUpdate(5, buf);
                      SerenaGraphics.PrintNumber(input, buf, 5);
                      buf++;
                      delay(85);
                      Beep();
                    }

                    if ((x >= 140) && (x <= 180)) //#6
                    {
                      input = SerenaSecurity.AutoArmUpdate(6, buf);
                      SerenaGraphics.PrintNumber(input, buf, 5);
                      buf++;
                      delay(85);
                      Beep();
                    }
                  }

                  // Buttons 7-9
                  if ((y >= 125) && (y <= 165))
                  {
                    if ((x >= 20) && (x <= 60)) //#7
                    {
                      input = SerenaSecurity.AutoArmUpdate(7, buf);
                      SerenaGraphics.PrintNumber(input, buf, 5);
                      buf++;
                      delay(85);
                      Beep();
                    }

                    if ((x >= 80) && (x <= 120)) //#8
                    {
                      input = SerenaSecurity.AutoArmUpdate(8, buf);
                      SerenaGraphics.PrintNumber(input, buf, 5);
                      buf++;
                      delay(85);
                      Beep();
                    }

                    if ((x >= 140) && (x <= 180)) //#9
                    {
                      input = SerenaSecurity.AutoArmUpdate(9, buf);
                      SerenaGraphics.PrintNumber(input, buf, 5);
                      buf++;
                      delay(85);
                      Beep();
                    }
                  }

                  // #0 Button
                  if ((x >= 20) && (x <= 80) && (y >= 175) && (y <= 205))
                  {
                    input = SerenaSecurity.AutoArmUpdate(0, buf);
                    SerenaGraphics.PrintNumber(input, buf, 5);
                    buf++;
                    delay(85);
                    Beep();
                  }


                  if ((x >= 190) && (x <= 290) && (y >= 70) && (y <= 110)) // "OK" button
                  {
                    bool correctpass = true;
                    for (int t = 0; t < 5; t++)
                    {
                      // Checking validity
                      if (*(SerenaSecurity.GetTempPass(5) + t) != *(SerenaSecurity.GetPass(5) + t))
                      {
                        correctpass = false;
                      }
                    }


                    if (correctpass)
                    {
                      SerenaSecurity.ResetInputs();
                      alarmarmed = false;
                      BTSerial.write(alarmarmed);
                      SerenaGraphics.SystemDisarmed();
                      LongBeep();
                      delay(1000);


                      int autoarmtime = SerenaSecurity.GetAutoTime('o');

                      SerenaGraphics.AlarmTitle();
                      while (autoarmtime >= 0)
                      {

SystemDisarmed:

                        if (autoarmtime > 0)
                        {
                          SerenaGraphics.AutoCountDown(autoarmtime);
                          delay(1000);
                        }

                        if (autoarmtime == 0)
                        {
                          autoarmtime = SerenaSecurity.GetAutoTime('o');
                          SerenaGraphics.SystemAutoArmed();
                          alarmarmed = true;
                          BTSerial.write(alarmarmed);
                          LongBeep();
                          delay(3000);
                          goto DisarmMenu;
                        }

                        if (digitalRead(pushbutton) == LOW)
                        {
                          delay(10);
                          if (digitalRead(pushbutton) == LOW)
                          {
                            SerenaGraphics.Title();
                            SerenaGraphics.PassInputMessage(6, 'o');
                            SerenaGraphics.DrawBackBig();
                            SerenaGraphics.DrawNextBig();

                            goto FullDisarm;
                          }
                        }
                        autoarmtime--;
                      }

FullDisarm:

                      while (true)
                      {
                        if (SerenaGraphics.Available())
                        {
                          x = SerenaGraphics.GetX();
                          y = SerenaGraphics.GetY();

                          if ((x >= 20) && (x <= 150) && (y >= 180) && (y <= 210)) // "BACK"
                          {
                            SerenaGraphics.ClearScreen();
                            goto SystemDisarmed;
                          }

                          if ((x >= 170) && (x <= 300) && (y >= 165) && (y <= 210))// Goes to 6-Digit Pass Input
                          {
                            SerenaSecurity.ResetInputs();
                            Beep();

FullDisarmInput:

                            int buf = 0;
                            SerenaGraphics.DrawKeyboard();

                            while (true)
                            {
                              if (SerenaGraphics.Available())
                              {
                                x = SerenaGraphics.GetX();
                                y = SerenaGraphics.GetY();

                                // "Back" button
                                if ((x >= 190) && (x <= 290) && (y >= 125) && (y <= 165))
                                {
                                  Beep();
                                  SerenaGraphics.ClearScreen();
                                  goto SystemDisarmed;
                                }

                                // "Clear" button
                                if ((x >= 190) && (x <= 290) && (y >= 15) && (y <= 55))
                                {
                                  buf = 0;
                                  SerenaSecurity.ResetInputs();
                                  SerenaGraphics.DrawKeyboard();
                                  Beep();
                                }

                                // Buttons 1-3
                                if ((y >= 15) && (y <= 55)) // Checking the first row
                                {
                                  if ((x >= 20) && (x <= 60)) //#1
                                  {
                                    input = SerenaSecurity.ManualUpdate(1, buf);
                                    SerenaGraphics.PrintNumber(input, buf, 6);
                                    buf++;
                                    delay(85);
                                    Beep();
                                  }

                                  if ((x >= 80) && (x <= 120)) //#2
                                  {
                                    input = SerenaSecurity.ManualUpdate(2, buf);
                                    SerenaGraphics.PrintNumber(input, buf, 6);
                                    buf++;
                                    delay(85);
                                    Beep();
                                  }

                                  if ((x >= 140) && (x <= 180)) //#3
                                  {
                                    input = SerenaSecurity.ManualUpdate(3, buf);
                                    SerenaGraphics.PrintNumber(input, buf, 6);
                                    buf++;
                                    delay(85);
                                    Beep();
                                  }
                                }

                                // Buttons 4-6
                                if ((y >= 70) && (y <= 110)) // Checking the second row
                                {
                                  if ((x >= 20) && (x <= 60)) //#4
                                  {
                                    input = SerenaSecurity.ManualUpdate(4, buf);
                                    SerenaGraphics.PrintNumber(input, buf, 6);
                                    buf++;
                                    delay(85);
                                    Beep();
                                  }

                                  if ((x >= 80) && (x <= 120)) //#5
                                  {
                                    input = SerenaSecurity.ManualUpdate(5, buf);
                                    SerenaGraphics.PrintNumber(input, buf, 6);
                                    buf++;
                                    delay(85);
                                    Beep();
                                  }

                                  if ((x >= 140) && (x <= 180)) //#6
                                  {
                                    input = SerenaSecurity.ManualUpdate(6, buf);
                                    SerenaGraphics.PrintNumber(input, buf, 6);
                                    buf++;
                                    delay(85);
                                    Beep();
                                  }
                                }

                                // Buttons 7-9
                                if ((y >= 125) && (y <= 165))
                                {
                                  if ((x >= 20) && (x <= 60)) //#7
                                  {
                                    input = SerenaSecurity.ManualUpdate(7, buf);
                                    SerenaGraphics.PrintNumber(input, buf, 6);
                                    buf++;
                                    delay(85);
                                    Beep();
                                  }

                                  if ((x >= 80) && (x <= 120)) //#8
                                  {
                                    input = SerenaSecurity.ManualUpdate(8, buf);
                                    SerenaGraphics.PrintNumber(input, buf, 6);
                                    buf++;
                                    delay(85);
                                    Beep();
                                  }

                                  if ((x >= 140) && (x <= 180)) //#9
                                  {
                                    input = SerenaSecurity.ManualUpdate(9, buf);
                                    SerenaGraphics.PrintNumber(input, buf, 6);
                                    buf++;
                                    delay(85);
                                    Beep();
                                  }
                                }

                                // #0 Button
                                if ((x >= 20) && (x <= 80) && (y >= 175) && (y <= 205))
                                {
                                  input = SerenaSecurity.ManualUpdate(0, buf);
                                  SerenaGraphics.PrintNumber(input, buf, 6);
                                  buf++;
                                  delay(85);
                                  Beep();
                                }

                                // "OK" button
                                if ((x >= 190) && (x <= 290) && (y >= 70) && (y <= 110))
                                {
                                  bool correctpass = true;
                                  for (int t = 0; t < 6; t++)
                                  {
                                    // Checking validity
                                    if (*(SerenaSecurity.GetTempPass(6) + t) != *(SerenaSecurity.GetPass(6) + t))
                                    {
                                      correctpass = false;
                                    }
                                  }

                                  if (correctpass)
                                  {
                                    SerenaSecurity.ResetInputs();
                                    autoarmtime = SerenaSecurity.GetAutoTime('o');
                                    SerenaGraphics.SystemFullyDisarmed();
                                    LongBeep();
                                    delay(2000);
                                    goto mainmenu;
                                  }
                                  else
                                  {
                                    SerenaSecurity.ResetInputs();
                                    SerenaGraphics.WrongPass();
                                    Beep();
                                    delay(1500);
                                    goto FullDisarmInput;
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                    else
                    {
                      SerenaSecurity.ResetInputs();
                      LongBeep();
                      goto DisarmInput;
                    }
                  }
                }
              }
            }
          }
        }

      } // ARM BUTTON END
    }
  }
} // void loop() end


void OKBeep()
{
  digitalWrite(buzzerpin, HIGH);
  delay(100);
  digitalWrite(buzzerpin, LOW);
}

void Beep()
{
  digitalWrite(buzzerpin, HIGH);
  delay(50);
  digitalWrite(buzzerpin, LOW);
}

void LongBeep()
{
  digitalWrite(buzzerpin, HIGH);
  delay(500);
  digitalWrite(buzzerpin, LOW);
}