#include "Nintendo.h"
/* This code uses the Nicohood Library
 * Use this code at your own risk
 * Code written by Simple Controllers, modified to liking by Tyler Barron. This code is open source.
 */

  //This makes the controller bidirection data line on pin number8
CGamecubeConsole GamecubeConsole(8);      //Defines a "Gamecube Console" sending data to the console on pin 8
Gamecube_Data_t d = defaultGamecubeData;   //Structure for data to be sent to console

//This is needed but you don't need a controller on pin 7
CGamecubeController GamecubeController1(7);

//This is the pinout of the controller.  Can be changed to your liking.  I may have mixed up some of the tilt pins but you can change that physically in your build or through the code.  Just do test runs along the way.
const int A = 22;
const int B = 24;
const int X = 26;
const int Y = 28;
const int Z = 30;
const int START = 31;

const int R = 34;
const int L = 35;
const int RLIGHT = 23;
//This is the value of analog shielding 74 is lightest possible on gamecube.  It varies from gamecube to dolphin no idea why.
const int RLIGHTv = 74;

const int LEFT = 38;
const int RIGHT = 39;
const int UP = 40;
const int DOWN = 41;

const int CLEFT = 48;
const int CRIGHT = 49;
const int CUP = 50;
const int CDOWN = 51;

const int LSTATE = 52;
const int SSTATE = 53;

const int MOD1 = 46;
const int MOD2 = 44;

void setup()
{
  //This is establishing the pin assignments up there to input pins
  pinMode(A, INPUT_PULLUP);
  pinMode(B, INPUT_PULLUP);
  pinMode(X, INPUT_PULLUP);
  pinMode(Y, INPUT_PULLUP);
  pinMode(Z, INPUT_PULLUP);
  pinMode(START, INPUT_PULLUP);
  
  pinMode(R, INPUT_PULLUP);
  pinMode(L, INPUT_PULLUP);
  pinMode(RLIGHT, INPUT_PULLUP);
  
  pinMode(LEFT, INPUT_PULLUP);
  pinMode(RIGHT, INPUT_PULLUP);
  pinMode(UP, INPUT_PULLUP);
  pinMode(DOWN, INPUT_PULLUP);
  
  pinMode(LSTATE, INPUT_PULLUP);
  pinMode(SSTATE, INPUT_PULLUP);

  pinMode(MOD1, INPUT_PULLUP);
  pinMode(MOD2, INPUT_PULLUP);
  
  pinMode(CLEFT, INPUT_PULLUP);
  pinMode(CRIGHT, INPUT_PULLUP);
  pinMode(CUP, INPUT_PULLUP);
  pinMode(CDOWN, INPUT_PULLUP);

  //This is needed to run the code.
  GamecubeController1.read();
}

void loop()
{
  //This resets and establishes all the values after the controller sends them to the console and helps with initial "zeroing"
  int pinA = 0;
  int pinB = 0;
  int pinX = 0;
  int pinY = 0;
  int pinZ = 0;
  int pinSTART = 0;

  int pinR = 0;
  int pinL = 0;
  int pinRLIGHT = 0;

  int pinLEFT = 0;
  int pinRIGHT = 0;
  int pinUP = 0;
  int pinDOWN = 0;

  int pinX1 = 0;
  int pinX2 = 0;
  int pinY1 = 0;
  int pinY2 = 0;

  int pinCLEFT = 0;
  int pinCRIGHT = 0;
  int pinCUP = 0;
  int pinCDOWN = 0;

  int pinxAxisC = 128;
  int pinyAxisC = 128;

  int pinLState = 0;
  int pinSState = 0;
  int pinTaunt = 0;
  
  int pinxAxis = 128;
  int xmod = 0;
  int pinyAxis = 128;

  int rightOne = 0;
  int leftOne = 0;
  int downOne = 0;
  int upOne = 0;
  int modA = 0;
  int modB = 0;
  int modC = 0;

  //This reads control stick as neutral when both left/right or up/down is pressed at the same time. Sets "__One" flags for use in logic later
  if (digitalRead(LEFT) == LOW && digitalRead(RIGHT) == HIGH)leftOne = 1;
  if (digitalRead(RIGHT) == LOW && digitalRead(LEFT) == HIGH)rightOne = 1;
  if (digitalRead(UP) == LOW && digitalRead(DOWN) == HIGH)upOne = 1;
  if (digitalRead(DOWN) == LOW && digitalRead(UP) == HIGH)downOne = 1;
  
  //MOD1 = modA; MOD2 = modB; MOD1+MOD2 = modC. Again, sets flags for use later
  if (digitalRead(MOD1) == LOW && digitalRead(MOD2) == HIGH)modA = 1;
  if (digitalRead(MOD2) == LOW && digitalRead(MOD1) == HIGH)modB = 1;
  if (digitalRead(MOD1) == LOW && digitalRead(MOD2) == LOW)modC = 1;

  //Using flags from earlier computes the grey stick values
  
  //IF SHIELD DROP
  if (downOne && (digitalRead(Z) == LOW || digitalRead(RLIGHT) == LOW || digitalRead(L) == LOW))
  {
    pinxAxis = 128;
    pinyAxis = 73;
  }
  //ELIF DIAG USE MODIFIER
  else if ((upOne || downOne) && (leftOne || rightOne) && (modA || modB || modC))
  {
    if (modA){
      pinxAxis = 128 + (rightOne - leftOne)*86;
      pinyAxis = 128 + (upOne - downOne)*27;
    }
    if (modB){
      pinxAxis = 128 + (rightOne - leftOne)*27;
      pinyAxis = 128 + (upOne - downOne)*86;
    }
    if (modC){
      pinxAxis = 128 + (rightOne - leftOne)*40;
      pinyAxis = 128 + (upOne - downOne)*60;
    }
 }
 //JUST CARDINAL, USES MODIFIERS DIFFERENTLY
 else
 {
    if (modA){
      pinxAxis = 128 + (rightOne - leftOne)*27;
      pinyAxis = 128 + (upOne - downOne)*27;
    }
    else if (modB){
      pinxAxis = 128 + (rightOne - leftOne)*55;
      pinyAxis = 128 + (upOne - downOne)*55;
    }
    else if (modC){
      pinxAxis = 128 + (rightOne - leftOne)*73;
      pinyAxis = 128 + (upOne - downOne)*73;
    }
    else{
      pinxAxis = 128 + (rightOne - leftOne)*87;
      pinyAxis = 128 + (upOne - downOne)*87;
    }
 }

  //Reads CStick pins, same logic as controlstick values.
  if (digitalRead(CLEFT) == HIGH && digitalRead(CRIGHT) == LOW)pinxAxisC = 255;
  if (digitalRead(CLEFT) == LOW && digitalRead(CRIGHT) == HIGH)pinxAxisC = 0;
  if (digitalRead(CDOWN) == HIGH && digitalRead(CUP) == LOW)pinyAxisC = 255;
  if (digitalRead(CDOWN) == LOW && digitalRead(CUP) == HIGH)pinyAxisC = 0;

  if (digitalRead(A) == LOW)pinA = 1;
  if (digitalRead(B) == LOW)pinB = 1;
  if (digitalRead(X) == LOW)pinX = 1;
  if (digitalRead(Y) == LOW)pinY = 1;
  if (digitalRead(Z) == LOW)pinZ = 1;
  if (digitalRead(START) == LOW)pinSTART = 1;

  //Save/Load state buttons. Press both to taunt.
  if (digitalRead(LSTATE) == LOW && digitalRead(SSTATE) == LOW)pinTaunt = 1;
  else if (digitalRead(LSTATE) == LOW)pinLState = 1;
  else if (digitalRead(SSTATE) == LOW)pinSState = 1;

  //This is for analog shield
  if (digitalRead(RLIGHT) == LOW)pinRLIGHT = RLIGHTv;
  //This is for digital shield
  if (digitalRead(R) == LOW)pinR = 1;
  if (digitalRead(L) == LOW)pinL = 1;
  
  //reports data
  d.report.a = pinA;
  d.report.b = pinB;
  d.report.x = pinX;
  d.report.y = pinY;
  d.report.z = pinZ;
  d.report.start = pinSTART;
  d.report.r = pinR;
  d.report.l = pinL;
  d.report.left = pinRLIGHT;
  d.report.xAxis = pinxAxis;
  d.report.yAxis = pinyAxis;
  d.report.cxAxis = pinxAxisC;
  d.report.cyAxis = pinyAxisC;
  d.report.dup = pinTaunt;
  d.report.dright = pinLState;
  d.report.ddown = 0;
  d.report.dleft = pinSState;

  //sends the complied data to console when console polls for the input
  GamecubeConsole.write(d);

}

