#include <Arduino.h>
// semaphores.ino
// Practical ESP32 Multitasking
// Binary Semaphores
#define LED1_GPIO 2
#define LED2_GPIO 32
#define BP 26

bool condition = false;
bool taskGlobal = false;
bool state = true;
SemaphoreHandle_t hsem;
TaskHandle_t taskA;
TaskHandle_t taskB;


void bpTask(void *argp) {
  
  for (;;) {
    
    if(digitalRead(BP) == HIGH && state == true){
      condition = !condition;
      state = false;}
    if(digitalRead(BP) == LOW && state == false ){state = true;}

    if(condition == true && taskGlobal != true){
      digitalWrite(LED1_GPIO , LOW);
      taskGlobal = true;
      vTaskSuspend(taskA);
      vTaskResume(taskB);
      }

    else if(condition == false && taskGlobal != false){
      taskGlobal = false;
      digitalWrite(LED2_GPIO , LOW);
      vTaskSuspend(taskB);
      vTaskResume(taskA);}
   
    } 
 }

void ledA(void *argp) {

  for (;;) { 
    digitalWrite(LED1_GPIO,digitalRead(LED1_GPIO)^1);
    delay(500);
    } 
 }

void ledB(void *argp) {
  
  for (;;) {
    digitalWrite(LED2_GPIO,digitalRead(LED2_GPIO)^1);  
    delay(250);
  }
    
 }
 
void setup() {
  Serial.begin(115200);
  pinMode(LED1_GPIO , OUTPUT);
  pinMode(LED2_GPIO , OUTPUT);
  pinMode(BP,INPUT_PULLDOWN);

  digitalWrite(LED1_GPIO , HIGH);
  digitalWrite(LED2_GPIO , HIGH);
  delay(2000);
  digitalWrite(LED1_GPIO , LOW);
  digitalWrite(LED2_GPIO , LOW);

  int app_cpu = xPortGetCoreID();


  xTaskCreatePinnedToCore(
    ledA, // Function
    "taskA", // Task name
    3000, // Stack size (void*)LED1_GPIO, // arg
    NULL, // arg
    0, // Priority
    &taskA, // No handle returned
    app_cpu); // CPU

  xTaskCreatePinnedToCore(
    ledB, // Function
    "taskB", // Task name
    3000, // Stack size (void*)LED2_GPIO, // argument
    NULL, // argument
    0, // Priority
    &taskB, // No handle returned
    app_cpu); // CPU

  xTaskCreatePinnedToCore(
    bpTask, // Function
    "taskBp", // Task name
    3000, // Stack size (void*)LED1_GPIO, // arg
    NULL, // arg
    0, // Priority
    NULL, // No handle returned
    0); // CPU

    vTaskSuspend(taskB);
    vTaskSuspend(taskA);
}

// Not used
void loop() {
 vTaskDelete(nullptr); 
 }