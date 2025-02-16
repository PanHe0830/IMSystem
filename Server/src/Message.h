#ifndef MESSAGE_H
#define MESSAGE_H

// ==========================================================
#define CLIENT_MESSAGE           10000 // 客户端消息
#define CLIENT_REGISTER_REQ      10001 // 客户端注册请求消息码
#define CLIENT_REGISTER_ACK      10002 // 客户端注册回复消息码
#define CLIENT_COMMIT_REQ        10003 // 客户端登录请求消息码
#define CLIENT_COMMIT_ACK        10004 // 客户端登录回复消息码
#define CLIENT_FRIEND_REQ        10005 // 客户端好友请求消息码
#define CLIENT_FRIEND_ACK        10006 // 客户端好友回复消息码
#define CLIENT_FRIEND_QUERY_REQ  10007 // 客户端好友查询请求消息码
#define CLIENT_FRIEND_QUERY_ACK  10008 // 客户端好友查询回复消息码
#define CLIENT_MESSAGE_CHAT      10009 // 客户端聊天消息消息码
#define CLIENT_MESSAGE_HEART_REQ 10010 // 客户端心跳消息码请求
#define CLIENT_MESSAGE_HEART_ACK 10011 // 客户端心跳消息码回复
#define SERVER_FRIEND_CHECK      10012 // 服务器给客户端发送朋友信息
#define CLIENT_VIDEO_REQ         10013 // 客户端通话请求
#define CLIENT_VIDEO_ACK         10014 // 客户端通话回复
#define CLIENT_VIDEO_CLOSE       10015 // 客户结束通话
#define CLIENT_VIDEO_DATA        10016 // 视频数据
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
// ==========================================================
#define CLIENT_VIDEO_SUCCESS       1     // 视频同意
#define CLIENT_VIDEO_FAILED        0     // 视频不同意
#define CLIENT_VIDEO_DEFAULT       -1    // 视频默认
// ==========================================================
// ==========================================================
#define CLIENT_MESSAGE_MAX      20  // 消息大小
#define CLIENT_CHAT_MESSAGE_MAX 200 // 聊天消息内容大小
#define CLIENT_ACCOUNT          20  // 发送消息时哪个用户的消息，IMS号码(位数最大值)
// ==========================================================


#include <cstring>
#include <vector>

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
    char passWord[CLIENT_MESSAGE_MAX];
    char nName[CLIENT_MESSAGE_MAX];
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
    char Account[CLIENT_MESSAGE_MAX];
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
    char account[CLIENT_MESSAGE_MAX];
    char password[CLIENT_MESSAGE_MAX];
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
    char sourceAccount[CLIENT_MESSAGE_MAX];
    char tarAccount[CLIENT_MESSAGE_MAX];
};

// 客户端朋友回复消息
struct CFriend_ACK
{
    CFriend_ACK()
    {
        head.MsgCode = CLIENT_FRIEND_ACK;
        head.nSize = sizeof(CFriend_ACK);
        flag = CLIENT_FRIEND_DEFAULT;
        memset(&tarAccount, 0, sizeof(tarAccount));
        memset(&sourceAccount, 0, sizeof(sourceAccount));
    }
    MsgHead head;
    char sourceAccount[CLIENT_MESSAGE_MAX];
    char tarAccount[CLIENT_MESSAGE_MAX];
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
    char tarAccount[CLIENT_MESSAGE_MAX];
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

// 客户端发送消息 client1 -> client2
struct CSendMessage
{
    CSendMessage()
    {
        head.MsgCode = CLIENT_MESSAGE_CHAT;
        head.nSize = sizeof(CSendMessage);
        memset(usrAccount, 0, sizeof(CLIENT_ACCOUNT));
        memset(tarAccount, 0, sizeof(CLIENT_ACCOUNT));
        memset(message, 0, sizeof(CLIENT_CHAT_MESSAGE_MAX));
    }
    MsgHead head;
    char usrAccount[CLIENT_ACCOUNT];
    char tarAccount[CLIENT_ACCOUNT];
    char message[CLIENT_CHAT_MESSAGE_MAX];
};

// 客户端心跳消息请求
struct CHeartMessage_REQ
{
    CHeartMessage_REQ()
    {
        head.MsgCode = CLIENT_MESSAGE_HEART_REQ;
        head.nSize = sizeof(CHeartMessage_REQ);
        memset(usrAccount, 0, sizeof(CLIENT_ACCOUNT));
    }
    MsgHead head;
    char usrAccount[CLIENT_ACCOUNT];
};

// 客户端心跳消息回复
struct CHeartMessage_ACK
{
    CHeartMessage_ACK()
    {
        head.MsgCode = CLIENT_MESSAGE_HEART_ACK;
        head.nSize = sizeof(CHeartMessage_ACK);
    }
    MsgHead head;
};

// 服务器告诉客户端朋友消息
struct SFriendCheck
{
    SFriendCheck()
    {
        head.MsgCode = SERVER_FRIEND_CHECK;
        head.nSize = sizeof(SFriendCheck);
        memset(friAccount, 0, sizeof(CLIENT_ACCOUNT));
        //memset(friGroup, 0, sizeof(CLIENT_ACCOUNT));
    }
    MsgHead head;
    char friAccount[CLIENT_ACCOUNT];
    //char friGroup[CLIENT_ACCOUNT]; // 暂定
};

// 视频通话请求
struct CVideo_REQ
{
    CVideo_REQ()
    {
        head.MsgCode = CLIENT_VIDEO_REQ;
        head.nSize = sizeof(CVideo_REQ);
        memset(usrAccount, 0, sizeof(CLIENT_ACCOUNT));
        memset(tarAccount, 0, sizeof(CLIENT_ACCOUNT));
    }
    MsgHead head;
    char usrAccount[CLIENT_ACCOUNT];
    char tarAccount[CLIENT_ACCOUNT];
};

// 视频通话回复
struct CVideo_ACK
{
    CVideo_ACK()
    {
        head.MsgCode = CLIENT_VIDEO_ACK;
        head.nSize = sizeof(CVideo_ACK);
        memset(usrAccount, 0, sizeof(CLIENT_ACCOUNT));
        memset(tarAccount, 0, sizeof(CLIENT_ACCOUNT));
        flag = CLIENT_VIDEO_DEFAULT;
    }
    MsgHead head;
    char usrAccount[CLIENT_ACCOUNT];
    char tarAccount[CLIENT_ACCOUNT];
    int flag;
};

// 视频关闭消息
struct CVideo_CLOSE
{
    CVideo_CLOSE()
    {
        head.MsgCode = CLIENT_VIDEO_CLOSE;
        head.nSize = sizeof(CVideo_CLOSE);
        memset(usrAccount, 0, sizeof(CLIENT_ACCOUNT));
        memset(tarAccount, 0, sizeof(CLIENT_ACCOUNT));
    }
    MsgHead head;
    char usrAccount[CLIENT_ACCOUNT]; // 谁结束的通话
    char tarAccount[CLIENT_ACCOUNT];
};

// 视频数据
struct CVideo_Data
{
    CVideo_Data()
    {
        head.MsgCode = CLIENT_VIDEO_DATA;
        head.nSize = sizeof(MsgHead) + sizeof(usrAccount) + sizeof(tarAccount);
        memset(usrAccount, 0, sizeof(CLIENT_ACCOUNT));
        memset(tarAccount, 0, sizeof(CLIENT_ACCOUNT));
    }
    MsgHead head;
    char usrAccount[CLIENT_ACCOUNT];
    char tarAccount[CLIENT_ACCOUNT];
    std::vector<unsigned char> videoBuff;

    // 序列化函数
    std::vector<unsigned char> serialize() const {
        std::vector<unsigned char> result;

        // 序列化头部
        result.resize(result.size() + sizeof(head));
        memcpy(result.data() + result.size() - sizeof(head), &head, sizeof(head));

        // 序列化用户和目标账户
        result.resize(result.size() + sizeof(usrAccount) + sizeof(tarAccount));
        memcpy(result.data() + result.size() - sizeof(usrAccount) - sizeof(tarAccount), usrAccount, sizeof(usrAccount));
        memcpy(result.data() + result.size() - sizeof(tarAccount), tarAccount, sizeof(tarAccount));

        // 序列化视频数据缓冲区
        result.insert(result.end(), videoBuff.begin(), videoBuff.end());

        return result;
    }

    // 反序列化函数
    static CVideo_Data deserialize(const std::vector<unsigned char>& data) {
        CVideo_Data videoData;
        size_t offset = 0;

        // 反序列化头部
        memcpy(&videoData.head, data.data() + offset, sizeof(videoData.head));
        offset += sizeof(videoData.head);

        // 反序列化用户和目标账户
        memcpy(videoData.usrAccount, data.data() + offset, sizeof(videoData.usrAccount));
        offset += sizeof(videoData.usrAccount);
        memcpy(videoData.tarAccount, data.data() + offset, sizeof(videoData.tarAccount));
        offset += sizeof(videoData.tarAccount);

        // 反序列化视频数据缓冲区
        videoData.videoBuff.assign(data.begin() + offset, data.end());

        return videoData;
    }
};

#endif // MESSAGE_H
