//
// Created by bear on 2/4/2023.
//

#pragma once

#include <lightlox/generator.h>

#include <string>
#include <utility>
#include <format>

#include <magic_enum.hpp>

namespace lightlox
{

enum token_type
{
	// Single-character tokens.
	TOKEN_LEFT_PAREN, TOKEN_RIGHT_PAREN,
	TOKEN_LEFT_BRACE, TOKEN_RIGHT_BRACE,
	TOKEN_COMMA, TOKEN_DOT, TOKEN_MINUS, TOKEN_MINUS_MINUS, TOKEN_PLUS, TOKEN_PLUS_PLUS,
	TOKEN_COLON, TOKEN_SEMICOLON, TOKEN_SLASH, TOKEN_STAR,TOKEN_STAR_STAR,
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

	TOKEN_ERROR, TOKEN_EOF
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
	token_type type;
	std::string lexeme;

	token(source_information _src, token_type _type, std::string _lexeme)
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
	token make_token(token_type type);

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

	token_type identifier_type();

	token_type keyword_match_rest(int start, const std::string &rest, token_type type);

	std::string code_{};

	std::string::const_iterator start_;
	std::string::const_iterator current_;

	int col_{};
	int line_{};
};
}

namespace std
{

template<>
struct formatter<lightlox::token_type>
{
	constexpr auto parse(std::format_parse_context &ctx)
	{
		auto pos = ctx.begin();
		while (pos != ctx.end() && *pos != '}')
		{
			if (*pos == 'd' || *pos == 'D')
			{
				detailed_ = true;
			}
			++pos;
		}

		return pos;
	}

	auto format(lightlox::token_type t, std::format_context &fc)
	{
		auto name = magic_enum::enum_name(t);
		if (detailed_)
		{
			return std::format_to(fc.out(), "{}<{}>", name, int(t));
		}
		else
		{
			return std::format_to(fc.out(), "{}", name);
		}
	}

private:
	bool detailed_{false};
};

template<>
struct formatter<lightlox::source_information>
{
	constexpr auto parse(std::format_parse_context &ctx)
	{
		auto pos = ctx.begin();
		while (pos != ctx.end() && *pos != '}')
		{
			if (*pos == 'd' || *pos == 'D')
			{
				detailed_ = true;
			}
			++pos;
		}

		return pos;
	}

	auto format(lightlox::source_information si, std::format_context &fc)
	{
		if (detailed_)
		{
			return std::format_to(fc.out(), "at line {}, column {}", si.line, si.column);
		}
		else
		{
			return std::format_to(fc.out(), "({}, {})", si.line, si.column);
		}
	}

private:
	bool detailed_{false};
};

template<>
struct formatter<lightlox::token>
{
	constexpr auto parse(std::format_parse_context &ctx)
	{
		auto pos = ctx.begin();
		while (pos != ctx.end() && *pos != '}')
		{
			if (*pos == 't' || *pos == 'T')
			{
				show_type_ = true;
			}
			else if (*pos == 'l' || *pos == 'L')
			{
				show_lexem_ = true;
			}
			else if (*pos == 'i' || *pos == 'I')
			{
				show_info_ = true;
			}
			++pos;
		}

		if (!show_type_ && !show_lexem_ && !show_info_)
		{
			show_lexem_ = true;
		}

		return pos;
	}

	auto format(lightlox::token tk, std::format_context &ctx)
	{
		std::format_to(ctx.out(), "Token< ");

		if (show_type_)
		{
			std::format_to(ctx.out(), "Type: {:d}, ", tk.type);
		}

		if (show_lexem_)
		{
			std::format_to(ctx.out(), "Lexeme: \"{}\", ", tk.lexeme);
		}

		if (show_info_)
		{
			std::format_to(ctx.out(), "At: {:s}", tk.src);
		}

		return std::format_to(ctx.out(), " >");
	}

private:
	bool show_type_{false};
	bool show_lexem_{false};
	bool show_info_{false};
};
}