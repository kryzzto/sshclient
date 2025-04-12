#pragma once

#include <libssh/libssh.h>
#include <string>

class SSHClient 
{
private: 
	std::string host_;
	int port_;
	ssh_session session_;
private:
	bool IsConnected() const;
	void CleanupSession();
public:
	SSHClient(const std::string& host, int port = 22);
	~SSHClient();

	bool Connect(const std::string& username, const std::string& password);
	void Disconnect();
	bool ExecuteCommand(const std::string& command, std::string& output);
};