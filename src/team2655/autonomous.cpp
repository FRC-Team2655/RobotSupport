/**
 * autonomous.cpp
 * See autonomous.hpp for details.
 *
 * Copyright (c) 2018 FRC Team 2655 - The Flying Platypi
 * See LICENSE file for details
 */

#include "autonomous.hpp"

#include <chrono>
#include <sstream>
#include <regex>
#include <fstream>


using namespace team2655::autonomous;

////////////////////////////////////////////////////////////////////////
/// AutoCommand
////////////////////////////////////////////////////////////////////////

long int AutoCommand::currentTimeMillis(){
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

bool AutoCommand::hasTimedOut(){
	return currentTimeMillis()  - startTime >= timeout;
}

bool AutoCommand::hasStarted(){
	return _hasStarted;
}

bool AutoCommand::isComplete(){
	return _isComplete();
}

void AutoCommand::setTimeout(int timeoutMs){
	this->timeout = timeoutMs;
}

int AutoCommand::getTimeout(){
	return this->timeout;
}

void AutoCommand::start(std::vector<std::string> args){
	this->arguments = args;
	this->startTime = currentTimeMillis();
	this->_hasStarted = true;
}

void AutoCommand::process(){
	// If the command has timed out complete the command
	if(hasTimedOut())
		complete();
	// If the command is completed or not started do not do anything
	if(_isComplete || !_hasStarted)
		return;
}

void AutoCommand::complete(){
	this->_isComplete = true;
}

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

bool AutoManager::loadScript(std::string scriptName){
	std::ifstream scriptFile;
	scriptFile.open("test.csv");

	if(!scriptFile.good()){
		scriptFile.close();
		return false; // Some error accessing the file
	}

	// Get the file contents
	std::stringstream fileContents;
	fileContents << scriptFile.rdbuf();
	scriptFile.close();

	// Remove spaces after commas
	std::string csvData = std::regex_replace(fileContents.str(), std::regex(",\\s+"), ",");

	// Standardize line endings convert any line ending int '\n'
	csvData = std::regex_replace(csvData, std::regex("(\r\n|\r|\n)"), "\n");

	std::vector<std::string> lines = split(csvData, '\n'); // Separate each line

	// Separate each column of the CSV
	for(size_t i = 0; i < lines.size(); i++){
		std::vector<std::string> columns = split(lines[i], ','); // All the columns in the CSV
		loadedCommands.push_back(columns[0]); // This is the command
		columns.erase(columns.begin()); // Remove the command from the list of columns. This will leave only arguments
		loadedArguments.push_back(columns);
	}

	// Reset
	currentCommandIndex = -1;
	currentCommand.release();

	return true;
}

void AutoManager::addCommand(std::string command, std::vector<std::string> arguments, int pos = -1){

	// Any position beyond the end of the vector is converted to -1 (aka the end)
	if(pos > loadedCommands.size())
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

void AutoManager::putScript(std::vector<std::string> commands, std::vector<std::vector<std::string>> arguments){
	killAuto();
	loadedCommands = commands;
	loadedArguments = arguments;

	// Reset
	currentCommandIndex = -1;
	currentCommand.release();
}
