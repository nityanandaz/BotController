#include <Adafruit_NeoPixel.h>
#include <EasyNeoPixels.h>

#include <Servo.h>
#include <IFX9201_XMC1300_StepperMotor.h>

/*
   Alpha Wheel
*/
#define DIR_PIN 4   // Pin 9 is standard DIR Pin
#define STP_PIN 3   // Pin 10 is standard STP Pin
#define DIS_PIN 2   // Pin 11 is standard DIS Pin

const int StepsPerRevolution = 200 * 8; // change this to fit the total number of steps per revolution for your motor

Stepper_motor AlphaWheelStepper = Stepper_motor(StepsPerRevolution, DIR_PIN, STP_PIN, DIS_PIN);

/*
   Spray Servo
*/
Servo SprayCanServo;

/*
   Commands
*/
#define DONOTHING 0

#define SPRAY 1

#define ROTATECLOCKWISE 2
#define ROTATEANTICLOCKWISE 3

/*
   Input
*/
String inputString = "";
int InputCommand = 0;

/*
   Framework
*/
void setup()
{
  // Spray Can
  SprayCanServo.attach(6);
  SprayCanServo.write(0);
  
  // USB
  Serial.begin(9600);

  // Alpha Wheel
  AlphaWheelStepper.begin(); // set pins' mode as OUTPUT, set default speed and enable the stepper motor
  AlphaWheelStepper.setSpeed(10); //changed from 40

  // LEDs
  pinMode(10, OUTPUT);
  setupEasyNeoPixels(11, 14);
}

void loop()
{
  readInput();
  executeCommand();
}

/*
   Commands
*/
void spray()
{
  // Actual
  int pos = 0;
  
  for (pos = 0; pos <= 60; pos += 2) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    SprayCanServo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  // delay(5000);
  for (pos = 60; pos >= 0; pos -= 2) { // goes from 180 degrees to 0 degrees
    SprayCanServo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }

  digitalWrite(5, HIGH);
  delay(1000);
  digitalWrite(5, LOW);
}

void lightOn()
{
  // Mock
  digitalWrite(10, HIGH);
  writeEasyNeoPixel(0, HIGH);
  writeEasyNeoPixel(13, 255, 255, 0);
}

void lightOff()
{
  writeEasyNeoPixel(0, LOW);
  digitalWrite(10, LOW);
}

void rotateClockwise()
{
  AlphaWheelStepper.move_degree(14.4);
  delay(1000);
}

void rotateAnticlockwise()
{
  AlphaWheelStepper.move_degree(-14.4);
  delay(1000);
}

/*
   Input
*/
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

void executeCommand()
{
  switch (InputCommand)
  {
    case DONOTHING: break;
    case SPRAY: spray(); break;
    case ROTATECLOCKWISE: rotateClockwise(); break;
    case ROTATEANTICLOCKWISE: rotateAnticlockwise(); break; // prefer
    
    case 4: lightOn(); delay(1000); lightOff(); break;

    default: 
      if (InputCommand < 200 && InputCommand > 100)
      {
        int steps = InputCommand % 100;
        
        AlphaWheelStepper.move_degree(-14.4 * steps);
        delay(1000);
      }
      break;
  }

  InputCommand = DONOTHING;
}
