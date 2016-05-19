// BefunRun.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "HelperMethods.h"
#include "IBefungeRunner.h"
#include "BFRunException.h"

#include "BefungeRunner0.h"
#include "BefungeRunner1.h"
#include "BefungeRunner2.h"
#include "BefungeRunner3.h"
#include "BefungeRunnerInfo.h"

void showHelp();
int execute(int argc, char* argv[]);


int main(int argc, char* argv[])
{
	try
	{
		return execute(argc, argv);
	}
	catch (const std::exception& ex)
	{
		std::cerr << "internal exception in program: " << std::endl;
		std::cerr << ex.what() << std::endl;

		return RESULT_EC_UNKNOWNEXCEPTION;
	}
	catch (...)
	{
		std::cerr << "Unknown internal exception in file " << __FILE__ << " at line " << __LINE__ << std::endl;

		return RESULT_EC_UNKNOWNEXCEPTION;
	}

	std::getchar();
}

int execute(int argc, char* argv[])
{
	if (argc <= 1) {
		showHelp();
		return RESULT_EC_SUCCESS;
	}

	std::string fileInput = argv[1];
	int errorlevel = 0;
	int limit = -1;
	bool info = false;
	bool help = false;
	
	for (int i = 1; i < argc; i++)
	{
		getStringArg(fileInput, argv[i], "file");
		getIntArg(errorlevel, argv[i], "errorlevel");
		getIntArg(limit, argv[i], "limit");
		getBoolArg(info, argv[i], "info");
		getBoolArg(help, argv[i], "help");
	}

	if (help) {
		showHelp();
		return RESULT_EC_SUCCESS;
	}

	if (errorlevel < 0 || errorlevel > 3)
	{
		std::cerr << "Invalid value for errorlevel: " << errorlevel << std::endl;

		return RESULT_EC_INVALIDERRORLEVEL;
	}

	int width = 0;
	int height = 0;
	std::vector<std::string> lines;

	try 
	{
		std::ifstream file(fileInput);

		if (!file.is_open())
		{
			std::cerr << "Cannot open file: " << fileInput << std::endl;

			return RESULT_EC_FILEREADEXCEPTION;
		}

		std::string str;
		bool first = true;
		while (std::getline(file, str))
		{
			// UTF-8 BOM
			if (first && str.length() >= 3 && (uint8_t)str.at(0) == 0xEF && (uint8_t)str.at(1) == 0xBB && (uint8_t)str.at(2) == 0xBF)
			{
				str = str.substr(3);
			}

			lines.push_back(str);
			width = std::max(width, (int)str.length());
			height++;

			first = false;
		}
	}
	catch (const std::exception& ex)
	{
		std::cerr << "Cannot open/read file " << fileInput << std::endl;
		std::cerr << ex.what() << std::endl;

		return RESULT_EC_FILEREADEXCEPTION;
	}
	catch (...)
	{
		std::cerr << "Unknown internal exception in file " << __FILE__ << " at line " << __LINE__ << std::endl;

		return RESULT_EC_UNKNOWNEXCEPTION;
	}

	if (width <= 0 || height <= 0)
	{
		std::cerr << "The program has an size of zero" << std::endl;

		return RESULT_EC_INVALIDSIZE;
	}

	IBefungeRunner *runner;

	if (info)
	{
		runner = new BefungeRunnerInfo(width, height);
	}
	else if (errorlevel == 0)
	{
		runner = new BefungeRunner0(width, height);
	}
	else if (errorlevel == 1)
	{
		runner = new BefungeRunner1(width, height);
	}
	else if (errorlevel == 2)
	{
		runner = new BefungeRunner2(width, height);
	}
	else if (errorlevel == 3)
	{
		runner = new BefungeRunner3(width, height);
	}
	else
	{
		std::cerr << "Cannot create a befunge runner for the supplied parameters" << std::endl;
		return RESULT_EC_INVALIDRUNNER;
	}

	runner->Init(lines);
	runner->SetLimit(limit);

	try
	{
		runner->Run();
	}
	catch (const BFRunException& ex)
	{
		std::cerr << ex.what() << std::endl;

		return ex.exitCode;
	}
	catch (const std::exception& ex)
	{
		std::cerr << "Internal exception while running program:" << std::endl;
		std::cerr << ex.what() << std::endl;

		return RESULT_EC_UNKNOWNEXCEPTION;
	}
	catch (...)
	{
		std::cerr << "Unknown internal exception in file " << __FILE__ << " at line " << __LINE__ << std::endl;

		return RESULT_EC_UNKNOWNEXCEPTION;
	}

	delete runner;

	return RESULT_EC_SUCCESS;
}

void showHelp()
{
	std::cout << "                                                                               " << std::endl;
	std::cout << "BefunRun file [parameter]                                                      " << std::endl;
	std::cout << "                                                                               " << std::endl;
	std::cout << "########## Parameter ##########                                                " << std::endl;
	std::cout << "                                                                               " << std::endl;
	std::cout << "file              : The input sourcecode file in *.tf format                   " << std::endl;
	std::cout << "                                                                               " << std::endl;
	std::cout << "errorlevel        : Set the errorlevel                                         " << std::endl;
	std::cout << "   - errorlevel=0 : Never abort                                                " << std::endl;
	std::cout << "   - errorlevel=1 : Abort when:                                                " << std::endl;
	std::cout << "                     - reading from empty stack                                " << std::endl;
	std::cout << "                     - reading/writing invalid coordinates                     " << std::endl;
	std::cout << "                     - executing an invalid command character                  " << std::endl;
	std::cout << "   - errorlevel=2 : Abort on previous conditions and when:                     " << std::endl;
	std::cout << "                     - moving PC over the program edge                         " << std::endl;
	std::cout << "   - errorlevel=3 : Abort on previous conditions and when:                     " << std::endl;
	std::cout << "                     - executing a previously modified cell                    " << std::endl;
	std::cout << "                                                                               " << std::endl;
	std::cout << "limit             : Abort after n steps                                        " << std::endl;
	std::cout << "                                                                               " << std::endl;
	std::cout << "info              : Don't show output. Show program informations               " << std::endl;
	std::cout << "                                                                               " << std::endl;
}