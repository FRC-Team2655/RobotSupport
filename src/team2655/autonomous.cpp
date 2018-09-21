/**
 * autonomous.cpp
 * See autonomous.hpp for details.
 *
 * Copyright (c) 2018 FRC Team 2655 - The Flying Platypi
 * See LICENSE file for details
 */

#include "autonomous.hpp"


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
