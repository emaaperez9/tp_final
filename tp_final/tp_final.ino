#include "HX711.h"
#include "soc/rtc.h"
//define sound speed in cm/uS
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

const int estado_tacho_1 = 15;
int estado = 0;
const int optico_pin = 2;
uint8_t estado_optico =0;
const int trigPin = 5;
const int echoPin = 18;
long duration;
float distanceCm;
float distanceInch;

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 16;
const int LOADCELL_SCK_PIN = 4;

HX711 scale;

void setup() {
  Serial.begin(115200);
  rtc_clk_cpu_freq_set(RTC_CPU_FREQ_80M);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(optico_pin,INPUT);
  Serial.println("HX711 Demo");

  Serial.println("Inicializando la escala");

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  Serial.println("Antes de configurar la báscula:");
  Serial.print("Lectura: \t\t");
  Serial.println(scale.read());      // imprime una lectura sin procesar del ADC

  Serial.print("Lectura promedio: \t\t");
  Serial.println(scale.read_average(20));   // imprimir el promedio de 20 lecturas del ADC

  Serial.print("Obtener valor: \t\t");
  Serial.println(scale.get_value(5));   // imprimir el promedio de 5 lecturas del ADC menos el peso de tara (aún no configurado)

  Serial.print("Obtener unidades: \t\t");
  Serial.println(scale.get_units(5), 1);  // imprimir el promedio de 5 lecturas del ADC menos el peso de tara (no configurado) dividido
                                          // por el parámetro ESCALA (aún no configurado)
            
  scale.set_scale(1);
  //scale.set_scale(-471.497);                      //este valor se obtiene calibrando la balanza con pesos conocidos; vea el LÉAME para más detalles
  scale.tare();               // reset the scale to 0

  Serial.println("Después de configurar la báscula:");

  Serial.print("Lectura: \t\t");
  Serial.println(scale.read());                 //imprimir una lectura sin procesar del ADC

  Serial.print("lectura promedio: \t\t");
  Serial.println(scale.read_average(20));       // imprime el promedio de 20 lecturas del ADC

  Serial.print("Obtener valor: \t\t");
  Serial.println(scale.get_value(5));   // imprime el promedio de 5 lecturas del ADC menos el peso de tara, establecido con tare()

  Serial.print("obtener unidades: \t\t");
  Serial.println(scale.get_units(5), 1);        // imprime el promedio de 5 lecturas del ADC menos el peso de tara, dividido
            // by the SCALE parameter set with set_scale

  Serial.println("Readings:");
}

void loop() {
  Serial.print("one reading:\t");
  Serial.print(scale.get_units(), 1);
  Serial.print("\t| average:\t");
  Serial.println(scale.get_units(10), 5);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance
  distanceCm = duration * SOUND_SPEED/2;
  
  // Convert to inches
  distanceInch = distanceCm * CM_TO_INCH;
  
  // Prints the distance in the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
  Serial.print("Distance (inch): ");
  Serial.println(distanceInch);
  
  estado = digitalRead(estado_tacho_1); 
  estado_optico = digitalRead(optico_pin);
if(estado_optico == LOW)
  Serial.println("Lejos"); 
  else
  Serial.println("Cerca");// muestro estado 

if(estado == LOW)
  Serial.println("Ocupado"); 
  else
  Serial.println("vacío");// muestro estado

  scale.power_down();             // put the ADC in sleep mode
  delay(1500);
  scale.power_up();

}
