#include <LiquidCrystal.h>
#include <Time.h>
#include <TimeLib.h>
#include <SimpleTimer.h>



LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

#define up 0
#define right 1
#define left 2
#define down 3
#define select 4
#define none 5

void printInitTime(byte *initTime) {
  lcd.setCursor(0, 1);
  lcd.print(initTime[0]);
  lcd.print(initTime[1]);
  lcd.print(":");
  lcd.print(initTime[2]);
  lcd.print(initTime[3]);
}

void checkCurr(byte *curr, byte *initTime){
  switch (curr){
    case 0:
      if(initTime[curr] == 3){
        initTime[curr] = 0;
      }else if(initTime[curr] == 255){
        initTime[curr] = 2;
      }
      break;
    case 1:
      if(initTime[0] == 2){
        if(initTime[curr] == 5){
          initTime[curr] = 0;
        }else if(initTime[curr] == 255){
          initTime[curr] = 4;
        }
      }else{
        if(initTime[curr] == 10){
          initTime[curr] = 0;
        }else if(initTime[curr] = 255){
          initTime[curr] = 9;
        }
      }
      break;
    case 2:
      if(initTime[curr] == 7){
        initTime[curr] = 0;
      }else if(initTime[curr] == 255){
        initTime[curr] = 6;
      }
      break;
    case 3:
      if(initTime[curr] == 10){
        initTime[curr] = 0;
      }else if(initTime[curr] = 255){
        initTime[curr] = 9;
    }
  }
}

void initTime() { // Initializing time on startup
  bool timeSet = false;
  byte initTime[4] = {0, 0, 0, 0};
  lcd.print("Enter time:");
  printInitTime(initTime);
  lcd.blink();
  byte curr = 0;
  lcd.setCursor(curr, 1);
  while (!timeSet) {
    bool change = true;
    switch (readLCDButton()) {
      case up:
        initTime[curr]++;
        break;

      case down:
        initTime[curr]--;
        break;

      case left:
        curr--;
        break;

      case right:
        curr++;
        break;

      case select:
        timeSet = true;
        break;

      case none:
        change = false;
        break;
    }

    if(change){
      checkOverflow(curr, initTime);
      printInitTime(initTime);
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

byte readLCDButton() { // Reads LCD buttons, varies slightly product to product
  int aButton = analogRead(0);

  if (aButton > 1000) { // Actual value of none was 1023, 1000 to be safe
    return none;
  } else if (aButton < 50) { // Actual value was 0 on my machine
    return right;
  } else if (aButton < 150) { // Actual value - 99
    return up;
  } else if (aButton < 300) { // Actual value - 257
    return down;
  } else if (aButton < 500) { // Actual value - 410
    return left;
  } else {
    return select;
  }
}

void setup() {
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  initTime();

}

void loop() {
  lcd.setCursor(0, 1);
  
}
