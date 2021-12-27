#include "SocketFramework.h"
#include <QUdpSocket>
#include <QDataStream>
#include <QDateTime>
#include <QNetworkInterface>
#include <QHostInfo>
#include <QNetworkDatagram>

//#include "spdlog/sinks/basic_file_sink.h" // support for basic file logging
//#include "spdlog/sinks/rotating_file_sink.h" // support for rotating file logging

#define cout qDebug()<<"["<<__FILE__<<__func__<<__LINE__<<"]"

//using namespace spdlog;

SocketFramework::SocketFramework(QObject *parent) : QObject(parent),
    udpSocket(new QUdpSocket(this))
{
//    myfsrv = new FileSrvDlg(this);
//    connect(myfsrv, SIGNAL(sendFileName(QString)), this, SLOT(getSfileName(QString)));
    // ShareAddress 允许其他的服务（进程）去绑定这个IP和端口
    // ReuseAddressHint 尝试重新绑定服务
    port = 13140;
    udpSocket->bind(port, QUdpSocket::ShareAddress|QUdpSocket::ReuseAddressHint);

//    // 使用spdlog写循环日志
//    logger = spdlog::rotating_logger_mt("SocketFramework", "myfilename.txt", 1024 * 1024 * 5, 3);
//    QString str={"中文"};
//    logger->info(str.toStdString());

//    auto my_logger = spdlog::basic_logger_mt("basic_logger", "basic.txt");
//    my_logger->info(str.toUtf8());
    connect(udpSocket, &QUdpSocket::readyRead, this, &SocketFramework::ReceiveData);
}

//读取对方发送的内容
void SocketFramework::ReceiveData()
{
    QHostAddress originAddr; // 对方ip
    quint16 originPort;   // 对方发送的端口号
    while (udpSocket->hasPendingDatagrams())
    {
        QByteArray qba;
        // 通过pendingDatagramSize() 获取当前可供读取的 UDP 数据报大小，并据此分配缓冲区qba
        qba.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(qba.data(), qba.size(), &originAddr, &originPort);
        cout << originAddr.toString();
//        QNetworkDatagram datagram = udpSocket->receiveDatagram();

        QDataStream readData(&qba, QIODevice::ReadOnly);
        int msgType;
        // 1.首先获取消息的类型
        readData >> msgType;

        QString senderName, hostIp, chatMsg, rname, fname;
//        QDateTime currentTime = QDateTime::currentDateTime()/*.toString("yyyy-MM-dd hh:mm:ss")*/;
        // 2.再获取其他信息
        switch (msgType)
        {
        // 获取其中的用户名、主机 IP 和聊天内容信息
        case ChatMsg: {
            readData >> senderName >> chatMsg >> hostIp;
            cout << "readData "<<senderName<<chatMsg<<hostIp;
            emit ReceiveMsgSignal(senderName, chatMsg);
            break;
        }
        // 新用户上线，获取用户名和 IP，使用 onLine() 函数进行新用户登录的处理。
        case Online:
            readData >> senderName >> hostIp;
            cout<<"new user online"<<senderName<<hostIp;
            emit NewUserOnlineSignal(senderName, hostIp);
            break;
        // 用户离线，获取用户名，然后使用 offLine() 函数进行处理
        case Offline:
            readData >> senderName >> hostIp;
            emit UserOfflineSignal(senderName,hostIp);
            break;
//        case FileName:
//            readData >> userName >> hostIp >> rname >> fname;
//            recvFileName(userName, hostIp, rname, fname);
//            break;
//        case RefFile:
//            readData >> userName >> hostIp >> rname;
//            if (myname == rname) myfsrv->cntRefused();
//            break;
        }
    }
}

void SocketFramework::SendData(MsgType msgType, QString name, QString friendIP, QString chatMsg)
{
    myname = name;
    QByteArray byteData;
    QDataStream sendDataStream(&byteData, QIODevice::WriteOnly);
    QString localHostIp = GetLocalIP();

    QHostAddress originIP(friendIP);
    // 要发送的数据中写入消息类型 msgType 、用户名
    sendDataStream << msgType << myname;
    // 再写入其他信息
//    // 广播write发送数据应该绑定ip，否则多网卡发送不正确.目前测试未解决问题
//    udpSocket->bind(QHostAddress::LocalHost,port,QUdpSocket::ShareAddress);
    switch (msgType)
    {
    case ChatMsg:
        sendDataStream << chatMsg << localHostIp;
        udpSocket->writeDatagram(byteData, byteData.length(), originIP, port);
        cout<<"ChatMsg"<<chatMsg<<originIP;
        break;
    case Online:
        sendDataStream << localHostIp;
        udpSocket->writeDatagram(byteData, byteData.length(), QHostAddress::Broadcast, port);
        cout<<"Online";
        break;
    case Offline:
        sendDataStream << localHostIp;
        udpSocket->writeDatagram(byteData, byteData.length(), QHostAddress::Broadcast, port);
        break;
    case FileName:
        sendDataStream << localHostIp << name << FileName;
        break;
    case RefFile:
        sendDataStream << localHostIp << name;
        break;
    }
    //TODO:确认是广播出去？
    // 完成对消息的处理后，使用套接口的 writeDatagramO 函数广播出去。
//    udpSocket->writeDatagram(byteData, byteData.length(), QHostAddress::Broadcast, port);
}

// 返回本地IP
QString SocketFramework::GetLocalIP()
{
    // 返回主机上面发现的所有IP地址。
//    QList<QHostAddress> addrlist = QNetworkInterface::allAddresses();
//    foreach (QHostAddress addr, addrlist)
//    {
//        if (addr.protocol() == QAbstractSocket::IPv4Protocol)
//        {
//            cout<<addr.toString();
////            return addr.toString();
//        }
//    }

    QHostInfo info = QHostInfo::fromName(QHostInfo::localHostName());
    foreach(QHostAddress ip,info.addresses())
    {
        cout << "ip:" <<ip.toString();
        // 只取ipv4协议的地址
        if(ip.protocol() == QAbstractSocket::IPv4Protocol)
        {
            return ip.toString();
        }
    }
    return QString();
}

