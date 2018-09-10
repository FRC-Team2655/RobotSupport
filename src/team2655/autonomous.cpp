#include "autonomous.hpp"

using namespace team2655::autohelper;

/////////////////////////////////////////////////////////////////////////
/// AutoCommand
/////////////////////////////////////////////////////////////////////////

AutoCommand::AutoCommand(AutoManager *manager){
	this->manager = manager;
}

AutoCommand::~AutoCommand(){
	delete manager;
}

/**
 * End this command
 */
void AutoCommand::complete(){
	if(manager != nullptr)
		manager->nextCommand();
	_isFinished = true;
}

/**
 * Start this command
 * @param command The command string
 * @param args The arguments for the command
 */
void AutoCommand::init(std::vector<std::string> args){
	arguments = args;
}

/**
 * Get the current number of milliseconds since the epoch
 * @return
 */
long int AutoCommand::getTimeMillis(){
	return std::chrono::duration_cast<std::chrono::milliseconds>(
	    std::chrono::system_clock::now().time_since_epoch()
	).count();
}

void AutoCommand::process(){
	// Handle the timeout if needed
	if((getTimeMillis() - startTime) >= timeout){
		complete();
	}
}

bool AutoCommand::isFinished(){
	return _isFinished;
}
