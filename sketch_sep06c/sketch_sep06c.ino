#include <Servo.h>
#include <IFX9201_XMC1300_StepperMotor.h>

/*
 * Alpha Wheel
 */
#define DIR_PIN 4    // Pin 9 is standard DIR Pin
#define STP_PIN 3   // Pin 10 is standard STP Pin
#define DIS_PIN 2   // Pin 11 is standard DIS Pin

const int StepsPerRevolution = 200 * 8; // change this to fit the total number of steps per revolution for your motor

Stepper_motor MyStepperMotor = Stepper_motor(StepsPerRevolution, DIR_PIN, STP_PIN, DIS_PIN);

/*
 * Spray Servo
 */
Servo BanksyServo;
Servo BotServo;

/*
 * Commands
 */
// Nothing
#define DONOTHING 0

#define SPRAY 1

#define ROTATECLOCKWISE 2
#define ROTATEANTICLOCKWISE 3

#define TURNLEFT 4
#define TURNRIGHT 5

const int STEPSFORSHIFT = 2000;

/*
 * Input
 */
String inputString = "";
int InputCommand = 0;

void setup()
{
  pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);

  digitalWrite(2, LOW);
  digitalWrite(4, LOW);
  
  Serial.begin(9600);
  
  BanksyServo.attach(A0); // verknüpft den Servomotor an Pin 0 mit dem Servo-Objekt
  BanksyServo.write(60); // überträgt die Zielposition an den Servomotors

  BotServo.attach(A1);
  BotServo.write(50);
  
  // set pins' mode as OUTPUT, set default speed and enable the stepper motor
  MyStepperMotor.begin();

  // set the speed at 10 rpm:
  MyStepperMotor.setSpeed(40);

  // initialize the serial port:
  Serial.begin(9600);
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
    case DONOTHING: break;
    case SPRAY: spray(); break;
    case ROTATECLOCKWISE: rotateClockwise(); break;
    case ROTATEANTICLOCKWISE: rotateAnticlockwise(); break;
    
    case TURNLEFT: turn(); break;
    case TURNRIGHT: turn(); break;
    default: break;
  }

  InputCommand = DONOTHING;
}

void spray()
{
  int pos = 0;
  for (pos = 60; pos >= 0; pos -= 2) { // goes from 180 degrees to 0 degrees
    BanksyServo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 0; pos <= 60; pos += 2) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
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

void rotateClockwise()
{
  Serial.println("moves 180 degree in clockwise direction");
  MyStepperMotor.move_degree(360);
  //MyStepperMotor.step(100);
  delay(1000);

  Serial.println("moves 180 degree in clockwise direction");
  MyStepperMotor.move_degree(-360);
  //MyStepperMotor.step(-100);
  //delay(1000);

  //digitalWrite(DIS_PIN, LOW);
  /*
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
  */
}

void rotateAnticlockwise()
{
  MyStepperMotor.move_degree(-360);
  delay(1000);
  MyStepperMotor.move_degree(360);
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
