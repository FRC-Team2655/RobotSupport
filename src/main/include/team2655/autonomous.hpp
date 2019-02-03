/**
 * autonomous.hpp
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

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <functional>
#include <algorithm>

#include <frc/commands/Command.h>
#include <frc/commands/CommandGroup.h>

namespace team2655{

typedef std::unique_ptr<frc::CommandGroup> CmdGroupPointer;
typedef std::function<frc::Command*()> CmdCreator;

template<class T>
frc::Command *CommandCreator(){
	if(!std::is_base_of<frc::Command, T>::value){
		std::cerr << "WARNING: Creator cannot create command. Given type is not a valid frc::Command." << std::endl;
		return nullptr;
	}
	return new T();
}

struct RegisteredCommand{
	CmdCreator creator;
	bool isBackground;
};

/**
 * A class to handle loading of autonomous command scripts and running AutoCommand objects
 */
class AutoManager{
protected:
	// Data for the current script
	std::vector<std::string> loadedCommands;
	std::vector<std::vector<std::string>> loadedArguments;

	// Registered commands. Used to get a command from a command name (string)
	std::unordered_map<std::string, RegisteredCommand> registeredCommands;

	/**
	 * Split a string by a character delimiter
	 * @param s The string to split
	 * @param delimiter The delimiter to split by
	 * @return A vector of strings (the specific segments)
	 */
	std::vector<std::string> split(const std::string& s, char delimiter);

public:

	/**
	 * Register a command with the AutoManager
	 * @param creator The CommandCreator for the command (use CommandCreator<T>)
	 * @param name The name to register the command with
	 */
	void registerCommand(CmdCreator creator, bool isBackground, std::string name);

	/**
	 * Register a command with the AutoManager
	 * @param creator The CommandCreator for the command (use CommandCreator<T>)
	 * @param names A set of names to register the command with
	 */
	void registerCommand(CmdCreator creator, bool isBackground, std::vector<std::string> names);

	/**
	 * Register a background command with the auto manager
	 * @param names A set of names to register the command with
	 */
	template<class T>
	void registerBackgroundCommand(std::vector<std::string> names){
		if(!std::is_base_of<BackgroundAutoCommand, T>::value){
			std::cerr << "WARNING: Cannot register background command. Given type is not a valid BackgroundAutoCommand." << std::endl;
			return;
		}

		for(size_t i = 0; i < names.size(); ++i){
			registerBackgroundCommand<T>(names[i]);
		}
	}

	/**
	 * Unregister all commands
	 */
	void unregisterAll();

	/**
	 * Load an autonomous CSV script from the script path
	 * @param fileName The full path to the script to load
	 * @return Was the script successfully loaded
	 */
	bool loadScript(std::string fileName);

	/**
	 * Add a command to autonomous
	 * @param command The command
	 * @param arguments The arguments for the command
	 * @param pos The position to insert the command at (-1 for the end of the loaded script).
	 */
	void addCommand(std::string command, std::vector<std::string> arguments, int pos = -1);

	/**
	 * Add a set of commands to the end of autonomous
	 * @param commands The commands (command names) of the commands to execute
	 * @param arguments The arguments for each command
	 * @param pos THe position to insert the command at (-1 for the end of the loaded script).
	 */
	void addCommands(std::vector<std::string> commands, std::vector<std::vector<std::string>> arguments, int pos = -1);

	/**
	 * Remove all loaded commands
	 */
	void clearCommands();

	/**
	 * Get the number of loaded (and added) commands
	 * @return The number of commands loaded from a script (and added manually)
	 */
	size_t loadedCommandCount();

	/**
	 * Get a CommandGroup for the loaded script
	 * @return The CommandGroup setup for the loaded script
	 */
	CmdGroupPointer getScriptCommand();


	virtual ~AutoManager() {  }
};


}
