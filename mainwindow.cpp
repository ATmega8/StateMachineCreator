#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "codegenerate.h"

#include "statemodel.h"

#include <QMessageBox>
#include <QFile>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QStringList headers;
    headers << "State" << "Event" << "Condition" << "Action" << "Next State";

    ui->setupUi(this);
    ui->treeView->setColumnWidth(0, 200);
    ui->treeView->setColumnWidth(1, 92);
    ui->treeView->setColumnWidth(2, 92);

    state_model = new StateModel(headers);

    project_dialog = new ProjectDialog();
    var_dialog = new VariableDialog();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    int rowCount;

    rowCount = ui->tableWidget->rowCount();

    ui->tableWidget->setRowCount(rowCount+1);
}

void MainWindow::on_pushButton_2_clicked()
{
    QString receiveFileName;
    QString tableData;
    int rowCount, colCount, i, j;

    /*打开文件*/
    receiveFileName = QFileDialog::getSaveFileName(this,
        tr("Open Receive File"), "/home/life", tr("Any files(*)"));

    receiveFile.setFileName(receiveFileName);

    if(!receiveFile.open(QIODevice::ReadWrite))
            ui->statusBar->showMessage(tr("打开文件失败"));
    else
        ui->statusBar->showMessage(tr("打开文件成功"));

    /*得到行宽度*/
    rowCount = ui->tableWidget->rowCount();

    /*得到列宽度*/
    colCount = ui->tableWidget->columnCount();

    if((rowCount != 0) && (colCount != 0))
    {
        /*写入数据*/
        for(i = 0; i < rowCount; i++)
        {
            for(j = 0; j < colCount; j++)
            {
                /*读取项目*/
                if(ui->tableWidget->item(i, j) != 0)
                    tableData = ui->tableWidget->item(i, j)->text();

                /*添加分隔符*/
                tableData.append(",");

                receiveFile.write(tableData.toStdString().data());
            }

            /*添加换行符*/
            receiveFile.write("\n");
        }
    }

   /*关闭文件*/
   receiveFile.close();
   ui->statusBar->showMessage(tr("文件已关闭"));
}

void MainWindow::on_pushButton_3_clicked()
{
     int i, j, rowCount = 0, colCount = 0;
     QString receiveFileName, itemText, stateText;
     QStringList itemTexts;

     char readData[1024];

     /*打开文件*/
     receiveFileName = QFileDialog::getOpenFileName(this,
        tr("Open Receive File"), "/home/life", tr("Text files(*.dat)"));

     receiveFile.setFileName(receiveFileName);

     if(!receiveFile.open(QIODevice::ReadWrite))
            ui->statusBar->showMessage(tr("打开文件失败"));
     else
        ui->statusBar->showMessage(tr("打开文件成功"));

    while(receiveFile.readLine(readData, sizeof(readData)) > 0)
    {
        ui->tableWidget->setRowCount(rowCount+1);

        for(i = 0; i < 1024; i++)
        {
            if((readData[i] != ',') && (readData[i] != '\n'))
                itemText.append(readData[i]);
            else if(readData[i] == ',')
            {

                /*写入条目*/
                QTableWidgetItem *newItem = new QTableWidgetItem(itemText);

                if(itemText == "NONE")
                    newItem->setTextColor(Qt::red);

                ui->tableWidget->setItem(rowCount, colCount, newItem);

                itemText.clear();

                colCount++;
            }
            else if(readData[i] == '\n')
                break;
        }

        rowCount++;
        colCount = 0;
    }

    receiveFile.close();
    ui->statusBar->showMessage(tr("文件已关闭"));

    /*生成状态树*/
    rowCount = ui->tableWidget->rowCount();

    for(j = 0; j < rowCount; j++)
    {
        QString s = ui->tableWidget->item(j, 0)->text();
        QString e = ui->tableWidget->item(j, 1)->text();
        QString c = ui->tableWidget->item(j, 2)->text();
        QString a = ui->tableWidget->item(j, 4)->text();
        QString n = ui->tableWidget->item(j, 3)->text();
        state_model->insertTransition(s, e, c, a, n);
    }

    ui->treeView->setModel(state_model);
}

QStringList MainWindow::findState(QTableWidget *table)
{
    QStringList states;
    QString itemText;
    int rowCount, i;

    /*获取行数*/
    rowCount = table->rowCount();

    /*获取第一个状态*/
    itemText = table->item(0, STATE_COL)->text();
    states.append(itemText);

    /*获取之后的状态*/
    for(i = 1; i < rowCount; i++)
    {
        if(table->item(i, STATE_COL)->text() != itemText)
        {
           itemText = table->item(i, STATE_COL)->text();
           states.append(itemText);
        }
    }

    return states;
}

QStringList MainWindow::findEventOfState(QTableWidget* table, QString state)
{
    int rowCount, i;
    QStringList events;
    QString itemText;

    rowCount = table->rowCount();

    for(i = 0; i < rowCount; i++)
    {
        if(table->item(i, STATE_COL)->text() == state)
        {
            if(itemText != table->item(i, EVENT_COL)->text())
            {
                   itemText = table->item(i, EVENT_COL)->text();
                   events.append(itemText);
            }
        }
    }

    return events;
}

QList<QTreeWidgetItem*> MainWindow::findGuardOfEvent(QTableWidget* table, QString event, QString state)
{
    int rowCount, i;
    QList<QTreeWidgetItem*> guards;
    QString itemText;

    rowCount = table->rowCount();

    for(i = 0; i < rowCount; i++)
    {
        if((table->item(i, STATE_COL)->text() == state) && (table->item(i, EVENT_COL)->text() == event))
        {
            guards.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList(table->item(i, GUARD_COL)->text())));
            guards.at(guards.length()-1)->setText(1, table->item(i, NSTATE_COL)->text());
            guards.at(guards.length()-1)->setTextColor(1, QColor::fromRgb(128, 0, 128)); /*紫色*/
            guards.at(guards.length()-1)->setText(2, table->item(i, ACTION_COL)->text());
        }
    }

    return guards;
}

QStringList MainWindow::findAllEvent(QTableWidget* table)
{
    QStringList allEvent;
    QString itemText;

    int i;

    for(i = 0; i < table->rowCount(); i++)
    {
        if(table->item(i, EVENT_COL)->text() != "NONE")
        {
            if(allEvent.indexOf(table->item(i, EVENT_COL)->text()) == -1)
                allEvent.append(table->item(i, EVENT_COL)->text());
        }
    }

    return allEvent;
}

void MainWindow::on_pushButton_4_clicked()
{
    QFileDialog dialog;
    QFile       file;
    QString fileName;

    if(projectName.isEmpty())
    {
        QMessageBox::critical(this, tr("NOT PROJECT NAME"),
                              tr("Please set a projecr name"), QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    else
    {
            fileName = dialog.getExistingDirectory(this,
                tr("Open Receive File Directory"), "/home/life", QFileDialog::ShowDirsOnly);

            file.setFileName(fileName+"/"+projectName.toLower()+".h");

            file.open(QIODevice::WriteOnly);

            CodeGenerate* codeG = new CodeGenerate();

            codeG->generateHeadFile(&file, projectName, this->findAllEvent(ui->tableWidget),
                                    this->findState(ui->tableWidget), var_dialog->stateVariable());

            file.close();

            file.setFileName(fileName+"/"+projectName.toLower()+".c");

            file.open(QIODevice::WriteOnly);

            codeG->generateSourceFile(&file, projectName, state_model);
    }
}

void MainWindow::on_pushButton_6_clicked()
{
   if(project_dialog->exec() == true)
   {
        if(!(project_dialog->getProjectName().isEmpty()))
        {
            projectName = project_dialog->getProjectName();
        }
   }
}

void MainWindow::on_pushButton_5_clicked()
{
    if(var_dialog->exec() == true)
    {
        var_dialog->stateVariable();
    }
}
