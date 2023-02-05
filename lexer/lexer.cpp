//
// Created by bear on 2/4/2023.
//

#include <lexer/lexer.h>

#include <utility>

using namespace std;
using namespace lightlox;

token token::error(source_information src, std::string msg)
{
	return token{src, TOKEN_ERROR, std::move(msg)};
}

scanner::scanner(std::string code)
	: code_(std::move(code)),
	  start_(code_.begin()),
	  current_(code_.begin())
{
}

generator<token> scanner::scan_all_tokens()
{
	while (start_ <= current_ && !at_end())
	{
		auto token = scan_next_token();
		co_yield token;
	}
	co_return;
}

token scanner::scan_next_token()
{
	skip_whitespaces();
	start_ = current_;

	char c = advance();

	switch (c)
	{
	case '(':
		return make_token(TOKEN_LEFT_PAREN);
	case ')':
		return make_token(TOKEN_RIGHT_PAREN);
	case '{':
		return make_token(TOKEN_LEFT_BRACE);
	case '}':
		return make_token(TOKEN_RIGHT_BRACE);
	case ';':
		return make_token(TOKEN_SEMICOLON);
	case ',':
		return make_token(TOKEN_COMMA);
	case '.':
		return make_token(TOKEN_DOT);
	case '-':
		return make_token(TOKEN_MINUS);
	case '+':
		return make_token(TOKEN_PLUS);
	case '/':
		return make_token(TOKEN_SLASH);
	case '*':
		return make_token(TOKEN_STAR);

	case '!':
		return make_token(
			match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
	case '=':
		return make_token(
			match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
	case '<':
		return make_token(
			match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
	case '>':
		return make_token(
			match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
	}

	return token::error(source_information(line_, col_), "Unexpected character.");
}

token scanner::make_token(token_types type)
{
	string lexeme{start_, current_ + 1};

	return {source_information(line_, col_),
			type,
			lexeme};
}
char scanner::advance()
{
	col_++;
	return *current_++;
}

void scanner::new_line()
{
	line_++;
	col_ = 0;
}

bool scanner::match(char expected)
{
	if (at_end())
	{
		return false;
	}
	if (*current_ != expected)
	{
		return false;
	}
	current_++;
	return true;
}
void scanner::skip_whitespaces()
{
	while (!at_end())
	{
		auto c = peek();
		switch (c)
		{
		case ' ':
		case '\r':
		case '\t':
			advance();
			break;
		case '\n':
			new_line();
			advance();
			break;
		case '/':
			skip_comments();
			break;
		default:
			return;
		}
	}
}

void scanner::skip_comments()
{
	if (peek(1))
	{
		while (peek() != '\n' && !at_end())
		{
			advance();
		}
	}
}

char scanner::peek(int len)
{
	if (len > code_.end() - current_)
	{
		return 0;
	}
	return *(current_ + len);
}

bool scanner::at_end()
{
	return current_ == code_.end();
}

