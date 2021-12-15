#include "OptionDialog.h"
#include "ui_OptionDialog.h"

OptionDialog::OptionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OptionDialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("设置"));
    // 此方法会导致打不开QDialog
//    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
    setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
}

OptionDialog::~OptionDialog()
{
    delete ui;
}
