#include <lightlox/lightlox.h>
#include <lightlox/io.h>

#include <lexer/lexer.h>

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <sstream>

#include <argparse/argparse.hpp>

using namespace lightlox;

using namespace std;
using namespace argparse;

int run_file(const std::string &filename)
{
	ifstream file{filename};
	if (!file.is_open() || file.bad())
	{
		throw runtime_error{"Cannot open the file"};
	}

	string code = slurp(file);

	scanner scanner{code};
	auto tokens = scanner.scan_all_tokens();

	DEBUG_ONLY
	{
		stringstream token_ss{};
		for (const auto &t: tokens)
		{
			token_ss << std::format("{:tli}", t) << endl;
		}
		logger::instance().log(log_type::DEBUG, token_ss.str());
	}



	return 0;
}

int main(int argc, char **argv)
{
	ArgumentParser args(argv[0]);

	args.add_argument("file").required();

	args.add_argument("--log");

	int verbosity = 0;
	args.add_argument("-v", "--verbose")
		.action([&verbosity](const auto &)
				{ ++verbosity; })
		.append()
		.default_value(false)
		.implicit_value(true)
		.nargs(0);

	try
	{
		args.parse_args(argc, argv);
	}
	catch (const std::runtime_error &err)
	{
		std::cerr << err.what() << std::endl;
		std::cerr << args;
		std::exit(1);
	}

	logger::instance().set_verbosity(verbosity);

	auto source = args.get<string>("file");

	int ret = 0;
	try
	{
		ret = run_file(source);
	}
	catch (const std::runtime_error &err)
	{
		logger::instance().log(log_type::ERROR, std::format("Compiler internal error: {}", err.what()));
		return 1;
	}

	if (auto log = args.present("--log"))
	{
		logger::instance().dump(log.value());
	}

	return ret;
}
