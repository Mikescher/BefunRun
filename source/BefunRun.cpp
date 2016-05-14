// BefunRun.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "HelperMethods.h"

void showHelp();

int main(int argc, char* argv[])
{
	if (argc <= 1) {
		showHelp();
		return 0;
	}

	std::string fileInput = argv[1];
	int errorlevel = 0;
	int limit = -1;
	bool info = false;
	
	for (int i = 1; i < argc; i++)
	{
		getStringArg(fileInput, argv[i], "file");
		getIntArg(errorlevel, argv[i], "errorlevel");
		getIntArg(limit, argv[i], "limit");
		getBoolArg(info, argv[i], "info");
	}

	if (errorlevel < 0 || errorlevel > 3)
	{
		std::cerr << "Invalid value for errorlevel: " << errorlevel << std::endl;

		return RESULT_EC_INVALIDERRORLEVEL;
	}

	try 
	{
		std::ifstream file(fileInput);
		std::string str;
		while (std::getline(file, str))
		{
			std::cout << str << std::endl;
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
		std::cerr << "Unknown internal exception in file " << __FILE__ << "at line " << __LINE__ << std::endl;

		return RESULT_EC_UNKNOWNEXCEPTION;
	}

	std::getchar();
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