#include <fstream>
#include <sstream>
#include "tokenization.h"


int main(int argc, char* argv[]) {
	
	std::stringstream buffer;
	std::string inputPath = "test.html";
	{
		std::ifstream file(inputPath);

		if (!file.is_open()) {
			std::cerr << "Error: Could not open input file: " << inputPath << "\n";
			exit(EXIT_FAILURE);
		}

		buffer << file.rdbuf();
	}

	Tokenizator t(buffer.str());
	auto tokens = t.Tokenize();


	return 0;

}