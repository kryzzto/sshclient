#pragma once

#include <string>

struct Credentials
{
	std::string host_;
	std::string user_;
	std::string password_;
};

Credentials PromptCredentials();