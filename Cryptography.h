#ifndef Cryptography_h
#define Cryptography_h

class _cryptography
{
public:
	std::string encrypt(std::string str);
	std::string decrypt(char *str, int bytes);
	std::string password(std::string str);
};

#endif // Cryptography_h