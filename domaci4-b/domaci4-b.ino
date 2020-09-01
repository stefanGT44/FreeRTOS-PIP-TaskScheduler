#include <Arduino_FreeRTOS.h>

int LEDS[] = {5, 6, 7};
Task tasks[5];
Sem sems[5];
/*

3 3 2 1 2 1 2

 */


void setup() {
  Serial.begin(9600);
  while (1){
    if (Serial.available()){
      int num = Serial.parseInt();
      Serial.read();

      for (int i = 0; i < num; i++){
        tasks[i].priority = Serial.parseInt();
        tasks[i].priority++;
        tasks[i].activeLED = LEDS[i];
        Serial.read();
      }

      int semNum = Serial.parseInt();
      Serial.read();

      for (int i = 0; i < semNum; i++){
        sems[i].queueCount = 0;
        sems[i].locked = 0;
        Serial.read();
      }

      for (int i = 0; i < num; i++){
        pinMode(LEDS[i], OUTPUT);
      }

      createMyTask(&tasks[2], led_task1, 200);
      createMyTask(&tasks[1], led_task2, 200);
      createMyTask(&tasks[0], led_task3, 200);

      break;
    }
  }
}

static void led_task1(void* pvParameters){
  Task *task = pvParameters;
  int flag = 0;
  int flag2 = 0;
  int flag3 = 0;
  int flag4 = 0;
  while(1){

    if((byte)task->activeLED < 8){
      PORTD = 1 << (byte)task->activeLED;
      PORTB = 0;
    }
    else {
      PORTB = 1 << (byte)task->activeLED - 8;
      PORTD = 0;
    }

    if (flag2 == 0){
      pi_wait(&sems[0]);
      flag2 = 1;
    }

    if (flag == 0 && getCurrentTaskRunTime() >= 2){
      startTask(1);
      flag = 1;
    }

    if (flag3 == 0 && getCurrentTaskRunTime() >= 4){
      startTask(0);
      flag3 = 1;
    }

    if (flag4 == 0 && getCurrentTaskRunTime() >= 6){
      pi_signal(&sems[0]);
      flag4 = 1;
    }
    
  }
}

static void led_task2(void* pvParameters){
  Task *task = pvParameters;
  int flag = 0;
  int flag2 = 0;
  int flag3 = 0;
  int flag4 = 0;
  while(1){
    
    if((byte)task->activeLED < 8){
      PORTD = 1 << (byte)task->activeLED;
      PORTB = 0;
    }
    else {
      PORTB = 1 << (byte)task->activeLED - 8;
      PORTD = 0;
    }

     if (flag == 0){
      pi_wait(&sems[1]);
      flag = 1;
     }

     if (flag2 == 0 && getCurrentTaskRunTime() >= 2){
      pi_wait(&sems[0]);
      flag2 = 1;
     }

     if (flag3 == 0 && getCurrentTaskRunTime() >= 4){
      pi_signal(&sems[0]);
      flag3 = 1;
     }

     if (flag4 == 0 && getCurrentTaskRunTime() >= 6){
      pi_signal(&sems[1]);
      flag4 = 1;
     }

     if (getCurrentTaskRunTime() >= 8)
      endTask();
     
  }
}

static void led_task3(void* pvParameters){
  Task *task = pvParameters;
  int flag = 0;
  int flag1 = 0;

  while(1){
    
    if((byte)task->activeLED < 8){
      PORTD = 1 << (byte)task->activeLED;
      PORTB = 0;
    }
    else {
      PORTB = 1 << (byte)task->activeLED - 8;
      PORTD = 0;
    }

    if (flag == 0 && getCurrentTaskRunTime() >= 2){
      pi_wait(&sems[1]);
      flag = 1;
    }

    if (flag1 == 0 && getCurrentTaskRunTime() >= 4){
      pi_signal(&sems[1]);
      flag1 = 1;
    }

    if (getCurrentTaskRunTime() >= 6)
      endTask();
    
  }
}

void loop() {
}
