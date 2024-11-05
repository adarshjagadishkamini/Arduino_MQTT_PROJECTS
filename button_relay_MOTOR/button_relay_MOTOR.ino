// Pin definitions

const int motorAPin1 = 7;       // L298N IN1 pin
const int motorAPin2 = 6;       // L298N IN2 pin
  
const int button1Pin = 2;       // Push button 1 pin
const int button2Pin = 3;       // Push button 2 pin
const int button3Pin = 4;
const int solenoid_relay = 8;
void setup() {
  // Initialize motor control pins as outputs
 
  pinMode(motorAPin1, OUTPUT);
  pinMode(motorAPin2, OUTPUT);
  pinMode(solenoid_relay, OUTPUT);


  // Initialize push button pins as inputs with internal pull-up resistors
  pinMode(button1Pin, INPUT_PULLUP);
  pinMode(button2Pin, INPUT_PULLUP);
  pinMode(button3Pin, INPUT_PULLUP);

  
}

void loop() {
  // Read the state of push buttons
  int button1State = digitalRead(button1Pin);
  int button2State = digitalRead(button2Pin);
  int button3State = digitalRead(button3Pin);
  // Check which button is pressed and control the motor accordingly
  if (button1State == LOW ) {
    // Button 1 is pressed, rotate the motor in one direction
    rotateMotorOneDirection();
    delay(5000);
    stopmotor();
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
