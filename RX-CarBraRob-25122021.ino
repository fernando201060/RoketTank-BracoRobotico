/*
---- Receiver Code ----
*/

#include <Servo.h>
#include <SPI.h>
#include "RF24.h"  
#define enA 5
#define enB 6
#define inA 7
#define inB 8
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define vermelho 3 // Assimila à palavra "vermelho" o numero 3
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// #define in1 3
// #define in2 4
// #define in3 9
// #define in4 10
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void acendeV(); // Funçao que controla o acendimento do led vermelho
void espera(); // Funçao delay
unsigned long tempoV=0; // Variavel para armazenar o tempo do LED vermelho
bool comutarV = false;
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int motorSpeedA = 0;
int motorSpeedB = 0;

int positionA = 90;
int positionB = 90;
int positionC = 90;
int positionD = 90;
// int positionE = 90;
// int positionF = 90;
Servo myServoA;
Servo myServoB;
Servo myServoC;
Servo myServoD;
// Servo myServoE;
// Servo myServoF;

long previousMillis = 0;
long interval = 15;

unsigned long SetTime;

boolean lastButton1 = LOW; 
boolean lastButton2 = LOW;
// boolean lastButton3 = LOW;
// boolean lastButton4 = LOW;
boolean ledOn1 = false; 
boolean ledOn2 = false;
// boolean ledOn3 = false;
// boolean ledOn4 = false;
int buttonState1 = 0;
int buttonState2 = 0;
// int buttonState3 = 0;
// int buttonState4 = 0;

int joystick[12];

RF24 radio(9,10);
const uint64_t pipe = 0x7878787878LL;

void setup()
{
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(inA, OUTPUT);
  pinMode(inB, OUTPUT); 
  // pinMode(in1, OUTPUT);
  // pinMode(in2, OUTPUT);
  // pinMode(in3, OUTPUT);
  // pinMode(in4, OUTPUT);
  myServoA.attach(14);
  myServoB.attach(15);
  myServoC.attach(16);
  myServoD.attach(17);
//   myServoE.attach(18);
//   myServoF.attach(19);
  //==================
  pinMode(2, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(0, OUTPUT);
//++++++++++++++++++++++++++
  pinMode(vermelho, OUTPUT);
//++++++++++++++++++++++++++
  radio.begin();
  radio.openReadingPipe(1, pipe);   
  radio.startListening();       
}

void loop()
{
//+++++++++++
  acendeV();
  espera();
//+++++++++++
  
  if(radio.available())
  {       
    bool done = false;      
    while (!done) 
    {
      done = radio.read(joystick, sizeof(joystick));

      unsigned long currentMillis = millis();
      if(currentMillis - previousMillis > interval)

      {
        previousMillis = currentMillis;
        
      if (millis() - SetTime < 30) 
      {
        digitalWrite(0, HIGH);
      } 
      else if (millis() - SetTime < 1000)
      {
        digitalWrite(0, LOW);
      }
      else
      {
        SetTime = millis();
      }

      //WHEEL MOTORS
      int xAxis = joystick[0];
      int yAxis = joystick[1];

      if (yAxis < 470)
      {
      /*  digitalWrite(in1, HIGH);
          digitalWrite(in2, LOW);
          digitalWrite(in3, HIGH);
          digitalWrite(in4, LOW);
      */
        digitalWrite(enA, HIGH);
        digitalWrite(enB, HIGH);
        // digitalWrite(inA, HIGH);
        // digitalWrite(inB, LOW);
        digitalWrite(inA, LOW);
        digitalWrite(inB, HIGH);
        
        motorSpeedA = map(yAxis, 470, 0, 0, 255);
        motorSpeedB = map(yAxis, 470, 0, 0, 255);
      }
      else if (yAxis > 550)
      {
     /* digitalWrite(in1, LOW);
        digitalWrite(in2, HIGH);
        digitalWrite(in3, LOW);
        digitalWrite(in4, HIGH);
      */  
        digitalWrite(enA, HIGH);
        digitalWrite(enB, HIGH);
        // digitalWrite(inA, LOW);
        // digitalWrite(inB, HIGH);
        digitalWrite(inA, HIGH);
        digitalWrite(inB, LOW);
        
        motorSpeedA = map(yAxis, 550, 1023, 0, 255);
        motorSpeedB = map(yAxis, 550, 1023, 0, 255);
      }
      else
      {
        motorSpeedA = 0;
        motorSpeedB = 0;
      }
      if (xAxis < 400)
      {
        int xMapped = map(xAxis, 400, 0, 0, 255);
        motorSpeedA = motorSpeedA - xMapped;
        motorSpeedB = motorSpeedB + xMapped;
        if (motorSpeedA < 0)
        {
          motorSpeedA = 0;
        }
        if (motorSpeedB > 255)
        {
          motorSpeedB = 255;
        }
      }
      if (xAxis > 620)
      {
        int xMapped = map(xAxis, 620, 1023, 0, 255);
        motorSpeedA = motorSpeedA + xMapped;
        motorSpeedB = motorSpeedB - xMapped;
        if (motorSpeedA > 255)
        {
          motorSpeedA = 255;
        }
        if (motorSpeedB < 0)
        {
          motorSpeedB = 0;
        }
      }
      if (motorSpeedA < 70)
      {
        motorSpeedA = 0;
      }
      if (motorSpeedB < 70)
      {
        motorSpeedB = 0;
      }

      analogWrite(enA, motorSpeedA);
      analogWrite(enB, motorSpeedB);

      //SERVOs for Joint 1 and Joint 2

        // if(joystick[2] < 412 && positionA < 170)
        if(joystick[2] < 412 && positionA < 180)
        {
          positionA+=2;
          myServoA.write(positionA);
        }
        // if(joystick[2] > 612 && positionA > 10)
        if(joystick[2] > 612 && positionA > 0)
        {
          positionA-=2;
          myServoA.write(positionA);
        }
        // if(joystick[3] < 412 && positionB < 170)
        if(joystick[3] < 412 && positionB < 180)
        {
          positionB+=2;
          myServoB.write(positionB);
        }
        // if(joystick[3] > 612 && positionB > 10)
        if(joystick[3] > 612 && positionB > 0)
        {
          positionB-=2;
          myServoB.write(positionB);
        }

      //SERVOs for Joint 3 and Gripper

        // if(joystick[4] < 412 && positionC < 170)
        if(joystick[4] < 412 && positionC < 180)
        {
          positionC+=2;
          myServoC.write(positionC);
        }
        // if(joystick[4] > 612 && positionC > 10)
        if(joystick[4] > 612 && positionC > 0)
        {
          positionC-=2;
          myServoC.write(positionC);
        }
        // if(joystick[5] < 412 && positionD < 170)
        if(joystick[5] < 412 && positionD < 180)
        {
          positionD+=2;
          myServoD.write(positionD);
        }
        // if(joystick[5] > 612 && positionD > 10)
        if(joystick[5] > 612 && positionD > 0)
        {
          positionD-=2;
          myServoD.write(positionD);
        }

/*      //SERVOs for Camera
      if(ledOn4 == HIGH)
      {  
        int joy6 = map(joystick[6], 0, 1023, 0, 179);
        myServoE.write(joy6);
        int joy7 = map(joystick[7], 0, 1023, 0, 179);
        myServoF.write(joy7);
       }

      if(ledOn4 == LOW)
      {
        if(joystick[6] < 412 && positionE < 170)
        {
          positionE+=2;
          myServoE.write(positionE);
        }
        if(joystick[6] > 612 && positionE > 10)
        {
          positionE-=2;
          myServoE.write(positionE);
        }
        if(joystick[7] < 412 && positionF < 170)
        {
          positionF+=2;
          myServoF.write(positionF);
        }
        if(joystick[7] > 612 && positionF > 10)
        {
          positionF-=2;
          myServoF.write(positionF);
        }
      }
 */       
      buttonState1 = joystick[8];
      if (buttonState1 == HIGH && lastButton1 == LOW) 
      {
        ledOn1 = !ledOn1;
        lastButton1 = HIGH;
      }
      else 
      {
        lastButton1 = buttonState1;
      }

      buttonState2 = joystick[9];
      if (buttonState2 == HIGH && lastButton2 == LOW) 
      {
        ledOn2 = !ledOn2;
        lastButton2 = HIGH;
      }
      else 
      {
        lastButton2 = buttonState2;
      }
      digitalWrite(1, !ledOn1);
      digitalWrite(2, !ledOn2);
      // digitalWrite(2, !joystick[10]);
  //++++++++++++++++++++++++++++++++++++
  /*
  //  
      buttonState3 = joystick[11];
    if (buttonState3 == HIGH && lastButton3 == LOW)
    {
        ledOn3 = !ledOn3;
        lastButton3 = HIGH;
      }
      else
      {
      lastButton3 = buttonState3;
     }
     digitalWrite(3, !ledOn3);  
     digitalWrite(3, !joystick[11]);
     
  //++++++++++++++++++++++++++++++++++++
      buttonState4 = joystick[11];
      if (buttonState4 == HIGH && lastButton4 == LOW)
      {
        ledOn4 = !ledOn4;
        lastButton4 = HIGH;
      }
      else
      {
        lastButton4 = buttonState4;
      }
   
    */   
    }          
  }
}
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void acendeV(){
  if(comutarV){
    digitalWrite(vermelho, !digitalRead(vermelho)); // Comando que comuta o estado do pino, se era alto vira baixo e vice-versa
    tempoV=millis();
    comutarV=false;
  }
}
void espera(){
  //-------LED vermelho
  if(!comutarV && millis()-tempoV>=1000){
    comutarV = true;
  }
  }
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
