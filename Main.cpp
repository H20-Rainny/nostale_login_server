#include <iostream>
#include <string>
#include <map>
#include <winsock2.h>
#include <vector>
#include <process.h>
#include <mutex>
#include <mysql.h>

MYSQL *conn;
bool maintenance;

#include "Functions.h"
#include "Database.h"
#include "Cryptography.h"
#include "Threads.h"
#include "Protection.h"

#pragma comment(lib,"ws2_32.lib")

int main()
{
	system("title Nostale");

	std::cout << "-----------------------------------------" << std::endl
		<< "AUTHENTICATION SERVER" << std::endl
		<< "-----------------------------------------" << std::endl
		<< "Data: 11-08-2013" << std::endl
		<< "Author: Smoke" << std::endl << std::endl;

	_beginthread((void(__cdecl*)(void*))reset, (unsigned int) 0, (void*) 60000);

	std::cout << "protection thread started !" << std::endl;

	_database *database = new _database();
	_protection *protection = new _protection();
	_cryptography *cryptography = new _cryptography();
	_functions *functions = new _functions();

	conn = database->initialize(NULL);
	
	if (!database->connection(conn, "127.0.0.1", "root", "123456", "nostale"))
	{
		std::cout << "could not connect to 127.0.0.1:3306" << std::endl;
		getchar();
		return 1;
	}

	maintenance = false;
	functions->msg_init(msg_maintenance,msg_case,msg_online,msg_ban);

	WSAData wsa;
	WSAStartup(0x0201, &wsa);

	SOCKET sock_ls = socket(AF_INET, SOCK_STREAM, 0), sock = NULL;

	SOCKADDR_IN sock_addr;
	sock_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = htons(4003);

	bind(sock_ls, (SOCKADDR*) &sock_addr, sizeof(sock_addr));

	std::cout << "server successfully started !" << std::endl;

	_beginthread((void(__cdecl*)(void*) )commands, (unsigned int) 0, (void*) 0);

	std::cout << "commands thread started !" << std::endl;

	int ls_result = 0, sin_size = 0;
	while (ls_result >= 0)
	{
		ls_result = listen(sock_ls, 0x7FFFFFFF);
		if (ls_result < 0)
		{
			WSACleanup();
		}
		else
		{
			sin_size = sizeof(struct sockaddr_in);
			sock = accept(sock_ls, (SOCKADDR*) &sock_addr, &sin_size);

			if (protection->check(inet_ntoa(sock_addr.sin_addr)) == 0)
			{
				_beginthread((void(__cdecl*)(void*))client, (unsigned int) 0, (void*) sock);
			}
			else
			{
				std::string msg = "fail Your ip has been blocked due to repeated login attempts.";
				msg += 0x0D;
				msg += "You will be unlocked in a span of 60 seconds.";
				msg = cryptography->encrypt(msg);
				send(sock, msg.c_str(), msg.size(), 0);
			}
		}
		sock = NULL;
	}

	getchar();
	return 0;
}