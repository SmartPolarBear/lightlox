//
// Created by bear on 2/8/2023.
//

#pragma once

#include <lexer/lexer.h>
#include <lightlox/generator.h>

#include <parser/expression.h>
#include <parser/statement.h>

#include <unordered_map>
#include <functional>

namespace lightlox
{

class parser
{
public:
	using scanner = generator<token>;
	using token_iterator = generator<token>::iterator;

	explicit parser(scanner &&tokens) :
		tokens_(std::move(tokens)),
		current_(tokens_.begin())
	{
	}

	generator<statement> parse();

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

	using prefix_parse_fn = expression(lightlox::parser::*)();
	using infix_parse_fn = expression(lightlox::parser::*)(expression &&);

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
	expression binary(expression &&left);
	expression ternary(expression &&prefix);
	expression primary();
	expression literal();

	std::unordered_map<token_type, parse_rule> rules_ = {
		{TOKEN_LEFT_PAREN, parse_rule{&parser::grouping, nullptr, PREC_CALL}},
		{TOKEN_RIGHT_PAREN, parse_rule{nullptr, nullptr, PREC_NONE}},
		{TOKEN_LEFT_BRACE, parse_rule{nullptr, nullptr, PREC_NONE}},
		{TOKEN_RIGHT_BRACE, parse_rule{nullptr, nullptr, PREC_NONE}},
		{TOKEN_COMMA, parse_rule{nullptr, nullptr, PREC_NONE}},
		{TOKEN_DOT, parse_rule{nullptr, nullptr, PREC_CALL}},
		{TOKEN_MINUS, parse_rule{&parser::unary, &parser::binary, PREC_TERM}},
		{TOKEN_PLUS, parse_rule{nullptr, &parser::binary, PREC_TERM}},
		{TOKEN_SEMICOLON, parse_rule{nullptr, nullptr, PREC_NONE}},
		{TOKEN_SLASH, parse_rule{nullptr, &parser::binary, PREC_FACTOR}},
		{TOKEN_STAR, parse_rule{nullptr, &parser::binary, PREC_FACTOR}},
		{TOKEN_BANG, parse_rule{&parser::unary, nullptr, PREC_NONE}},
		{TOKEN_BANG_EQUAL, parse_rule{nullptr, &parser::binary, PREC_EQUALITY}},
		{TOKEN_EQUAL, parse_rule{nullptr, nullptr, PREC_NONE}},
		{TOKEN_EQUAL_EQUAL, parse_rule{nullptr, &parser::binary, PREC_EQUALITY}},
		{TOKEN_GREATER, parse_rule{nullptr, &parser::binary, PREC_COMPARISON}},
		{TOKEN_GREATER_EQUAL, parse_rule{nullptr, &parser::binary, PREC_COMPARISON}},
		{TOKEN_LESS, parse_rule{nullptr, &parser::binary, PREC_COMPARISON}},
		{TOKEN_LESS_EQUAL, parse_rule{nullptr, &parser::binary, PREC_COMPARISON}},
		{TOKEN_IDENTIFIER, parse_rule{nullptr, nullptr, PREC_NONE}},
		{TOKEN_STRING, parse_rule{nullptr, nullptr, PREC_NONE}},
		{TOKEN_NUMBER, parse_rule{nullptr, nullptr, PREC_NONE}},
		{TOKEN_AND, parse_rule{nullptr, nullptr, PREC_AND}},
		{TOKEN_CLASS, parse_rule{nullptr, nullptr, PREC_NONE}},
		{TOKEN_ELSE, parse_rule{nullptr, nullptr, PREC_NONE}},
		{TOKEN_FALSE, parse_rule{&parser::literal, nullptr, PREC_NONE}},
		{TOKEN_FOR, parse_rule{nullptr, nullptr, PREC_NONE}},
		{TOKEN_FUN, parse_rule{nullptr, nullptr, PREC_NONE}},
		{TOKEN_IF, parse_rule{nullptr, nullptr, PREC_NONE}},
		{TOKEN_NIL, parse_rule{&parser::literal, nullptr, PREC_NONE}},
		{TOKEN_OR, parse_rule{nullptr, nullptr, PREC_OR}},
		{TOKEN_PRINT, parse_rule{nullptr, nullptr, PREC_NONE}},
		{TOKEN_RETURN, parse_rule{nullptr, nullptr, PREC_NONE}},
		{TOKEN_SUPER, parse_rule{nullptr, nullptr, PREC_NONE}},
		{TOKEN_THIS, parse_rule{nullptr, nullptr, PREC_NONE}},
		{TOKEN_TRUE, parse_rule{&parser::literal, nullptr, PREC_NONE}},
		{TOKEN_VAR, parse_rule{nullptr, nullptr, PREC_NONE}},
		{TOKEN_WHILE, parse_rule{nullptr, nullptr, PREC_NONE}},
		{TOKEN_ERROR, parse_rule{nullptr, nullptr, PREC_NONE}},
		{TOKEN_EOF, parse_rule{nullptr, nullptr, PREC_NONE}},
	};

	token_iterator advance();
	token_iterator consume(token_type type, std::string_view msg);
	bool match(token_type type);

	template<typename ...TTokenTypes>
	bool match_any(TTokenTypes... types)
	{
		return (match(types) || ...);
	}

	[[noreturn]] void error_at_current(std::string_view msg);
	[[noreturn]] void error(std::string_view msg);
	[[noreturn]] static void error_at(const token &tk, std::string_view msg);

	void synchronize();

	scanner tokens_;

	token_iterator current_;
	token_iterator previous_;
};

}