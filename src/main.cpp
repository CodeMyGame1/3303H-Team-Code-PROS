#include "main.h"

#include "config.hpp"
#include "autons.hpp"

pros::Controller master(pros::E_CONTROLLER_MASTER);
	pros::Motor leftMotorA(13);
	pros::Motor leftMotorB(9); 
	pros::Motor leftMotorC(6);
	pros::MotorGroup left_mg({13, 9, 6},pros::MotorGearset::green);
	pros::Motor rightMotorA(1);
	pros::Motor rightMotorB(2);
	pros::Motor rightMotorC(12);
	pros::MotorGroup right_mg({1, 2, 12},pros::MotorGearset::green);  
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
	lemlib::Chassis chassis(drivetrain, // drivetrain settings
							lateral_controller, // lateral PID settings
							angular_controller, // angular PID settings
							odom, // odometry sensors
							&throttle_curve, // throttle curve
							&steer_curve // steer curve
	);

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}


/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");
	// Initialize the Auton Selector (Add the Selector Soon)

	pros::lcd::register_btn1_cb(on_center_button);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
	// baseAuton(); 
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	while (true) {
        // get left y and right y positions
        int leftY = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int rightY = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);

        // move the robot
        chassis.tank(leftY, rightY);
		
		// Toggle the Pistons State
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
			togglePiston(); 
		}

		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)){
			//Intake.move_velocity(200);
			moveIntake(true);
			//Belt.move_velocity(600);
			moveBelt(true);
		}
		else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)){
			moveIntake(false);
			moveBelt(false); 
			//Intake.move_velocity(-200);
			//Belt.move_velocity(-600);
		}
		else{
			Intake.brake();
			Belt.brake();
		}

        // delay to save resources
        pros::delay(25);
    }
}