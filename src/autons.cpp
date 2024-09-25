#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "src/config.cpp"
#include "include/autons.h"

void moveIntake(bool val){
	if(val == true){
		Intake.move_velocity(200);
	}
	else{
		Intake.move_velocity(-200);
	}
}
void moveBelt(bool val){
	if(val == true){
		Belt.move_velocity(600);
	}
	else{
		Belt.move_velocity(-600);
	}
}
void grab(){
    moveIntake(true); 
    moveBelt(true);
}
void stop(){
    Intake.brake();
    Belt.brake(); 
}
ASSET(basicAuton_txt);
void baseAuton(){
    chassis.setPose(-57,872,-33.559,0);
    chassis.follow(basicAuton_txt,15,2000); 
    //chassis.moveToPoint(-60.276,-1.013,4000); //Move to the first point
}
void autonOne(){
    chassis.setPose(-58.44,-23,337,0);
    chassis.moveToPoint(-59.198,-8.004,0); 
    grab(); 
    chassis.moveToPose(-56.169,12.062,50,4000); 
    stop(); 
    chassis.moveToPose(-42.161,17.93,120,4000);
    chassis.moveToPose(-24.745,14.712,180,4000);
}