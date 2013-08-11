#ifndef Database_h
#define Database_h

class _database
{
public:
	MYSQL *initialize(MYSQL *conn);
	bool connection(MYSQL *conn, char *host, char *user, char *pass, char *name);
	bool query(MYSQL *conn, std::string qry);
	MYSQL_RES *store_result(MYSQL *conn);
	my_ulonglong __stdcall num_rows(MYSQL_RES *res);
	MYSQL_ROW __stdcall fetch_row(MYSQL_RES *res);
	void __stdcall free_result(MYSQL_RES *res);
	int getUsers(MYSQL *conn, int num, std::mutex &t_mutex);
};

#endif // Database_h