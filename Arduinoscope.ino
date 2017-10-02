#include <Servo.h>
#include <Stepper.h>
#include <SoftwareSerial.h>
Servo servo;
//SoftwareSerial blue(2,3);



//--- STEPPER---

const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution
// for your motor

// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);



int stepperPos = 0;         // current step position of the stepper

int servoPos = 0;           // Current positon of servo



 
float compensation = .1;   // The servo's inaccuracy per degree.
                                                                            // Stepper --> RA. Servo -->Dec


void setup() {
  Serial.begin(9600);
  servo.attach(6);
  servo.write(180);
  servoPos = 90;                                                            //  + 90 is max, north pole
  
  //Serial.println(servo.read());

}

void loop() {
    delay(500);
  
}

                              //Takes serial data when given as a program interrupt
void serialEvent() {
   char whichMot = Serial.read();
   // Serial.println("INSIDE- "+ whichMot);

    String readString;
    char c;
    if( whichMot == 'R'|| whichMot == 'r'){                                               // Command starting with R communicates to the servo, and command starting from O communicates with Servo
                Serial.println("INSIDE STEPPER");
  
        while (Serial.available()) {
              
                c = Serial.read();  //gets one byte from serial buffer
                readString += c; //makes the string readString
                delay(2);  //slow looping to allow buffer to fill with next character
        }
        
        if (readString.length() >0) {
               
               int n = readString.toInt();  //convert readString into a number
              Serial.println(n);  //so you can see the captured string 
               stepperMove(n);
        }
    }
      
    else if (whichMot == 'O' || whichMot == 'o'){
    
          Serial.println("INSIDE SERVO");
           while (Serial.available()) {
                c = Serial.read();  //gets one byte from serial buffer
                readString += c; //makes the string readString
                delay(2);  //slow looping to allow buffer to fill with next character
          }
        
          if (readString.length() >0) {
                // Serial.println(readString);  //so you can see the captured string 
                int n = readString.toInt();  //convert readString into a number
            
                
              
                moveServoTo(n);
                //Serial.println(servo.read() + " NEW POS ");
          }
    }


    if( whichMot == 'C'|| whichMot == 'c'){                                               
  
        while (Serial.available()) {
              
                c = Serial.read();  //gets one byte from serial buffer
                readString += c; //makes the string readString
                delay(2);  //slow looping to allow buffer to fill with next character
        }
        
        if (readString.length() >0) {
               // Serial.println(readString);  //so you can see the captured string 
               int n = readString.toInt();  
               
               // ADD CODE TO SET THE CURRENT RA OF STEPPER
        }
    }
    

}


// Moves the servo to the given position (-90 to 90)
void moveServoTo(int dec){
    servo. write(90 + (dec - (int)(dec*compensation)));
    delay(2000);
}

//---- STEPPER ----
//chr
// moves the stepper to the position given as input (based on an absoolute reference)
void stepperMove(int deg){
    Serial.print("INSIDE STEPPER MOVE    ");

    int finStep =  round ((deg*1.0)/1.8);
    Serial.print("final step is  " + finStep);

    int toMove = stepperPos - finStep;
    
    Serial.println("No of steps to take are : " + toMove);
    (toMove < 0) ? step(-1,-toMove) : step(1,toMove);
   
}

// Makes the stepper move the a given number of steps in a particular direction
void step(int dir, int steps){
  if(steps == 0) return;
  myStepper.step(dir);
  stepperPos+= 1*dir;
  delay(100);
  step(dir,steps-1);

}

// Gives the position of stepper in degrees
int getStepperPos(){
  return (stepperPos * 1.8);
}


