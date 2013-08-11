#include <string>
#include <sstream>

#include "Cryptography.h"

std::string _cryptography::encrypt(std::string str)
{
	std::string str_enc;
	for (unsigned int i = 0; i < str.size(); i++)
		str_enc += str[i] + 0xF;
	return str_enc += 0x19;
}

std::string _cryptography::decrypt(char *str, int bytes)
{
	std::string str_dec;
	for (int i = 0; i < bytes; i++)
		str_dec += (str[i] - 0xF) ^ 0xC3;
	return str_dec;
}

std::string _cryptography::password(std::string str)
{
	int rm = str.size() % 4;
	str = str.erase(0, rm != 0 ? rm : 4);
	std::string str_dec;
	for (unsigned int i = 1; i < str.size(); i += 2) str_dec += str[i];
	str = str_dec;
	str_dec.clear();
	for (unsigned int i = 0; i < str.size(); i++)
	{
		if (i != 0 && i % 2 == 0) str_dec += " ";
		str_dec += str[i];
	}
	std::stringstream ss(str_dec);
	str.clear();
	str_dec.clear();
	int convert;
	while (ss >> std::hex >> convert) str_dec.push_back(convert);
	return str_dec;
}