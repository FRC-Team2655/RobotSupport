# C++RobotSupport
Team 2655 Helper code for FRC Robots in C++. All code is contained within the team2655 namespace. More specific code will be located in sub-namespaces. To use in a robot project add the team2655 directory to the robot project's src folder


## Joystick Helper Code
**Header:** `team2655/joystick.hpp`

**Namespace:** `team2655::jshelper`

The joystick helper code is a compilation of functions for using WPILib Joystick axes with applied cubic transformations and deadbands. It uses a polynomial fitting routine to generate cubic functions for Joystick axes supporting multiple settings for the generate the function. It also supports simple deadbands without touching the deadband.

Settings:
- deadband: A dead zone on the joystick. The scaled value will always be zero unitl the axis is moved beyond the deadband.
- min power: The minimum amount of power to apply in the scaled value (this will be the value after the deadband threshold is passed)
- mid power: The amount of power that is the "controllable" or middle speed. This is (about) where the "flat" section of the cubic function will be.

NOTE: Deadband only mode (`createDeadbandConfig`) only supports the deadband setting.

###### What it looks like:
![cubic-demo.png](cubic-demo.png)

## Autonomous Helper Code
**Header:** `team2655/autonomous.hpp`

The autonomous helper code handles script-based autonomous routines. Scripts are stored on the RoboRIO as CSV files and are made up of commands and arguments. Each row of the CSV file must be a single command. The first column of each row is the command. Following columns are the arguments for the command.

The helper code consists of two classes, `AutoManager` and `AutoCommand`. `AutoManager` handles loading of script files, reading commands and arguments, and executing `AutoCommand`s. 

For every command the robot will need to execute in autonomous a custom `AutoCommand` class (a child class of `AutoCommand`) will be needed. The `AutoManager` will then be used to map command strings (from the script file) to different `AutoCommand` classes. Each `AutoCommand` child class will need to implement three methods:
- `start`: This method is run when the command is started. A list of arguments is passed as a `vector` of strings.
- `process`: This will be called periodically while the command is running. This is where action code should be for the command. This function should check if the command has finished executing. If so it should call its own `complete` function to end the command.
- `complete`: This is run when the command is done executing. This should stop any action the command is taking.

**Make sure that the parent cass method is called for each of these or the `AutoManager` may not correctly progress from command to command!**

For example:
In the `process` function of a child class make sure to call `AutoCommand::process()`. This should be the first thing each of these methods does.

To set a timeout for a command use the `setTimeout` method. This should usually be called in the command's `start` function. Once the command has been running for the specified amount of time, it's `complete` function will be run automatically. 

**Once again: Make sure the parent class methods are called in `start`, `process`, and `complete` or this may not work!**

To use the commands the `AutoManager` will be used. Create a class that derives from `AutoManger` and implement the two abstract functions `getScriptDir` and `getCommand`. `getScriptDir` should return the path where the scripts are stored on the RoboRIO (as a string). The `getCommand` function will return a `unique_ptr` to an `AutoCommand` based on the command name (string) it is given. This is how custom `AutoCommand`s are mapped to command strings. To use the custom `AutoManager` in autonomous mode, create an instance of the class in the Robot code. When autonomous starts use the `loadScript` or `addCommand` functions to load a script or add one manually. The `process` function (of the AutoManager instance) must be called periodically in autonomous mode (if using an `IterativeRobot` this is the `AutonomousPeriodic` function). This will handle executing each command and moving to the next command when the current one is finished.

**Namespace:** `team2655`

## Example
An example of using the autonomous and joystick helper code is available on [github](https://github.com/FRC-Team2655/Team2655ExampleRobot).