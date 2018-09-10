/**
 * autonomous.hpp
 * Contains FRC Team 2655's autonomous helper code
 * The AutoManager class handles loading scripts and mapping command names to AutoCommand objects
 * The AutoCommand class handles a single command.
 * Both classes must be overridden as no default commands exist.
 *
 * @author Marcus Behel
 * @version 1.1 8-30-2018 Changed names to make easier to understand and implement
 *
 * Copyright (c) 2018 FRC Team 2655 - The Flying Platypi
 * See LICENSE file for details
 */

#pragma once

#include <vector>
#include <string>
#include <chrono>

namespace team2655{
	namespace autohelper{
		class AutoManager;

		class AutoCommand{
		private:
			AutoManager *manager = nullptr;
			std::vector<std::string> arguments;
			bool _isFinished = false;
			int timeout = 0;
			long int startTime = -1;

		public:
			AutoCommand(AutoManager *manager);
			virtual ~AutoCommand();
			virtual void init(std::vector<std::string> args);
			virtual void process();
			virtual void complete();
			long int getTimeMillis();
			bool isFinished();
		};
		class AutoManager{
		private:
			std::vector<std::string> commands;
			std::vector<std::vector<std::string>> arguments;
		public:
			virtual ~AutoManager(){ }
			bool loadScript(std::string script, std::string path = "/auto-scripts");
			void startAutonomous();
			void nextCommand();
			virtual AutoCommand getCommandForName(std::string commandName) = 0;
		};
	}
}
