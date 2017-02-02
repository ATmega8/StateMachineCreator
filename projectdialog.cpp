#include "projectdialog.h"
#include "ui_projectdialog.h"

ProjectDialog::ProjectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProjectDialog)
{
    ui->setupUi(this);
}

ProjectDialog::~ProjectDialog()
{
    delete ui;
}

QString ProjectDialog::getProjectName()
{
    return ui->lineEdit->text();
}
