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
#include <chrono>
#include <memory>

class AutoCommand{
protected:
	bool _hasStarted = false;
	bool _isComplete = false;
	int timeout = 0;
	long int startTime = 0;

	/**
	 * Get the current time as milliseconds from the epoch
	 * @return Number of milliseconds since the epoch
	 */
	long int currentTimeMillis();

	/**
	 * Check if the command has timed out
	 * @return
	 */
	bool hasTimedOut();
public:
	/**
	 * Has the command been started (init called)
	 * @return true if started, false if not
	 */
	bool hasStarted();

	/**
	 * Has the command completed (complete called)
	 * @return true if completed, false if not
	 */
	bool isComplete();

	/**
	 * Set the timeout for this command
	 * @param timeoutMs The timeout in milliseconds
	 */
	void setTimeout(int timeoutMs);

	/**
	 * Get the timeout for this command
	 * @return The timeout for this command in milliseconds
	 */
	int getTimeout();

	/**
	 * Start the command
	 * @param args The arguments provided for the command
	 */
	virtual void start(std::vector<std::string> args);

	/**
	 * Process the command while it is running
	 */
	virtual void process();

	/**
	 * Finish the command and stop running
	 */
	virtual void complete();

	virtual ~AutoCommand() {  }
};

/**
 * A class to handle loading of autonomous command scripts and running AutoCommand objects
 */
class AutoManager{
protected:
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
	std::unique_ptr<AutoCommand> currentCommand;

	/**
	 * Get the directory for autonomous scripts
	 * @return A path to the directory where scripts are stored
	 */
	virtual std::string getScriptDir() = 0;

	/**
	 * Get a unique_ptr to a new AutoCommand object based on the command name
	 * @param commandName The name of the command
	 * @return A unique pointer to a new AutoCommand child class
	 */
	virtual std::unique_ptr<AutoCommand> getCommand(std::string commandName) = 0;

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
	 * End the current command calling its complete method so that everything ends properly then move to the end of the script
	 */
	void killAuto(); //TODO: This should set the commandIndex to the last index in the script so process will return false

	virtual ~AutoManager() {  }
};
