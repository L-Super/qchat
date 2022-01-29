#pragma once

#include <QMainWindow>
#include <map>

QT_BEGIN_NAMESPACE
namespace Ui { class ChatWindow; }
QT_END_NAMESPACE

//前置声明
class QDateTime;
class OptionDialog;
class SocketFramework;
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

class ChatWindow : public QMainWindow
{
    Q_OBJECT

public:
    ChatWindow(QWidget *parent = nullptr);
    ~ChatWindow();

    // 从配置文件读取账号昵称等信息
    QString ReadInfoFromIni();

    void ReceiveData();
    void SendData(MsgType msgType, QString friendIP = "", QString msg = "");
    QString GetLocalIP();

    // 展示对方发来的消息
    void ShowReceiveData(const QString& senderName, const QString& sentMsg);
    void NewUserOnline(const QString& newUser,const QString& ip);
    void UserOffline(const QString& user,const QString& ip);

private:
//    // 该功能似乎需要重写键盘过滤事件
//    // 设置快捷键
//    void SetShortcutKey();
    void AddUserToListWidget(const QString& user);
    QString SelectedFriend();
    QString GetSelectFriendIP();

protected:
    // 重写关闭窗口方法以便发送通知离开消息
    void closeEvent(QCloseEvent *event);

private slots:
    void on_sendBtn_clicked();

    void on_settingBtn_clicked();

    void on_fileBtn_clicked();

private:
    Ui::ChatWindow *ui;
    OptionDialog* optionDialog;
    QString mName;// 自己的昵称
    std::map<QString,QString> friendList;//map<user,ip>
    QUdpSocket* udpSocket;
    qint16 port;
};
