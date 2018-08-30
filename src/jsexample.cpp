/**
 * jsexample.cpp
 * An example showing how to use the team2655 jshelper code
 *
 * @author Marcus Behel
 * @version 1.0 8-30-2018
 *
 * Copyright (c) 2018 FRC Team 2655 - The Flying Platypi
 * See LICENSE file for details
 */
#include <IterativeRobot.h>
#include <Drive/DifferentialDrive.h>
#include <Talon.h>
#include <Joystick.h>
#include <array>
#include <iostream>

#include "team2655/joystick.hpp"

// Use the team2655 namespace (for team2655 helper code)
using namespace team2655;

/**
 * Our example robot
 * This uses a cubic configuration for the drive axis and a deadband configuration for the rotate axis
 */
class Robot : public frc::IterativeRobot {
private:
	Joystick js0;
	Talon left, right;
	DifferentialDrive robotDrive;
	jshelper::AxisConfig driveAxisConfig, rotateAxisConfig;
public:
	/**
	 * Create our joystick object, robot drive object, and axis configs
	 */
	Robot() : js0(0),
	          left(0),
			  right(1),
	          robotDrive(left, right),
		      driveAxisConfig(jshelper::createAxisConfig(0.1, 0.2, 0.6)),
	          rotateAxisConfig(jshelper::createAxisConfig(0.1)){

	}
	/**
	 * Called every 20ms in teleop mode
	 */
	void TeleopPeriodic() override{
		// Get the value of the drive axis using the cubic configuration
		double speed = jshelper::getAxisValue(driveAxisConfig, js0.GetRawAxis(1));
		// Or get the power axis only using the deadband (no cubic function)
		// double speedDeadbandOnly = jshelper::getAxisValue(driveAxisConfig, js0.GetRawAxis(1), true);

		// This will be deadband only because the config is a deadband config (not a cubic config)
		double rotation = jshelper::getAxisValue(rotateAxisConfig, js0.GetRawAxis(2));

		// Drive the robot
		robotDrive.ArcadeDrive(speed, rotation, false); // Drive the robot
	}
};

// Start the robot
START_ROBOT_CLASS(Robot)
