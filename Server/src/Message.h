#ifndef MESSAGE_H
#define MESSAGE_H

#define CLIENT_MESSAGE      10000 // 客户端消息
#define CLIENT_COMMIT       10001 // 客户端注册消息码
#define CLIENT_REGISTER     10002 // 客户端登录消息码

#include <cstring>

// 消息头
struct MsgHead
{
    // 当前消息的消息码
    int MsgCode;
    // 当前包的大小
    int nSize;
};

// 客户端登录消息
struct CCommit
{
    CCommit()
    {
        head.MsgCode = CLIENT_COMMIT;
        head.nSize = sizeof(CCommit);
        Account = nullptr;
        Password = nullptr;
    }
    MsgHead head;

    char* Account;      // 账户
    char* Password;     // 密码
};

// 客户端注册消息
struct CRegister
{
    CRegister()
    {
        head.MsgCode = CLIENT_REGISTER;
        head.nSize = sizeof(CRegister);
        memset(&msg,0,sizeof(msg));
        memset(&msgPass,0,sizeof(msg));
    }
    MsgHead head;
    char msg[200];
    char msgPass[200];
};

#endif // MESSAGE_H
