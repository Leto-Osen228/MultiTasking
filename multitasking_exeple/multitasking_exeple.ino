#define BTN1 4
#define BTN2 5
#define LED1 32
#define LED2 33
#define LED3 34

#include "leds_lib.h"
LED red(LED1, true);
LED blue(LED2, true);
LED green(LED3, true);

#include "multitasking.h"
MTask blinkRed((const char*)"red");
MTask blinkBlue((const char*)"blue");
MTask blinkGreen((const char*)"green");

#include "wdt_lib.h"
char thisFun[10];

void setup() {
  blinkRed.attach(fun1);
  blinkRed.startInterval(500);
  
  blinkBlue.attach(fun2);
  blinkBlue.startInterval(200);
  
  blinkGreen.attach(fun3);
  //blinkGreen.startInterval(900);
  
  Serial.begin(115200);

  pinMode(BTN1, INPUT_PULLUP);
  pinMode(BTN2, INPUT_PULLUP);
  wdt_enable(INTERRUPT_RESET_MODE, WDT_PRESCALER_32, wdt_isr);
}

bool btn1, flag1;
bool btn2, flag2;
void loop() {
  blinkRed.tick(thisFun);
  blinkBlue.tick(thisFun);
  blinkGreen.tick(thisFun);
  btn1 = !digitalRead(BTN1);
  if (btn1 == 0 && flag1 == 1){
    flag1 = 0;
    blinkGreen.startTimeOut(1000);
  }else if (btn1 == 1 && flag1 == 0){
    flag1 = 1;
  }
  
  btn2 = !digitalRead(BTN2);
  if (btn2 == 0 && flag2 == 1){
    flag2 = 0;
    blinkBlue.stop();
  }else if (btn2 == 1 && flag2 == 0){
    flag2 = 1;
    blinkBlue.start();
  }

  wdt_reset();
}

void fun1(){
  red.toggle();
  Serial.println(thisFun);
}
void fun2(){
  blue.toggle();
  Serial.println(thisFun);
}
void fun3(){
  green.toggle();
  Serial.println(thisFun);
}

void wdt_isr() {
  Serial.println("!  WARNING  !");
  Serial.println(thisFun);
  Serial.println("!  WARNING  !");
}
