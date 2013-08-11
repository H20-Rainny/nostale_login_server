#ifndef Functions_h
#define Functions_h

class _functions
{
public:
	std::vector<std::string> split(const std::string str, char delim);
	bool AntiSqlInjection(std::string str);
	void msg_init(std::string &msg_maintenance, std::string &msg_case, std::string &msg_online, std::string &msg_ban);
	int random(int start, int end);
	std::string toString(int num);
	int getColor(int users);
};

#endif // Functions_h