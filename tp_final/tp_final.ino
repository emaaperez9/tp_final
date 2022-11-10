
#include "HX711.h"
#include "soc/rtc.h"
//#include <Servo.h>
//define sound speed in cm/uS
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701
#define DISTANCIA_MINIMA 2.00
#define DISTANCIA_MAXIMA 10.00
#define OPTICO_PIN 4
//#define SERVO_PIN 26 // ESP32 pin GIOP26 connected to servo motor

//Servo servoMotor;
//HX711 scale;

//const int estado_tacho_1 = 15;
int estado = 0;
const int optico_pin = 4;
uint8_t lectura_Optico = 1;
uint8_t lectura_Capacitivo = 0 ;
uint8_t lectura_Inductivo = 0;
const int trigPin = 5;
const int echoPin = 18;
long duracion;
float distancia_Cm;
//float distanceInch;
// the number of the LED pin
const int ledPin = 16;  // 16 corresponds to GPIO16

// setting PWM properties
const int freq = 1000;
const int ledChannel = 0;
const int resolution = 8;
// HX711 circuit wiring
/*const int LOADCELL_DOUT_PIN = 16;
const int LOADCELL_SCK_PIN = 4;*/



void setup() {
  Serial.begin(115200);
  rtc_clk_cpu_freq_set(RTC_CPU_FREQ_80M);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output para medición de distancia
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(optico_pin,INPUT);

  //ledcSetup(ledChannel, freq, resolution); // Configura el pwm en el pin 17
  
  // attach the channel to the GPIO to be controlled
  //ledcAttachPin(ledPin, ledChannel);
}


void loop() {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Lee el echoPin, devuelve el tiempo de viaje de la onda de sonido en microsegundos
  duration = pulseIn(echoPin, HIGH);
  
  // Calcula la distancia
  distanceCm = duracion * SOUND_SPEED/2;
  
  // Convert to inches
 // distanceInch = distanceCm * CM_TO_INCH;
  
  // Prints the distance in the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distancia_Cm);
  /*Serial.print("Distance (inch): ");
  Serial.println(distanceInch);*/
  
 // estado = digitalRead(estado_tacho_1); 
  estado_optico = digitalRead(optico_pin);
if(estado_optico == LOW)
  Serial.println("Detecta"); 
  else
  Serial.println("No detecta");// muestro estado del sensor óptico
  
  
 // changing the LED brightness with PWM
 //   ledcWrite(ledChannel, 127);
 //   delay(15);

 while(distancia_Cm >= DISTANCIA_MINIMA && distancia_Cm < DISTANCIA_MAXIMA)
 {
  if (lectura_Optico == 1 && lectura_Capacitivo == 0 && lectura_Inductivo == 0)
  {
    Serial.println("Plástico"); 
  }
  else if (lectura_Optico == 0 && lectura_Capacitivo == 0 && lectura_Inductivo == 0)
          {
            Serial.println("PAPEL");
          }
          else if(lectura_Optico == 0 && lectura_Capacitivo == 1 && lectura_Inductivo == 0)
                  {
                    Serial.println("Carton");
                  }
                else if(lectura_Optico == 1 && lectura_Capacitivo == 1 && lectura_Inductivo == 0)
                        {
                          Serial.println("vidrio");
                        }
                        else if(lectura_Optico == 1 && lectura_Capacitivo == 1 && lectura_Inductivo == 1)
                                {
                                  Serial.println("Metal");
                                }
                                else {
                                  Serial.println("Este material no se recicla")
                                }
 }                                
  





/*
if(estado == LOW)
  Serial.println("Ocupado"); 
  else
  Serial.println("vacío");// muestro estado

  scale.power_down();             // put the ADC in sleep mode
  delay(1500);
  scale.power_up();*/

  /* // rotates from 0 degrees to 180 degrees
    for (int pos = 0; pos <= 180; pos += 1) {
    // in steps of 1 degree
    servoMotor.write(pos);
    delay(15); // waits 15ms to reach the position
  }

  // rotates from 180 degrees to 0 degrees
  for (int pos = 180; pos >= 0; pos -= 1) {
    servoMotor.write(pos);
    delay(15); // waits 15ms to reach the position
  }
*/
}
