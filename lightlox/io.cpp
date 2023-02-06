//
// Created by bear on 2/6/2023.
//

#include "lightlox/io.h"

void lightlox::logger::log(lightlox::log_type type, std::string msg)
{

}

void lightlox::logger::set_verbosity(int verb)
{
	verbosity_ = verb;
}
