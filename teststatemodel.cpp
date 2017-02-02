#include "teststatemodel.h"

#include <QTest>

TestStateModel::TestStateModel(QObject *parent) : QObject(parent)
{

}

void TestStateModel::initTestCase()
{
    QStringList headers;

    headers << "State" << "Event" << "Condition" << "Action" << "Next State";

    state_model = new StateModel(headers);

   QCOMPARE(state_model->headerData(0, Qt::Horizontal), QVariant("State"));
   QCOMPARE(state_model->headerData(1, Qt::Horizontal), QVariant("Event"));
   QCOMPARE(state_model->headerData(2, Qt::Horizontal), QVariant("Condition"));
   QCOMPARE(state_model->headerData(3, Qt::Horizontal), QVariant("Action"));
   QCOMPARE(state_model->headerData(4, Qt::Horizontal), QVariant("Next State"));
}

void TestStateModel::test_instertStates()
{
   QString s1= QString("S1");
   QString s2= QString("S2");
   QString s3= QString("S3");

   state_model->insertState(s1);
   state_model->insertState(s2);
   state_model->insertState(s3);
   state_model->insertState(s3);

   QModelIndex index = state_model->index(0, 0);
   QVERIFY(index.isValid());

   QCOMPARE(state_model->data(index, Qt::DisplayRole), QVariant("S1"));

   index = state_model->index(3, 0);
   QCOMPARE(state_model->data(index, Qt::DisplayRole), QVariant());

}

void TestStateModel::test_insertEvent()
{
    state_model->insertEvent(QString("S1"), QString("Run"));
    state_model->insertEvent(QString("S1"), QString("Run"));
    state_model->insertEvent(QString("S4"), QString("Run"));
    state_model->insertEvent(QString("S4"), QString("Stop"));
}

void TestStateModel::test_instertTransition()
{

}
