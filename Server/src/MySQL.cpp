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
		std::cout << "����ʧ�ܣ�" << "����ԭ��" << mysql_error(mysql) << std::endl;
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
		std::cout << "��ѯ����" << mysql_error(mysql) << std::endl;
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
	mysql_close(mysql);
	return;
}

void MySQL::MySqlFreeResult(MYSQL_RES* res)
{
	if (res == nullptr) return;
	mysql_free_result(res);
	return;
}



MYSQL_RES* result = NULL;
MYSQL_ROW  row = NULL;
int num_rows, num_fields;
result = mysql_store_result(&mysql);
num_rows = mysql_num_rows(result);
num_fields = mysql_num_fields(result);
printf("������%d,����:%d\n", num_rows, num_fields);

for (int i = 0; i < num_rows; i++)
{
	row = mysql_fetch_row(result);//�ӽ�����л�ȡ��һ��
	for (int j = 0; j < num_fields; j++)
	{
		printf("%s\t", row[j]);
	}
	printf("\n");
}
��������������������������������

��Ȩ����������Ϊ����ԭ�����£���ѭ CC 4.0 BY - SA ��ȨЭ�飬ת���븽��ԭ�ĳ������Ӻͱ�������

ԭ�����ӣ�https ://blog.csdn.net/JMW1407/article/details/107612185