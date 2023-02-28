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

template<typename T>
using ast_ptr = std::unique_ptr<T>;

using expression = std::variant<ast_ptr<class binary_expression>,
								ast_ptr<class ternary_expression>,
								ast_ptr<class prefix_expression>,
								ast_ptr<class postfix_expression>,
								ast_ptr<class literal_expression>,
								ast_ptr<class grouping_expression>,
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

struct ternary_expression : public annotatable
{
	ternary_expression(expression c, expression t, token col, expression f) :
		annotatable(),
		condition(std::move(c)),
		true_branch(std::move(t)),
		colon(std::move(col)),
		false_branch(std::move(f))
	{}

	expression condition;
	expression true_branch;
	token colon;
	expression false_branch;
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

struct grouping_expression : public annotatable
{
	explicit grouping_expression(expression c) :
		annotatable(),
		child(std::move(c))
	{}

	expression child;
};


}
