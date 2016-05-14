#pragma once

#define RESULT_EC_SUCCESS 0

#define RESULT_EC_INVALIDERRORLEVEL 101
#define RESULT_EC_FILEREADEXCEPTION 102
#define RESULT_EC_INVALIDSIZE       103
#define RESULT_EC_INVALIDRUNNER     104

#define RESULT_EC_LIMITEXCEEDED     201
#define RESULT_EC_INVALIDCOMMAND    202

#define RESULT_EC_UNKNOWNEXCEPTION  999


enum STR2INT_ERROR { S2IE_SUCCESS, S2IE_OVERFLOW, S2IE_UNDERFLOW, S2IE_INCONVERTIBLE };


bool hasEnding(std::string const &fullString, std::string const &ending);
bool hasBeginning(std::string const &fullString, std::string const &beginning);

STR2INT_ERROR str2int(int &i, std::string s, int base = 0);

bool getStringArg(std::string &target, const std::string argument, const std::string argname);
bool getIntArg(int &target, const std::string argument, const std::string argname);
bool getBoolArg(bool &target, const std::string argument, const std::string argname);