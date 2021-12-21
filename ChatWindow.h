#pragma once

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class ChatWindow; }
QT_END_NAMESPACE

//前置声明
class OptionDialog;

class ChatWindow : public QMainWindow
{
    Q_OBJECT

public:
    ChatWindow(QWidget *parent = nullptr);
    ~ChatWindow();

private:
//    // 该功能似乎需要重写键盘过滤事件
//    // 设置快捷键
//    void SetShortcutKey();

private slots:
    void on_sendBtn_clicked();

    void on_settingBtn_clicked();

    void on_fileBtn_clicked();

private:
    Ui::ChatWindow *ui;
    OptionDialog* optionDialog;
};
