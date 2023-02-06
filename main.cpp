#include <iostream>
#include <argparse/argparse.hpp>

using namespace std;
using namespace argparse;

int main(int argc, char **argv)
{
	ArgumentParser args(argv[0]);

	args.add_argument("file").required();

	args.add_argument("-v", "--verbose")
		.default_value(false)
		.implicit_value(true);

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



	return 0;
}
