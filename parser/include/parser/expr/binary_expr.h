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
	binary_expression(expression_owned l, token o, expression_owned r) :
		annotatable(),
		left(std::move(l)),
		op(std::move(o)),
		right(std::move(r))
	{}

	expression_owned left;
	token op;
	expression_owned right;
};
}