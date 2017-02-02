#include "mainwindow.h"
#include <QApplication>

#ifndef TEST
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
#else
#include <QTest>

#include "testcodegenerate.h"
#include "teststatemodel.h"

int main(int argc, char *argv[])
{
    TestCodeGenerate test1;
    TestStateModel   test2;

    QTest::qExec(&test1, argc, argv);
    QTest::qExec(&test2, argc, argv);
}
#endif
