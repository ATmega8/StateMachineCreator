#ifndef STATEMODEL_H
#define STATEMODEL_H

#include <QVariant>
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QList>

#include "treeitem.h"
#include "treemodel.h"

class StateModel : public TreeModel
{
public:
    StateModel(const QStringList &headers, QObject *parent = 0);
    ~StateModel();

    void insertState(const QString &stateName);
    void insertEvent(const QString &state, const QString &event);
    void insertTransition(const QString &state, const QString &event, const QString &condition, const QString &action, const QString &nextState);

private:

};

#endif // STATEMODEL_H
