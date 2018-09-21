/**
 * joystick.hpp
 * Contains FRC Team 2655's autonomous helper code
 * Team 2655's CSV script based autonomous
 *
 * @author Marcus Behel
 * @version 1.0.0 9-20-2018 Initial Version
 *
 * Copyright (c) 2018 FRC Team 2655 - The Flying Platypi
 * See LICENSE file for details
 */

#pragma once

#include <string>
#include <vector>
#include <memory>
#include <cstdio>

class AutoCommand{

};

/**
 * A class to handle loading of autonomous command scripts and running AutoCommand objects
 */
class AutoManager{
private:
	/**
	 * A list of commands loaded from a file
	 */
	std::vector<std::string> loadedCommands;

	/**
	 * A list of arguments for each command (each command can have multiple arguments)
	 */
	std::vector<std::vector<std::string>> arguments;

	/**
	 * The index of the command that is currently being executed
	 */
	int currentCommandIndex = -1;

	/**
	 * The object for the command that is currently being executed
	 */
	AutoCommand currentCommand;
protected:

	/**
	 * Get the directory for autonomous scripts
	 * @return A path to the directory where scripts are stored
	 */
	virtual std::string getScriptDir() = 0;

	/**
	 * Get an AutoCommand object based on the command name
	 * @param commandName The name of the command
	 * @return The command object
	 */
	virtual AutoCommand getCommand(std::string commandName) = 0;

public:

	/**
	 * Load an autonomous CSV script from the script path
	 * @param scriptName The name of the script to load
	 * @return Was the script successfully loaded
	 */
	bool loadScript(std::string scriptName);

	/**
	 * Add a delay to the start of autonomous
	 * @param delayMs How long to wait
	 */
	void addStartDelay(double delayMs);

	/**
	 * Manually give a set of commands and arguments to mimic a script
	 * @param commands The commands (command names) of the commands to execute
	 * @param arguments The arguments for each command
	 */
	void putScript(std::vector<std::string> commands, std::vector<std::vector<std::string>> arguments);

	/**
	 * Process the current command (and move on if needed)
	 * @return Is done running all commands
	 */
	bool process();

	/**
	 * End the current command calling its complete method so that everything ends properly
	 */
	void killAuto();

	virtual ~AutoManager() {  }
};
