#ifndef _MY_SQL_
#define _MY_SQL_

#include <mysql.h>
#include <stdio.h>//mysql �ļ�
#include <iostream>

class MySQL
{
public:
	MySQL();
	~MySQL();

public:
	/**
	*	@name		InitMySql
	*	@function	��ʼ�����ݿ�
	*	@author		PH
	*	@return		true �ɹ�  false ʧ��
	*	@param		��
	*	@note		
	*/
	bool MySqlInit();

	/**
	*	@name		SetMySqlOption
	*	@function	�������ݿ�ѡ��
	*	@author		PH
	*	@return		0 �ɹ� 
	*	@param		option -- mysqlö��ѡ��  str option��Ӧ����
	*	@note		Ӧ��InitMySql()������MySqlConnent()����ǰ
	*/
	int MySqlSetOption(enum mysql_option option, const char* str);

	/**
	*	@name		MySqlConnent
	*	@function	���ݿ�����
	*	@author		PH
	*	@return		���ݿ���
	*	@param		host - ip��ַ  user - ��¼�û���  passwd - ����  db - ���ݿ���  port - �˿ں�  unix_socket - Ӧʹ�õ��׽��ֻ������ܵ�  clientflag - Mysql����ΪODBC���ݿ�ı��
	*	@note		
	*/
	MYSQL* MySqlConnent(const char* host,
						const char* user, const char* passwd,
						const char* db, unsigned int port,
						const char* unix_socket = nullptr,
						unsigned long clientflag = 0);
	/**
	*	@name		MySqlQuery
	*	@function	���ݿ���ɾ�Ĳ�
	*	@author		PH
	*	@return		true �ɹ� false ʧ��
	*	@param		sql���
	*	@note		
	*/
	bool MySqlQuery(const char* sqlLine);

	/**
	*	@name		MySqlGetResult
	*	@function	��ý����
	*	@author		PH
	*	@return		�����
	*	@param
	*	@note
	*/
	MYSQL_RES* MySqlGetResult();

	/**
	*	@name		MySqlColse
	*	@function	�ر����ݿ�
	*	@author		PH
	*	@return		
	*	@param		
	*	@note
	*/
	void MySqlColse();

	/**
	*	@name		MySqlFreeResult
	*	@function	�رս����
	*	@author		PH
	*	@return
	*	@param		res - �����
	*	@note
	*/
	void MySqlFreeResult(MYSQL_RES* res);

	/**
	*	@name		MySqlPrintfResult
	*	@function	��ӡ�����������
	*	@author		PH
	*	@return
	*	@param		result - �����
	*	@note
	*/
	void MySqlPrintfResult(MYSQL_RES* result);

	inline bool GetIsInit() { return isInit; };
private:
	MYSQL* mysql;//���ݿ���
	bool isInit = false;
};

#endif