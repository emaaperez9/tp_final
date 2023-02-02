/*************************************************************************************
Librerías
**************************************************************************************/
#include "HX711.h"
#include "soc/rtc.h"
#include <WiFi.h>
//#include <Servo.h>
//define sound speed in cm/uS

/*******************************************************************************************************************
Variables definidas
********************************************************************************************************************/
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701
#define DISTANCIA_MINIMA 2.00
#define DISTANCIA_MAXIMA 10.00
#define OPTICO_PIN 4
#define TRIG_PIN 5
#define ECHO_PIN 18
#define CAPACITIVO_PIN 36
#define INDUCTIVO_PIN 39
#define SERVO_PIN 17 // ESP32 pin GIOP26 connected to servo motor
#define TACHO_1_PIN 15
#define TACHO_2_PIN 7
#define TACHO_3_PIN 8

/**********************************************************************************************
Variables globales
***********************************************************************************************/
uint8_t lectura_Optico;
uint8_t lectura_Capacitivo;
uint8_t lectura_Inductivo;
long duracion;
float distancia_Cm;
float lectura_Distancia;
int habilitar_Servo=0;
int habilitar_tacho_1=0; //PLASTICO - METAL
int habilitar_tacho_2=0; //CARTON - PAPEL
int habilitar_tacho_3=0; //VIDRIO
int posicion_tacho_1;
int posicion_tacho_2;
int posicion_tacho_3;
// setting PWM properties
const int freq = 1000;
const int ledChannel = 0;
const int resolution = 8;
const char* ssid = "NoPreguntes";
const char* password = "kaon8401128A6D6E";


/************************************************************
Funciones
*************************************************************/
float distancia();
void initWiFi();
/************************************************************
Configuración ESP32
*************************************************************/
void setup() {
  Serial.begin(115200);
  rtc_clk_cpu_freq_set(RTC_CPU_FREQ_80M);
  initWiFi();
  pinMode(TRIG_PIN, OUTPUT); // Sets the trigPin as an Output para medición de distancia
  pinMode(ECHO_PIN, INPUT); // Sets the echoPin as an Input
  pinMode(CAPACITIVO_PIN,INPUT_PULLUP);
  pinMode(INDUCTIVO_PIN,INPUT);
  pinMode(TACHO_1_PIN,INPUT);
  pinMode(TACHO_2_PIN,INPUT);
  pinMode(TACHO_3_PIN,INPUT);
  pinMode(SERVO_PIN,OUTPUT);

  //ledcSetup(ledChannel, freq, resolution); // Configura el pwm en el pin 17
  
  // attach the channel to the GPIO to be controlled
  //ledcAttachPin(ledPin, ledChannel);
}


void loop() {
  lectura_Distancia = distancia();
  Serial.println(lectura_Distancia);
  delay(100);

  //Serial.println(distancia());
  lectura_Capacitivo = digitalRead(CAPACITIVO_PIN);
  Serial.print("Capacitivo: ");
  Serial.println(lectura_Capacitivo);
  delay(100);
 
  lectura_Inductivo = digitalRead(INDUCTIVO_PIN);
  Serial.print("Inductivo: ");
  Serial.println(lectura_Inductivo);
  delay(100);
 

  lectura_Optico = digitalRead(OPTICO_PIN);
  Serial.print("OPTICO_PIN: ");
  Serial.println(lectura_Optico);
  delay(100);
  
/* while(lectura_Distancia >= DISTANCIA_MINIMA && lectura_Distancia < DISTANCIA_MAXIMA)
 {
  //habilitar_Servo = 1;
  if (lectura_Optico == 1 && lectura_Capacitivo == 0 && lectura_Inductivo == 0)
  {
    Serial.println("PLASTICO"); 
    habilitar_tacho_1 = 1;
    break;
  }
  else if (lectura_Optico == 0 && lectura_Capacitivo == 0 && lectura_Inductivo == 0)
          {
            Serial.println("PAPEL");
            habilitar_tacho_2 = 1;
            break;
          }
          else if(lectura_Optico == 0 && lectura_Capacitivo == 1 && lectura_Inductivo == 0)
                   {
                     Serial.println("CARTON");
                     habilitar_tacho_2 = 1;
                     break;
                    }
                    else if(lectura_Optico == 1 && lectura_Capacitivo == 1 && lectura_Inductivo == 0)
                          {
                            Serial.println("VIDRIO");
                            habilitar_tacho_3 = 1;
                            break;
                            }
                            else if(lectura_Optico == 1 && lectura_Capacitivo == 1 && lectura_Inductivo == 1)
                                  {
                                    Serial.println("METAL");
                                    habilitar_tacho_1 = 1;
                                    break;
                                  }
                                  else {
                                          Serial.println("Este material no se recicla");
                                          //habilitar_Servo = 0;
                                          break;
                                        }
 
}*/
/********************************************************************************************************************
Luego de determinar el tipo de residuo, debemos mover el tacho n a la posición indicada.
 Cada tacho tendra un final de carrera que indicará si este se encuentra en posición indicada para poder activar 
 el servo y mover el residuo al contenedor correspondiente. Para esto corroboramos que cada tacho corresponda con su 
 final de carrera.
Antes de 
*********************************************************************************************************************/
/*
while(habilitar_Servo == 1)
{
  if (habilitar_tacho_1 == 1 && posicion_tacho_1 == 1 && habilitar_Servo == 1)
  {
    //mover servo 
    habilitar_Servo = 0;
    break;
  }
  else if(habilitar_tacho_2 == 1 && posicion_tacho_2 == 2 && habilitar_Servo == 1)
         {
          //mover servo
          habilitar_Servo = 0;
          break;
         }
          else if(habilitar_tacho_3 == 1 && posicion_tacho_3 == 1 && habilitar_Servo == 1)
                  {
                    //mover servo
                    habilitar_Servo = 0;
                    break;
                  }
  }

}*/
}

/*****************************************************************
Funciones
******************************************************************/
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

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}
