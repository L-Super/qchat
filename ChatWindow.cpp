#include "ChatWindow.h"
#include "ui_ChatWindow.h"
#include "OptionDialog.h"
#include <QDebug>
#include <QGraphicsDropShadowEffect>
#include <QTime>
#include <QDateTime>
#include <QFileDialog>
#include <QStandardPaths>
#include <QSettings>
#include <QUdpSocket>
#include <QHostInfo>


#define cout qDebug()<<"["<<__FILE__<<__func__<<__LINE__<<"]"

ChatWindow::ChatWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ChatWindow),
      udpSocket(new QUdpSocket(this))
{
    ui->setupUi(this);

//    // 1、设置无边框，背景透明
//    this->setWindowFlags(Qt::FramelessWindowHint);
//    this->setAttribute(Qt::WA_TranslucentBackground);
//    // 2、设置阴影边框;
//    QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect(this);
//    // 阴影偏移
//    shadowEffect->setOffset(0, 0);
//    // 阴影颜色;
//    shadowEffect->setColor(QColor(68, 68, 68));
//    // 阴影半径;
//    shadowEffect->setBlurRadius(10);
//    // 给窗口设置上当前的阴影效果;
//    this->centralWidget()->setGraphicsEffect(shadowEffect);
//    this->setGraphicsEffect(shadowEffect);

//    ui->listWidget->setCurrentRow(0);
    QListWidgetItem *userItem = new QListWidgetItem("user");
    ui->listWidget->addItem(userItem);
    ui->listWidget->setCurrentItem(userItem);

    // ShareAddress 允许其他的服务（进程）去绑定这个IP和端口
    // ReuseAddressHint 尝试重新绑定服务
    port = 13140;
    udpSocket->bind(port, QUdpSocket::ShareAddress|QUdpSocket::ReuseAddressHint);

    //TODO:左侧自己头像部分，考虑用label显示头像，并加上昵称
    mName = ReadInfoFromIni();
    ui->headLabel->setText(mName);

    //TODO:bug:会造成死循环
    SendData(Online, mName);

    // itemSelectionChanged itemClicked 效果一致
    connect(ui->listWidget, &QListWidget::itemClicked, this, &ChatWindow::SelectedFriend);
    connect(udpSocket, &QUdpSocket::readyRead, this, &ChatWindow::ReceiveData);
}

ChatWindow::~ChatWindow()
{
    delete ui;
}

// 从配置文件读取账号昵称等信息
QString ChatWindow::ReadInfoFromIni()
{
    QString iniPath = QCoreApplication::applicationDirPath() + "/userinfo.ini";
    QFile iniFile(iniPath);
    if (!iniFile.exists())
    {
        cout<<"ini file not exist,so create it";
        //写入ini配置文件
        QSettings settings(iniPath, QSettings::IniFormat);
        settings.beginGroup("DefaultUser");
        settings.setValue("user", "Listening");
        settings.endGroup();
        settings.beginGroup("TestIP");
        settings.setValue("IP1", "192.168.136.78");
        settings.endGroup();
    }

    //读取ini
    QSettings readIni(iniPath, QSettings::IniFormat);
    return readIni.value("DefaultUser/user").toString();
}

void ChatWindow::ReceiveData()
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
            ShowReceiveData(senderName, chatMsg);
            break;
        }
        // 新用户上线，获取用户名和 IP，使用 onLine() 函数进行新用户登录的处理。
        case Online:
            readData >> senderName >> hostIp;
            cout<<"new user online"<<senderName<<hostIp;
            NewUserOnline(senderName, hostIp);
            break;
        // 用户离线，获取用户名，然后使用 offLine() 函数进行处理
        case Offline:
            readData >> senderName >> hostIp;
            UserOffline(senderName,hostIp);
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

void ChatWindow::SendData(MsgType msgType, QString friendIP, QString chatMsg)
{
    QByteArray byteData;
    QDataStream sendDataStream(&byteData, QIODevice::WriteOnly);
    QString localHostIp = GetLocalIP();

    QHostAddress originIP(friendIP);
    // 要发送的数据中写入消息类型 msgType 、用户名
    sendDataStream << msgType << mName;
    // 再写入其他信息
//    // 广播write发送数据应该绑定ip，否则多网卡发送不正确.目前测试未解决问题
//    udpSocket->bind(QHostAddress::LocalHost,port,QUdpSocket::ShareAddress);
    switch (msgType)
    {
    case ChatMsg:
        sendDataStream << chatMsg << localHostIp;
        udpSocket->writeDatagram(byteData, byteData.length(), originIP, port);
        cout<<"ChatMsg"<<chatMsg<<"friend ip"<<originIP;
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
//    case FileName:
//        sendDataStream << localHostIp << mName << FileName;
//        break;
//    case RefFile:
//        sendDataStream << localHostIp << mName;
//        break;
    }
    //TODO:确认是广播出去？
    // 完成对消息的处理后，使用套接口的 writeDatagramO 函数广播出去。
//    udpSocket->writeDatagram(byteData, byteData.length(), QHostAddress::Broadcast, port);

}

QString ChatWindow::GetLocalIP()
{
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

// 接收对方发送的信息
void ChatWindow::ShowReceiveData(const QString &senderName, const QString &sentMsg)
{
    cout<<"ShowSentData() func";
    //TODO:处理发送者，在列表中突出

    AddUserToListWidget(senderName);

    //    QString curTime = currentTime.toString("yyyy-MM-dd hh:mm:ss");
    auto currentTime =  QDateTime::currentDateTime();
    QString curTime = currentTime.toString("hh:mm:ss");
    ui->textBrowser->setAlignment(Qt::AlignLeft);
    ui->textBrowser->setTextBackgroundColor(QColor(1, 0, 0, 0));
    cout<<"receive msg"<<curTime<<sentMsg;
    ui->textBrowser->append(curTime + "\n" + sentMsg);
//    ui->textBrowser->append(sentMsg);
}

// 好友上线
void ChatWindow::NewUserOnline(const QString &newUser, const QString &ip)
{
    friendList.insert(std::pair<QString,QString> (newUser,ip));

    AddUserToListWidget(newUser);
    // 通知对方更新自己的好友列表
    SendData(Online, mName);
}

// 好友离线
void ChatWindow::UserOffline(const QString &user, const QString &ip)
{
    cout<<user<<ip;
    auto foundUser = ui->listWidget->findItems(user,Qt::MatchExactly).first();
    ui->listWidget->removeItemWidget(foundUser);
    friendList.erase(user);
}

// 将在线用户添加到listwidget
void ChatWindow::AddUserToListWidget(const QString &user)
{
    //Qt::MatchFixedString
    auto result = ui->listWidget->findItems(user, Qt::MatchContains);
    if(result.isEmpty())
    {
        QListWidgetItem *userItem = new QListWidgetItem(user);
        ui->listWidget->addItem(userItem);
    }
    //        QListWidgetItem *configButton = new QListWidgetItem(contentsWidget);
    //        configButton->setIcon(QIcon(":/images/config.png"));
    //        configButton->setText(tr("Configuration"));
    //        configButton->setTextAlignment(Qt::AlignHCenter);
    //        configButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
}

// 选择对应好友后，聊天窗口切换
QString ChatWindow::SelectedFriend()
{
    cout<<"clicked listwidget";
    ui->textBrowser->clear();
    auto selectItemList = ui->listWidget->selectedItems();
    auto selectFriend = selectItemList.at(0)->text();
    ui->friendNameLabel->setText(selectFriend);
    ui->friendNameStatusLabel->setText(GetSelectFriendIP());
    return selectFriend;
}

// 返回选择的好友IP
QString ChatWindow::GetSelectFriendIP()
{
    //测试用
//    QString friendIP = "192.168.136.78";
    QSettings readIni("userinfo.ini", QSettings::IniFormat);
    QString friendIP = readIni.value("TestIP/IP1").toString();

//    auto selectItemList = ui->listWidget->selectedItems();
    auto selectItemList = ui->listWidget->currentItem();
    auto selectFriend = selectItemList->text();
//    auto selectFriend = selectItemList.at(0)->text();
    cout<<selectFriend;
    auto findResult = friendList.find(selectFriend);
    if(findResult != friendList.end())
    {
        friendIP = findResult->second;
        cout<<"friendIP" <<friendIP;
    }
    return friendIP;
}


void ChatWindow::closeEvent(QCloseEvent *)
{
    SendData(Offline,mName);
    cout <<"close offline";
}

//TODO:聊天气泡实现
void ChatWindow::on_sendBtn_clicked()
{
    cout<<"send button clicked!";

    auto text = ui->plainTextEdit->toPlainText();
    if(text.isEmpty())
    {
        cout<<"text is empty";
        return;
    }

    ui->plainTextEdit->clear();
    ui->plainTextEdit->setFocus();

    //TODO:后期可能会使用QTextCursor设置格式
    ui->textBrowser->setTextBackgroundColor(QColor(158,234,106));
//    ui->textBrowser->setTextColor(QColor(Qt::darkGreen));

    ui->textBrowser->setAlignment(Qt::AlignRight);
    ui->textBrowser->append(QTime::currentTime().toString());
    ui->textBrowser->append(text.trimmed());

    cout<<"send text" << text;

    auto friendIP = GetSelectFriendIP();
    // text.toHtmlEscaped()测试发现无区别，暂不确定需不需要
    SendData(ChatMsg, friendIP, text);
}

//TODO:弹出设置界面,后期考虑是否用label设置（信号与槽是否支持），实现图标靠左贴
void ChatWindow::on_settingBtn_clicked()
{
    cout<<"setting dialog";

//    optionDialog = new OptionDialog(this);
    optionDialog = OptionDialog::GetInstance();
    optionDialog->show();

    connect(optionDialog, &OptionDialog::UserNameRefresh, this, [&]{
        mName = ReadInfoFromIni();
        ui->headLabel->setText(mName);
    });
}

void ChatWindow::on_fileBtn_clicked()
{
    auto filename = QFileDialog::getOpenFileName(this,
                       tr("选择发送的文件"),
                       QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
                       tr("所有文件(*.*)"));

    cout<<filename;
}
