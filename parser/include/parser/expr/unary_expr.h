//
// Created by bear on 2/8/2023.
//

#pragma once

#include <parser/annotation.h>
#include <parser/expression.h>

#include <lexer/lexer.h>

namespace lightlox
{
struct unary_expression : public annotatable
{
	unary_expression(token o, expression_owned c) :
		annotatable(),
		op(std::move(o)),
		child(std::move(c))
	{}

	token op;
	expression_owned child;
};
}