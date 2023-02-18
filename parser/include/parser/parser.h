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
	using scanner = generator<token>;
	using token_iterator = generator<token>::iterator;

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
	using infix_parse_fn = std::function<expression(expression&&)>;

	struct parse_rule
	{
		prefix_parse_fn prefix;
		infix_parse_fn infix;

		precedence precedence;
	};

	expression expr();
	expression hierarchical_exprs(precedence prec);
	expression grouping();
	expression unary();
	expression binary(expression&& left);
	expression ternary(expression&& prefix);
	expression primary();

	std::unordered_map<token_type, parse_rule> rules_ = {
		{TOKEN_PLUS, parse_rule{}},
	};

	token_iterator advance();
	token_iterator consume(token_type type,std::string_view msg);

	void error_at_current(std::string_view msg);
	void error(std::string_view msg);
	static void error_at(const token &tk, std::string_view msg);
	void synchronize();

	scanner tokens_;

	token_iterator current_;
	token_iterator previous_;
};

}