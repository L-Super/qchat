#include "ChatWindow.h"
#include "ui_ChatWindow.h"
#include "OptionDialog.h"
#include "SocketFramework.h"
#include <QDebug>
#include <QGraphicsDropShadowEffect>
#include <QTime>
#include <QDateTime>
#include <QFileDialog>
#include <QStandardPaths>
#include <mutex>

#define cout qDebug()<<"["<<__FILE__<<__func__<<__LINE__<<"]"

ChatWindow::ChatWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ChatWindow),
      socketer(new SocketFramework())
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
    connect(ui->listWidget,&QListWidget::itemClicked,this,&ChatWindow::SelectedFriend);

    connect(socketer,&SocketFramework::ReceiveMsgSignal,this,&ChatWindow::ShowSentData);
    connect(socketer,&SocketFramework::NewUserOnlineSignal,this,&ChatWindow::NewUserOnline);
    //TODO:左侧自己头像部分，考虑用label显示头像，并加上昵称
    mName = "Listening";
    ui->headLabel->setText(mName);
    socketer->SendData(Online, mName);
}

ChatWindow::~ChatWindow()
{
    delete ui;
}

// 接收对方发送的信息
void ChatWindow::ShowSentData(const QString &senderName, const QString &sentMsg)
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
    ui->textBrowser->append(curTime);
    ui->textBrowser->append(sentMsg);
}

// 好友上线
void ChatWindow::NewUserOnline(const QString &newUser, const QString &ip)
{
    friendList.insert(std::pair<QString,QString> (newUser,ip));

    AddUserToListWidget(newUser);
    // 通知对方更新自己的好友列表
//    socketer->SendData(Online, mName);
}

// 好友离线
void ChatWindow::UserOffline(const QString &user, const QString &ip)
{
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
    return selectFriend;
}

// 返回选择的好友IP
QString ChatWindow::GetSelectFriendIP()
{
    //测试用
    QString friendIP = "127.0.0.1";

//    auto selectItemList = ui->listWidget->selectedItems();
    auto selectItemList = ui->listWidget->currentItem();
    auto selectFriend = selectItemList->text();
//    auto selectFriend = selectItemList.at(0)->text();
    cout<<selectFriend;
    auto findResult = friendList.find(selectFriend);
    if(findResult != friendList.end())
    {
        friendIP = findResult->second;
    }
    return friendIP;
}


void ChatWindow::closeEvent(QCloseEvent *event)
{
    socketer->SendData(Offline,mName);
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
    socketer->SendData(ChatMsg, mName, friendIP, text);
}

//TODO:弹出设置界面,后期考虑是否用label设置（信号与槽是否支持），实现图标靠左贴
void ChatWindow::on_settingBtn_clicked()
{
    cout<<"setting dialog";
    optionDialog = new OptionDialog(this);
    optionDialog->show();
}

void ChatWindow::on_fileBtn_clicked()
{
    auto filename = QFileDialog::getOpenFileName(this,
                       tr("选择发送的文件"),
                       QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
                       tr("所有文件(*.*)"));

    cout<<filename;
}
