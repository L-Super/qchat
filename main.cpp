#include "ChatWindow.h"
#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // 加载css
    QFile uiStyle(":/resource/QssStyle.css");
    if( uiStyle.open(QFile::ReadOnly))
    {
        qApp->setStyleSheet(uiStyle.readAll());
        uiStyle.close();
    }
    //TODO:后期考虑开放主题设置
//     QFile uiStyle("./resource/Ubuntu.qss");
//     if( uiStyle.open(QFile::ReadOnly))
//         {
//             qApp->setStyleSheet(uiStyle.readAll());
//             uiStyle.close();
//         }

    ChatWindow w;
    w.show();
    return a.exec();
}
