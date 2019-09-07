#include <Servo.h>

int Position = 0;
int Distance = 0; // Record the number of steps we've taken
String inString = "";
Servo myservo; // erstellt ein Servo-Objekt um einen Servomotor zu steuern

void setup()
{
  pinMode(2, OUTPUT);

  pinMode(4, OUTPUT);

  digitalWrite(2, LOW);

  digitalWrite(4, LOW);
  Serial.begin(9600);
  myservo.attach(A0); // verknüpft den Servomotor an Pin 0 mit dem Servo-Objekt

  myservo.write(50); // überträgt die Zielposition an den Servomotors
}

void loop()
{
  readInput();
  
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

    myservo.write(100); // überträgt die Zielposition an den Servomotors
    delay(500);
    myservo.write(50); // überträgt die Zielposition an den Servomotors
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
      inString += (char)inChar;
    }
    
    // if you get a newline, print the string, then the string's value:
    if (inChar == '\n')
    {
      Serial.print("String: ");
      Serial.println(inString);
      
      Position = (inString.toInt());
      
      Serial.print("Value: ");
      Serial.println(Position);
      
      // clear the string for new input:
      inString = "";
    }
  }
}
