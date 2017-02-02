#include "variabledialog.h"
#include "ui_variabledialog.h"

#include <QTabWidget>
#include <QTableWidget>

VariableDialog::VariableDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VariableDialog)
{
    ui->setupUi(this);
}

VariableDialog::~VariableDialog()
{
    delete ui;
}

QVector<QStringList> VariableDialog::stateVariable()
{
   QVector<QStringList> res;
   int i, count;

   count = this->stateVariableCount();

   for(i = 0; i < count; i++)
   {
       if(!this->getStateVariableItem(i).isEmpty())
       {
           res << this->getStateVariableItem(i);
       }
   }

   return res;
}

QStringList VariableDialog::getStateVariableItem(int index)
{
    QStringList var;

    if(ui->SVWidget->item(index, 0) && ui->SVWidget->item(index, 1))
    {
        var << ui->SVWidget->item(index, 0)->text() \
            << ui->SVWidget->item(index, 1)->text();
    }

    return var;
}

int VariableDialog::stateVariableCount()
{
    return ui->SVWidget->rowCount();
}

void VariableDialog::on_pushButton_clicked()
{
    int index = ui->tabWidget->currentIndex();

    if(index == 0)
    {
        ui->SVWidget->setRowCount(ui->SVWidget->rowCount() + 1);
    }
    else if(index == 1)
    {
       ui->EVWidget->setRowCount(ui->EVWidget->rowCount() + 1);
    }
}
