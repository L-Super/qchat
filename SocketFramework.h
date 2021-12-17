#pragma once

#include <QObject>

class QUdpSocket;

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

