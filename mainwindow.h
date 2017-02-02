#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QFile>
#include <QFileDialog>

#include <QTreeWidget>
#include <QTreeWidgetItem>

#include <QTableWidget>
#include <QTableWidgetItem>

#include "projectdialog.h"
#include "variabledialog.h"
#include "statemodel.h"

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

    void on_pushButton_4_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::MainWindow *ui;
    QFile receiveFile;

    ProjectDialog* project_dialog;
    VariableDialog* var_dialog;

    QStringList findState(QTableWidget* table);
    QStringList findEventOfState(QTableWidget* table, QString state);
    QList<QTreeWidgetItem *> findGuardOfEvent(QTableWidget* table, QString event, QString state);
    QStringList findAllEvent(QTableWidget* table);
    QStringList getAllState(QTableWidget* table);

    QString projectName;
    StateModel* state_model;


};

#endif // MAINWINDOW_H
