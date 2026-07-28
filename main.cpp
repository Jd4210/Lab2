// Universidad del Valle de Guatemala//
// BE3029 Electrónica Digital II//
// Jorge Diab 241253//
// Lab 2//

#include <Arduino.h>
#include <stdio.h>
#include <stdint.h>
#include <driver/gpio.h>

//Variables
//Touch
const int touchPin = 4;
//Leds
const int LED = 23;
const int LED2 = 22;
const int LED3 = 1;
const int LED4 = 3;
const int LED5 = 21;
const int LED6 = 19;
const int LED7 = 18;
const int LED8 = 5;
const int LED9 = 17;


//Botones
int boton = 2;
int boton2 = 15;

//Variables de control
int num = 0;
int num2 = 0;
bool estadoLED9 = false;

int alarma = 25000; //0.25 segundos


// put function declarations here:
void configTimer(void);
void IRAM_ATTR TimerISR();


//ISR para aumentar el contador manual 
void IRAM_ATTR subida(){
  //cada alarma
  num++;
  if ( num > 15) {
    num = 0;
  }
}
//ISR para disminuir el contador manual
void IRAM_ATTR bajada(){
  //cada alarma
  num--;
  if ( num < 0) {
    num = 15;
  }
}

//Instanciar timer1
hw_timer_t * Timer1 = NULL;



//ISR del timer1
void IRAM_ATTR TimerISR(){
  num2++;
  if ( num2 > 15) {
    num2 = 0;
  }
  
}




void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
  //**Pins**
  //LEDS
  //Contador manual
  pinMode(LED, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);

  
  //Contador automático
  pinMode(LED5, OUTPUT);
  pinMode(LED6, OUTPUT);
  pinMode(LED7, OUTPUT);
  pinMode(LED8, OUTPUT);

  //LED alarma
  pinMode(LED9, OUTPUT);


  digitalWrite(LED, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);

  digitalWrite(LED5, LOW);
  digitalWrite(LED6, LOW);
  digitalWrite(LED7, LOW);
  digitalWrite(LED8, LOW);

  digitalWrite(LED9, LOW);




  //Botones
  pinMode(boton, INPUT_PULLDOWN);
  pinMode(boton2, INPUT_PULLDOWN);
  
  //ATTACH INTERRUPT
  attachInterrupt(digitalPinToInterrupt(boton), subida, RISING);

  attachInterrupt(digitalPinToInterrupt(boton2), bajada, RISING);


  //Configurar timer1
  configTimer();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(touchRead(touchPin) < 40){
  num2 = 0;
}

 //manual
  digitalWrite(LED, bitRead(num, 0));
  digitalWrite(LED2, bitRead(num, 1));
  digitalWrite(LED3, bitRead(num, 2));
  digitalWrite(LED4, bitRead(num, 3));


  //automatico
  digitalWrite(LED5, bitRead(num2, 0));
  digitalWrite(LED6, bitRead(num2, 1));
  digitalWrite(LED7, bitRead(num2, 2));
  digitalWrite(LED8, bitRead(num2, 3));


  //comparar los contadores y encender el LED de alarma
  if(num == num2){
    estadoLED9 = !estadoLED9;
    digitalWrite(LED9, estadoLED9); 
    num2 = 0; //reiniciar el contador automatico
}

}



// put function definitions here:
void configTimer(void){
  //Configurar Timer1
  Timer1 = timerBegin(0, 800, true);
  //Definir ISR
  timerAttachInterrupt(Timer1, &TimerISR, true);
  //defnir alarma
  timerAlarmWrite(Timer1, alarma, true);
  //iniciar la alarma
  timerAlarmEnable(Timer1);
}

