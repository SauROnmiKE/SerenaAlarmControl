#include <SoftwareSerial.h>
SoftwareSerial BTserial(10, 11); // TX - RX

int relayPin = 8;
const byte btstate = 13;

bool state = false;

void setup() {
  // initialize digital pin 8 as an output.

  Serial.begin(9600);
  BTserial.begin(38400);

  pinMode(relayPin, OUTPUT);
  pinMode(btstate, INPUT);

}

void loop()
{
  // Timer for checking lost connection (solves instant arming when the slave is powered and not connected to master yet - 3 or 5 sec)
  if (digitalRead(btstate) == LOW)
  {
    digitalWrite(relayPin, HIGH);
//    state = false;
  }

  if (BTserial.available() > 0)
  { // Checks whether data is comming from the serial port
    state = BTserial.read(); // Reads the data from the serial port

    // Controlling the LED
    if (state == true)
    {
      digitalWrite(relayPin, HIGH); // ALARM ON
//      state = false;
    }
    else if (state == false)
    {
      digitalWrite(relayPin, LOW); // ALARM OFF
//      state = false;
    }
  }

}
