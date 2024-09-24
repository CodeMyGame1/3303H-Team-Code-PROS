#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
pros::Controller master(pros::E_CONTROLLER_MASTER);
	pros::Motor leftMotorA(13);
	pros::Motor leftMotorB(9); 
	pros::Motor leftMotorC(6);
	pros::MotorGroup left_mg({13, 9, 6},pros::MotorGearset::green);
	pros::Motor rightMotorA(1);
	pros::Motor rightMotorB(2);
	pros::Motor rightMotorC(12);
	pros::MotorGroup right_mg({1, 2, 12},pros::MotorGearset::green); 
	pros::Motor Intake(5,pros::MotorGearset::green); 
	pros::Motor Belt(4,pros::MotorGearset::blue); 
	pros::adi::Pneumatics Piston('A',true); 
	pros::IMU dtInertial(10); 
	// Wheel Diameter = 3.25
	lemlib::Drivetrain drivetrain(&left_mg,
						&right_mg,
						15, //Check Later
						lemlib::Omniwheel::OLD_325,
						200,
						2);

	// lateral PID controller
	lemlib::ControllerSettings lateral_controller(10, // proportional gain (kP)
												0, // integral gain (kI)
												3, // derivative gain (kD)
												3, // anti windup
												1, // small error range, in inches
												100, // small error range timeout, in milliseconds
												3, // large error range, in inches
												500, // large error range timeout, in milliseconds
												20 // maximum acceleration (slew)
	);

	// angular PID controller
	lemlib::ControllerSettings angular_controller(2, // proportional gain (kP)
												0, // integral gain (kI)
												10, // derivative gain (kD)
												3, // anti windup
												1, // small error range, in degrees
												100, // small error range timeout, in milliseconds
												3, // large error range, in degrees
												500, // large error range timeout, in milliseconds
												0 // maximum acceleration (slew)
	);
	lemlib::OdomSensors odom(nullptr, // vertical tracking wheel 1, set to null
                            nullptr, // vertical tracking wheel 2, set to nullptr as we are using IMEs
                            nullptr, // horizontal tracking wheel 1
                            nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
                            &dtInertial // inertial sensor;
	);  
	// input curve for throttle input during driver control
	lemlib::ExpoDriveCurve throttle_curve(3, // joystick deadband out of 127
										10, // minimum output where drivetrain will move out of 127
										1.019 // expo curve gain
	);

	// input curve for steer input during driver control
	lemlib::ExpoDriveCurve steer_curve(3, // joystick deadband out of 127
									10, // minimum output where drivetrain will move out of 127
									1.019 // expo curve gain
	);
	// create the chassis
	extern lemlib::Chassis chassis(drivetrain, // drivetrain settings
							lateral_controller, // lateral PID settings
							angular_controller, // angular PID settings
							odom, // odometry sensors
							&throttle_curve, // throttle curve
							&steer_curve // steer curve
	);
