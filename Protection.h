#ifndef Protection_h
#define Protection_h

std::map<std::string, short> counter;
std::mutex p_mutex;

class _protection
{
public:
	short check(std::string ip)
	{
		if (ip == "127.0.0.1") return 0;

		bool result = false;

		p_mutex.lock();

		if (counter.find(ip) == counter.end())
			result = false;
		else
			result = true;

		if (result)
		{
			if (counter[ip] > 0 && counter[ip] < 3)
			{
				counter[ip]++;
				p_mutex.unlock();
				return 0;
			}
			else
			{
				p_mutex.unlock();
				return -1;
			}
		}
		else
		{
			counter.insert(std::pair<std::string, short>(ip, 1));
			p_mutex.unlock();
			return 0;
		}
	}
};

void reset(void *arg)
{
	int n = int(arg);
	while (true)
	{
		p_mutex.lock();
		counter.clear();
		p_mutex.unlock();
		Sleep(n);
	}
}

#endif // Protection_h