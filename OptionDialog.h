#pragma once

#include <QDialog>
#include <QTabWidget>

namespace Ui {
class OptionDialog;
}

class OptionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OptionDialog(QWidget *parent = nullptr);
    ~OptionDialog();
    void LocalAddress();
    // 获取主题及主题设置
    void GetThemeFile();
    void SetTheme();

    void AboutTabWidget();

private:
    Ui::OptionDialog *ui;
};

//TODO:后期再考虑tabwidget方向问题
//// 重写TabWidget
//class TabWidget : public QTabWidget
//{
//    Q_OBJECT
//public:
//    explicit TabWidget(QWidget *parent = nullptr);

//    void updateselect();                          //用来设置一开始选中的状态
//    int addTab(QWidget *widget, const QString &); //重载

//signals:

//public slots:
//    void slotSetLabelColor(int color);            //自定义槽(响应tabBar点击事件)
//};
