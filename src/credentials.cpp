#include "credentials.h"
#include <iostream>
#include <conio.h>

std::string GetHiddenPassword()
{
	std::string password;
	char ch;

	while ((ch = _getch()) != '\r')
	{
		if (ch == '\b' && !password.empty())
		{
			password.pop_back();
			std::cout << '\b \b';
		}
		else
		{
			password += ch;
			std::cout << '*';
		}
	}

	std::cout << '\n';
	return password;
}

Credentials PromptCredentials()
{
	Credentials c;
	std::cout << "Hostname (or IP Address): ";
	std::cin >> c.host_;

	std::cout << "Username: ";
	std::cin >> c.user_;
	std::cin.get();

	std::cout << "Password: ";
	c.password_ = GetHiddenPassword();

	return c;
}