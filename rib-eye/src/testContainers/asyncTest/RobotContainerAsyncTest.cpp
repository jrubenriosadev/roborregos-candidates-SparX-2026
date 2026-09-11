#include "RobotContainerAsyncTest.h"

RobotContainerAsyncTest::RobotContainerAsyncTest()
    : ultrasonic_1(Pins::Ultrasonic::kEcho1, Pins::Ultrasonic::kTrigger1) ,
      ultrasonic_2(Pins::Ultrasonic::kEcho2, Pins::Ultrasonic::kTrigger2)  {}

void RobotContainerAsyncTest::init() {
    drive.init();
    ultrasonic_1.init();
    ultrasonic_2.init();

}

void RobotContainerAsyncTest::update() {
   if(ultrasonic_1.readAsync(dist1, 80)) {
        if(dist1 > 0.0f && dist1 <= 10.0f) {
            drive.enableLeft(true);
        } else {
            drive.enableLeft(false);
        }
   }

   if(ultrasonic_2.readAsync(dist2, 80)) {
        if(dist2 > 0.0f && dist2 <= 10.0f) {
            drive.enableRight(true);
        } else {
            drive.enableRight(false);
        }
   }

}