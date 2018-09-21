/**
 * autonomous.cpp
 * See autonomous.hpp for details.
 *
 * Copyright (c) 2018 FRC Team 2655 - The Flying Platypi
 * See LICENSE file for details
 */

#include "autonomous.hpp"


////////////////////////////////////////////////////////////////////////
/// AutoCommand
////////////////////////////////////////////////////////////////////////

long int AutoCommand::currentTimeMillis(){
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}
