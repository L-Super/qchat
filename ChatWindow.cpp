#include "ChatWindow.h"
#include "ui_ChatWindow.h"
#include <QDebug>
#include <QGraphicsDropShadowEffect>

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

}

ChatWindow::~ChatWindow()
{
    delete ui;
}


void ChatWindow::on_sendBtn_clicked()
{
    cout<<"send button clicked!";
}
