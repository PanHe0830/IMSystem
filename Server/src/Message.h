#ifndef MESSAGE_H
#define MESSAGE_H

// ==========================================================
#define CLIENT_MESSAGE          10000 // 客户端消息
#define CLIENT_REGISTER_REQ     10001 // 客户端注册请求消息码
#define CLIENT_REGISTER_ACK     10002 // 客户端注册回复消息码
#define CLIENT_COMMIT_REQ       10003 // 客户端登录请求消息码
#define CLIENT_COMMIT_ACK       10004 // 客户端登录回复消息码
#define CLIENT_FRIEND_REQ       10005 // 客户端好友请求消息码
#define CLIENT_FRIEND_ACK       10006 // 客户端好友回复消息码
#define CLIENT_FRIEND_QUERY_REQ 10007 // 客户端好友查询请求消息码
#define CLIENT_FRIEND_QUERY_ACK 10008 // 客户端好友查询回复消息码
// ==========================================================
// ==========================================================
#define CLIENT_COMMIT_SUCCESS   1     // 客户端登录成功
#define CLIENT_COMMIT_FAILED    0     // 客户端登录失败
#define CLIENT_COMMIT_DEFAULT   -1    // 客户端登录默认
// ==========================================================
// ==========================================================
#define CLIENT_FRIEND_SUCCESS   1     // 客户端朋友请求同意
#define CLIENT_FRIEND_FAILED    0     // 客户端朋友请求不同意
#define CLIENT_FRIEND_DEFAULT   -1    // 客户端朋友请求默认
// ==========================================================
// ==========================================================
#define CLIENT_FRIEND_QUERY_EXIST       1     // 客户端朋友查询同意
#define CLIENT_FRIEND_QUERY_NOEXIST     0     // 客户端朋友查询回复不同意
#define CLIENT_FRIEND_QUERY_DEFAULT     -1    // 客户端朋友查询回复默认
// ==========================================================


#include <cstring>

// 消息头
struct MsgHead
{
    // 当前消息的消息码
    int MsgCode;
    // 当前包的大小
    int nSize;
};

// 客户端注册请求消息
struct CRegister_REQ
{
    CRegister_REQ()
    {
        head.MsgCode = CLIENT_REGISTER_REQ;
        head.nSize = sizeof(CRegister_REQ);
        memset(&passWord, 0, sizeof(passWord));
        memset(&nName, 0, sizeof(nName));
    }
    MsgHead head;
    char passWord[20];
    char nName[20];
};

// 客户端注册回复消息
struct CRegister_ACK
{
    CRegister_ACK()
    {
        head.MsgCode = CLIENT_REGISTER_ACK;
        head.nSize = sizeof(CRegister_ACK);
        memset(&Account, 0, sizeof(Account));
    }
    MsgHead head;
    char Account[20];
};


// 客户端登录请求消息
struct CCommit_REQ
{
    CCommit_REQ()
    {
        head.MsgCode = CLIENT_COMMIT_REQ;
        head.nSize = sizeof(CCommit_REQ);
        memset(&account, 0, sizeof(account));
        memset(&password, 0, sizeof(password));
    }
    MsgHead head;
    char account[20];
    char password[20];
};

// 客户端登录回复消息
struct CCommit_ACK
{
    CCommit_ACK()
    {
        head.MsgCode = CLIENT_COMMIT_ACK;
        head.nSize = sizeof(CCommit_ACK);
        flag = CLIENT_COMMIT_DEFAULT;
    }
    MsgHead head;
    int flag;
};

// 客户端朋友请求消息
struct CFriend_REQ
{
    CFriend_REQ()
    {
        head.MsgCode = CLIENT_FRIEND_REQ;
        head.nSize = sizeof(CFriend_REQ);
        memset(&tarAccount, 0, sizeof(tarAccount));
        memset(&sourceAccount, 0, sizeof(sourceAccount));
    }
    MsgHead head;
    char sourceAccount[20];
    char tarAccount[20];
};

// 客户端朋友回复消息
struct CFriend_ACK
{
    CFriend_ACK()
    {
        head.MsgCode = CLIENT_FRIEND_ACK;
        head.nSize = sizeof(CFriend_ACK);
        flag = CLIENT_FRIEND_DEFAULT;
    }
    MsgHead head;
    int flag;
};

// 客户端朋友查询请求消息
struct CFriendQuery_REQ
{
    CFriendQuery_REQ()
    {
        head.MsgCode = CLIENT_FRIEND_QUERY_REQ;
        head.nSize = sizeof(CFriendQuery_REQ);
        memset(tarAccount, 0, sizeof(tarAccount));
    }
    MsgHead head;
    char tarAccount[20];
};

// 客户端朋友查询回复消息
struct CFriendQuery_ACK
{
    CFriendQuery_ACK()
    {
        head.MsgCode = CLIENT_FRIEND_QUERY_ACK;
        head.nSize = sizeof(CFriendQuery_ACK);
        flag = CLIENT_FRIEND_QUERY_DEFAULT;
    }
    MsgHead head;
    int flag;
};

#endif // MESSAGE_H
