#pragma once

#include <libssh/libssh.h>
#include <string>

class MySQLTunnel
{
private:
	ssh_session session_;
	ssh_channel channel_;
	std::string sshHost_;
	int sshPort_;
	int localPort_;
	bool isConnected_;
public:
	MySQLTunnel(const std::string& sshHost, int sshPort = 22);
	~MySQLTunnel();

	bool Connect(const std::string& user, const std::string& password);
	bool StartTunnel(int localPort, const std::string& remoteHost = "127.0.0.1", int remotePort = 3306);
	void StopTunnel();
	int GetLocalPort() const;
};