#include "mysql.h"
#include "credentials.h"
#include <iostream>

MySQLTunnel::MySQLTunnel(const std::string& sshHost, int sshPort)
	: sshHost_(sshHost), sshPort_(sshPort), session_(nullptr), channel_(nullptr),
	  localPort_(0), isConnected_(false) {}

MySQLTunnel::~MySQLTunnel()
{
	StopTunnel();
	if (session_)
	{
		ssh_disconnect(session_);
		ssh_free(session_);
	}
}

bool MySQLTunnel::Connect(const std::string& user, const std::string& password)
{
	session_ = ssh_new();
	if (!session_)
		return false;

	ssh_options_set(session_, SSH_OPTIONS_HOST, sshHost_.c_str());
	ssh_options_set(session_, SSH_OPTIONS_PORT, &sshPort_);
	ssh_options_set(session_, SSH_OPTIONS_USER, user.c_str());

	if (ssh_connect(session_) != SSH_OK)
	{
		std::cerr << "SSH connection failed: " << ssh_get_error(session_) << std::endl;
		return false;
	}

	if (ssh_userauth_password(session_, user.c_str(), password.c_str()) != SSH_AUTH_SUCCESS)
	{
		std::cerr << "SSH authentification failed: " << ssh_get_error(session_) << std::endl;
		return false;
	}

	isConnected_ = true;
	return true;
}

bool MySQLTunnel::StartTunnel(int localPort, const std::string& remoteHost, int remotePort)
{
	if (!isConnected_)
		return false;

	localPort_ = localPort;

	std::string command = "ssh -N -L " + std::to_string(localPort_) + ":" + remoteHost + ":" + std::to_string(remotePort) + " " + sshHost_ + " -p " + std::to_string(sshPort_);

	int result = system(command.c_str());

	if (result == SSH_OK)
	{
		std::cout << "Tunnel established on local port " << localPort_ << std::endl;
		return true;
	}
	else
	{
		std::cout << "Failed to start tunnel. SSH command exited with status " << result << std::endl;
		return false;
	}
/*
	channel_ = ssh_channel_new(session_);
	if (!channel_)
	{
		std::cout << "Failed to create channel\n";
		return false;
	}

	if (ssh_channel_open_session(channel_) != SSH_OK)
	{
		std::cout << "Failed to open SSH channel for port forwarding\n";
		ssh_channel_free(channel_);
		return false;
	}

	if (ssh_channel_request_exec(channel_, command.c_str()) != SSH_OK)
	{
		std::cerr << "Port forwarding failed\n";
		ssh_channel_close(channel_);
		ssh_channel_free(channel_);
		return false;
	}

	return true;
*/
}

void MySQLTunnel::StopTunnel()
{
	if (channel_)
	{
		ssh_channel_send_eof(channel_);
		ssh_channel_close(channel_);
		ssh_channel_free(channel_);
		channel_ = nullptr;
	}
}

int MySQLTunnel::GetLocalPort() const
{
	return localPort_;
}