/*Written by Nick Biederman
 * See github.com/biednick/bads
 * CC BY-SA 4.0
 * 
 * 03MAY18 v0.0: First commit and working code. No comments, serial debugging stuff left in. Still rough, but funtional. There may be some unused variables.
 */

#define CLK 2

#define IN1 3
#define IN2 4
#define IN3 5
#define IN4 6
#define IN5 7
#define IN6 8

#define OUT1 9
#define OUT2 10
#define OUT3 11
#define OUT4 12
#define OUT5 13
#define OUT6 A0

#define CLK_POT A2 //ok
#define MULT_SWT A1 //ok 14/15, 296/297, 545, 780/781, 1023 
#define STEP_SWT A3 //ok
#define CLK_OUT A4 //ok
#define PLAY A5 //ok
#define RECORD A6

int triggers[6][64];
int mult;
int steps;
int beat = 0;
bool triggerState = false;
bool play = true;
bool record = false;
float clkTime;
bool clkState = true;
unsigned long clkChange = false;
unsigned long triggerOn;
unsigned long triggerIndv[6] = {};
float multRaw;

void setup() {
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);
  pinMode(8, INPUT);



  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(A0, OUTPUT);

  pinMode(STEP_SWT, INPUT);
  pinMode(PLAY, INPUT);
  pinMode(RECORD, INPUT);
  pinMode(CLK, INPUT_PULLUP);
  pinMode(CLK_OUT, OUTPUT);
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(CLK), trigger, CHANGE);

}

void loop() {

  clkTime = analogRead(CLK_POT);
  // Serial.println(clkTime);
  if (clkState && (millis() - clkChange > clkTime)) {
    digitalWrite(CLK_OUT, HIGH);
    Serial.println("Hi");
    clkState = !clkState;
    clkChange = millis();
  }
  if (!clkState && millis() - clkChange > clkTime) {
    digitalWrite(CLK_OUT, LOW);
    Serial.println("Lo");
    clkState = !clkState;
    clkChange = millis();
  }

  multRaw = analogRead(MULT_SWT);
 // Serial.println(multRaw);
  mult = scale(multRaw);
  //Serial.println(multRaw);
  if (digitalRead(STEP_SWT) == HIGH) {
    steps = 4;
    // Serial.println('4');
  }
  else {
    steps = 3;
    // Serial.println('3');
  }

  if (digitalRead(PLAY) == LOW) {
    play = false;
    //  Serial.println("Pause");
  }
  else {
    play = true;
  }

  if (digitalRead(RECORD) == LOW) {
    record = true;
     Serial.println("LOW");
  }
  else {
    record = false;
     Serial.println("HIGH");
  }

  if (millis() - triggerOn < 20) {
    triggerOff();
  }
  if (digitalRead(IN1) == HIGH) {
    triggerIndv[0] = millis();
    //Serial.println('1');
    digitalWrite(OUT1, HIGH);
    if (record) {
      triggers[0][beat] = 1;
    }
  }
  if (digitalRead(IN2) == HIGH) {
    triggerIndv[1] = millis();
    digitalWrite(OUT2, HIGH);
    if (record) {
      triggers[1][beat] = 1;
    }
  }
  if (digitalRead(IN3) == HIGH) {
    triggerIndv[2] = millis();
    digitalWrite(OUT3, HIGH);
    if (record) {
      triggers[2][beat] = 1;
    }
  }
  if (digitalRead(IN4) == HIGH) {
    triggerIndv[3] = millis();
    digitalWrite(OUT4, HIGH);
    if (record) {
      triggers[3][beat] = 1;
    }
  }
  if (digitalRead(IN5) == HIGH) {
    triggerIndv[4] = millis();
    digitalWrite(OUT5, HIGH);
    if (record) {
      triggers[4][beat] = 1;
    }
  }
  if (digitalRead(IN6) == HIGH) {
    triggerIndv[5] = millis();
    digitalWrite(OUT6, HIGH);
    if (record) {
      triggers[5][beat] = 1;
    }
  }
  for (int i = 0; i < 6; i++) {
    if (millis() - triggerIndv[i] < 20) {
      digitalWrite(i + 9, LOW);
    }
  }
}

void trigger() {
  Serial.println("Trigger");
  if (play) {
    //Serial.println("Play");
    for (int i = 0; i < 6; i++) {
      if (triggers[i][beat] == 1) {
        digitalWrite(i + 9, HIGH);
      }
      triggerState = true;
      triggerOn = millis();
    }
    beat++;
  }
  if (beat >= (mult * steps)) {
    beat = 0;
  }
 // Serial.println(beat);
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

