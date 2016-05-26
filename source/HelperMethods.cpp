#include "stdafx.h"
#include "HelperMethods.h"

bool hasEnding(std::string const &fullString, std::string const &ending) 
{
	if (fullString.length() >= ending.length()) 
	{
		return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
	}
	else 
	{
		return false;
	}
}

bool hasBeginning(std::string const &fullString, std::string const &beginning) 
{
	if (fullString.length() >= beginning.length()) 
	{
		return (0 == fullString.compare(0, beginning.length(), beginning));
	}
	else 
	{
		return false;
	}
}

STR2INT_ERROR str2int(int &i, std::string s, int base)
{
	char *end;
	long  l;
	errno = 0;
	l = strtol(s.c_str(), &end, base);
	if ((errno == ERANGE && l == LONG_MAX) || l > INT_MAX) {
		return S2IE_OVERFLOW;
	}
	if ((errno == ERANGE && l == LONG_MIN) || l < INT_MIN) {
		return S2IE_UNDERFLOW;
	}
	if (*end != '\0') {
		return S2IE_INCONVERTIBLE;
	}
	i = l;
	return S2IE_SUCCESS;
}

bool getStringArg(std::string &target, const std::string argument, const std::string argname)
{
	if (hasBeginning(argument, argname + "="))
	{
		target = argument.substr(0 + argname.length() + 1);

		if (hasBeginning(target, "\"") && hasEnding(target, "\"") && target.length() >= 2)
			target = target.substr(1, target.length() - 2);

		return true;
	}
	else if (hasBeginning(argument, "-" + argname + "="))
	{
		target = argument.substr(1 + argname.length() + 1);

		if (hasBeginning(target, "\"") && hasEnding(target, "\"") && target.length() >= 2)
			target = target.substr(1, target.length() - 2);

		return true;
	}
	else if (hasBeginning(argument, "--" + argname + "="))
	{
		target = argument.substr(2 + argname.length() + 1);

		if (hasBeginning(target, "\"") && hasEnding(target, "\"") && target.length() >= 2)
			target = target.substr(1, target.length() - 2);

		return true;
	}

	return false;
}

bool getIntArg(int &target, const std::string argument, const std::string argname)
{
	if (hasBeginning(argument, argname + "="))
	{
		int argInt;
		if (str2int(argInt, argument.substr(0 + argname.length() + 1)) == S2IE_SUCCESS)
		{
			target = argInt;
			return true;
		}
	}
	else if (hasBeginning(argument, "-" + argname + "="))
	{
		int argInt;
		if (str2int(argInt, argument.substr(1 + argname.length() + 1)) == S2IE_SUCCESS)
		{
			target = argInt;
			return true;
		}
	}
	else if (hasBeginning(argument, "--" + argname + "="))
	{
		int argInt;
		if (str2int(argInt, argument.substr(2 + argname.length() + 1)) == S2IE_SUCCESS)
		{
			target = argInt;
			return true;
		}
	}

	return false;
}

bool getBoolArg(bool &target, const std::string argument, const std::string argname)
{
	if (hasBeginning(argument, argname))
	{
		target = true;
		return true;
	}
	else if (hasBeginning(argument, "-" + argname))
	{
		target = true;
		return true;
	}
	else if (hasBeginning(argument, "--" + argname))
	{
		target = true;
		return true;
	}

	return false;
}