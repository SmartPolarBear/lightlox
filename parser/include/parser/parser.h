//
// Created by bear on 2/8/2023.
//

#pragma once

#include <parser/expression.h>
#include <lexer/lexer.h>
#include <lightlox/generator.h>

#include <unordered_map>
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
		PREC_PRIMARY,

		COUNT_OF_PERC,
	};

	using prefix_parse_fn = std::function<expression()>;
	using infix_parse_fn = std::function<expression(expression)>;
	using postfix_parse_fn = std::function<expression(expression)>;

	struct parse_rule
	{
		prefix_parse_fn prefix;
		infix_parse_fn infix;

		precedence precedence;
	};

	expression expr();
	expression hierarchical_exprs();
	expression primary();

	std::unordered_map<token_types, parse_rule> rules_ = {
		{TOKEN_PLUS, parse_rule{}},
	};

	void advance();

	void error_at_current(std::string_view msg);
	void error(std::string_view msg);
	static void error_at(const token &tk, std::string_view msg);

	generator<token> tokens_;

	generator<token>::iterator current_;
	generator<token>::iterator previous_;
};

}