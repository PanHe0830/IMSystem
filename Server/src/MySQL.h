#ifndef _MY_SQL_
#define _MY_SQL_

#include <mysql.h>
#include <stdio.h>//mysql �ļ�
#include <iostream>

enum SQL_SAFE_MODE
{
	SQL_SAFE_MODEL_NOSAFE = 0, // ���ݿ�ر�safe-updateģʽ
	SQL_SAFE_MODEL_SAFE,	//���ݿ⿪��safe-updateģʽ

	SQL_SAFE_MODEL_MAX = 255 // ö�����ֵ
};

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

	/**
	*	@name		MySqlChangeSafeModel
	*	@function	�ı����ݿ�safe-updatesģʽ
	*	@author		PH
	*	@return		false - ʧ�� true - �ɹ�
	*	@param	
	*	@note		ģʽ�ᵼ�·������������޷�ִ��update����delete����
	*/
	bool MySqlChangeSafeModel(SQL_SAFE_MODE mode);

	/**
	*	@name		MySqlChangeSafeModel
	*	@function	�ı����ݿ�safe-updatesģʽ
	*	@author		PH
	*	@return		false - OFF true - ON
	*	@param
	*	@note		ģʽ�ᵼ�·������������޷�ִ��update����delete����
	*/
	bool MySqlShowSafeModel();

	inline bool GetIsInit() { return isInit; };
private:
	MYSQL* mysql;//���ݿ���
	bool isInit = false;
};

#endif