
#include "HX711.h"
#include "soc/rtc.h"
//#include <Servo.h>
//define sound speed in cm/uS
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701
#define DISTANCIA_MINIMA 2.00
#define DISTANCIA_MAXIMA 10.00
#define OPTICO_PIN 4
#define TRIG_PIN 5
#define ECHO_PIN 18
//#define SERVO_PIN 26 // ESP32 pin GIOP26 connected to servo motor

//Servo servoMotor;
//HX711 scale;

//const int estado_tacho_1 = 15;
int estado = 0;
uint8_t lectura_Optico = 1;
uint8_t lectura_Capacitivo = 0 ;
uint8_t lectura_Inductivo = 0;
//const int trigPin = 5;
//const int echoPin = 18;
long duracion;
float distancia_Cm;
float prueba_distancia;
int estado_optico;
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

float distancia();


void setup() {
  Serial.begin(115200);
  rtc_clk_cpu_freq_set(RTC_CPU_FREQ_80M);
  pinMode(TRIG_PIN, OUTPUT); // Sets the trigPin as an Output para medición de distancia
  pinMode(ECHO_PIN, INPUT); // Sets the echoPin as an Input
  pinMode(OPTICO_PIN,INPUT);

  //ledcSetup(ledChannel, freq, resolution); // Configura el pwm en el pin 17
  
  // attach the channel to the GPIO to be controlled
  //ledcAttachPin(ledPin, ledChannel);
}


void loop() {
  prueba_distancia = distancia();
  Serial.println(prueba_distancia);
 /* digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  // Lee el echoPin, devuelve el tiempo de viaje de la onda de sonido en microsegundos
  duracion = pulseIn(ECHO_PIN, HIGH);*/
  
 
 
  
  // Convert to inches
 // distanceInch = distanceCm * CM_TO_INCH;
  
  // Prints the distance in the Serial Monitor
  
  /*Serial.print("Distance (inch): ");
  Serial.println(distanceInch);*/
  
 // estado = digitalRead(estado_tacho_1); 
 /* estado_optico = digitalRead(OPTICO_PIN);
if(estado_optico == LOW)
  Serial.println("Detecta"); 
  else
  Serial.println("No detecta");// muestro estado del sensor óptico
  */
  
 // changing the LED brightness with PWM
 //   ledcWrite(ledChannel, 127);
 //   delay(15);
 
Serial.println("NO Hay nada");
 while(prueba_distancia >= DISTANCIA_MINIMA && prueba_distancia < DISTANCIA_MAXIMA)
 {
  if (lectura_Optico == 1 && lectura_Capacitivo == 0 && lectura_Inductivo == 0)
  {
    Serial.println("Plástico"); 
    //bandera plastico, crear variable
  }
  else if (lectura_Optico == 0 && lectura_Capacitivo == 0 && lectura_Inductivo == 0)
          {
            Serial.println("PAPEL");
            // bandera papel y carton
          }
          else if(lectura_Optico == 0 && lectura_Capacitivo == 1 && lectura_Inductivo == 0)
                  {
                    Serial.println("Carton");
                    //bandera papel y carton
                  }
                else if(lectura_Optico == 1 && lectura_Capacitivo == 1 && lectura_Inductivo == 0)
                        {
                          Serial.println("vidrio");
                          //bandera vidrio
                        }
                        else if(lectura_Optico == 1 && lectura_Capacitivo == 1 && lectura_Inductivo == 1)
                                {
                                  Serial.println("Metal");
                                  //bandera metal
                                }
                                else {
                                  Serial.println("Este material no se recicla");
                                }
                                prueba_distancia =distancia();
 }
}


float distancia(){
    digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  // Lee el echoPin, devuelve el tiempo de viaje de la onda de sonido en microsegundos
  duracion = pulseIn(ECHO_PIN, HIGH);
  
 
  distancia_Cm = duracion * SOUND_SPEED/2;

  return(distancia_Cm);
}