//
// Created by bear on 2/8/2023.
//

#pragma once

#include <parser/annotation.h>
#include <parser/expression.h>

#include <lexer/lexer.h>

#include <memory>
#include <utility>

namespace lightlox
{
struct binary_expression : public annotatable
{
	binary_expression(expression l, token o, expression r) :
		annotatable(),
		left(std::move(l)),
		op(std::move(o)),
		right(std::move(r))
	{}

	expression left;
	token op;
	expression right;
};
}