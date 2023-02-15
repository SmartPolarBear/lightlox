//
// Created by bear on 2/8/2023.
//

#pragma once

#include <parser/annotation.h>
#include <parser/expression.h>

#include <lexer/lexer.h>

#include <variant>
#include <memory>
#include <utility>

namespace lightlox
{
using expression = std::variant<std::unique_ptr<class binary_expression>,
								std::unique_ptr<class prefix_expression>,
								std::unique_ptr<class postfix_expression>,
								std::unique_ptr<class literal_expression>>;

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

struct literal_expression : public annotatable
{
	explicit literal_expression(token t) :
		annotatable(),
		value(std::move(t))
	{}

	token value;
};
}
