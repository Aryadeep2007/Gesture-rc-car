// HW-130 / L293D shield (Adafruit Motor Shield V1 layout)
const int MOTORLATCH  = 12;
const int MOTORCLK    = 4;
const int MOTORENABLE = 7;
const int MOTORDATA   = 8;

const int BITS[4][2] = { {2, 3}, {1, 4}, {5, 7}, {0, 6} };
const int PWM_PIN[4] = { 11, 3, 6, 5 };
const bool INVERT[4] = { true, true, false, true };

const int SPEED = 150;   // 0-255, keep modest for testing

uint8_t latchState = 0;

void latchTx() {
  digitalWrite(MOTORLATCH, LOW);
  shiftOut(MOTORDATA, MOTORCLK, MSBFIRST, latchState);
  digitalWrite(MOTORLATCH, HIGH);
}

void motor(int m, int dir, int speed) {
  if (INVERT[m]) dir = -dir;
  latchState &= ~((1 << BITS[m][0]) | (1 << BITS[m][1]));
  if (dir > 0) latchState |= (1 << BITS[m][0]);
  if (dir < 0) latchState |= (1 << BITS[m][1]);
  latchTx();
  analogWrite(PWM_PIN[m], speed);
}

// left/right: -255 (full backward) to 255 (full forward)
void drive(int left, int right) {
  int ld = (left > 0) - (left < 0);
  int rd = (right > 0) - (right < 0);
  int ls = abs(left);
  int rs = abs(right);
  motor(1, ld, ls);   // M2 is now LEFT
  motor(2, ld, ls);   // M3 is now LEFT
  motor(0, rd, rs);   // M1 is now RIGHT
  motor(3, rd, rs);   // M4 is now RIGHT
}

void setup() {
  Serial.begin(115200);
  pinMode(MOTORLATCH, OUTPUT);
  pinMode(MOTORCLK, OUTPUT);
  pinMode(MOTORENABLE, OUTPUT);
  pinMode(MOTORDATA, OUTPUT);
  for (int i = 0; i < 4; i++) pinMode(PWM_PIN[i], OUTPUT);
  digitalWrite(MOTORENABLE, LOW);
  drive(0, 0);
  Serial.println("Ready. w=forward s=backward a=left d=right x=stop");
}

unsigned long lastRefresh = 0;

void loop() {
  if (Serial.available() > 0) {
    char c = Serial.read();
    switch (c) {
      case 'w': drive(SPEED, SPEED);   Serial.println("FORWARD");  break;
      case 's': drive(-SPEED, -SPEED); Serial.println("BACKWARD"); break;
      case 'a': drive(-SPEED, SPEED);  Serial.println("LEFT");     break;
      case 'd': drive(SPEED, -SPEED);  Serial.println("RIGHT");    break;
      case 'x': drive(0, 0);           Serial.println("STOP");     break;
    }
  }

  // Re-send the shift register every 50 ms to repair noise glitches
  if (millis() - lastRefresh >= 50) {
    latchTx();
    lastRefresh = millis();
  }
}