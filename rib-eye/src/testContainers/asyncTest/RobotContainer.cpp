#include "RobotContainer.h"

RobotContainer::RobotContainer()
    : ultrasonic_1(Pins::Ultrasonic::kEcho1, Pins::Ultrasonic::kTrigger1) ,
      ultrasonic_2(Pins::Ultrasonic::kEcho2, Pins::Ultrasonic::kTrigger2)  {}

void RobotContainer::init() {
    drive.init();
    ultrasonic_1.init();
    ultrasonic_2.init();

    drive.forward();
}

void RobotContainer::update() {
    /*
    float distance = 0.0f;
    if (ultrasonic.readAsync(distance, 100)) {
        if (distance > 0.0f) {
            Serial.print("Distancia: ");
            Serial.print(distance);
            Serial.println(" cm");
        } else {
            Serial.println("Distancia: Fuera de rango");
        }
    }*/

   ultrasonic_1.readAsync(dist1, 60);
   ultrasonic_2.readAsync(dist2, 60);

   if(dist1 > 0.0f && dist1 <= 10.0f) {
        drive.enableLeft(true);
   } else {
        drive.enableLeft(false);
   }

   if(dist2 > 0.0f && dist2 <= 10.0f) {
        drive.enableRigt(true);
   } else {
        drive.enableRigt(false);
   }

}