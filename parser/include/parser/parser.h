//
// Created by bear on 2/8/2023.
//

#pragma once

#include <parser/expression.h>

#include <functional>

namespace lightlox
{

class parser
{
public:
private:
	enum precedence
	{
		PREC_NONE,
		PREC_TERNARY,     // ?:
		PREC_ASSIGNMENT,  // =
		PREC_OR,          // or
		PREC_AND,         // and
		PREC_EQUALITY,    // == !=
		PREC_COMPARISON,  // < > <= >=
		PREC_TERM,        // + -
		PREC_FACTOR,      // * /
		PREC_POSTFIX,     // ++ --
		PREC_PREFIX,       // ! - ++ --
		PREC_CALL,        // . ()
		PREC_PRIMARY
	};

	using prefix_parse_fn = std::function<expression()>;
	using infix_parse_fn = std::function<expression(expression)>;

	struct parse_rule
	{
		prefix_parse_fn prefix;
		infix_parse_fn infix;
		precedence precedence;
	};

	expression expr();
	expression hierarchical_exprs();
	expression primary();
};
}