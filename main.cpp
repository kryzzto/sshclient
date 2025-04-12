#include "credentials.h"
#include "ssh.h"
#include "mysql.h"
#include <iostream>
#include <string>

int main()
{
	try 
	{
		Credentials credentials = PromptCredentials();
		SSHClient sshClient(credentials.host_);
		MySQLTunnel mysqlTunnel(credentials.host_);

		// Step 1: Establish SSH connection
		if (!sshClient.Connect(credentials.user_, credentials.password_))
		{
			std::cerr << "SSH connection failed\n";
			return 1;
		}

		std::string command, output;
		while (true)
		{
			std::cout << "ssh> ";
			std::getline(std::cin, command);

			if (command == "exit" || command == "quit")
				break;

			if (command.empty())
				continue;

			if (command.find("mysql") != std::string::npos)
			{
				// Start MySQL tunnel
				if (!mysqlTunnel.Connect(credentials.user_, credentials.password_))
				{
					std::cerr << "MySQL tunnel connection failed\n";
					continue;
				}

				if (mysqlTunnel.StartTunnel(3306))
					std::cout << "MySQL tunnel established on local port 3306\n";
			}

			// Handle other SSH commands (non-MySQL related)
			if (sshClient.ExecuteCommand(command, output))
				std::cout << output << std::endl;
			else
				std::cerr << "Command failed\n";
		}

		sshClient.Disconnect();
		mysqlTunnel.StopTunnel();
	}

	catch (const std::exception& ex)
	{
		std::cerr << "Exception: " << ex.what() << std::endl;
		return 1;
	}
}