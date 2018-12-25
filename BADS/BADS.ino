#define TRIGGERTIME 20    //Sets length of trigger time
#define DEBOUNCE 250

#define CLK 2             //clock in; must be an interrupt

#define IN1 3             //Inputs for arcade buttons and jacks
#define IN2 4
#define IN3 5
#define IN4 6
#define IN5 7
#define IN6 8

#define OUT1 9            //Outputs wired to jacks and button LED
#define OUT2 10
#define OUT3 11
#define OUT4 12
#define OUT5 13
#define OUT6 A0

#define CLK_POT A2        //Analog pin to set internal clock time
#define MULT_SWT A1       //Analog pin to read rotary switch
#define STEP_SWT A3       //Choose between 3 and 4 steps. Used as digital pin
#define CLK_OUT A4        //Output of clock, which is fed into CLK. Used as digital pin.
#define PLAY A5           //Play pause switch. Used as digital pin.
#define RECORD A6         //Record on/off. Used as analog pin becaouse A6 and A7 cannot be digital.
#define STEP_SWT A7       //Manually step
#define CLEARCHAN 1       //Clear channel. Wired to momentary switch.
#define CLEARALL 0        //clear all. Wired to momentary switch

int triggers[6][64];      //Array of 6 channels holding up to 64 beats
int mult;                 //Multiplier set to 1, 2, 4, 8, or 16. Increaces steps from 3 or 4. Set by MULT_SWT
int steps;                //Steps, 3 or 4. Set by STEP_SWT.
int beat = 0;             //Increments with each intterupt to trigger the correct beat in triggers array
bool triggerState = false; //flag used to turn off triggers after TRIGGERTIME ms
bool play = true;         //play/pause flag
bool record = false;      //record flag
float clkTime;            //Time between clock on and off; set by CLK_POT
bool clkState = true;     //flag to turn clock on and off
unsigned long clkChange;  //Time clock last changed state
unsigned long triggerOn;  //Time trigger turned on, used triggering all channels at once
unsigned long triggerIndv[6] = {}; //time trigger turned on for individual channels, used when playing manually with buttons or external inout
float multRaw;            //Raw data from MULT_SWT
int clearState;           //flag for clearing channels

void setup() {
  pinMode(3, INPUT);    //Set up input pins. Not pullups so they are active high, allowing external inputs to be used.
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);
  pinMode(8, INPUT);



  pinMode(9, OUTPUT);   //Set up outputs for triggers and LEDs
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(A0, OUTPUT);

  pinMode(CLEARCHAN, INPUT_PULLUP); //Set up other inputs. Clear inputs are pulled high, others are not because the switches are wired to ground. This should be changed if ON-OFF switches are used, and logic must be inverted.
  pinMode(CLEARALL, INPUT_PULLUP);
  pinMode(STEP_SWT, INPUT);
  pinMode(PLAY, INPUT);

  pinMode(CLK, INPUT_PULLUP);   //Set up the clock input and outputs.
  pinMode(CLK_OUT, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(CLK), interruptDriver, CHANGE); //Attach clock to hardware interrupt

}

void loop() {

  clearState = digitalRead(CLEARCHAN);   //Get state of clear switch. This is done here rather than in the if statements so it only needs to be read once.

  if (digitalRead(CLEARALL) == LOW) {   //If CLEARALL is active, clear all 6 channels
    for (int i = 0; i < 6; i++) {
      clearChannel(i);
    }
  }
  if (clearState == LOW && digitalRead(IN1) == HIGH) {  //If CLEARCHAN is active, look for activation of a channel. Upon activation, clear that channel. This is done 6 times, once per channel.
    clearChannel(0);
  }
  if (clearState == LOW && digitalRead(IN2) == HIGH) {
    clearChannel(1);
  }
  if (clearState == LOW && digitalRead(IN3) == HIGH) {
    clearChannel(2);
  }
  if (clearState == LOW && digitalRead(IN4) == HIGH) {
    clearChannel(3);
  }
  if (clearState == LOW && digitalRead(IN5) == HIGH) {
    clearChannel(4);
  }
  if (clearState == LOW && digitalRead(IN6) == HIGH) {
    clearChannel(5);
  }


  clkTime = analogRead(CLK_POT);        //Get reading of CLK_POT to set clock time
  if (clkState && (millis() - clkChange > clkTime)) { //Compare last change of CLK to current time and turn on.
    digitalWrite(CLK_OUT, HIGH);
    clkState = !clkState;     //Change clock state
    clkChange = millis();     //Update timer
  }
  if (!clkState && millis() - clkChange > clkTime) {  //Compare last change of CLK to current time and turn off.
    digitalWrite(CLK_OUT, LOW);
    clkState = !clkState;   //Change clock state
    clkChange = millis();   //Update timer
  }

  multRaw = analogRead(MULT_SWT);   //Read rotart switch
  mult = scale(multRaw);   //Pass raw reading from rotary switch to sclae function to set to 1, 2, 4, 8, or 16.


  if (digitalRead(STEP_SWT) == HIGH) {   //Read position of STEP_SWT and set to 3 or 4
    steps = 4;
  }
  else {
    steps = 3;
  }

  if (digitalRead(PLAY) == LOW) {     //Read position of PLAY switch and set flag to true or false
    play = false;
  }
  else {
    play = true;
  }

  if (analogRead(RECORD) > 500) {     //Read position of RECORD and set flag to true or false. Since A6 must be analog, analogRead(A6) > 500 is used rather than digitalRead(A6) == LOW.
    record = true;
  }
  else {
    record = false;
  }

  if (millis() - triggerOn > TRIGGERTIME) {  //Turn off all channels after TRIGGERTIME ms
    triggerOff();
  }

  if (analogRead(STEP_SWT) < 250) {
    trigger(false);
  }

  readButtons();

  for (int i = 0; i < 6; i++) {
    if (millis() - triggerIndv[i] < TRIGGERTIME) {
      digitalWrite(i + 9, LOW);
    }
  }
}

void readButtons() {
  if (digitalRead(IN1) == HIGH && millis() - triggerIndv[0] > DEBOUNCE) {
    triggerIndv[0] = millis();
    digitalWrite(OUT1, HIGH);
    if (record) {
      triggers[0][beat] = 1;
    }
  }
  if (digitalRead(IN2) == HIGH && millis() - triggerIndv[1] > DEBOUNCE) {
    triggerIndv[1] = millis();
    digitalWrite(OUT2, HIGH);
    if (record) {
      triggers[1][beat] = 1;
    }
  }
  if (digitalRead(IN3) == HIGH && millis() - triggerIndv[2] > DEBOUNCE) {
    triggerIndv[2] = millis();
    digitalWrite(OUT3, HIGH);
    if (record) {
      triggers[2][beat] = 1;
    }
  }
  if (digitalRead(IN4) == HIGH && millis() - triggerIndv[3] > DEBOUNCE) {
    triggerIndv[3] = millis();
    digitalWrite(OUT4, HIGH);
    if (record) {
      triggers[3][beat] = 1;
    }
  }
  if (digitalRead(IN5) == HIGH && millis() - triggerIndv[4] > DEBOUNCE) {
    triggerIndv[4] = millis();
    digitalWrite(OUT5, HIGH);
    if (record) {
      triggers[4][beat] = 1;
    }
  }
  if (digitalRead(IN6) == HIGH && millis() - triggerIndv[5] > DEBOUNCE) {
    triggerIndv[5] = millis();
    digitalWrite(OUT6, HIGH);
    if (record) {
      triggers[5][beat] = 1;
    }
  }
}

void interruptDriver() {
  trigger(true);
}

void trigger(bool interruptCall) {
  if (!play && interruptCall) {
    return;
  }
  for (int i = 0; i < 6; i++) {
    if (triggers[i][beat] == 1) {
      digitalWrite(i + 9, HIGH);
    }
    triggerState = true;
  }
  beat++;

  if (beat >= (mult * steps)) {
    beat = 0;
  }

  triggerOn = millis();
}

void triggerOff() {
  for (int i = 9; i < 15; i++)
    digitalWrite(i, LOW);
}

int scale(int multRaw) {
  if (multRaw < 200)
    return 1;
  if (multRaw < 300)
    return 2;
  if (multRaw < 600)
    return 4;
  if (multRaw < 800)
    return 8;
  if (multRaw < 1024)
    return 16;
}

void clearChannel(int toClear) {
  for (int i = 0; i < 64; i++) {
    triggers[toClear][i] = 0;
  }
}
