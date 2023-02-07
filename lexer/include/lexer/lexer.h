//
// Created by bear on 2/4/2023.
//

#pragma once

#include <lightlox/generator.h>

#include <string>
#include <utility>
#include <format>

namespace lightlox
{

enum token_types
{
	// Single-character tokens.
	TOKEN_LEFT_PAREN, TOKEN_RIGHT_PAREN,
	TOKEN_LEFT_BRACE, TOKEN_RIGHT_BRACE,
	TOKEN_COMMA, TOKEN_DOT, TOKEN_MINUS, TOKEN_PLUS,
	TOKEN_SEMICOLON, TOKEN_SLASH, TOKEN_STAR,
	// One or two character tokens.
	TOKEN_BANG, TOKEN_BANG_EQUAL,
	TOKEN_EQUAL, TOKEN_EQUAL_EQUAL,
	TOKEN_GREATER, TOKEN_GREATER_EQUAL,
	TOKEN_LESS, TOKEN_LESS_EQUAL,
	// Literals.
	TOKEN_IDENTIFIER, TOKEN_STRING, TOKEN_NUMBER,
	// Keywords.
	TOKEN_AND, TOKEN_CLASS, TOKEN_ELSE, TOKEN_FALSE,
	TOKEN_FOR, TOKEN_FUN, TOKEN_IF, TOKEN_NIL, TOKEN_OR,
	TOKEN_PRINT, TOKEN_RETURN, TOKEN_SUPER, TOKEN_THIS,
	TOKEN_TRUE, TOKEN_VAR, TOKEN_WHILE,

	TOKEN_ERROR, // TOKEN_EOF
};

struct source_information
{
	int line, column;

	source_information(int _line, int _column)
		: line(_line), column(_column)
	{
	}
};

struct token
{
	source_information src;
	token_types type;
	std::string lexeme;

	token(source_information _src, token_types _type, std::string _lexeme)
		: src(_src), type(_type), lexeme(std::move(_lexeme))
	{
	}

	static token error(source_information src, std::string msg);
};

class scanner
{
public:
	explicit scanner(std::string code);

	generator<token> scan_all_tokens();
private:
	token make_token(token_types type);

	token scan_next_token();

	char advance();

	char peek(int len = 0);

	void skip_whitespaces();

	void skip_comments();

	void new_line();

	bool match(char expected);

	bool at_end();

	token string_literal();

	token numeric_literal();

	token identifier();

	token_types identifier_type();

	token_types keyword_match_rest(int start, const std::string &rest, token_types type);

	std::string code_{};

	std::string::const_iterator start_;
	std::string::const_iterator current_;

	int col_{};
	int line_{};
};
}

namespace std
{
template<class CharT>
struct formatter<lightlox::source_information, CharT>
{
	template<typename FormatParseContext>
	auto parse(FormatParseContext &pc)
	{
		// parse formatter args like padding, precision if you support it
		return pc.end(); // returns the iterator to the last parsed character in the format string, in this case we just swallow everything
	}

	template<typename FormatContext>
	auto format(lightlox::source_information si, FormatContext &fc)
	{
		return std::format_to(fc.out(), "at line {}, column {}", si.line, si.column);
	}
};
}