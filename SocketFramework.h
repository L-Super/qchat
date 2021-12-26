#pragma once

#include <QObject>

class QUdpSocket;
class QHostAddress;

// 区分不同的广播消息
enum MsgType{
    ChatMsg,// 聊天信息
    Online,//在线
    Offline,//离线
    FileName,//文件名
    RefFile//拒收文件
};

//TODO:如何实现像qq或者微信一样发送消息
class SocketFramework : public QObject
{
    Q_OBJECT
public:
    explicit SocketFramework(QObject *parent = nullptr);
    void ReceiveData();
    void SendData(MsgType msgType, QString myselfName = "", QString friendIP = "", QString msg = "");

    QString GetLocalIP();

signals:
    // 接收发送的信息。发送者，发送的信息
    void ReceiveMsgSignal(QString senderName, QString sentMsg);
    void NewUserOnlineSignal(const QString& newUser,const QString& ip);
    void UserOfflineSignal(const QString& user,const QString& ip);

private:
    QUdpSocket* udpSocket;
    qint16 port;
    QString myname = "";//本端用户名
    QString fileName; // 文件名
};

