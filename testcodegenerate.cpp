#include "testcodegenerate.h"

#include <QtTest/QtTest>
#include <QString>

TestCodeGenerate::TestCodeGenerate(QObject *parent) : QObject(parent)
{
   codeG = new CodeGenerate();
}
