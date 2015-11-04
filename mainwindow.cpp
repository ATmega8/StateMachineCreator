#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <math.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
     qint64 colCount = 0, rowCount = 0, i;
     QString receiveFileName;
     QString itemText;
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
}
