/*************************************************************************************
Librerías
**************************************************************************************/
#include "HX711.h"
#include "soc/rtc.h"
#include <WiFi.h>
//#include <ESP32Servo.h>
//#include <LiquidCrystal_I2C.h>
#include <Servo.h>
//define sound speed in cm/uS

/*******************************************************************************************************************
Variables definidas
********************************************************************************************************************/
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701
#define DISTANCIA_MINIMA 2.00
#define DISTANCIA_MAXIMA 10.00
#define OPTICO_PIN 15
#define TRIG_PIN 18
#define ECHO_PIN 5
#define CAPACITIVO_PIN 13
#define INDUCTIVO_PIN 4
#define SERVO_PIN 17 // ESP32 pin 17 connected to servo motor
#define PIN_FINAL_CARRERA_INICIAL 25
#define PIN_FINAL_CARRERA_MEDIO 26
#define PIN_FINAL_CARRERA_FINAL 27
#define DISPLAY_SDA 21
#define DISPLAY_SCL 22
#define PIN_DIRECCION 23
#define PIN_PASOS 19
#define LCD_COLUMNAS 16
#define LCD_FILAS 2
#define VELOCIDAD 25
#define DIRECCION_INICIAL 1
#define DIRECCION_FINAL   0
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
uint8_t posicion_tacho_1;
uint8_t posicion_tacho_2;
uint8_t posicion_tacho_3;
// setting PWM properties
const int freq = 1000;

const int ledChannel = 0;
const int resolution = 8;
const char* ssid = "NoPreguntes";
const char* password = "kaon8401128A6D6E";
Servo myservo;
int pos = 0;
//LiquidCrystal_I2C lcd(0x27, LCD_COLUMNAS, LCD_FILAS); 
int  gi_pin_mover_inicial;
int  gi_pin_mover_final;

int  gi_valor_final_carrera_inicial;
int  gi_valor_final_carrera_final;


/************************************************************
Funciones
*************************************************************/
float distancia();
void MueveMotorPosicionFinal();
void MueveMotorPosicionInicial();
//void initWiFi();
//void abrirCesto();
//void cerrarCesto();
/************************************************************
Configuración ESP32
*************************************************************/
void setup() {
  Serial.begin(115200);
  rtc_clk_cpu_freq_set(RTC_CPU_FREQ_80M);
  //initWiFi();
  pinMode(TRIG_PIN, OUTPUT); // Sets the trigPin as an Output para medición de distancia
  pinMode(ECHO_PIN, INPUT); // Sets the echoPin as an Input
  pinMode(CAPACITIVO_PIN,INPUT);
  pinMode(INDUCTIVO_PIN,INPUT);
  pinMode(PIN_PASOS, OUTPUT);
  pinMode(PIN_DIRECCION, OUTPUT);
  pinMode(PIN_FINAL_CARRERA_INICIAL, INPUT);
  pinMode(PIN_FINAL_CARRERA_FINAL, INPUT);
  pinMode(SERVO_PIN,OUTPUT);
  pinMode(OPTICO_PIN,INPUT);
  //servo.attach(SERVO_PIN, 300, 3000);
  myservo.attach(SERVO_PIN);
  //MueveMotorPosicionInicial();
  //lcd.begin();    // initialize LCD                 
  //lcd.backlight(); // turn on LCD backlight

  //ledcSetup(ledChannel, freq, resolution); // Configura el pwm en el pin 17
  
  // attach the channel to the GPIO to be controlled
  //ledcAttachPin(ledPin, ledChannel);
}


void loop() {
  lectura_Distancia = distancia();
  Serial.println(lectura_Distancia);
  delay(50);
  //Serial.println(distancia());

  lectura_Capacitivo = digitalRead(CAPACITIVO_PIN);
  Serial.print("Capacitivo: ");
  Serial.println(lectura_Capacitivo);
  delay(50);
 
  lectura_Inductivo = digitalRead(INDUCTIVO_PIN);
  Serial.print("Inductivo: ");
  Serial.println(lectura_Inductivo);
  delay(50);

  lectura_Optico = digitalRead(OPTICO_PIN);
  Serial.print("OPTICO_PIN: ");
  Serial.println(lectura_Optico);
  delay(50);
  /*posicion_tacho_1 = digitalRead(TACHO_1_PIN);
  Serial.print("TACHO_PIN_1: ");
  Serial.println(posicion_tacho_1);
  delay(50);

  posicion_tacho_2 = digitalRead(TACHO_2_PIN);
  Serial.print("TACHO_PIN_2: ");
  Serial.println(posicion_tacho_2);
  delay(50);

  posicion_tacho_3 = digitalRead(TACHO_3_PIN);
  Serial.print("TACHO_PIN_3: ");
  Serial.println(posicion_tacho_3);
  delay(50);
  lcd.setCursor(0, 0);
  lcd.print("Puto el que lee");
  lcd.setCursor(0, 1);
  lcd.print("MATAME EL BICHO");
*/
MueveMotorPosicionInicial();
if (lectura_Distancia >= DISTANCIA_MINIMA && lectura_Distancia < DISTANCIA_MAXIMA) {
}

 while(lectura_Distancia >= DISTANCIA_MINIMA && lectura_Distancia < DISTANCIA_MAXIMA)
 {
  
  if (lectura_Optico == 0 && lectura_Capacitivo == 0 && lectura_Inductivo == 0)
  {
    Serial.println("PLASTICO"); 
    //habilitar_tacho_1 = 1;
    tiraresiduo();
    break;
  }
  else if (lectura_Optico == 0 && lectura_Capacitivo == 0 && lectura_Inductivo == 0)// el optico 0 detecta ppapel/carton!!!
          {
            Serial.println("PAPEL");
            habilitar_tacho_2 = 1;
            //habilitar_Servo = 1;
            tiraresiduo();
            break;
          }
          else if(lectura_Optico == 0 && lectura_Capacitivo == 1 && lectura_Inductivo == 0)
                   {
                     Serial.println("CARTON");
                     MueveMotorPosicionFinal();
                     //habilitar_tacho_2 = 1;
                     //habilitar_Servo = 1;
                     tiraresiduo();
                     break;
                    }
                    else if(lectura_Optico == 1 && lectura_Capacitivo == 1 && lectura_Inductivo == 0)/// el optico en 1 indica que no detecta nada
                          {
                            Serial.println("VIDRIO");
                            MueveMotorPosicionInicial();
                            //habilitar_tacho_3 = 1;
                            //habilitar_Servo = 1;
                            tiraresiduo();
                            break;
                            }
                            else if(/*lectura_Optico == 0 && lectura_Capacitivo == 1 && */lectura_Inductivo == 1)
                                  {
                                    Serial.println("METAL");

                                    habilitar_tacho_1 = 1;
                                    MueveMotorPosicionFinal();
                                   
                                    //habilitar_Servo = 1;
                                    tiraresiduo();
                                    break;
                                  }
                                  else {
                                         Serial.println("Este material no se recicla");
                                          habilitar_Servo = 0;
                                          break;
                                        }

}
/********************************************************************************************************************
Luego de determinar el tipo de residuo, debemos mover el tacho n a la posición indicada.
 Cada tacho tendra un final de carrera que indicará si este se encuentra en posición indicada para poder activar 
 el servo y mover el residuo al contenedor correspondiente. Para esto corroboramos que cada tacho corresponda con su 
 final de carrera.
Antes de 
*********************************************************************************************************************/
/*while(habilitar_Servo == 1)
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

/*void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}*/
void tiraresiduo(){
  //moviemiento del PaP
  abrirCesto();
  //lcd.clear();
 // lcd.display("abrió");
  delay(500);
  cerrarCesto();
  //lcd.clear();
  //lcd.display("cerró");
  delay(50);
}
void abrirCesto(){
    for (pos = 360; pos <= 0; pos -= 1) {
    //Movemos el servo a los grados que le entreguemos
    myservo.write(pos);
    //Esperamos 18 milisegundos
    delay(5);
  }
  
}
void cerrarCesto(){
  for (pos = 0; pos >= 360; pos += 1) {
    //Movemos el servo a los grados que le entreguemos
    myservo.write(pos);
    //Esperamos 15 milisegundos
    delay(5);
  }
}

void MueveMotorPosicionFinal()
{
digitalWrite(PIN_DIRECCION,DIRECCION_FINAL);

gi_valor_final_carrera_final = digitalRead(PIN_FINAL_CARRERA_FINAL);
while(gi_valor_final_carrera_final != 0)
  {
    digitalWrite(PIN_PASOS, HIGH);
    delay(VELOCIDAD);
    digitalWrite(PIN_PASOS, LOW);
    delay(VELOCIDAD);
    gi_valor_final_carrera_final = digitalRead(PIN_FINAL_CARRERA_FINAL);
  }
  delay(1000);    
}
void MueveMotorPosicionInicial()
{
digitalWrite(PIN_DIRECCION,DIRECCION_INICIAL);

gi_valor_final_carrera_inicial = digitalRead(PIN_FINAL_CARRERA_INICIAL);
while(gi_valor_final_carrera_inicial != 0)
  {
    digitalWrite(PIN_PASOS, HIGH);
    delay(VELOCIDAD);
    digitalWrite(PIN_PASOS, LOW);
    delay(VELOCIDAD);
    gi_valor_final_carrera_inicial = digitalRead(PIN_FINAL_CARRERA_INICIAL);
  }
  delay(1000);    
}