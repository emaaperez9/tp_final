#include <Servo.h>
 
Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards
 
int pos = 0;    // variable to store the servo position
 
void setup() {
  Serial.begin(115200);
  myservo.attach(26);  // attaches the servo on pin 13 to the servo object
}
 
void loop() {
        while (Serial.available()) {
                // read the incoming byte:
                pos = Serial.read();
                Serial.println(pos);
                myservo.write(pos);
                
        }

}
