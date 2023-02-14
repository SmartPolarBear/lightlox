//
// Created by bear on 2/8/2023.
//

#pragma once

#include <parser/annotation.h>
#include <parser/expression.h>

#include <lexer/lexer.h>

namespace lightlox
{
struct prefix_expression : public annotatable
{
	prefix_expression(token o, expression c) :
		annotatable(),
		op(std::move(o)),
		child(std::move(c))
	{}

	token op;
	expression child;
};

struct postfix_expression : public annotatable
{
	postfix_expression(expression c, token o) :
		annotatable(),
		child(std::move(c)),
		op(std::move(o))
	{}

	expression child;
	token op;
};
}