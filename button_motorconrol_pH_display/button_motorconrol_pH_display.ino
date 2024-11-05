// Pin definitions

#define SensorPin 0          //pH meter Analog output to Arduino Analog Input 0
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x20,16,2); 

unsigned long int avgValue;  //Store the average value of the sensor feedback
float b;
int buf[10],temp;

const int motorAPin1 = 7;       // L298N IN1 pin
const int motorAPin2 = 6;       // L298N IN2 pin
  
const int button1Pin = 2;       // Push button 1 pin
const int button2Pin = 3;       // Push button 2 pin
const int button3Pin = 4;
const int solenoid_relay = 8;
const int conveyer_belt = 9;

void setup() {
  // Initialize motor control pins as outputs
 
  pinMode(motorAPin1, OUTPUT);
  pinMode(motorAPin2, OUTPUT);
  pinMode(solenoid_relay, OUTPUT);
  pinMode(conveyer_belt, OUTPUT);


  // Initialize push button pins as inputs with internal pull-up resistors
  pinMode(button1Pin, INPUT_PULLUP);
  pinMode(button2Pin, INPUT_PULLUP);
  pinMode(button3Pin, INPUT_PULLUP);
  Serial.begin(9600);  
 
  Serial.println("Ready");    //Test the serial monitor
  lcd.init();                      // initialize the lcd 
 
  // Print a message to the LCD.
  lcd.backlight();
  
}

void loop() {

  for(int i=0;i<10;i++)       //Get 10 sample value from the sensor for smooth the value
  { 
    buf[i]=analogRead(SensorPin);
    delay(10);
  }
  for(int i=0;i<9;i++)        //sort the analog from small to large
  {
    for(int j=i+1;j<10;j++)
    {
      if(buf[i]>buf[j])
      {
        temp=buf[i];
        buf[i]=buf[j];
        buf[j]=temp;
      }
    }
  }
  avgValue=0;
  for(int i=2;i<8;i++)                      //take the average value of 6 center sample
    avgValue+=buf[i];
  float phValue=(float)avgValue*5.0/1024/6; //convert the analog into millivolt
  phValue=3.5*phValue;                      //convert the millivolt into pH value
  Serial.print("    pH:");  
  Serial.print(phValue,2);
  lcd.print("pH:");
  lcd.setCursor(0,1);
  lcd.print(phValue);
  Serial.println(" ");
  delay(1000);
  lcd.clear();
  // Read the state of push buttons
  int button1State = digitalRead(button1Pin);
  int button2State = digitalRead(button2Pin);
  int button3State = digitalRead(button3Pin);
  // Check which button is pressed and control the motor accordingly
  if (button1State == LOW ) {
    // Button 1 is pressed, rotate the motor in one direction
    digitalWrite(conveyer_belt, HIGH);
    delay(2000);
    rotateMotorOneDirection();
    delay(5000);
    stopmotor();
    digitalWrite(conveyer_belt, LOW);
  }  
  if ( button2State == LOW ) {
    // Button 2 is pressed, rotate the motor in the opposite direction
    rotateMotorOppositeDirection();
    delay(5000);
    stopmotor();
  } 
  if ( button3State == LOW) {
    // Button 2 is pressed, rotate the motor in the opposite direction
    rotateMotorOppositeDirection();
    digitalWrite(solenoid_relay,HIGH);
    delay(10000);
    stopmotor();
    digitalWrite(solenoid_relay,LOW);
  }
  
}

void rotateMotorOneDirection() {
  // Rotate the motor in one direction
  

  digitalWrite(motorAPin1, HIGH);
  digitalWrite(motorAPin2, LOW);
 
}

void rotateMotorOppositeDirection() {
  // Rotate the motor in the opposite direction
  

  digitalWrite(motorAPin1, LOW);
  digitalWrite(motorAPin2, HIGH);
 
}

void stopmotor()
{ digitalWrite(motorAPin1, LOW);
  digitalWrite(motorAPin2, LOW);
}
