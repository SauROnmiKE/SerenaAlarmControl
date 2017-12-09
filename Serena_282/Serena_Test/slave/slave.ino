#include <SoftwareSerial.h>
SoftwareSerial BTserial(10, 11);

int ledpin = 8;
const byte btstate = 13;

bool state = false;

void setup() {
  // initialize digital pin 8 as an output.

  Serial.begin(9600);
  BTserial.begin(38400);

  pinMode(ledpin, OUTPUT);
  pinMode(btstate, INPUT);

}

void loop() 
{

  if (digitalRead(btstate) == LOW)
  {
	digitalWrite(ledpin, HIGH);
	state = false;
  }

  if (BTserial.available() > 0)
  { // Checks whether data is comming from the serial port
    state = BTserial.read(); // Reads the data from the serial port

    // Controlling the LED
    if (state == true)
    {
      digitalWrite(ledpin, HIGH); // LED ON
      state = false;
    }
    else if (state == false)
    {
      digitalWrite(ledpin, LOW); // LED ON
      state = false;
    }
  }

}
