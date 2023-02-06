//
// Created by bear on 2/4/2023.
//

#include <lexer/lexer.h>

#include <utility>
#include <string>

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

	if (std::isalpha(c) || c == '_')
	{
		return identifier();
	}
	else if (std::isdigit(c))
	{
		return numeric_literal();
	}

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

	case '"':
		return string_literal();
	}

	return token::error(source_information(line_, col_), "Unexpected character.");
}

token scanner::make_token(token_types type)
{
	std::string lexeme{start_, current_ + 1};

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
	if (peek(1) == '/')
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

token scanner::string_literal()
{
	while (peek() != '"' && !at_end())
	{
		if (peek() == '\n')
		{
			new_line();
		}
		advance();
	}

	if (at_end())
	{
		return token::error(source_information{line_, col_},
							"Unterminated string_literal.");
	}
	advance(); // terminating quote

	std::string content;
	for (auto i = start_; i != current_;)
	{
		if (*i == '\\')
		{
			if (i + 1 == current_)
			{
				return token::error(source_information{line_, col_},
									"Unknown escape character");
			}

			switch (*++i)
			{
			case 'n':
				content.push_back('\n');
				break;
			case 'r':
				content.push_back('\r');
				break;
			case 't':
				content.push_back('\t');
				break;
			case '\\':
				content.push_back('\\');
				break;
			default:
				return token::error(source_information{line_, col_},
									"Unknown escape character");
			}
			i++;
		}
		else
		{
			content.push_back(*i++);
		}
	}

	return token{source_information{line_, col_},
				 TOKEN_STRING,
				 content};
}

token scanner::numeric_literal()
{
	while (std::isdigit(peek()) || peek() == 'x' || peek() == 'b')
		advance();

	// Look for a fractional part.
	if (peek() == '.' && std::isdigit(peek(1)))
	{
		// Consume the ".".
		advance();

		while (std::isdigit(peek()))
			advance();
	}

	return make_token(TOKEN_NUMBER);
}

token scanner::identifier()
{
	while (std::isalpha(peek()) || std::isdigit(peek()) || peek() == '_')
	{
		advance();
	}
	return make_token(identifier_type());
}

token_types scanner::identifier_type()
{
	switch (*start_)
	{
	case 'a':
		return keyword_match_rest(1, "nd", TOKEN_AND);
	case 'c':
		return keyword_match_rest(1, "lass", TOKEN_CLASS);
	case 'e':
		return keyword_match_rest(1, "lse", TOKEN_ELSE);
	case 'f':
		if (current_ - start_ > 1)
		{
			switch (*(start_ + 1))
			{
			case 'a':
				return keyword_match_rest(2, "lse", TOKEN_FALSE);
			case 'o':
				return keyword_match_rest(2, "r", TOKEN_FOR);
			case 'u':
				return keyword_match_rest(2, "n", TOKEN_FUN);
			}
		}
		break;
	case 'i':
		return keyword_match_rest(1, "f", TOKEN_IF);
	case 'n':
		return keyword_match_rest(1, "il", TOKEN_NIL);
	case 'o':
		return keyword_match_rest(1, "r", TOKEN_OR);
	case 'p':
		return keyword_match_rest(1, "rint", TOKEN_PRINT);
	case 'r':
		return keyword_match_rest(1, "eturn", TOKEN_RETURN);
	case 's':
		return keyword_match_rest(1, "uper", TOKEN_SUPER);
	case 't':
		if (current_ - start_ > 1)
		{
			switch (*(start_ + 1))
			{
			case 'h':
				return keyword_match_rest(2, "is", TOKEN_THIS);
			case 'r':
				return keyword_match_rest(2, "ue", TOKEN_TRUE);
			}
		}
		break;
	case 'v':
		return keyword_match_rest(1, "ar", TOKEN_VAR);
	case 'w':
		return keyword_match_rest(1, "hile", TOKEN_WHILE);
	}
	return TOKEN_IDENTIFIER;
}

token_types scanner::keyword_match_rest(int start, const string &rest, token_types type)
{
	const auto length = rest.length();

	if (current_ - start_ == start + length)
	{
		for (auto it1 = start_ + start, it2 = rest.begin();
			 it1 != current_ && it2 != rest.end(); it1++, it2++)
		{
			if (*it1 != *it2)
			{
				return TOKEN_IDENTIFIER;
			}
		}
		return type;
	}

	return TOKEN_IDENTIFIER;
}

