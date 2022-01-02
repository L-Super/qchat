#include "OptionDialog.h"
#include "ui_OptionDialog.h"
#include "about.h"
#include <QHostInfo>
#include <QFileInfo>
#include <QSettings>
#include <QDir>

#define cout qDebug()<<"["<<__FILE__<<__func__<<__LINE__<<"]"

OptionDialog::OptionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OptionDialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("设置"));
    // 去掉对话框的问号按钮
    // 此方法会导致打不开QDialog
//    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
    setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);

    ui->themeComboBox->addItem("default");

    // 获取昵称
    ui->userNameLineEdit->setText(GetUserName());
    GetThemeFile();
    LocalAddress();
    AboutTabWidget();
}

OptionDialog::~OptionDialog()
{
    cout <<"析构";
    delete ui;
}

QString OptionDialog::GetUserName()
{
    userIniPath = QCoreApplication::applicationDirPath() + "/userinfo.ini";

    QFile iniFile(userIniPath);
    if (!iniFile.exists())
    {
        cout<< "user ini file not exist,read failed";
    }

    //读取ini
    QSettings readIni(userIniPath, QSettings::IniFormat);
    QString userName = readIni.value("DefaultUser/user").toString();
    cout << userName;
    return userName;
}

void OptionDialog::SetUserName()
{

    QFile iniFile(userIniPath);

    auto setUserName = ui->userNameLineEdit->text();

    QSettings ini(userIniPath, QSettings::IniFormat);
    ini.beginGroup("DefaultUser");
    ini.setValue("user", setUserName);
    ini.endGroup();
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
    ui->themeComboBox->addItems(themeList);
}

//TODO:主题设置失败
void OptionDialog::SetTheme()
{
    QString selectThemeName = ui->themeComboBox->currentText();
    selectThemeName.append(".css");
    cout<<selectThemeName;

    QFile style("./theme/" + selectThemeName);
    if( style.open(QFile::ReadOnly))
    {
        cout<<"open success";
        qApp->setStyleSheet(style.readAll());
        style.close();
    }
}

void OptionDialog::AboutTabWidget()
{
    About& about = About::GetInstance();

    ui->authorLabel->setText(about.GetAuthor());
    ui->introduceLabel->setText(about.GetIntroduce());
    ui->versionLabel->setText(about.GetVersion());

    // 设置默认打开页
    ui->tabWidget->setCurrentIndex(0);
}

//TODO：按下按钮后，按钮高度会变化，待修复
//TODO:由于ChatWindow初始化时，就new OptionDialog，导致配置文件外部修改时，无法及时更新显示。
//      但不初始化时new，connect()就会失败
void OptionDialog::on_confirmPushButton_clicked()
{
    SetUserName();
    SetTheme();
    emit UserNameRefresh();
}


void OptionDialog::closeEvent(QCloseEvent *event)
{
//    emit UserNameRefresh();
//    this->deleteLater();
}
