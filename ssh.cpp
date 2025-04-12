#include "ssh.h"
#include <iostream>

SSHClient::SSHClient(const std::string& host, int port)
	: host_(host), port_(port), session_(ssh_new())
{
	if (!session_)
		throw std::runtime_error("Failed to create SSH session");

	ssh_options_set(session_, SSH_OPTIONS_HOST, host.c_str());
	ssh_options_set(session_, SSH_OPTIONS_HOST, &port_);
}

SSHClient::~SSHClient()
{
	Disconnect();
	CleanupSession();
}

bool SSHClient::Connect(const std::string& username, const std::string& password)
{
	ssh_options_set(session_, SSH_OPTIONS_USER, username.c_str());
	
	if (ssh_connect(session_) != SSH_OK)
	{
		std::cerr << "Connection failed: " << ssh_get_error(session_) << std::endl;
		return false;
	}

	int rc = ssh_userauth_password(session_, nullptr, password.c_str());
	if (rc != SSH_AUTH_SUCCESS)
	{
		std::cerr << "Authentification failed: " << ssh_get_error(session_) << std::endl;
		return false;
	}

	return true;
}

void SSHClient::Disconnect()
{
	if (IsConnected())
		ssh_disconnect(session_);
}

bool SSHClient::ExecuteCommand(const std::string& command, std::string& output)
{
	if (!IsConnected())
	{
		std::cerr << "Not connected\n";
		return false;
	}

	ssh_channel channel = ssh_channel_new(session_);
	if (!channel)
	{
		std::cerr << "Failed to create SSH channel\n";
		return false;
	}

	if (ssh_channel_open_session(channel) != SSH_OK)
	{
		std::cerr << "Failed to open channel: " << ssh_get_error(session_) << std::endl;
		ssh_channel_free(channel);
		return false;
	}

	int ssh_channel_request = ssh_channel_request_exec(channel, command.c_str());
	if (ssh_channel_request != SSH_OK)
	{
		std::cerr << "Command execution failed: " << ssh_get_error(session_) << std::endl;

		ssh_channel_close(channel);
		ssh_channel_free(channel);
		return false;
	}

	char buffer[256];
	int nbytes;
	output.clear();
	while ((nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0)) > 0)
		output.append(buffer, nbytes);

	while ((nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 1)) > 0)
		output.append(buffer, nbytes);

	ssh_channel_send_eof(channel);
	ssh_channel_close(channel);
	ssh_channel_free(channel);

	return true;
}

bool SSHClient::IsConnected() const
{
	return session_ && ssh_is_connected(session_);
}

void SSHClient::CleanupSession()
{
	if (session_)
	{
		ssh_free(session_);
		session_ = nullptr;
	}
}