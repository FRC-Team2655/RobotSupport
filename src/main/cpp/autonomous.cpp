/**
 * autonomous.cpp
 * See autonomous.hpp for details.
 *
 * Copyright (c) 2018 FRC Team 2655 - The Flying Platypi
 * See LICENSE file for details
 */

#include <team2655/autonomous.hpp>

#include <chrono>
#include <sstream>
#include <regex>
#include <fstream>
#include <iostream>
#include <algorithm>

#include <frc/DriverStation.h>

using namespace team2655;

////////////////////////////////////////////////////////////////////////
/// AutoManager
////////////////////////////////////////////////////////////////////////

std::vector<std::string> AutoManager::split(const std::string& s, char delimiter){
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(s);
	while (std::getline(tokenStream, token, delimiter)){
		tokens.push_back(token);
	}
	return tokens;
}

// Registration methods

void AutoManager::registerCommand(CmdCreator creator, bool isBackground, std::string name){
	std::transform(name.begin(), name.end(), name.begin(), ::tolower);

	// Only one command *or* background command can have a key.
	if(registeredCommands.find(name) == registeredCommands.end()){
		registeredCommands[name] = RegisteredCommand{creator, isBackground};
	}else{	
		frc::DriverStation::ReportError("Cannot register command with name \"" + name + "\". A command is already registered with that name.");
	}
}

void AutoManager::registerCommand(CmdCreator creator, bool isBackground, std::vector<std::string> names){
	for(size_t i = 0; i < names.size(); ++i){
		registerCommand(creator, isBackground, names[i]);
	}
}

void AutoManager::unregisterAll(){
	registeredCommands.clear();
}

// Script management

bool AutoManager::loadScript(std::string fileName){

	clearCommands();

	std::ifstream scriptFile;
	scriptFile.open(fileName);

	if(!scriptFile.good()){
		std::cerr << "Script file: \"" << fileName << "\" not found." << std::endl;
		scriptFile.close();
		return false; // Some error accessing the file
	}

	// Get the file contents
	std::stringstream fileContents;
	fileContents << scriptFile.rdbuf();
	scriptFile.close();

	// Remove spaces before and after commas
	// Commands and args cannot start or end with spaces
	//std::string csvData = std::regex_replace(fileContents.str(), std::regex(",\\ +"), ",");

	// Standardize line endings convert any line ending int '\n'
	std::string csvData = std::regex_replace(fileContents.str(), std::regex("(\r\n|\r|\n)"), "\n");

	std::vector<std::string> lines = split(csvData, '\n'); // Separate each line

	// Separate each column of the CSV
	for(size_t i = 0; i < lines.size(); i++){
		std::vector<std::string> columns = split(lines[i], ','); // All the columns in the CSV
		loadedCommands.push_back(columns[0]); // This is the command
		columns.erase(columns.begin()); // Remove the command from the list of columns. This will leave only arguments
		loadedArguments.push_back(columns);
	}

	return true;
}

void AutoManager::addCommand(std::string command, std::vector<std::string> arguments, int pos){

	// Any position beyond the end of the vector is converted to -1 (aka the end)
	if((pos > ((int)loadedCommands.size())) || pos < -1)
		pos = -1;

	// Add to the end otherwise insert at a position
	if(pos == -1){
		loadedCommands.push_back(command);
		loadedArguments.push_back(arguments);
	}else{
		loadedCommands.insert(loadedCommands.begin() + pos, command);
		loadedArguments.insert(loadedArguments.begin() + pos, arguments);
	}

}

void AutoManager::addCommands(std::vector<std::string> commands, std::vector<std::vector<std::string>> arguments, int pos){

	if(commands.size() != arguments.size()){
		std::cerr << "AutoManagerError: addCommands: must have same number of commands and arguments" << std::endl;
		return;
	}

	// Any position beyond the end of the vector is converted to -1 (aka the end)
	if(pos > ((int)loadedCommands.size()) || pos < -1)
		pos = -1;

	loadedCommands.insert((pos == -1) ? loadedCommands.end() : loadedCommands.begin() + pos,
			              commands.begin(),
						  commands.end());
	loadedArguments.insert((pos == -1) ? loadedArguments.end() : loadedArguments.begin() + pos,
						   arguments.begin(),
						   arguments.end());
}

size_t AutoManager::loadedCommandCount(){
	return loadedCommands.size();
}

void AutoManager::clearCommands(){
	loadedCommands.clear();
	loadedArguments.clear();
}

CmdGroupPointer AutoManager::getScriptCommand(){
	CmdGroupPointer cmdGroup = std::unique_ptr<frc::CommandGroup>(new frc::CommandGroup());
	for(size_t i = 0; i < loadedCommands.size(); ++i){
		std::string cmd = loadedCommands[i];
		if(registeredCommands.find(cmd) == registeredCommands.end()){
			frc::DriverStation::ReportError("Script referenced command (command #" + std::to_string(i) + ") \"" + cmd + "\", but no command was registered with that name.");
		}else{
			if(registeredCommands[cmd].isBackground){
				cmdGroup.get()->AddParallel(registeredCommands[cmd].creator());
			}else{
				cmdGroup.get()->AddSequential(registeredCommands[cmd].creator());
			}
		}
	}
	return std::move(cmdGroup);
}
