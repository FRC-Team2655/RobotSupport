# C-RobotSupport
Team 2655 Helper code for FRC Robots in C++.
All code is contained within the team2655 namespace.
More specific code will be included in sub-namespaces (ex. js, auto, tele, etc...)
To use in robot project add the team2655 directory to the robot project's src folder


## Joystick Helper Code
**Header:** `team2655/joystick.hpp`

**Namespace:** `team2655::jshelper`

**Example:** `jsexample.cpp`

The joystick helper code is a compilation of functions for using WPILib Joystick axes with applied cubic transformations and deadbands. It uses a polynomial fitting routine to generate cubic functions for Joystick axes supporting multiple settings for the generate the function. It also supports simple deadbands without touching the deadband.

Settings:
- deadband: A dead zone on the joystick. The scaled value will always be zero unitl the axis is moved beyond the deadband.
- min power: The minimum amount of power to apply in the scaled value (this will be the value after the deadband threshold is passed)
- mid power: The amount of power that is the "controllable" or middle speed. This is (about) where the "flat" section of the cubic function will be.

NOTE: Deadband only mode (`createDeadbandConfig`) only supports the deadband setting.

###### What it looks like:
![cubic-demo.png](cubic-demo.png)