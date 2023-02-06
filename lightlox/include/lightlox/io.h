//
// Created by bear on 2/4/2023.
//

#pragma once
#include <lightlox/singleton.h>

#include <string>

namespace lightlox
{
enum class log_type
{
	ERROR,
	WARNING,
	INFO,
	DEBUG,
};

class logger final
	: public singleton<logger>
{
public:
	void log(log_type type, std::string msg);

	void set_verbosity(int verb);
private:
	int verbosity_{0};
};

}