#include <ArduinoRobot.h>
#include <Wire.h>
#include <SPI.h>

int left2;
int left1;
int center;
int right1;
int right2;

double rightSpeed;
double leftSpeed;
int defSpeed = 100;
int lastDirRight = -1;

void printData() {
  Robot.stroke(0, 0, 0);
  Robot.text(left2, 0, 0);
  Robot.text(left1, 20, 0);
  Robot.text(center, 40, 0);
  Robot.text(right1, 60, 0);
  Robot.text(right2, 80, 0);
  Robot.text((int)leftSpeed, 0, 20);
  Robot.text((int)rightSpeed, 30, 20);
  //delay(50);
  Robot.stroke(255, 255, 255);
  Robot.text(left2, 0, 0);
  Robot.text(left1, 20, 0);
  Robot.text(center, 40, 0);
  Robot.text(right1, 60, 0);
  Robot.text(right2, 80, 0);
  Robot.text((int)leftSpeed, 0, 20);
  Robot.text((int)rightSpeed, 30, 20);
}

void getSensors() {
  Robot.updateIR();
  /*
  left2 = 1000-Robot.IRarray[0];
  left1 = 1000-Robot.IRarray[1];
  center = 1000-Robot.IRarray[2];
  right1 = 1000-Robot.IRarray[3];
  right2 = 1000-Robot.IRarray[4];
  */

  left2 = 1000-Robot.IRarray[4];
  left1 = 1000-Robot.IRarray[3];
  center = 1000-Robot.IRarray[2];
  right1 = 1000-Robot.IRarray[1];
  right2 = 1000-Robot.IRarray[0];

  
  leftSpeed  = defSpeed - (left1/1000.0) *50 - (left2/1000.0) *210 + (right1/1000.0)*50 + (right2/1000.0)*140;
  rightSpeed = defSpeed - (right1/1000.0)*50 - (right2/1000.0)*210 + (left1/1000.0) *50 + (left2/1000.0) *140;
  
  /*
  leftSpeed  = (right2/1000.0)*180 + (right1/1000.0)*90;
  rightSpeed = (left2/1000.0) *180 + (left1/1000.0)*90;
  */

  /*
  Serial.print(left2);    Serial.print("  ");
  Serial.print(left1);    Serial.print("  ");
  Serial.print(center);   Serial.print("  ");
  Serial.print(right1);   Serial.print("  ");
  Serial.println(right2);
  */
  
  /*Serial.println(leftSpeed);
  Serial.println(rightSpeed);*/
}

void setup() {
  // initialize the robot
  Robot.begin();
  // Robot.beginTFT();
  

  // initialize the serial port
  Serial.begin(9600);
}

void checkForSplit() {
  // If we hit a split
  if ((left1 > 400)&&(center>500)&&(right1>400)) {
    Robot.motorsWrite(0, 0);
    delay(100);
    int direct = rand() % 5;
    if ((direct == 0)||(direct == 1)){
      leftSpeed  = defSpeed - (right1/1000.0)*60 - (right2/1000.0)*180;
      rightSpeed = defSpeed + (left1/1000.0) *60 + (left2/1000.0) *180;
    }
    else{
      leftSpeed  = defSpeed + (right1/1000.0)*60 + (right2/1000.0)*180;
      rightSpeed = defSpeed - (left1/1000.0) *60 - (left2/1000.0) *180;
    }
  
    delay(100);
    getSensors(); 
    } 
    else if(left2 > 600 && right2 > 600 & center < 500){
      Robot.motorsWrite(-80,80);
      delay(150);
    }
}  

void sharpTurn() {

  // -1 - no direction recorded
  // 0  - 90 degrees to the left
  // 1  - 90 degrees to the right

  // Determine which sensors were high last time
  // This will be the direction we need to go
  if ( (left2>500)&&(left1>500) ) {
    lastDirRight = 0;
    
  } else if ( (right2>500)&&(right1>500) ) {
    lastDirRight = 1;
  }

  Serial.println(lastDirRight);

  // If we hit a white
  if ((left2<100)&&
      (left1<100)&&
      (center<100)&&
      (right1<100)&&
      (right2<100)
  ) {
    delay(100);

  if (lastDirRight == 0) {
    Serial.println("Turn left");
    Robot.motorsWrite(-80,80);
    delay(150);
    
  } else if (lastDirRight == 1) {
    Serial.println("Turn right");
    Robot.motorsWrite(80,-80);
    delay(150);
  }
    
  }
  
}

void loop() {
  getSensors();
  sharpTurn();

  Robot.motorsWrite((int)leftSpeed, (int)rightSpeed);

  delay(7);

  //printData();
}
