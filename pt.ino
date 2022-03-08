
//The included libaries for the zumo to funtion
#include <QTRSensors.h>
#include <Zumo32U4LineSensors.h>
#include "TurnSensor.h"
#include <Zumo32U4Encoders.h>
#include <Zumo32U4Buzzer.h>
#include <Zumo32U4Motors.h>

//Creating an object from the libaries from the zumo
Zumo32U4Motors motors;
Zumo32U4LineSensors lineSensors;
L3G gyro;
Zumo32U4LCD lcd;
Zumo32U4ProximitySensors proxSensors;
Zumo32U4Encoders encode;
Zumo32U4Buzzer buzzer;


//Defining data is more efficinet and readable
#define NUM_SENSORS 3
#define CALIBERATE_SPEED  200

unsigned int lineSensorValues[NUM_SENSORS];

//All the global variables Used within the program
int speedr = 0;        
int speedl = 0;         
bool autoMode = true;
int calibrateData[3];                                   
unsigned int sensor_values[NUM_SENSORS];                
boolean atCorner = false;
boolean man = false;
boolean atRoom = false;
char option = ']';
int mode = 0; 
int roomsSeached = 0;






//A funtion for the gyro scope to return the angle in degrees.

int32_t getAngle() {
  
  return (((int32_t)turnAngle >> 16) * 360) >> 16;
}

//A rotational algrithum
void rotationAl(int angle){
  delay(500);
  turnSensorReset();
  turnSensorUpdate();
  while(getAngle()!=-angle){
             turnSensorUpdate();
             Serial.println(getAngle());
            motors.setSpeeds(100, -100);
   } 
 }


// My calibration funtion which sets up all the sensors I will use.
void calibrateSensors()
{  
  
  delay(50);
  encode.init();
  delay(50);
  proxSensors.initFrontSensor();
  delay(50);
  turnSensorSetup();
  
  delay(100);
  
  lineSensors.initThreeSensors();

  for(uint16_t i = 0; i < 60; i++){
    if (i > 15 && i <= 45)
    {
      motors.setSpeeds(-50, 50);
    }
    else
    {
      motors.setSpeeds(50, -50);
    }
    
     lineSensors.calibrate();
     
     delay(10);

  }

     for (int i = 0; i < NUM_SENSORS; i++){
      calibrateData[i] = lineSensors.calibratedMaximumOn[i];
  }


  motors.setSpeeds(0, 0);
}

//The initial setup for the progrma to start, I wait for user input here
void setup()
{

   Serial.begin(9600);
   Serial1.begin(9600);
   char n;
   while(n!='c'){
    n = Serial1.read();
   }
   calibrateSensors();
  
      
}

//This checks at the start if the input from the user is P to activate search room mode
void checkAtRoom(){

    if(option == 'p'){
     Serial1.println("s");
      mode = 3;
      }
      
  
 }
 //this checks if the user has pressed k, which activates manual mode
 void checkMan(){

    if(option == 'k'){
      
      mode = 1;
      }
      
  
 }

// This is the code that runs infinately or untill stopped, I check what mode the zumo is in every time it loops, allowing me to run the correct funtions
void loop(){

lineSensors.read(sensor_values);
 turnSensorUpdate();
// option = Serial.read();
 option = Serial1.read();
 

 checkAtRoom();
 checkMan();


if(mode == 0){

    autoFunc();
}

 else if (mode == 1){
  
    manFunc();
  }

  else if(mode == 2){
    
    
    cornerControl();
    
  }
  else if (mode == 3){
    
    searchRoomControl();
    
    }


  

    
}


// This contains the algorithums I have used to check if the zumo has hit a line.
void autoFunc(){


     if(((sensor_values[2] > calibrateData[2]+100) && (sensor_values[0] > calibrateData[0]+100) && (sensor_values[1] > calibrateData[1]))||sensor_values[1] > calibrateData[1]+100){
      Serial1.println("c");
     motors.setRightSpeed(0);
     motors.setLeftSpeed(0);
    mode = 2;     
   }

   else if(sensor_values[0] > calibrateData[0]+100){

       motors.setRightSpeed(0);
     motors.setLeftSpeed(100);
  }
   
  else if(sensor_values[2] > calibrateData[2]+100){

     motors.setRightSpeed(100);
     motors.setLeftSpeed(0);
  }
  

  else{

     speedr= 100;
   speedl= 100;
   
  motors.setRightSpeed(speedr);
  motors.setLeftSpeed(speedl);
    }
 
  
  }


// THis checks if the proxy sensor has gotten a high reading indicating there is a person, it returns a boolean value
  boolean checkPeople(){
      
    proxSensors.read();
    uint8_t leftValue = proxSensors.countsFrontWithLeftLeds();
    uint8_t rightValue = proxSensors.countsFrontWithRightLeds();
 

  if(leftValue > 4 || rightValue > 4){
          return true;
    }
    
    else{
        
        return false;

  }
  
  }
// this is the movement for the scanning of the room I use unit16_t interger for efficiency and memory ssaving.
  void scanRoom(){
    boolean isPerson = false;

for(uint16_t i = 0; i <120 ; i++){
    if (i > 30 && i <= 90)
    {
      motors.setSpeeds(-100, 100);
    }
    else
    {
      motors.setSpeeds(100, -100);
    }
     delay(10);
     isPerson = checkPeople();
  }
        motors.setSpeeds(0, 0);


       delay(100);
       if(isPerson){
              buzzer.playFrequency(500,250,10);
            Serial1.println("+");
            
        
        }
        
    
    
    }
  // This moves the robot forwards
  void  moveF(){
     
      encode.getCountsAndResetRight();

            while(encode.getCountsRight()<400){
                motors.setLeftSpeed(100);
                motors.setRightSpeed(100);
            }
            
            motors.setSpeeds(0, 0);
      

   
    }
    // this moves the zumo backwards
    void  moveB(){
     
      encode.getCountsAndResetRight();

            while(encode.getCountsRight()>-400){
                motors.setLeftSpeed(-100);
                motors.setRightSpeed(-100);
            }
            
            motors.setSpeeds(0, 0);
      

   
    }
  

  void turnRight(){
       encode.getCountsAndResetLeft();
            while(encode.getCountsLeft()<700){
                motors.setLeftSpeed(100);
                motors.setRightSpeed(-100);

            }
                motors.setSpeeds(0, 0);

    }

    void turnLeft(){
       encode.getCountsAndResetRight();
            while(encode.getCountsRight()<610){
                motors.setLeftSpeed(-100);
                motors.setRightSpeed(100);
            }
            
            motors.setSpeeds(0, 0);
      
      
      }
  
    void turnAround(){
      
      encode.getCountsAndResetRight();
            while(encode.getCountsRight()<1220){
                motors.setLeftSpeed(-100);
                motors.setRightSpeed(100);
            }
            
            motors.setSpeeds(0, 0);
      
      
      }
 

    
  
  void searchRoomControl(){
      
     motors.setSpeeds(0, 0);

    doScan();
    
          if(option == 'c'){
             Serial1.println("b");
             mode = 0;
          
         }
         

  }
//this puts all the movements realated to scanning a room together, based upon what value the user has given
void doScan(){
  
  
  if(option == 'r'){
           
            turnRight();
            delay(500);
            moveF();
            delay(500);
            scanRoom();
            delay(500);
            moveB();
            delay(500);
            turnLeft();

        }
        else if(option == 'l'){
          
            turnLeft();         
             delay(500);
            moveF();
            delay(500);
            scanRoom();
            delay(500);
            moveB();
            delay(500);
            turnRight();
            

         }
  
  
  
  
  }
  
// this is what runs when a corner has been hit, waiting for the users input.
void cornerControl(){

        if(option == 'r'){
                      
          turnRight();
          
        }
        if(option == 'l'){
                      
          turnLeft();
          
        }
        else if(option == 'z'){

           turnAround();
          
         }
        else if(option == 'c'){
            Serial1.println("b");
             mode = 0;
      
         }  
  
  
  }



// this is my manual funtion which speeds up or down the robot based on the input recieved
void manFunc(){
  //This will speed down the engine unless a command to speed up is received
  if(abs(speedr)>0){speedr=speedr-speedr/5;}
  if(abs(speedl)>0){speedl=speedl-speedl/5;}
  motors.setRightSpeed(speedr);
  motors.setLeftSpeed(speedl);
  
//  if (Serial1.available() > 0){
    switch(option){
      case 'a':// turn left
        speedr=speedr+400;
        break;
     case 'd': // turn right
        speedl=speedl+400;
        break;
     case 'w': // forward
        speedr=speedr+400;
        speedl=speedl+400;
        break;
     case 's':// backward
        speedr=speedr-400;
        speedl=speedl-400;
        break;
     case'q':
        turnLeft();
        break;
     case'e':
        turnRight();
        break;
     case'x':
        speedr = 0;
        speedl = 0; 
        break;
      case'z':
        turnAround();
        break;
     case 'o':
        mode = 0;
        break;
      case'l':
        doScan();
        break;
     case 'r':
        doScan();
        break;
     case 'v':
        buzzer.playFrequency(500,250,10);
        break;
      }
      
      delay(10);
//  }
//  }
}
