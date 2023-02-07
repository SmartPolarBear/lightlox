//
// Created by bear on 2/4/2023.
//

#pragma once
#include <lightlox/singleton.h>

#include <string>
#include <vector>
#include <format>

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
	void dump(const std::string& pathname);
private:
	int verbosity_{0};
	std::vector<std::string> logs_{};
};

}

namespace std
{
template<class CharT>
struct formatter<lightlox::log_type, CharT>
{
	template<typename FormatParseContext>
	auto parse(FormatParseContext &pc)
	{
		// parse formatter args like padding, precision if you support it
		return pc.end(); // returns the iterator to the last parsed character in the format string, in this case we just swallow everything
	}

	template<typename FormatContext>
	auto format(lightlox::log_type t, FormatContext &fc)
	{
		switch (t)
		{
		case lightlox::log_type::ERROR :
			return std::format_to(fc.out(), "Error");
		case lightlox::log_type::WARNING :
			return std::format_to(fc.out(), "Warning");
		case lightlox::log_type::INFO :
			return std::format_to(fc.out(), "Info");
		case lightlox::log_type::DEBUG :
			return std::format_to(fc.out(), "Debug");
		default:
			return std::format_to(fc.out(), "Unknown");
		}
	}
};
}