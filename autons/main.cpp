#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "src/config.cpp"

ASSET(basicAuton_txt);
void baseAuton(){
    chassis.setPose(-57,872,-33.559,0);
    chassis.follow(basicAuton_txt,15,2000); 
    //chassis.moveToPoint(-60.276,-1.013,4000); //Move to the first point
}
