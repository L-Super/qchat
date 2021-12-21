#include "ChatWindow.h"
#include "ui_ChatWindow.h"
#include "OptionDialog.h"
#include <QDebug>
#include <QGraphicsDropShadowEffect>
#include <QTime>
#include <QFileDialog>
#include <QStandardPaths>

#define cout qDebug()<<"["<<__FILE__<<__func__<<__LINE__<<"]"

ChatWindow::ChatWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ChatWindow)
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

    //TODO:左侧自己头像部分，考虑用label显示头像，并加上昵称
}

ChatWindow::~ChatWindow()
{
    delete ui;
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
    cout<<text;
    ui->plainTextEdit->clear();
    ui->textBrowser->setTextBackgroundColor(QColor(158,234,106));
//    ui->textEdit->setTextColor(QColor(Qt::green));

    //TODO:后期可能会使用QTextCursor设置格式
    ui->textBrowser->setAlignment(Qt::AlignRight);
    ui->textBrowser->append(QTime::currentTime().toString());
    ui->textBrowser->append(text.trimmed());

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
