//
// Created by bear on 2/13/2023.
//

#pragma once

#include <parser/annotation.h>
#include <parser/expression.h>

#include <lexer/lexer.h>

namespace lightlox
{
struct literal_expression : public annotatable
{
	explicit literal_expression(token t) :
		annotatable(),
		value(std::move(t))
	{}

	token value;
};
}