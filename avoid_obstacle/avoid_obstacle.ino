/* Avoid obstacles while driving */

#include <Shieldbot.h>
#include <NewPing.h>

#define TRIGGER_PIN  3  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     2  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 400 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

const int MIN_DISTANCE = 8; // robot stops when object is nearer (in inches)
const int CLEAR_DISTANCE = 24; // distance in inches considered attractive to move
const int UPDATE_DELAY = 200;

int distance;

Shieldbot shieldbot = Shieldbot();
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

void setup(){
  Serial.begin(9600);
  shieldbot.setMaxSpeed(70);//255 is max
}

void loop(){
  updateDistance();
  
  if (!canMoveForward()) {
    shieldbot.stop();
    lookAround();
  }
  
  shieldbot.forward();
}

boolean canMoveForward() {
  return distance > MIN_DISTANCE;
}

void lookAround() {
  shieldbot.drive(-100, 100);
  delay(500);
  
  int left_distance = sonar.ping_cm();
  if (left_distance > 50) {
    return;
  }
  
  shieldbot.drive(100, -100);
  delay(1000);
  
  int right_distance = sonar.ping_cm();
  if (right_distance > 50) {
    return;
  }
  
  delay(500);
  
  int right_distance2 = sonar.ping_cm();
  if (right_distance2 > 50) {
    return;
  }
  
  shieldbot.drive(-100, 100);
  delay(2000);
  
  int left_distance2 = sonar.ping_cm();
  if (left_distance2 > 50) {
    return;
  }
  
  shieldbot.drive(100, -100);
  delay(1000);
  
  shieldbot.backward();
  
  delay(2000);
  
  shieldbot.stop();
}

void updateDistance() {
  static unsigned long time = 0;
  
  if (millis() - time < UPDATE_DELAY) {
    return;
  }
  
  int d = sonar.ping_cm();
  
  if (d > 0) {
    distance = d;
  }
}

