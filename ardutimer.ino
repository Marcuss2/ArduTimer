#include <LiquidCrystal.h>
#include <Time.h>
#include <TimeLib.h>



LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

#define UP 0
#define RIGHT 1
#define LEFT 2
#define DOWN 3
#define SELECT 4
#define NONE 5
#define BUZZPIN 6 // Pin with the piezo speaker, must be PWM


int lastButton = NONE;

int secondsLeft = 0;

int minutesLeft = 0;

unsigned long lastUpdate = 0;

int mode = 0; // 0 - Idle, 1 - Ticking DOWN, 2 - Ringing

int pos = 0;

int currentButton = NONE;

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

void initTime() { // Initializing time on startUP
  bool timeSet = false;
  int initTime[4] = {0, 0, 0, 0};
  lcd.print("Enter time:");
  printInitTime(initTime);
  lcd.blink();
  int curr = 0;
  lcd.setCursor(curr, 1);

  while (!timeSet) {
    bool change = true;
    currentButton = readLCDButton();

    if (lastButton != currentButton) {

      switch (currentButton) {
        case UP:
          initTime[curr]++;
          break;

        case DOWN:
          initTime[curr]--;
          break;

        case LEFT:
          curr--;
          break;

        case RIGHT:
          curr++;
          break;

        case SELECT:
          timeSet = true;
          break;

        case NONE:
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

void clearLine(byte line) {
  lcd.setCursor(0, line);
  for (int i = 0; i < 16; i++) {
    lcd.print(" ");
  }
}

void updateDisplay() {
  clearLine(0);
  clearLine(1);
  lcd.setCursor(0, 0);
  lcd.print(hour());
  lcd.print(":");
  if(minute() < 10){
    lcd.print("0");
  }
  lcd.print(minute());
  lcd.print(":");
  if(second() < 10){
    lcd.print("0");
  }
  lcd.print(second());
  lcd.setCursor(0, 1);
  if(minutesLeft < 10){
    lcd.print("0");
  }
  lcd.print(minutesLeft);
  lcd.print(":");
  if(secondsLeft < 10){
    lcd.print("0");
  }
  lcd.print(secondsLeft);
  lcd.setCursor(((pos < 2) ? pos : pos + 1), 1);
}

int readLCDButton() { // Reads LCD buttons, varies slightly product to product
  delay(5);
  int aButton = analogRead(0);

  if (aButton > 1000) { // Actual value of NONE was 1023, 1000 to be safe
    return NONE;
  } else if (aButton < 50) { // Actual value was 0 on my machine
    return RIGHT;
  } else if (aButton < 150) { // Actual value - 99
    return UP;
  } else if (aButton < 300) { // Actual value - 257
    return DOWN;
  } else if (aButton < 500) { // Actual value - 410
    return LEFT;
  } else if (aButton < 700) { // Actual value - 610
    return SELECT;
  }
}

void checkOverflow(){
  if(secondsLeft == -1){
    secondsLeft = 59;
    minutesLeft--;
  }
}

void setup() {
  pinMode(BUZZPIN, OUTPUT);
  noTone(BUZZPIN);
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  initTime();

}

void loop() {
  if(abs(millis() - lastUpdate) > 1000){
    lastUpdate = millis();
    if(mode == 1){
      secondsLeft--;
      if(minutesLeft == 0 && secondsLeft == 0){
        mode = 2;
        tone(BUZZPIN, 1000, 10000);
      }else{
        checkOverflow();
      }
    }
    updateDisplay();
  }

  currentButton = readLCDButton();
  
  if(currentButton != lastButton){
    lastButton = currentButton;
    if(mode == 0){
      switch(currentButton){
        case UP:
          switch(pos){
            case 0:
              minutesLeft = minutesLeft + 10;
              if(minutesLeft > 99){
                minutesLeft = minutesLeft - 100;
              }
            break;
            case 1:
              minutesLeft++;
              if(minutesLeft > 99){
                minutesLeft = minutesLeft - 100;
              }
            break;
            case 2:
              secondsLeft = secondsLeft + 10;
              if(secondsLeft > 59){
                secondsLeft = secondsLeft - 60;
              }
              break;
            case 3:
              secondsLeft++;
              if(secondsLeft > 59){
                secondsLeft = secondsLeft - 60;
              }
              break;
          }
          break;
        case RIGHT:
          pos++;
          if(pos > 3){
            pos = 0;
          }
          break;
        case LEFT:
          pos--;
           if(pos == -1){
            pos = 3;
           }
          break;
        case DOWN:
          switch(pos){
            case 0:
              minutesLeft = minutesLeft - 10;
              if(minutesLeft < 0){
                minutesLeft = minutesLeft + 100;
              }
            break;
            case 1:
              minutesLeft--;
              if(minutesLeft < 0){
                minutesLeft = minutesLeft + 100;
              }
            break;
            case 2:
              secondsLeft = secondsLeft - 10;
              if(secondsLeft < 0){
                secondsLeft = secondsLeft + 60;
              }
              break;
            case 3:
              secondsLeft--;
              if(secondsLeft < 0){
                secondsLeft = secondsLeft + 60;
              }
              break;
          }
          break;
        case SELECT:
          if(secondsLeft != 0 || minutesLeft != 0){
            mode = 1;
            lcd.noBlink();
          }
          break;
      }
    }else{
      if(currentButton == SELECT){
        mode = 0;
        noTone(BUZZPIN);
        lcd.blink();
      }
    }
    updateDisplay();
  }
}
