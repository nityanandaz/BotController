#include <Servo.h>

// Nothing
const int DONOTHING = 0;
// Banksy
const int SPRAY = 1;
const int SHIFTLEFT = 2;
const int SHIFTRIGHT = 3;
// Bot
const int TURNLEFT = 4;
const int TURNRIGHT = 5;

const int STEPSFORSHIFT = 2000;

String inputString = "";
int InputCommand = 0;

Servo BanksyServo;
Servo BotServo;

void setup()
{
  pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);

  digitalWrite(2, LOW);
  digitalWrite(4, LOW);
  
  Serial.begin(9600);
  
  BanksyServo.attach(A0); // verknüpft den Servomotor an Pin 0 mit dem Servo-Objekt
  BanksyServo.write(0); // überträgt die Zielposition an den Servomotors

  BotServo.attach(A1);
  BotServo.write(50);
}

void loop()
{
  readInput();
  executeCommand();
}

void executeCommand()
{
  switch(InputCommand)
  {
    case DONOTHING:
      // Serial.println("Did nothing");
      break;
    case SPRAY:
      spray(); 
      break;
    case SHIFTLEFT:
      shiftLeft();
      break;
    case SHIFTRIGHT:
      shiftRight();
      break;
    case TURNLEFT:
      turn();
      break;
    case TURNRIGHT:
      turn();
      break;
    default: 
      break;
  }

  InputCommand = DONOTHING;
}

void spray()
{
  int pos = 0;
  for (pos = 0; pos <= 90; pos += 2) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    BanksyServo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 90; pos >= 0; pos -= 2) { // goes from 180 degrees to 0 degrees
    BanksyServo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  /*
  // überträgt die Zielposition an den Servomotors
  BanksyServo.write(100);
  delay(500);
  // überträgt die Zielposition an den Servomotors
  BanksyServo.write(50);
  */
}

void shiftLeft()
{
  if(digitalRead(2) == LOW)
  {
    digitalWrite(2, HIGH);
  }
  
  for(int i = 0; i < STEPSFORSHIFT; i++)
  {
    digitalWrite(4, HIGH);
    delayMicroseconds(500);
    digitalWrite(4, LOW);
    delayMicroseconds(500);
  }
}

void shiftRight()
{
  if(digitalRead(2) == HIGH)
  {
    digitalWrite(2, LOW);
  }
  for(int i = 0; i < STEPSFORSHIFT; i++)
  {
    digitalWrite(4, HIGH);
    delayMicroseconds(500);
    digitalWrite(4, LOW);
    delayMicroseconds(500);
  }
}

void turn()
{
  // überträgt die Zielposition an den Servomotors
  BotServo.write(100);
  delay(500);
  // überträgt die Zielposition an den Servomotors
  BotServo.write(50);
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
      
      InputCommand = (inputString.toInt());
      
      Serial.print("Value: ");
      Serial.println(InputCommand);
      
      // clear the string for new input:
      inputString = "";
    }
  }
}
