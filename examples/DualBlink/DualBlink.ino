#include "TaskScheduler.h"

int led1 = 12;
int led2 = 13;

void setup() {
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  
  scheduler.Begin(blink1, blink2);
  scheduler.SetTimers(250, 500);

}

void loop() {
  scheduler.Run();
}

void blink1() {
  digitalWrite(led1, !digitalRead(led1));
}

void blink2() {
  digitalWrite(led2, !digitalRead(led2));
}
