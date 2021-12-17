#include "SocketFramework.h"
#include <QUdpSocket>

#define cout qDebug()<<"["<<__FILE__<<__func__<<__LINE__<<"]"

SocketFramework::SocketFramework(QObject *parent) : QObject(parent),
    udpSocket(new QUdpSocket(this))
{
    udpSocket->bind(13140);
    connect(udpSocket, &QUdpSocket::readyRead, this, &SocketFramework::ReceiveData);
}

//读取对方发送的内容
QString SocketFramework::ReceiveData()
{
    char buf[1024] = {0};
    QString receiveStr;
    QHostAddress addr; //对方地址
    quint16 port;    //对方端口

    qint64 len = udpSocket->readDatagram(buf, sizeof(buf), &addr, &port);
    if(len > 0)
    {
        cout<<"received data!";
        //格式化 [192.68.2.2:8888]aaaa
        receiveStr = QString("[%1:%2] %3").arg(addr.toString()).arg(port).arg(buf);
    }
    return receiveStr;
}

void SocketFramework::SendData(const QString& ip, const QString& iport, const QString& text)
{
    cout<<ip<<iport<<text;
    //给指定的IP发送数据
    // TODO:测试中文是否需要.toUtf8(),以及为什么需要toutf8()才能成功
    quint16 port = iport.toUShort();
    udpSocket->writeDatagram(text.toUtf8(), QHostAddress(ip), port);
}
