#include <iomanip>
#include <locale>

#include "file.h"
#include "tree.h"
#include "types.h"

#include "boost/program_options.hpp"
namespace po = boost::program_options;

// global parameter
po::variables_map vm;

int main(int argc, char* argv[])
{
	try {
		// command-line setup
		std::locale::global(std::locale("en-US.UTF-8"));
		std::cout.imbue(std::locale());
		std::cout << std::fixed << std::setprecision(3);

		// argument parser setup
		std::string input_path;
		std::string output_path;

		po::options_description desc("Options");
		desc.add_options()
			("help,h", "print help message")
			("verbose,v", "show additional info")
			("input-path", po::value<std::string>(&input_path), "path to the file that you want to compress/decompress\n(positional)")
			("output-path", po::value<std::string>(&output_path), "path to where to save the file\n(positional, optional)")
			;

		po::positional_options_description pos;
		pos.add("input-path", 1)
		   .add("output-path", 1);

		po::store(po::command_line_parser(argc, argv).options(desc).positional(pos).run(), vm);
		po::notify(vm); // notify if arguments are missing

		// program logic
		bool no_input = true;

		if (vm.count("help")) {
			no_input = false;
			std::cout << desc << "\n";
			std::cout << "Example:\nHZIP file_to_compress.txt C:/Users/[USER]/Documents/result.huf\n\n";
			return 0;
		}

		if (vm.count("input-path")) {
			no_input = false;
			std::shared_ptr<File> file = File::getInstance(input_path, output_path);
		}

		if (no_input) {
			std::cout << "HZIP\n - a simple compression program which uses dynamic Huffman encoding\nto try to reduce file size without loss of information.\n\n";
		}
	}
	catch (const std::exception& e) {
		std::cout << "error: " << e.what() << "\n";
		return 1;
	}
	catch (...) {
		std::cout << "Exception of unknown type!\n";
	}

	return 0;
}