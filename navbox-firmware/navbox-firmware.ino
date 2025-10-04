#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

// Configure the number of buttons.  Be careful not
// to use a pin for both a digital button and analog
// axis.  The pullup resistor will interfere with
// the analog voltage.
const int numButtons = 28;  // 16 for Teensy, 32 for Teensy++
const int numEncoders = 5;
const int buttonInputStart = 10; //11th pin (pin 10)
const int pushButtonCount = numButtons - buttonInputStart;
byte A1Input = 0;
byte A2Input = 1;
byte B1Input = 2;
byte B2Input = 3;
byte C1Input = 5;
byte C2Input = 4;
byte D1Input = 6;
byte D2Input = 7;
byte E1Input = 8;
byte E2Input = 9;

int ledPin = 13;

byte lastStateA = 0;
byte lastStateB = 0;
byte lastStateC = 0;
byte lastStateD = 0;
byte lastStateE = 0;
byte stepsA = 0;
byte stepsB = 0;
byte stepsC = 0;
byte stepsD = 0;
byte stepsE = 0;
int  cwA = 0;
int  cwB = 0;
int  cwC = 0;
int  cwD = 0;
int cwE = 0;
volatile byte VA1State = 0;
volatile byte VA2State = 0;
volatile byte VB1State = 0;
volatile byte VB2State = 0;
volatile byte VC1State = 0;
volatile byte VC2State = 0;
volatile byte VD1State = 0;
volatile byte VD2State = 0;
volatile byte VE1State = 0;
volatile byte VE2State = 0;
byte A1State = 0;
byte A2State = 0;
byte B1State = 0;
byte B2State = 0;
byte C1State = 0;
byte C2State = 0;
byte D1State = 0;
byte D2State = 0;
byte E1State = 0;
byte E2State = 0;

//byte pinstates[numEncoders*2];
byte AState = 0;
byte BState = 0;
byte CState = 0;
byte DState = 0;
byte EState = 0;
bool ADialMoveRight = false;
bool ADialMoveLeft = false;
bool BDialMoveRight = false;
bool BDialMoveLeft = false;
bool CDialMoveRight = false;
bool CDialMoveLeft = false;
bool DDialMoveRight = false;
bool DDialMoveLeft = false;
bool EDialMoveRight = false;
bool EDialMoveLeft = false;
int ARotateRightButton = 1;
int ARotateLeftButton = 0;
int BRotateRightButton = 3;
int BRotateLeftButton = 2;
int CRotateLeftButton = 5;
int CRotateRightButton = 4;
int DRotateLeftButton = 6;
int DRotateRightButton = 7;
int ERotateLeftButton = 8;
int ERotateRightButton = 9;
int ARightFalseCount = -1;
int ALeftFalseCount = -1;
int BRightFalseCount = -1;
int BLeftFalseCount = -1;
int CRightFalseCount = -1;
int CLeftFalseCount = -1;
int DRightFalseCount = -1;
int DLeftFalseCount = -1;
int ERightFalseCount = -1;
int ELeftFalseCount = -1;

const int globalRotateDelay = 80;
int ARotateResetDelay = 15;
int BRotateResetDelay = 15;
int CRotateResetDelay = 15;
int DRotateResetDelay = globalRotateDelay;
int ERotateResetDelay = 20;
unsigned long ALastTickTime = 0;
unsigned long BLastTickTime = 0;
unsigned long CLastTickTime = 0;
const int AMinimumMsForExtendedMove = 50;
const int BMinimumMsForExtendedMove = 50;
const int CMinimumMsForExtendedMove = 50;
int AExtendedMoveMs = 300;
int BExtendedMoveMs = 300;
int CExtendedMoveMs = 300;

void setup() {
  Serial.begin(9600);
  // configure the joystick to manual send mode.  This gives precise
  // control over when the computer receives updates, but it does
  // require you to manually call Joystick.send_now().
  Joystick.useManualSend(true);
  for (int i = 0; i < numButtons; i++) {
    pinMode(i, INPUT_PULLUP);
  }

  //ISR(PCINT0_vect)
  //{
  //  doChange();
  //}

  //ISR(PCINT1_vect)
  //{
  //  doChange();
  //}

  //rotary encoder A
  pinMode(A1Input, INPUT_PULLUP);
  pinMode(A2Input, INPUT_PULLUP);
  //rotary encoder B
  pinMode(B1Input, INPUT_PULLUP);
  pinMode(B2Input, INPUT_PULLUP);
  //encoder C
  pinMode(C1Input, INPUT_PULLUP);
  pinMode(C2Input, INPUT_PULLUP);
  //encoder D
  pinMode(D1Input, INPUT_PULLUP);
  pinMode(D2Input, INPUT_PULLUP);
  //encoder E
  pinMode(E1Input, INPUT_PULLUP);
  pinMode(E2Input, INPUT_PULLUP);


  //attachInterrupt(digitalPinToInterrupt(A1Input), doChangeA1, RISING);
  //attachInterrupt(digitalPinToInterrupt(A2Input), doChangeA2, RISING);
  //attachInterrupt(digitalPinToInterrupt(A1Input), doChangeA1R, RISING);
  //attachInterrupt(digitalPinToInterrupt(A1Input), doChangeA1F, FALLING);

  Serial.println("Begin Complete Joystick Test");
}

void doChangeA1R()
{
  
}

void doChangeA1F()
{
  
}

void doChangeA1()
{
  VA1State = digitalRead(A1Input);
}

void doChangeA2()
{
  VA2State = digitalRead(A2Input) << 1;
}


byte allButtons[numButtons];
byte prevButtons[numButtons];
int angle = 0;

void loop() {

  encoderA();
  encoderB();
  encoderC();
  encoderD();
  encoderE();

  // read 6 analog inputs and use them for the joystick axis
  //Joystick.X(analogRead(0));
  //Joystick.Y(analogRead(1));
  //Joystick.Z(analogRead(2));
  //Joystick.Zrotate(analogRead(3));
  //Joystick.sliderLeft(analogRead(4));
  //Joystick.sliderRight(analogRead(5));


  // read digital 'button' pins
  for (int i = buttonInputStart; i < numButtons; i++) {

    //if(i != ARotateRightButton && i != ARotateLeftButton) {
    //  if (digitalRead(i)) {
    //    allButtons[i] = 0;
    //  } else {
    //    allButtons[i] = 1;//low is button on (pullup resistor is applied)
    //  }

    if (digitalRead(i)) {
      allButtons[i] = 0;
    } else {
      allButtons[i] = 1;
    }

    if (i != ledPin) {
      Joystick.button(i + 1, allButtons[i]);
    }
  }

  //special case of switches, duplicate negated state
  Joystick.button(29, !allButtons[24]);
  Joystick.button(30, !allButtons[25]);
  Joystick.button(31, !allButtons[26]);
  Joystick.button(32, !allButtons[27]);

 
  for (int i = 0; i < numEncoders * 2; i++)
  {
    if (i != ledPin) {
      Joystick.button(i + 1, allButtons[i]);
    }
  }

  //map encoder values through
  //for(int i = 0;i<numEncoders*2;i++)
  //{
  //  if(digitalRead(i))
  //  {
  //    allButtons[i] = 0;
  //  } else {
  //    allButtons[i] = 1;
  //  }
  //  Joystick.button(i + 1, allButtons[i]);
  //}







  //Joystick.button(i, allButtons[i]);


  Joystick.send_now();

  boolean anyChange = false;
  for (int i = 0; i < numButtons; i++) {
    if (allButtons[i] != prevButtons[i]) anyChange = true;
    prevButtons[i] = allButtons[i];
  }

  if (ADialMoveRight || ADialMoveLeft)
  {
    anyChange = true;
  }

  //if (anyChange) {
  //  Serial.print("Buttons: ");
  //  for (int i=0; i<numButtons; i++) {
  //    Serial.print(allButtons[i], DEC);
  //  }
  //  Serial.println();
  //}

  delay(1);
}

void encoderA() {
  // read the input pin:
  cli();
  //A1State = VA1State; //digitalRead(A1Input);
  //A2State = VA2State; //digitalRead(A2Input) << 1;
  sei();
  A1State = digitalRead(A1Input);
  A2State = digitalRead(A2Input) << 1;

  AState = A1State | A2State;

  bool isExtendedMove = false;
  int currentRotateResetDelay = ARotateResetDelay;

  //AMinimumMsForExtendedMove = 50;
  //AExtendedMoveMs = 200;

  if (lastStateA != AState) {

    //state has changed, is it close enough to last time that we have to extend 'on' time?
    unsigned int nowms = millis();
    if (ALastTickTime > nowms)
    {
      //millis reset to start (once every 50 days)
    } else {
      if ((nowms - ALastTickTime) < AMinimumMsForExtendedMove)
      {
        isExtendedMove = true;
        currentRotateResetDelay = AExtendedMoveMs;
      }
    }
    ALastTickTime = millis();


    switch (AState) {
      case 0:
        if (lastStateA == 2) {
          stepsA++;
          cwA = 1;
          ADialMoveRight = true;
          //ADialMoveLeft = false;
        }
        else if (lastStateA == 1) {
          stepsA--;
          cwA = -1;
          //ADialMoveRight = false;
          ADialMoveLeft = true;
        }
        break;
      case 1:
        if (lastStateA == 0) {
          stepsA++;
          cwA = 1;
          ADialMoveRight = true;
          //ADialMoveLeft = false;
        }
        else if (lastStateA == 3) {
          stepsA--;
          cwA = -1;
          //ADialMoveRight = false;
          ADialMoveLeft = true;
        }
        break;
      case 2:
        if (lastStateA == 3) {
          stepsA++;
          cwA = 1;
          ADialMoveRight = true;
          //ADialMoveLeft = false;
        }
        else if (lastStateA == 0) {
          stepsA--;
          cwA = -1;
          //ADialMoveRight = false;
          ADialMoveLeft = true;
        }
        break;
      case 3:
        if (lastStateA == 1) {
          stepsA++;
          cwA = 1;
          ADialMoveRight = true;
          //ADialMoveLeft = false;
        }
        else if (lastStateA == 2) {
          stepsA--;
          cwA = -1;
          //ADialMoveRight = false;
          ADialMoveLeft = true;
        }
        break;
    }
  }

  lastStateA = AState;
  //Serial.print(AState);
  //Serial.print("\t");
  //Serial.print(cwA);
  //Serial.print("\t");
  //Serial.println(stepsA);


  if (ADialMoveRight == true)
  {
    allButtons[ARotateRightButton] = 1;
    ADialMoveRight = false;
    ADialMoveLeft = false;
    ARightFalseCount = currentRotateResetDelay;
  } else {
    if (ARightFalseCount != -1) {
      ARightFalseCount--;
    }
    if (ARightFalseCount <= 0) {
      allButtons[ARotateRightButton] = 0;
    }
  }
  if (ADialMoveLeft == true)
  {
    allButtons[ARotateLeftButton] = 1;
    ADialMoveLeft = false;
    ADialMoveRight = false;
    ALeftFalseCount = currentRotateResetDelay;
  } else {
    if (ALeftFalseCount != -1) {
      ALeftFalseCount--;
    }
    if (ALeftFalseCount <= 0) {
      allButtons[ARotateLeftButton] = 0;
    }
  }

}

void encoderB() {
  cli();
  //B1State = VA1State; //digitalRead(B1Input);
  //B2State = VA2State; //digitalRead(B2Input) << 1;
  sei();
  B1State = digitalRead(B1Input);
  B2State = digitalRead(B2Input) << 1;

  BState = B1State | B2State;

  bool isExtendedMove = false;
  int currentRotateResetDelay = BRotateResetDelay;

  //BMinimumMsForExtendedMove = 50;
  //BExtendedMoveMs = 200;

  if (lastStateB != BState) {

    //state has changed, is it close enough to last time that we have to extend 'on' time?
    unsigned int nowms = millis();
    if (BLastTickTime > nowms)
    {
      //millis reset to start (once every 50 days)
    } else {
      if ((nowms - BLastTickTime) < BMinimumMsForExtendedMove)
      {
        isExtendedMove = true;
        currentRotateResetDelay = BExtendedMoveMs;
      }
    }
    BLastTickTime = millis();


    switch (BState) {
      case 0:
        if (lastStateB == 2) {
          stepsB++;
          cwB = 1;
          BDialMoveRight = true;
          //BDialMoveLeft = false;
        }
        else if (lastStateB == 1) {
          stepsB--;
          cwB = -1;
          //ADialMoveRight = false;
          BDialMoveLeft = true;
        }
        break;
      case 1:
        if (lastStateB == 0) {
          stepsB++;
          cwB = 1;
          BDialMoveRight = true;
          //BDialMoveLeft = false;
        }
        else if (lastStateB == 3) {
          stepsB--;
          cwB = -1;
          //BDialMoveRight = false;
          BDialMoveLeft = true;
        }
        break;
      case 2:
        if (lastStateB == 3) {
          stepsB++;
          cwB = 1;
          BDialMoveRight = true;
          //BDialMoveLeft = false;
        }
        else if (lastStateB == 0) {
          stepsB--;
          cwB = -1;
          //BDialMoveRight = false;
          BDialMoveLeft = true;
        }
        break;
      case 3:
        if (lastStateB == 1) {
          stepsB++;
          cwB = 1;
          BDialMoveRight = true;
          //BDialMoveLeft = false;
        }
        else if (lastStateB == 2) {
          stepsB--;
          cwB = -1;
          //BDialMoveRight = false;
          BDialMoveLeft = true;
        }
        break;
    }
  }

  lastStateB = BState;
  //Serial.print(BState);
  //Serial.print("\t");
  //Serial.print(cwB);
  //Serial.print("\t");
  //Serial.println(stepsB);


  if (BDialMoveRight == true)
  {
    allButtons[BRotateRightButton] = 1;
    BDialMoveRight = false;
    BDialMoveLeft = false;
    BRightFalseCount = currentRotateResetDelay;
  } else {
    if (BRightFalseCount != -1) {
      BRightFalseCount--;
    }
    if (BRightFalseCount <= 0) {
      allButtons[BRotateRightButton] = 0;
    }
  }
  if (BDialMoveLeft == true)
  {
    allButtons[BRotateLeftButton] = 1;
    BDialMoveLeft = false;
    BDialMoveRight = false;
    BLeftFalseCount = currentRotateResetDelay;
  } else {
    if (BLeftFalseCount != -1) {
      BLeftFalseCount--;
    }
    if (BLeftFalseCount <= 0) {
      allButtons[BRotateLeftButton] = 0;
    }
  }
}

void encoderC() {
  cli();
  //C1State = VA1State; //digitalRead(C1Input);
  //C2State = VA2State; //digitalRead(C2Input) << 1;
  sei();
  C1State = digitalRead(C1Input);
  C2State = digitalRead(C2Input) << 1;

  CState = C1State | C2State;

  bool isExtendedMove = false;
  int currentRotateResetDelay = CRotateResetDelay;

  //CMinimumMsForExtendedMove = 50;
  //CExtendedMoveMs = 200;

  if (lastStateC != CState) {

    //state has changed, is it close enough to last time that we have to extend 'on' time?
    unsigned int nowms = millis();
    if (CLastTickTime > nowms)
    {
      //millis reset to start (once every 50 days)
    } else {
      if ((nowms - CLastTickTime) < CMinimumMsForExtendedMove)
      {
        isExtendedMove = true;
        currentRotateResetDelay = CExtendedMoveMs;
      }
    }
    CLastTickTime = millis();


    switch (CState) {
      case 0:
        if (lastStateC == 2) {
          stepsC++;
          cwC = 1;
          CDialMoveRight = true;
          //CDialMoveLeft = false;
        }
        else if (lastStateC == 1) {
          stepsC--;
          cwC = -1;
          //CDialMoveRight = false;
          CDialMoveLeft = true;
        }
        break;
      case 1:
        if (lastStateC == 0) {
          stepsC++;
          cwC = 1;
          CDialMoveRight = true;
          //CDialMoveLeft = false;
        }
        else if (lastStateC == 3) {
          stepsC--;
          cwC = -1;
          //CDialMoveRight = false;
          CDialMoveLeft = true;
        }
        break;
      case 2:
        if (lastStateC == 3) {
          stepsC++;
          cwC = 1;
          CDialMoveRight = true;
          //CDialMoveLeft = false;
        }
        else if (lastStateC == 0) {
          stepsC--;
          cwC = -1;
          //CDialMoveRight = false;
          CDialMoveLeft = true;
        }
        break;
      case 3:
        if (lastStateC == 1) {
          stepsC++;
          cwC = 1;
          CDialMoveRight = true;
          //CDialMoveLeft = false;
        }
        else if (lastStateC == 2) {
          stepsC--;
          cwC = -1;
          //CDialMoveRight = false;
          CDialMoveLeft = true;
        }
        break;
    }
  }

  lastStateC = CState;
  //Serial.print(CState);
  //Serial.print("\t");
  //Serial.print(cwB);
  //Serial.print("\t");
  //Serial.println(stepsC);


  if (CDialMoveRight == true)
  {
    allButtons[CRotateRightButton] = 1;
    CDialMoveRight = false;
    CDialMoveLeft = false;
    CRightFalseCount = currentRotateResetDelay;
  } else {
    if (CRightFalseCount != -1) {
      CRightFalseCount--;
    }
    if (CRightFalseCount <= 0) {
      allButtons[CRotateRightButton] = 0;
    }
  }
  if (CDialMoveLeft == true)
  {
    allButtons[CRotateLeftButton] = 1;
    CDialMoveLeft = false;
    CDialMoveRight = false;
    CLeftFalseCount = currentRotateResetDelay;
  } else {
    if (CLeftFalseCount != -1) {
      CLeftFalseCount--;
    }
    if (CLeftFalseCount <= 0) {
      allButtons[CRotateLeftButton] = 0;
    }
  }
}

void encoderD() {
  // read the input pin:
  //cli();
  //D1State = VD1State; //digitalRead(D1Input);
  //D2State = VD2State; //digitalRead(D2Input) << 1;
  //sei();
  D1State = digitalRead(D1Input);
  D2State = digitalRead(D2Input) << 1;

  DState = D1State | D2State;

  if (lastStateD != DState) {
    switch (DState) {
      case 0:
        if (lastStateD == 2) {
          stepsD++;
          cwD = 1;
          DDialMoveRight = true;
          //DDialMoveLeft = false;
        }
        else if (lastStateD == 1) {
          stepsD--;
          cwD = -1;
          //DDialMoveRight = false;
          DDialMoveLeft = true;
        }
        break;
      case 1:
        if (lastStateD == 0) {
          stepsD++;
          cwD = 1;
          DDialMoveRight = true;
          //DDialMoveLeft = false;
        }
        else if (lastStateD == 3) {
          stepsD--;
          cwD = -1;
          //DDialMoveRight = false;
          DDialMoveLeft = true;
        }
        break;
      case 2:
        if (lastStateD == 3) {
          stepsD++;
          cwD = 1;
          DDialMoveRight = true;
          //DDialMoveLeft = false;
        }
        else if (lastStateD == 0) {
          stepsD--;
          cwD = -1;
          //DDialMoveRight = false;
          DDialMoveLeft = true;
        }
        break;
      case 3:
        if (lastStateD == 1) {
          stepsD++;
          cwD = 1;
          DDialMoveRight = true;
          //DDialMoveLeft = false;
        }
        else if (lastStateD == 2) {
          stepsD--;
          cwD = -1;
          //DDialMoveRight = false;
          DDialMoveLeft = true;
        }
        break;
    }
  }

  lastStateD = DState;


  if (DDialMoveRight == true)
  {
    allButtons[DRotateRightButton] = 1;
    DDialMoveRight = false;
    DDialMoveLeft = false;
    DRightFalseCount = DRotateResetDelay;
  } else {
    if (DRightFalseCount != -1) {
      DRightFalseCount--;
    }
    if (DRightFalseCount <= 0) {
      allButtons[DRotateRightButton] = 0;
    }
  }
  if (DDialMoveLeft == true)
  {
    allButtons[DRotateLeftButton] = 1;
    DDialMoveLeft = false;
    DDialMoveRight = false;
    DLeftFalseCount = DRotateResetDelay;
  } else {
    if (DLeftFalseCount != -1) {
      DLeftFalseCount--;
    }
    if (DLeftFalseCount <= 0) {
      allButtons[DRotateLeftButton] = 0;
    }
  }
}

void encoderE() {
  // read the input pin:
  //cli();
  //E1State = VE1State; //digitalRead(E1Input);
  //E2State = VE2State; //digitalRead(E2Input) << 1;
  //sei();
  E1State = digitalRead(E1Input);
  E2State = digitalRead(E2Input) << 1;

  EState = E1State | E2State;

  if (lastStateE != EState) {
    switch (EState) {
      case 0:
        if (lastStateE == 2) {
          stepsE++;
          cwE = 1;
          EDialMoveRight = true;
          //EDialMoveLeft = false;
        }
        else if (lastStateE == 1) {
          stepsE--;
          cwE = -1;
          //EDialMoveRight = false;
          EDialMoveLeft = true;
        }
        break;
      case 1:
        if (lastStateE == 0) {
          stepsE++;
          cwE = 1;
          EDialMoveRight = true;
          //EDialMoveLeft = false;
        }
        else if (lastStateE == 3) {
          stepsE--;
          cwE = -1;
          //EDialMoveRight = false;
          EDialMoveLeft = true;
        }
        break;
      case 2:
        if (lastStateE == 3) {
          stepsE++;
          cwE = 1;
          EDialMoveRight = true;
          //EDialMoveLeft = false;
        }
        else if (lastStateE == 0) {
          stepsE--;
          cwE = -1;
          //EDialMoveRight = false;
          EDialMoveLeft = true;
        }
        break;
      case 3:
        if (lastStateE == 1) {
          stepsE++;
          cwE = 1;
          EDialMoveRight = true;
          //EDialMoveLeft = false;
        }
        else if (lastStateE == 2) {
          stepsE--;
          cwE = -1;
          //EDialMoveRight = false;
          EDialMoveLeft = true;
        }
        break;
    }
  }

  lastStateE = EState;


  if (EDialMoveRight == true)
  {
    allButtons[ERotateRightButton] = 1;
    EDialMoveRight = false;
    EDialMoveLeft = false;
    ERightFalseCount = ERotateResetDelay;
  } else {
    if (ERightFalseCount != -1) {
      ERightFalseCount--;
    }
    if (ERightFalseCount <= 0) {
      allButtons[ERotateRightButton] = 0;
    }
  }
  if (EDialMoveLeft == true)
  {
    allButtons[ERotateLeftButton] = 1;
    EDialMoveLeft = false;
    EDialMoveRight = false;
    ELeftFalseCount = ERotateResetDelay;
  } else {
    if (ELeftFalseCount != -1) {
      ELeftFalseCount--;
    }
    if (ELeftFalseCount <= 0) {
      allButtons[ERotateLeftButton] = 0;
    }
  }
}
