#ifndef Threads_h
#define Threads_h

std::mutex t_mutex;
std::string msg_maintenance, msg_case, msg_online, msg_ban;

void client(void *arg)
{
	SOCKET sock = (SOCKET) arg;

	_functions *functions = new _functions();
	_cryptography *cryptography = new _cryptography();
	_database *database = new _database();

	char packet[4096 + 1];
	int bytes = 0;

	try
	{
		bytes = recv(sock, packet, 4096, 0);
	}
	catch (...)
	{
		bytes = 0;
	}

	if (bytes >= 200 || bytes <= 60)
	{
		closesocket(sock);
		_endthread();
	}

	packet[bytes] = 0;
	std::vector<std::string> data = functions->split(cryptography->decrypt(packet, bytes), 0x20);

	if (data.size() != 7)
	{
		closesocket(sock);
		_endthread();
	}

	std::string ID = data[2], PW = cryptography->password(data[3]);
	std::vector<std::string> splVersion = functions->split(data[4], 0x0B);
	std::string clVersion = splVersion.size() == 2 ? splVersion[1] : "0";

	if (clVersion != "0.9.3.3019")
	{
		std::string msg = "fail The client is not the current version.";
		msg += 0x0D;
		msg += "Please, download and install the updated client from the website.";
		msg = cryptography->encrypt(msg);
		send(sock, msg.c_str(), msg.size(), 0);
		_endthread();
	}
	
	if (!functions->AntiSqlInjection(ID) || !functions->AntiSqlInjection(PW))
	{
		std::cout << ID << std::endl;
		std::cout << PW << std::endl;
		closesocket(sock);
		_endthread();
	}

	t_mutex.lock();
	if (!database->query(conn, "SELECT `Username`,`Password`,`Rank`,`Online`,`Ban` FROM `users` WHERE `Username`='" + ID + "' AND `Password`='" + PW + "'"))
	{
		t_mutex.unlock();
		std::cout << mysql_error(conn) << std::endl;
		closesocket(sock);
		_endthread();
	}
	t_mutex.unlock();

	MYSQL_RES *res = database->store_result(conn);

	if (database->num_rows(res) != 1)
	{
		std::string msg = cryptography->encrypt("fail Check your ID and Password.");
		send(sock, msg.c_str(), msg.size(), 0);
		_endthread();
	}

	MYSQL_ROW row;

	std::string id, pw;
	int rank, online, ban;
	while (row = database->fetch_row(res))
	{
		id = row[0];
		pw = row[1];
		try
		{
			rank = atoi(row[2]);
			online = atoi(row[3]);
			ban = atoi(row[4]);
		}
		catch (...)
		{
			rank = 0;
			online = ban = 1;
		}
	}

	database->free_result(res);

	if (maintenance && rank != 2)
	{
		send(sock, msg_maintenance.c_str(), msg_maintenance.size(), 0);
	}
	else if (ID != id || PW != pw)
	{
		send(sock, msg_case.c_str(), msg_case.size(), 0);
	}
	else if (online == 1)
	{
		send(sock, msg_online.c_str(), msg_online.size(), 0);
	}
	else if (ban == 1)
	{
		send(sock, msg_ban.c_str(), msg_ban.size(), 0);
	}
	else
	{
		std::string session = functions->toString(functions->random(10000, 99999));

		t_mutex.lock();
		if (!database->query(conn, "UPDATE `users` SET `Session`='" + session + "' WHERE  `Username`='" + ID + "'"))
		{
			t_mutex.unlock();
			std::cout << mysql_error(conn) << std::endl;
			closesocket(sock);
			_endthread();
		}
		t_mutex.unlock();

		std::string channels = "NsTeST " + session;
		for (int i = 1; i <= 5; i++)
			channels += " 127.0.0.1:" + functions->toString(4029 + i) + ":" + functions->toString(functions->getColor(database->getUsers(conn, i, t_mutex))) + ":1." + functions->toString(i) + ".S1-Aeros";
		channels = cryptography->encrypt(channels);
		send(sock, channels.c_str(), channels.size(), 0);
	}

	_endthread();
}

void commands(void *arg)
{
	std::string command;
	while (true)
	{
		std::cout << std::endl << "> ";
		std::cin >> command;

		if (command == "maintenance")
		{
			maintenance = maintenance ? false : true;
			maintenance ? std::cout << "maintenance -> on" << std::endl : std::cout << "maintenance -> off" << std::endl;
		}
		else if (command == "help")
		{
			std::cout << "-----------------------------------------" << std::endl
				<< "AVAILABLE COMMANDS" << std::endl
				<< "-----------------------------------------" << std::endl
				<< "maintenance < turn on/off maintenance >" << std::endl
				<< "help < show available commands >" << std::endl;
		}
		else
		{
			std::cout << "command not found, type 'help' for available commands." << std::endl;
		}
	}
}

#endif // Threads_h