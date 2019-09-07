#include <Servo.h>

const int SPRAY = 0;

const int SHIFTLEFT = 1;
const int SHIFTRIGHT = 2;

String inputString = "";

int Position = 0;
// Record the number of steps we've taken
int Distance = 0;
// Servo-Objekt um einen Servomotor zu steuern
Servo SprayServo;

void setup()
{
  pinMode(2, OUTPUT);

  pinMode(4, OUTPUT);

  digitalWrite(2, LOW);

  digitalWrite(4, LOW);
  Serial.begin(9600);
  SprayServo.attach(A0); // verknüpft den Servomotor an Pin 0 mit dem Servo-Objekt

  SprayServo.write(50); // überträgt die Zielposition an den Servomotors
}

void loop()
{
  readInput();
  movement();
}

void movement()
{
  
  if (Position != Distance)
  {
    Serial.println(Position);
    Serial.println(Distance);
    Distance = Distance + 1; // record this step // Check to see if we are at the end of our move
    digitalWrite(4, HIGH);
    delayMicroseconds(500);
    digitalWrite(4, LOW);
    delayMicroseconds(500);
  }
  if (Position - 1 == Distance)
  {

    SprayServo.write(100); // überträgt die Zielposition an den Servomotors
    delay(500);
    SprayServo.write(50); // überträgt die Zielposition an den Servomotors
  }

  // two rotation for 1/8 bridge and 1 rotation for 1/6 bridge (for this code)

  if (Distance == 3200)
  { // We are! Reverse direction (invert DIR signal)

    //if (digitalRead(2) == LOW) {

    //digitalWrite(2, HIGH); }

    //else {

    //digitalWrite(2, LOW);

    //} // Reset our Distance back to zero since we're // starting a new move

    Distance = 0; // Now pause for half a second delay(500);
  }
}

void readInput()
{
  while (Serial.available() > 0)
  {
    int inChar = Serial.read();
    if (isDigit(inChar))
    {
      // convert the incoming byte to a char and add it to the string:
      inputString += (char)inChar;
    }
    
    // if you get a newline, print the string, then the string's value:
    if (inChar == '\n')
    {
      Serial.print("String: ");
      Serial.println(inputString);
      
      Position = (inputString.toInt());
      
      Serial.print("Value: ");
      Serial.println(Position);
      
      // clear the string for new input:
      inputString = "";
    }
  }
}
