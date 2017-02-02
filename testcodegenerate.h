#ifndef TESTCODEGENERATE_H
#define TESTCODEGENERATE_H

#include <QObject>

#include "codegenerate.h"

class TestCodeGenerate : public QObject
{
    Q_OBJECT
public:
    explicit TestCodeGenerate(QObject *parent = 0);

private:
    CodeGenerate* codeG;

signals:

public slots:

private slots:

};

#endif // TESTCODEGENERATE_H
