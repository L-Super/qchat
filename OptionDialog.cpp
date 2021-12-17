#include "OptionDialog.h"
#include "ui_OptionDialog.h"
#include "about.h"
#include <QHostInfo>
#include <QFileInfo>
#include <QDir>

#define cout qDebug()<<"["<<__FILE__<<__func__<<__LINE__<<"]"

OptionDialog::OptionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OptionDialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("设置"));
    // 此方法会导致打不开QDialog
//    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
    setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);

    ui->themeComboBox->addItem("default");

    GetThemeFile();
    LocalAddress();
    AboutTabWidget();
}

OptionDialog::~OptionDialog()
{
    delete ui;
}

void OptionDialog::LocalAddress()
{
    QHostInfo info = QHostInfo::fromName(QHostInfo::localHostName());

//    QList<QHostAddress> ipList = info.addresses();
    foreach(QHostAddress ip,info.addresses())
    {
        cout << "ip:" <<ip.toString();
        // 只取ipv4协议的地址
        if(ip.protocol() == QAbstractSocket::IPv4Protocol)
        {
            // 默认取第一个ip地址
            ui->ipLineEdit->setText(ip.toString());
            break;
        }
    }
    //TODO:加上端口
}

void OptionDialog::GetThemeFile()
{
    QDir themes("./theme");
    if ( !themes.exists() )
        qWarning( "Cannot find the example directory" );
    themes.setFilter( QDir::Files | QDir::NoSymLinks );
    QStringList themeList;
    foreach(QFileInfo fileInfo, themes.entryInfoList())
    {
        QString file = fileInfo.fileName();
        // 去掉.qss后缀,并存入themeList
        themeList << file.section(".",0,0);
    }
    cout << themeList;

    ui->themeComboBox->addItems(themeList);
}

void OptionDialog::AboutTabWidget()
{
    About about;
    ui->authorLabel->setText(about.GetAuthor());
    ui->introduceLabel->setText(about.GetIntroduce());
    ui->versionLabel->setText(about.GetVersion());
}

