//
// Created by bear on 2/4/2023.
//

#pragma once

#include <lightlox/generator.h>

#include <string>

namespace lightlox
{

enum token_types
{
	// Single-character tokens.
	LEFT_PAREN, RIGHT_PAREN, LEFT_BRACKET, RIGHT_BRACKET, LEFT_BRACE, RIGHT_BRACE,
	COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

	// One or two character tokens.
	BANG, BANG_EQUAL,
	EQUAL, EQUAL_EQUAL,
	GREATER, GREATER_EQUAL,
	LESS, LESS_EQUAL,

	// Literals.
	IDENTIFIER, STRING, NUMBER,

	// Keywords.
	AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
	PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,

	END
};

struct source_information
{
	int line, column;
};

struct token
{
	source_information src;
	token_types token;
	std::string lexeme;
};

generator<token> scan_all_tokens(const std::string &src);

}