#include <GyverPower.h>
#include <TimerMs.h>

#define PIN_DOOR 2
#define PIN_ACC 6
#define PIN_RELAY_1 10
#define PIN_RELAY_2 11
#define PIN_RELAY_3 12
#define PIN_START 14
#define PIN_BRAKE 15
#define PIN_LED_1 16
#define PIN_LED_2 17
#define INTERVAL_1 6 // time in seconds

TimerMs tmr_led_1(500, 1, 0);
TimerMs tmr_sec(1000, 1, 0);


int ticks = 0;
bool ignition_started = false;
bool led_1_blink = true;
bool led_2_blink = false;

int car_state = 0; // 0 -- no ignition, 1 -- acc, 2 -- ignition, 3 - starter+ignition-acc
void setup() {
  pinMode(PIN_DOOR, INPUT_PULLUP); // door opened
  pinMode(PIN_ACC, INPUT);         // acc
  pinMode(PIN_BRAKE, INPUT);       // brake pedal
  pinMode(PIN_START, INPUT);       // strt-stop button
  pinMode(PIN_RELAY_1, OUTPUT);    // relay 1
  pinMode(PIN_RELAY_2, OUTPUT);    // relay 2
  pinMode(PIN_RELAY_3, OUTPUT);    // relay 3
  pinMode(PIN_LED_1, OUTPUT);       // led 1
  pinMode(PIN_LED_2, OUTPUT);       // led 2
  attachInterrupt(0, wakeup, FALLING);
  power.setSleepMode(POWERDOWN_SLEEP);
}

void loop() {
  if(tmr_led_1.tick()){
    if(led_1_blink){
      digitalWrite(PIN_LED_1, !digitalRead(PIN_LED_1));  
    }
    if(led_2_blink){
      digitalWrite(PIN_LED_2, !digitalRead(PIN_LED_2));
    }
  }
  if(digitalRead(PIN_ACC)){
    ticks = 0;
  }
  if(tmr_sec.tick()){
    ticks++;
  }
  if(digitalRead(PIN_START)){
    if(car_state < 3) {
      car_state++;
    }else{
      car_state = 0;
    }
    switch(car_state){
      case 0:
        led_1_blink = true;
        led_2_blink = false;
        digitalWrite(PIN_RELAY_1, false);
      case 1:
        led_1_blink = false;
        led_2_blink = true;
        digitalWrite(PIN_LED_1, true);
        digitalWrite(PIN_RELAY_1, true);
        break;
      case 2:
        break;
      case 3:
        break;
    }
  }
  if(!digitalRead(PIN_ACC) && ticks >= INTERVAL_1){
    digitalWrite(PIN_LED_1, false);
    digitalWrite(PIN_RELAY_1, false);
    power.sleep(SLEEP_FOREVER);
  }
}

void wakeup() {
  power.wakeUp();
  ticks = 0;
}
