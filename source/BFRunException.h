#pragma once

class BFRunException : public std::exception 
{

private:
	std::string err_msg;

public:
	int exitCode;

public:
	BFRunException(const std::string msg, int ec) : err_msg(msg), exitCode(ec) {};
	~BFRunException() throw() {};

	const char *what() const throw() { return this->err_msg.c_str(); };

};