#include <string>
#include <WinSock2.h>
#include <mysql.h>
#include <mutex>
#include <sstream>

#include "Database.h"

MYSQL *_database::initialize(MYSQL *conn)
{
	return mysql_init(conn);
}

bool _database::connection(MYSQL *conn, char *host, char *user, char *pass, char *name)
{
	mysql_init(conn);
	return !mysql_real_connect(conn, host, user, pass, name, 0, 0, 0) ? false : true;
}

bool _database::query(MYSQL *conn, std::string qry)
{
	return mysql_query(conn, qry.c_str()) != 0 ? false : true;
}

MYSQL_RES *_database::store_result(MYSQL *conn)
{
	return mysql_store_result(conn);
}

my_ulonglong _stdcall _database::num_rows(MYSQL_RES *res)
{
	return mysql_num_rows(res);
}

MYSQL_ROW _stdcall _database::fetch_row(MYSQL_RES *res)
{
	return mysql_fetch_row(res);
}

void __stdcall _database::free_result(MYSQL_RES *res)
{
	mysql_free_result(res);
}

int _database::getUsers(MYSQL *conn, int num, std::mutex &t_mutex)
{
	std::stringstream ss;
	ss << num;
	t_mutex.lock();
	if (!query(conn, "SELECT * FROM `users` WHERE `Online`='" + ss.str() + "'"))
	{
		t_mutex.unlock();
		return 0;
	}
	t_mutex.unlock();
	MYSQL_RES *res = store_result(conn);
	return (int) num_rows(res);
}