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

struct expression_placeholder_tag
{
};

static inline constexpr expression_placeholder_tag expression_placeholder{};

using expression = std::variant<std::unique_ptr<class binary_expression>,
								std::unique_ptr<class prefix_expression>,
								std::unique_ptr<class postfix_expression>,
								std::unique_ptr<class literal_expression>,
								expression_placeholder_tag>;

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
