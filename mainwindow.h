#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QFile>
#include <QFileDialog>

#include <QTreeWidget>
#include <QTreeWidgetItem>

#include <QTableWidget>
#include <QTableWidgetItem>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

#define STATE_COL  0
#define EVENT_COL  1
#define GUARD_COL  2
#define NSTATE_COL 3
#define ACTION_COL 4

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
    QFile receiveFile;

    QStringList findState(QTableWidget* table);
    QStringList findEventOfState(QTableWidget* table, QString state);
    QList<QTreeWidgetItem *> findGuardOfEvent(QTableWidget* table, QString event, QString state);
};

#endif // MAINWINDOW_H
