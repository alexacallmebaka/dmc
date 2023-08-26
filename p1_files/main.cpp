#include <fstream>
#include <string.h>

#include "errors.hpp"
#include "scanner.hpp"

using namespace std;
using namespace drewno_mars;

static void usageAndDie(){
	std::cerr << "Usage: dmc <infile> <options>\n"
	<< " [-t <tokensFile>]: Output tokens to <tokensFile>\n"
	<< "\n"
	;
	std::cout << std::flush;
	std::cerr << std::flush;
	exit(1);
}

static std::ifstream * openInput(const char * inputPath){
	if (inputPath == nullptr){ usageAndDie(); }

	std::ifstream * input = new std::ifstream(inputPath);
	if (input == NULL){ usageAndDie(); }
	if (!input->good()){
		std::cerr << "Bad path " <<  inputPath << std::endl;
		usageAndDie();
	}
	return input;
}

static void doTokenization(const char * inputPath, const char * outPath){
	std::ifstream * input = openInput(inputPath);

	drewno_mars::Scanner scanner(input);
	if (strcmp(outPath, "--") == 0){
		scanner.outputTokens(std::cout);
	} else {
		std::ofstream outStream(outPath);
		if (!outStream.good()){
			std::string msg = "Bad output file ";
			msg += outPath;
			throw new drewno_mars::InternalError(msg.c_str());
		}
		scanner.outputTokens(outStream);
	}
}

int main(const int argc, const char * argv[]){
	if (argc <= 1){ usageAndDie(); }

	const char * inFile = nullptr;
	const char * tokensFile = nullptr; // Output file if
	                                   // printing tokens
	bool useful = false;               // Check whether the command is 
	                                   // a no-op

	//Loop through the command line looking for options.
	// More options will be added as the compiler is extended
	for (int i = 1; i < argc; i++){
		if (argv[i][0] == '-'){
			if (argv[i][1] == 't'){
				i++;
				if (i >= argc){ usageAndDie(); }
				tokensFile = argv[i];
				useful = true;
			} else {
				std::cerr << "Unknown option"
				  << " " << argv[i] << "\n";
				usageAndDie();
			}
		} else {
			if (inFile == NULL){
				inFile = argv[i];
			} else {
				std::cerr << "Only 1 input file allowed";
				std::cerr << argv[i] << std::endl;
				usageAndDie();
			}
		}
	}
	if (inFile == nullptr){ usageAndDie(); }
	if (!useful){
		std::cerr << "You didn't specify an operation to do!\n";
		usageAndDie();
	}


	if (tokensFile != nullptr){
		doTokenization(inFile, tokensFile);
	}
	return 0;
}
