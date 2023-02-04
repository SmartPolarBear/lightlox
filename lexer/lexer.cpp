//
// Created by bear on 2/4/2023.
//

#include <lexer/lexer.h>

using namespace std;
using namespace lightlox;

#define SINGLE_CHAR_TOKEN(type, ch) \
case ch: \
    col++;                              \
    lexeme.clear();                      \
    lexeme+=ch;\
    co_yield token{source_information{col, line},\
        type, \
        lexeme};                        \
break                               \

lightlox::generator<lightlox::token> lightlox::scan_all_tokens(const std::string &src)
{
	string lexeme;
	int col = 0, line = 0;
	for (auto it = src.begin(); it != src.end();)
	{
		char c = *it++;
		switch (c)
		{
		SINGLE_CHAR_TOKEN(token_types::LEFT_PAREN, '(');
		SINGLE_CHAR_TOKEN(token_types::RIGHT_PAREN, ')');
		SINGLE_CHAR_TOKEN(token_types::LEFT_BRACKET, '[');
		SINGLE_CHAR_TOKEN(token_types::RIGHT_BRACKET, ']');
		SINGLE_CHAR_TOKEN(token_types::LEFT_BRACE, '{');
		SINGLE_CHAR_TOKEN(token_types::RIGHT_BRACE, '}');
		default:
			break;
		}
	}
	co_return;
}
