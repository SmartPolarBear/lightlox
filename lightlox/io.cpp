//
// Created by bear on 2/6/2023.
//

#include "lightlox/io.h"

#include <iostream>
#include <string>
#include <format>
#include <fstream>
#include <stdexcept>

using namespace std;

void lightlox::logger::log(lightlox::log_type type, std::string msg)
{
	string full = std::format("{}: {}", type, msg);
	if (static_cast<int>(type) <= verbosity_)
	{
		cout << full << endl;
	}
	logs_.push_back(full);
}

void lightlox::logger::set_verbosity(int verb)
{
	verbosity_ = verb;
}

void lightlox::logger::dump(const std::string& pathname)
{
	ofstream file{pathname};
	if (!file.is_open() || file.bad())
	{
		throw std::runtime_error("Cannot dump log to file");
	}

	for (const auto &line: logs_)
	{
		file << line << endl;
		if (!file.is_open() || file.bad())
		{
			throw std::runtime_error("Cannot write log line to file");
		}
	}
}
