#include "MySQL.h"

MySQL::MySQL()
{
	mysql = nullptr;
}

MySQL::~MySQL()
{

}

bool MySQL::MySqlInit()
{
	isInit = true;
	mysql =  mysql_init(nullptr);
	if (mysql == nullptr)
	{
		std::cout << "DataBase Init Failed" << std::endl;
		return false;
	}

	return true;
}

int MySQL::MySqlSetOption(enum mysql_option option , const char* str)
{
	return mysql_options(mysql, option, str);
}

MYSQL* MySQL::MySqlConnent( const char* host,
							const char* user, const char* passwd,
							const char* db, unsigned int port,
							const char* unix_socket,
							unsigned long clientflag)
{
	if (mysql == nullptr)
	{
		std::cout << "MYSQL is nullptr" << std::endl;
		return nullptr;
	}

	if (mysql_real_connect(mysql, host, user, passwd, db, port, unix_socket, clientflag) == nullptr)
	{
		std::cout << "连接失败！" << "错误原因" << mysql_error(mysql) << std::endl;
		return nullptr;
	}
	return mysql;
}

bool MySQL::MySqlQuery(const char* sqlLine)
{
	if (mysql == nullptr)
	{
		std::cout << "MYSQL is nullptr" << std::endl;
		return false;
	}

	if (mysql_query(mysql, sqlLine))
	{
		std::cout << "查询错误" << mysql_error(mysql) << std::endl;
		return false;
	}
	return true;
}

MYSQL_RES* MySQL::MySqlGetResult()
{
	if (mysql == nullptr) return nullptr;
	return mysql_store_result(mysql);
}

void MySQL::MySqlColse()
{
	isInit = false;
	mysql_close(mysql);
	return;
}

void MySQL::MySqlFreeResult(MYSQL_RES* res)
{
	if (res == nullptr) return;
	mysql_free_result(res);
	return;
}

void MySQL::MySqlPrintfResult(MYSQL_RES* result)
{
	if (result == nullptr) return;
	MYSQL_ROW  row = NULL;
	unsigned int num_rows, num_fields;
	num_rows = mysql_num_rows(result);
	num_fields = mysql_num_fields(result);
	printf("行数：%d,列数:%d\n", num_rows, num_fields);
	
	for (unsigned int i = 0; i < num_rows; i++)
	{
		row = mysql_fetch_row(result);//从结果集中获取一行
		for (unsigned int j = 0; j < num_fields; j++)
		{
			printf("%s\t", row[j]);
		}
		printf("\n");
	}
}