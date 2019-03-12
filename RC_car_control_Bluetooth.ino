//declare the pins used for the motor control board
int E1 = 3;
int M1 = 12;
int E2 = 11;
int M2 = 13;

//some constants for power output
#define OFF 0
#define SLOW 120
#define NORMAL 150
#define TURBO 200
#define MAX_FORWARD_POWER 3
#define MAX_REVERSE_POWER -3

//constants for wheel direction
#define LEFT 0
#define CENTER 1
#define RIGHT 2

//specify the motor variables
#define DRIVE_MOTOR_PIN           M1
#define DRIVE_MOTOR_PWM_PIN       E1
#define STEERING_MOTOR_PIN        M2
#define STEERING_MOTOR_PWM_PIN    E2
#define DRIVE_MOTOR_FORWARD       HIGH
#define DRIVE_MOTOR_REVERSE       LOW
#define STEERING_MOTOR_LEFT       HIGH
#define STEERING_MOTOR_RIGHT      LOW

//Variable for parsing serial commands
char val;

//Variables for motor control
//power for simplicity we will limit the power to 3 values above 0
//(1 = SLOW, 2 = NORMAL, 3 = TURBO)
//0 by default is always stopped for either direction
int power = OFF;
//Store the direction that the wheel is tilted (0 is left, 1 is center, 2 is right)
int wheel = CENTER;

void setup() {
  //setup serial output
  Serial.begin(9600);

  //setup pin outputs
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);

}

void loop() {

  // Check for incoming command
  if (Serial.available()) {
    val=Serial.read(); 
    Serial.println(val); // Use the IDE's Tools > Serial Monitor
    parseCommand(val); // parse the input
  }
  
}

//Serial command parser
//NOTE the numbers assigned to each button from the Giumig bluetooth controller app
void parseCommand(char input) {
  switch (input) {
    case '1': // LEFT
      //Turn wheel towards left
      if (wheel==RIGHT){
        center();
      } else if (wheel==CENTER){
        left();
      }
      break;
    case '2': // UP
      //Increase speed (throttle)
      if (power < MAX_FORWARD_POWER){
         power++;        
      }
      drive_motor(power);
      break;
    case '3': // RIGHT
      //Turn wheel towards right
      if (wheel==LEFT){
        center();
      } else if (wheel==CENTER){
        right();
      }
      break;
    case '4': // DOWN
      //Decrease speed (throttle)
      if (power > MAX_REVERSE_POWER){
         power--;        
      }
      drive_motor(power);
      break;
    case '5': // SELECT
      break;
    case '6': // START
      break;  
    case '7': // SQUARE
      //All stop
      all_stop_car();
      break; 
    case '8': // TRIANGLE
      //stop motor
      stop_car();
      break;  
    case '9': // X
      break;
    case '0': // CIRCLE
      break;
  }
}

//RC car control blocks:

void drive_motor(int power) {
  //check cases for power
  switch (power) {
    case -3:
      reverse(TURBO);
      break;
    case -2:
      reverse(NORMAL);
      break;
    case -1:
      reverse(SLOW);
      break;
    case 0:
      stop_car();
      break;
    case 1:
      forward(SLOW);
      break;
    case 2:
      forward(NORMAL);
      break;
    case 3:
      forward(TURBO);
      break;
    default:
      break;
  }
}

//Drive the RC motor forward at set power
void forward(int power){
  //Set motor direction
  digitalWrite(DRIVE_MOTOR_PIN, DRIVE_MOTOR_FORWARD);
  analogWrite(DRIVE_MOTOR_PWM_PIN, power);
}

//Drive the RC car reverse at set power
void reverse(int power){
  //Set motor direction
  digitalWrite(DRIVE_MOTOR_PIN, DRIVE_MOTOR_REVERSE);
  analogWrite(DRIVE_MOTOR_PWM_PIN, power);
}

//Stop drive motor for RC car
void stop_car() {
  //shut off circuit
  analogWrite(DRIVE_MOTOR_PWM_PIN, OFF);  
}

//All motors stop for RC car
void all_stop_car() {
  //shut off all circuit
  analogWrite(DRIVE_MOTOR_PWM_PIN, OFF);  
  center();
  wheel = CENTER;
  power = OFF;
}

//Turn steering wheels left
void left(){
  //Set motor direction
  digitalWrite(STEERING_MOTOR_PIN, STEERING_MOTOR_LEFT);
  analogWrite(STEERING_MOTOR_PWM_PIN, 255);
  //set wheel variable
  wheel = LEFT;
}

//Turn steering wheels right
void right(){
  //Set motor direction
  digitalWrite(STEERING_MOTOR_PIN, STEERING_MOTOR_RIGHT);
  analogWrite(STEERING_MOTOR_PWM_PIN, 255);
  //set wheel variable
  wheel = RIGHT;
}

//Turn steering wheels center
void center(){
  //Set motor direction
  digitalWrite(STEERING_MOTOR_PIN, STEERING_MOTOR_RIGHT);
  analogWrite(STEERING_MOTOR_PWM_PIN, OFF);
  //set wheel variable
  wheel = CENTER;
}
