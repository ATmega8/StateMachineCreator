#ifndef CODEGENERATE_H
#define CODEGENERATE_H

#include <QString>
#include <QFile>

#include "statemodel.h"

class CodeGenerate
{
public:
    CodeGenerate();
    void generateHeadFile(QFile* file, QString name, QStringList events, QStringList states,
                          QVector<QStringList> state_var);

    void generateSourceFile(QFile* file, QString name, StateModel* model);
    //QString generateEventDeclaration(void);

private:
   QString generateStateDeclaration(QString name, QString state);
   QString generateStateInit(QString name);
   QString generateStateDispatch(QString name);
   QString generateStateFSM(QString name, QString state_var);
   QString generateStateType(QString name);
   QString generateStateHandle(QString name);
   QString generateEventType(QString name);
   QString generateSignalTypeDef(QString name, QStringList signal_list);

   QString generateStateFunction(QString name, TreeItem* item);
   QString generateEventSwitch(QString name, QString state, TreeItem* item);
   QString generateCondition(QString name, QString state, TreeItem* item);

   QString generateDispatchFunction(QString name);
   QString generateInitFunction(QString name);
   QString generateStateFunctionHead(QString name, QString state);

   QString generateStateVariable(QVector<QStringList> state_var);
};

#endif // CODEGENERATE_H

