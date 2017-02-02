#include "statemodel.h"

StateModel::StateModel(const QStringList &headers, QObject *parent)
    : TreeModel(headers, parent)
{

}

StateModel::~StateModel()
{

}

void StateModel::insertState(const QString &stateName)
{
    int count, i;

    TreeItem* rootItem = this->getRootItem();

    count = rootItem->childCount();

    if(count == 0)
    {
        rootItem->insertChildren(count, 1, 1);
        rootItem->child(count)->setData(0, stateName);
    }
    else
    {
        for(i = 0; i < count; i++)
        {
            if(rootItem->child(i)->data(0) == stateName)
            {
                return;
            }
        }

        rootItem->insertChildren(count, 1, 1);
        rootItem->child(count)->setData(0, stateName);
    }
}

void StateModel::insertEvent(const QString &state, const QString &event)
{

    int count, e_count, i, j;

    TreeItem* rootItem = this->getRootItem();

    this->insertState(state);

    count = rootItem->childCount();

    for(i = 0; i < count; i++)
    {
        if(rootItem->child(i)->data(0) == state)
        {
            e_count = rootItem->child(i)->childCount();

            if(e_count == 0)
            {
                rootItem->child(i)->insertChildren(0, 1, 2);
                rootItem->child(i)->child(0)->setData(1, event);
            }
            else
            {
                for(j = 0; j < e_count; j++)
                {
                    if(rootItem->child(i)->child(j)->data(1) == event)
                    {
                        return;
                    }
                }

                rootItem->child(i)->insertChildren(e_count, 1, 2);
                rootItem->child(i)->child(e_count)->setData(1, event);
            }
        }
    }
}

void StateModel::insertTransition(const QString &state, const QString &event,
                                  const QString &condition, const QString &action, const QString &nextState)
{
    int s_count, e_count, o_count, i, j;

    this->insertEvent(state, event);

    TreeItem* rootItem = this->getRootItem();

    s_count = rootItem->childCount();

    for(i = 0; i < s_count; i++)
    {
        if(rootItem->child(i)->data(0) == state)
        {
            e_count = rootItem->child(i)->childCount();

            for(j = 0; j < e_count; j++)
            {
               if(rootItem->child(i)->child(j)->data(1) == event)
               {
                   o_count = rootItem->child(i)->child(j)->childCount();

                   rootItem->child(i)->child(j)->insertChildren(o_count, 1, 5);

                   rootItem->child(i)->child(j)->child(o_count)->setData(2, condition);
                   rootItem->child(i)->child(j)->child(o_count)->setData(3, action);
                   rootItem->child(i)->child(j)->child(o_count)->setData(4, nextState);
               }
            }
        }
    }
}
