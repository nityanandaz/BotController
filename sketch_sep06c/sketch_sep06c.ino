#include <Servo.h>

const int DONOTHING = 0;
const int SPRAY = 1;
const int SHIFTLEFT = 2;
const int SHIFTRIGHT = 3;

const int STEPSFORSHIFT = 2000;

String inputString = "";
int InputCommand = 0;

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
    default: 
      break;
  }

  InputCommand = DONOTHING;
}

void spray()
{
  // überträgt die Zielposition an den Servomotors
  SprayServo.write(100);
  delay(500);
  // überträgt die Zielposition an den Servomotors
  SprayServo.write(50);
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
