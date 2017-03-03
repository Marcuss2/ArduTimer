#include <LiquidCrystal.h>
#include <Time.h>
#include <TimeLib.h>



LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

#define up 0
#define right 1
#define left 2
#define down 3
#define select 4
#define none 5

int lastButton = none;



void printInitTime(int *initTime) {
  lcd.setCursor(0, 1);
  lcd.print(initTime[0]);
  lcd.print(initTime[1]);
  lcd.print(":");
  lcd.print(initTime[2]);
  lcd.print(initTime[3]);
}

int checkCurr(int curr, int *initTime) {

  switch (curr) {
    case -1:
      curr = 3;
      break;
    case 0:
      if (initTime[curr] == 3) {
        initTime[curr] = 0;
      } else if (initTime[curr] == -1) {
        initTime[curr] = 2;
      }

      if(initTime[curr] == 2 && initTime[1] > 3){
        initTime[1] = 3;
      }
      break;
    case 1:
      if (initTime[0] == 2) {
        if (initTime[curr] > 3) {
          initTime[curr] = 0;
        } else if (initTime[curr] == -1) {
          initTime[curr] = 3;
        }
      } else {
        if (initTime[curr] == 10) {
          initTime[curr] = 0;
        } else if (initTime[curr] == -1) {
          initTime[curr] = 9;
        }
      }
      break;
    case 2:
      if (initTime[curr] == 6) {
        initTime[curr] = 0;
      } else if (initTime[curr] == -1) {
        initTime[curr] = 5;
      }
      break;
    case 3:
      if (initTime[curr] == 10) {
        initTime[curr] = 0;
      } else if (initTime[curr] == -1) {
        initTime[curr] = 9;
      }
      break;
    case 4:
      curr = 0;
      break;
  }
  return curr;
}

void initTime() { // Initializing time on startup
  bool timeSet = false;
  int initTime[4] = {0, 0, 0, 0};
  lcd.print("Enter time:");
  printInitTime(initTime);
  lcd.blink();
  int curr = 0;
  lcd.setCursor(curr, 1);

  while (!timeSet) {
    bool change = true;
    int currentButton = readLCDButton();

    if (lastButton != currentButton) {

      switch (currentButton) {
        case up:
          initTime[curr]++;
          Serial.print("\n UP was pressed ");
          Serial.print(curr);
          break;

        case down:
          initTime[curr]--;
          Serial.print("\n DOWN was pressed ");
          Serial.print(curr);
          break;

        case left:
          curr--;
          Serial.print("\n LEFT was pressed ");
          Serial.print(curr);
          break;

        case right:
          curr++;
          Serial.print("\n RIGHT was pressed ");
          Serial.print(curr);
          break;

        case select:
          timeSet = true;
          Serial.print("\n SELECT was pressed ");
          break;

        case none:
          change = false;
          break;
      }
      lastButton = currentButton;

      curr = checkCurr(curr, initTime);
      printInitTime(initTime);
      lcd.setCursor(((curr < 2) ? curr : curr + 1), 1);
      
    }

  }
  setTime((initTime[0] * 10) + initTime[1], (initTime[2] * 10) + initTime[3], 0, 1, 1, 2017);
}

void updateDisplay() {
  clearLine(0);
  clearLine(1);
}

void clearLine(byte line) {
  lcd.setCursor(0, line);
  for (int i = 0; i < 16; i++) {
    lcd.print(" ");
  }
}

int readLCDButton() { // Reads LCD buttons, varies slightly product to product
  delay(5);
  int aButton = analogRead(0);

  if (aButton > 1000) { // Actual value of none was 1023, 1000 to be safe
    //Serial.print(none);
    return none;
  } else if (aButton < 50) { // Actual value was 0 on my machine
    //Serial.print(right);
    return right;
  } else if (aButton < 150) { // Actual value - 99
    //Serial.print(up);
    return up;
  } else if (aButton < 300) { // Actual value - 257
    //Serial.print(down);
    return down;
  } else if (aButton < 500) { // Actual value - 410
    //Serial.print(left);
    return left;
  } else if (aButton < 700) { // Actual value - 610
    //Serial.print(select);
    return select;
  }
}

void setup() {
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  Serial.begin(9600);
  initTime();

}

void loop() {
  lcd.setCursor(0, 1);

}
