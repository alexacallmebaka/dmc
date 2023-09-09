#include <iostream>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include "errors.hpp"
#include "scanner.hpp"

using namespace std;
using namespace drewno_mars;

static void usageAndDie(){
	std::cerr << "Usage: dmc <infile> <options>\n"
	<< " [-p]: Parse the input to check syntax\n"
	<< " [-t <tokensFile>]: Output tokens to <tokensFile>\n"
	<< "\n"
	;
	std::cout << std::flush;
	std::cerr << std::flush;
	exit(1);
}

static void writeTokenStream(const char * inPath, const char * outPath){
	std::ifstream inStream(inPath);
	if (!inStream.good()){
		std::string msg = "Bad input stream";
		msg += inPath;
		throw new InternalError(msg.c_str());
	}
	if (outPath == nullptr){
		std::string msg = "No tokens output file given";
		throw new InternalError(msg.c_str());
	}

	Scanner scanner(&inStream);
	if (strcmp(outPath, "--") == 0){
		scanner.outputTokens(std::cout);
	} else {
		std::ofstream outStream(outPath);
		if (!outStream.good()){
			std::string msg = "Bad output file ";
			msg += outPath;
			throw new InternalError(msg.c_str());
		}
		scanner.outputTokens(outStream);
		outStream.close();
	}
}

static bool parse(const char * inFile){
	std::ifstream inStream(inFile);
	if (!inStream.good()){
		std::string msg = "Bad input stream ";
		msg += inFile;
		throw new UserError(msg.c_str());
	}

	drewno_mars::Scanner scanner(&inStream);
	drewno_mars::Parser parser(scanner);
	int errCode = parser.parse();
	if (errCode != 0){ return false; }

	return true;
}

int main(const int argc, const char *argv[]){
	if (argc <= 1){ usageAndDie(); }

	const char * inFile = nullptr;
	const char * tokensFile = nullptr; // Output file if
	                                   // printing tokens
	bool checkParse = false;
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
			} else if (argv[i][1] == 'p'){
				i++;
				checkParse = true;
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

	try {
		if (tokensFile != nullptr){
			writeTokenStream(inFile, tokensFile);
		} if (checkParse){
			bool parsed = parse(inFile);
			if (!parsed){
				std::cerr << "Parse failed" << std::endl;
			}
		}
	} catch (ToDoError * e){
		std::cerr << "ToDo: " << e->msg() << std::endl;
		exit(1);
	} catch (InternalError * e){
		std::string msg = "Something in the compiler is broken: ";
		std::cerr << msg << e->msg() << std::endl;
		exit(1);
	} catch (UserError * e){
		std::string msg = "The user made a mistake: ";
		std::cerr << msg << e->msg() << std::endl;
		exit(1);
	}
	return 0;
}
