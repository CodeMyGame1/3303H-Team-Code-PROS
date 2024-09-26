#include "api.h"

#include "util.hpp"

pros::Motor Intake(5,pros::MotorGearset::green); 
pros::Motor Belt(4,pros::MotorGearset::blue); 
pros::adi::Pneumatics Piston('A',true);