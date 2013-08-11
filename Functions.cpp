#include <string>
#include <vector>
#include <sstream>

#include "Functions.h"
#include "Cryptography.h"

std::vector<std::string> _functions::split(const std::string str, char delim)
{
	std::vector<std::string> tmp;
	std::stringstream ss(str);
	std::string temp;
	while (std::getline(ss, temp, delim)) { tmp.push_back(temp); }
	return tmp;
}

bool _functions::AntiSqlInjection(std::string str)
{
	char sql_injection_chars [] = { 0x20, 0x7C, 0x5C, 0x22, 0x25, 0x26, 0x2F, 0x27, 0x2C, 0x3B, 0x3A, 0x2E, 0x60, 0x3D };
	for (int i = 0; i < sizeof(sql_injection_chars); i++)
		for (unsigned int j = 0; j < str.size(); j++)
			if (str[j] == sql_injection_chars[i]) { return false; }
	return true;
}

void _functions::msg_init(std::string &msg_maintenance, std::string &msg_case, std::string &msg_online, std::string &msg_ban, std::string &msg_block)
{
	_cryptography *cryptography = new _cryptography();
	
	msg_maintenance  = cryptography->encrypt("fail In this moment the server is in maintenance.");
	
	msg_case = cryptography->encrypt("fail Enter your data respecting the correct case.");
	
	msg_online = cryptography->encrypt("fail This ID is currently in use.");
	
	msg_ban = "fail Your account has been banned !";
	msg_ban += 0x0D;
	msg_ban += "Please, contact the Team of Game";
	msg_ban = cryptography->encrypt(msg_ban);
	
	msg_block = "fail Your ip has been blocked due to repeated login attempts.";
	msg_block += 0x0D;
	msg_block += "You will be unlocked in a span of 60 seconds.";
	msg_block = cryptography->encrypt(msg_block);
}

int _functions::random(int start, int end)
{
	return rand()%(end - start) + start;
}

std::string _functions::toString(int num)
{
	std::stringstream ss;
	ss << num;
	return ss.str();
}

int _functions::getColor(int users)
{
	if (users < 50) return 0;
	else if (users >= 50 && users < 100) return 4;
	else if (users >= 100 && users < 150) return 12;
	else if (users >= 150 && users < 200) return 19;
	else return 0;
}
