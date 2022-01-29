#pragma once

#include <QDialog>
#include <QTabWidget>
#include <mutex>

namespace Ui {
class OptionDialog;
}

class OptionDialog : public QDialog
{
    Q_OBJECT
public:
    static OptionDialog *GetInstance();
    class Deletor // 类中套类，用来释放对象
    {
      public:
        ~Deletor()
        {
            if(mInstance)
            {
                delete mInstance;
                mInstance = nullptr;
            }
        }
    };
private:
    explicit OptionDialog(QWidget *parent = nullptr);
    ~OptionDialog();
    static OptionDialog *mInstance; //静态成员变量
    static Deletor deleter;
    static std::mutex myMutex;

public:
    // 设置昵称
    QString GetUserName();
    void SetUserName();
    void LocalAddress();
    // 获取主题及主题设置
    void GetThemeFile();
    void SetTheme();

    void AboutTabWidget();

private slots:
    void on_confirmPushButton_clicked();

protected:
    void closeEvent(QCloseEvent *event);

signals:
    void UserNameRefresh();

private:
    Ui::OptionDialog *ui;
    QString userIniPath;
};

//TODO:后期再考虑tabwidget方向问题

