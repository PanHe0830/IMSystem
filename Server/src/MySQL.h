#ifndef _MY_SQL_
#define _MY_SQL_

#include <mysql.h>
#include <stdio.h>//mysql 文件
#include <iostream>
#include <vector>

enum SQL_SAFE_MODE
{
	SQL_SAFE_MODEL_NOSAFE = 0, // 数据库关闭safe-update模式
	SQL_SAFE_MODEL_SAFE,	//数据库开启safe-update模式

	SQL_SAFE_MODEL_MAX = 255 // 枚举最大值
};

class MySQL
{
public:
	MySQL();
	~MySQL();

public:
	/**
	*	@name		InitMySql
	*	@function	初始化数据库
	*	@author		PH
	*	@return		true 成功  false 失败
	*	@param		无
	*	@note		
	*/
	bool MySqlInit();

	/**
	*	@name		SetMySqlOption
	*	@function	设置数据库选项
	*	@author		PH
	*	@return		0 成功 
	*	@param		option -- mysql枚举选项  str option对应参数
	*	@note		应在InitMySql()函数后MySqlConnent()函数前
	*/
	int MySqlSetOption(enum mysql_option option, const char* str);

	/**
	*	@name		MySqlConnent
	*	@function	数据库连接
	*	@author		PH
	*	@return		数据库句柄
	*	@param		host - ip地址  user - 登录用户名  passwd - 密码  db - 数据库名  port - 端口号  unix_socket - 应使用的套接字或命名管道  clientflag - Mysql运行为ODBC数据库的标记
	*	@note		
	*/
	MYSQL* MySqlConnent(const char* host,
						const char* user, const char* passwd,
						const char* db, unsigned int port,
						const char* unix_socket = nullptr,
						unsigned long clientflag = 0);
	/**
	*	@name		MySqlQuery
	*	@function	数据库增删改查
	*	@author		PH
	*	@return		true 成功 false 失败
	*	@param		sql语句
	*	@note		
	*/
	bool MySqlQuery(const char* sqlLine);

	/**
	*	@name		MySqlGetResult
	*	@function	获得结果集
	*	@author		PH
	*	@return		结果集
	*	@param
	*	@note
	*/
	MYSQL_RES* MySqlGetResult();

	/**
	*	@name		MySqlColse
	*	@function	关闭数据库
	*	@author		PH
	*	@return		
	*	@param		
	*	@note
	*/
	void MySqlColse();

	/**
	*	@name		MySqlFreeResult
	*	@function	关闭结果集
	*	@author		PH
	*	@return
	*	@param		res - 结果集
	*	@note
	*/
	void MySqlFreeResult(MYSQL_RES* res);

	/**
	*	@name		MySqlVectorResult
	*	@function	将内容放入vector容器中
	*	@author		PH
	*	@return		返回查询到的结果或数据
	*	@param		result - 结果集
	*	@note
	*/
	std::vector<std::string> MySqlPrintfResult(MYSQL_RES* result);

	/**
	*	@name		MySqlChangeSafeModel
	*	@function	改变数据库safe-updates模式
	*	@author		PH
	*	@return		false - 失败 true - 成功
	*	@param	
	*	@note		模式会导致非主键条件下无法执行update或者delete命令
	*/
	bool MySqlChangeSafeModel(SQL_SAFE_MODE mode);

	/**
	*	@name		MySqlChangeSafeModel
	*	@function	改变数据库safe-updates模式
	*	@author		PH
	*	@return		false - OFF true - ON
	*	@param
	*	@note		模式会导致非主键条件下无法执行update或者delete命令
	*/
	bool MySqlShowSafeModel();

	inline bool GetIsInit() { return isInit; };
private:
	MYSQL* mysql;//数据库句柄
	bool isInit = false;
};

#endif