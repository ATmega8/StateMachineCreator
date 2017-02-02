#ifndef TESTSTATEMODEL_H
#define TESTSTATEMODEL_H

#include <QObject>

#include "statemodel.h"

class TestStateModel : public QObject
{
    Q_OBJECT
public:
    explicit TestStateModel(QObject *parent = 0);

private:
    StateModel* state_model;

signals:

public slots:

private slots:
    void initTestCase(void);
    void test_instertStates(void);
    void test_insertEvent(void);
    void test_instertTransition(void);
};

#endif // TESTSTATEMODEL_H
