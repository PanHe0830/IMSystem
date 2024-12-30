#ifndef MESSAGE_H
#define MESSAGE_H

#define CLIENT_MESSAGE          10000 // 客户端消息
#define CLIENT_REGISTER_REQ     10001 // 客户端注册请求消息码
#define CLIENT_REGISTER_ACK     10002 // 客户端注册回复消息码
#define CLIENT_COMMIT_REQ       10003 // 客户端登录请求消息码
#define CLIENT_COMMIT_ACK       10004 // 客户端登录回复消息码

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
        memset(&msg,0,sizeof(msg));
        memset(&msgPass,0,sizeof(msg));
    }
    MsgHead head;
    char msg[200];
    char msgPass[200];
};

// 客户端注册回复消息
struct CRegister_ACK
{
    CRegister_ACK()
    {
        head.MsgCode = CLIENT_REGISTER_ACK;
        head.nSize = sizeof(CRegister_ACK);
    }
    MsgHead head;
};


// 客户端登录请求消息
struct CCommit_REQ
{
    CCommit_REQ()
    {
        head.MsgCode = CLIENT_COMMIT_REQ;
        head.nSize = sizeof(CCommit_REQ);
    }
    MsgHead head;
};

// 客户端登录回复消息
struct CCommit_ACK
{
    CCommit_ACK()
    {
        head.MsgCode = CLIENT_COMMIT_ACK;
        head.nSize = sizeof(CCommit_ACK);
    }
    MsgHead head;
};

#endif // MESSAGE_H
