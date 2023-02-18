//
// Created by bear on 2/8/2023.
//

#include <lightlox/io.h>

#include <parser/parser.h>
#include <parser/expression.h>
#include <parser/statement.h>

#include <format>

using namespace std;
using namespace lightlox;

lightlox::expression lightlox::parser::expr()
{
	while (current_ != tokens_.end())
	{
		try
		{
			return hierarchical_exprs(PREC_TERNARY);
		}
		catch (const std::runtime_error &e)
		{
			synchronize();
		}
	}
}

lightlox::expression lightlox::parser::hierarchical_exprs(precedence prec)
{
	auto tk = advance();
	auto prefix = rules_[tk->type].prefix;
	if (!prefix)
	{
		error("Expected expression.");
		return expression_placeholder;
	}

	auto expr = prefix();

	while (prec <= rules_[current_->type].precedence)
	{
		tk = advance();
		auto infix = rules_[tk->type].infix;
		if (!infix)
		{
			error("Expected expression.");
			return expression_placeholder;
		}

		expr = infix(std::move(expr));
	}

	return expr;
}

lightlox::expression lightlox::parser::primary()
{
	return lightlox::expression();
}

parser::token_iterator lightlox::parser::advance()
{
	previous_ = current_;
	if (current_ == tokens_.end())
	{
		return previous_;
	}
	for (;;)
	{
		current_++;
		if (current_->type != TOKEN_ERROR)
		{
			break;
		}

		error_at_current(current_->lexeme);
	}
	return previous_;
}

void lightlox::parser::error_at_current(std::string_view msg)
{
	error_at(*current_, msg);
}

void lightlox::parser::error(std::string_view msg)
{
	error_at(*previous_, msg);
}

void lightlox::parser::error_at(const lightlox::token &tk, std::string_view msg)
{
	auto full = format("[Line {}, Column {}] Error ", tk.src.line, tk.src.column);

	if (tk.type == TOKEN_EOF)
	{
		full += "at end";
	}
	else if (tk.type == TOKEN_ERROR)
	{
		// Nothing.
	}
	else
	{
		full += format("at '{}'", tk.lexeme);
	}

	full += format(":\n {}", msg);
	logger::instance().log(log_type::ERROR, full);
	throw std::runtime_error(full);
}

void parser::synchronize()
{

}
