#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->treeWidget->setColumnCount(3);
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
        tr("Open Receive File"), "/home/life", tr("Any files(*)"));

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
    QList<QTreeWidgetItem*> topItem;

    itemTexts = findState(ui->tableWidget);

    for(i = 0; i < itemTexts.length(); i++)
        topItem.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList(itemTexts.at(i))));

    ui->treeWidget->insertTopLevelItems(0, topItem);

    itemTexts.clear();

    for(i = 0; i < ui->treeWidget->topLevelItemCount(); i++)
    {
        QList<QTreeWidgetItem*> eventItems;

        stateText = ui->treeWidget->topLevelItem(i)->text(0);

        itemTexts = findEventOfState(ui->tableWidget, stateText);

        for(j = 0; j < itemTexts.length(); j++)
        {
            QList<QTreeWidgetItem*> guardItems;

            eventItems.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList(itemTexts.at(j))));

            guardItems = findGuardOfEvent(ui->tableWidget, itemTexts.at(j), stateText);

            /*for(k = 0; k < guardTexts.length(); k++)
            {
                guardItems.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList(guardTexts.at(k))));
            }*/

            eventItems.at(j)->addChildren(guardItems);
        }

        ui->treeWidget->topLevelItem(i)->addChildren(eventItems);
    }


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
            if(itemText != table->item(i, GUARD_COL)->text())
            {
                   guards.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList(table->item(i, GUARD_COL)->text())));
                   guards.at(guards.length()-1)->setText(1, table->item(i, NSTATE_COL)->text());
                   guards.at(guards.length()-1)->setText(2, table->item(i, ACTION_COL)->text());
            }
        }
    }

    return guards;
}
