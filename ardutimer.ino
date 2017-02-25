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

void printInitTime(byte *initTime){
  lcd.setCursor(0, 1);
  lcd.print(initTime[0]);
  lcd.print(initTime[1]);
  lcd.print(":");
  lcd.print(initTime[2]);
  lcd.print(initTime[3]);
}

void initTime(){ // Initializing time on startup
  bool timeSet = false;
  byte initTime[4] = {0, 0, 0, 0};
  lcd.print("Enter time:");
  printInitTime(initTime);
  lcd.blink();
  byte curr = 0;
  lcd.setCursor(curr, 1);
  while(!timeSet){
    switch(readLCDButton()){
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
    }
  }
}

void updateDisplay(char updatestr[], byte line){
  clearLine(line);
  lcd.setCursor(0, line);
  lcd.print(updatestr);
}

void clearLine(byte line){
  lcd.setCursor(0, line);
  for(int i = 0; i < 16; i++){
    lcd.print(" ");
  }
}

byte readLCDButton(){ // Reads LCD buttons, varies slightly product to product
  int aButton = analogRead(0);

  if(aButton > 1000){ // Actual value of none was 1023, 1000 to be safe
    return none;
  }else if(aButton < 50){ // Actual value was 0 on my machine
    return right;
  }else if(aButton < 150){ // Actual value - 99
    return up;
  }else if(aButton < 300){ // Actual value - 257
    return down;
  }else if(aButton < 500){ // Actual value - 410
    return left;
  }else{
    return select;
  }
}

void setup() {
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  initTime();

}

void loop() {
  lcd.setCursor(0,1);

}
