#pragma once

#include <QObject>

class QUdpSocket;

// 区分不同的广播消息
enum MsgType{
    chatMsg,// 聊天信息
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
    QString ReceiveData();
    void SendData(const QString& ip, const QString& iport, const QString& text);

signals:

private:
    QUdpSocket* udpSocket;
};

