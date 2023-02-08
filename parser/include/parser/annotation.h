//
// Created by bear on 2/8/2023.
//

#pragma once

#include <lightlox/generator.h>

#include <variant>

namespace lightlox
{

using annotation = std::variant<>;

class annotatable
{
public:
	virtual void annotate() = 0;
	virtual generator<annotation> annotations() = 0;
};
}
