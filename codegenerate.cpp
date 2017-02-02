#include "codegenerate.h"

CodeGenerate::CodeGenerate()
{

}

QString CodeGenerate::generateStateDeclaration(QString name, QString state)
{
    QString return_value = name+"StateTypeDef";
    QString state_name = name.toUpper()+"_"+state+"State";
    QString parameter_type = name+"EventTypeDef*";
    QString parameter_name = "event";
    QString parameter_fsm = name+"FSMTypeDef* "+name.toLower();

    return return_value+" "+state_name+"("+parameter_fsm+ ", " +parameter_type+" "+parameter_name+");\n";
}

QString CodeGenerate::generateStateInit(QString name)
{
    QString parameter_type = name+"EventTypeDef*";
    QString parameter_name = "event";
    QString parameter_fsm = name+"FSMTypeDef* "+name.toLower();

    return "void "+name.toUpper()+"_StateMachineInit("+parameter_fsm+ ", " +parameter_type+" "+parameter_name+");\n";
}

QString CodeGenerate::generateStateDispatch(QString name)
{
    QString parameter_type = name+"EventTypeDef*";
    QString parameter_name = "event";
    QString parameter_fsm = name+"FSMTypeDef* "+name.toLower();

    return "void "+name.toUpper()+ \
            "_StateMachineDispatch("+parameter_fsm+ ", " +parameter_type+" "+parameter_name+");\n";
}

QString CodeGenerate::generateStateVariable(QVector<QStringList> state_var)
{
    QString res, var_type, var_name;
    QStringList var;
    int i;

    res.append("\n");

    for(i = 0; i < state_var.length(); i++)
    {
        var =state_var[i];

        var_type = var[0];
        var_name = var[1];

        res.append("    ");
        res.append(var_type);
        res.append(" ");
        res.append(var_name);

        res.append(";\n");
    }

    res.append("\n");

    return res;
}

QString CodeGenerate::generateStateFSM(QString name, QString state_var)
{
    QString head = "typedef struct\n{\n";
    QString tail = "} "+name+"FSMTypeDef;\n\n";
    QString content = "    "+name + "StateHandle state;\n";

    content.append(state_var);

    return head+content+tail;
}

QString CodeGenerate::generateEventType(QString name)
{
    QString head = "typedef struct\n{\n";
    QString tail = "} "+name+"EventTypeDef;\n\n";
    QString content = "    "+name+"SignalTypeDef signal;\n";

    return head+content+tail;
}

QString CodeGenerate::generateStateType(QString name)
{
    QString head = "typedef enum\n{\n";
    QString tail = "} "+name+"StateTypeDef;\n\n";
    QString s1   = "    "+name+"_Handled,\n";
    QString s2   = "    "+name+"_Tran,\n";
    QString s3   = "    "+name+"_Ignore\n";

    return head+s1+s2+s3+tail;
}

QString CodeGenerate::generateSignalTypeDef(QString name, QStringList signal_list)
{
    QString res;
    int i;

    QString head = "typedef enum\n{\n";

    for(i = 0; i < signal_list.length(); i++)
    {
        if(i != signal_list.length()-1)
        {
            res.append("    "+name+"_"+signal_list[i]+", \n");
        }
        else
        {
            res.append("    "+name+"_"+signal_list[i]+" \n");
        }
    }

    QString tail = "} "+name+"SignalTypeDef;\n\n";

    return head+res+tail;
}

QString CodeGenerate::generateStateHandle(QString name)
{
    QString s1 = "typedef " + name+"StateTypeDef \\\n";
    QString s2 = "    (*"+name+"StateHandle)( void* "+name.toLower()+", "+name+"EventTypeDef* event);\n\n";

    return s1+s2;
}

void CodeGenerate::generateHeadFile(QFile* file, QString name, QStringList events, QStringList states,
                                    QVector<QStringList> state_var)
{
    QString res;
    int i;

   if(file->isOpen())
   {
       /* generate signal typedef */
       res = this->generateSignalTypeDef(name, events);
       file->write(res.toLocal8Bit());

       /* generate event typedef */
       res = this->generateEventType(name);
       file->write(res.toLocal8Bit());

       /* generate state typedef */
       res = this->generateStateType(name);
       file->write(res.toLocal8Bit());

       /* generate state handle */
       res = this->generateStateHandle(name);
       file->write(res.toLocal8Bit());

       /* generate FSM typedef */
       res = this->generateStateFSM(name, this->generateStateVariable(state_var));
       file->write(res.toLocal8Bit());

      /* generate state function declaration */
       for(i = 0; i < states.length(); i++)
       {
            res = this->generateStateDeclaration(name, states[i]);
            file->write(res.toLocal8Bit());
       }

       /* generate state initial function */
       res = this->generateStateInit(name);
       file->write(res.toLocal8Bit());

       /* generate state dispatch function */
       res = this->generateStateDispatch(name);
       file->write(res.toLocal8Bit());
   }
}

QString CodeGenerate::generateStateFunctionHead(QString name, QString state)
{
    QString return_value = name+"StateTypeDef";
    QString state_name = name.toUpper()+"_"+state+"State";
    QString parameter_type = name+"EventTypeDef*";
    QString parameter_name = "event";
    QString parameter_fsm = name+"FSMTypeDef* "+name.toLower();

    return return_value+" "+state_name+"("+parameter_fsm+ ", " +parameter_type+" "+parameter_name+")\n";
}

QString CodeGenerate::generateCondition(QString name, QString state, TreeItem *item)
{
    int i, c_Count = item->childCount();
    QString res = "            ";

    for(i = 0; i < c_Count; i++)
    {
       if(item->child(i)->data(2).toString() == "NONE")
       {
            if(item->child(i)->data(3).toString() != "NONE")
            {
                res.append(item->child(i)->data(3).toString());
            }

            res.append(QString("\n"));

            if(item->child(i)->data(4).toString() == state)
            {
               res.append(QString("            return "));
               res.append(name);
               res.append(QString("_Handled;\n"));
            }
            else
            {
                res.append(QString("            "));
                res.append(name.toLower());
                res.append(QString("->state = ("));
                res.append(name);
                res.append(QString("StateHandle)("));
                res.append(name.toUpper());
                res.append(QString("_"));
                res.append(item->child(i)->data(4).toString());
                res.append("State);\n");

                res.append(QString("            return "));
                res.append(name);
                res.append(QString("_Tran;\n"));
            }
       }
       else
       {
            res.append("            if( ");
            res.append(item->child(i)->data(2).toString());
            res.append(" )\n            {\n                ");

            if(item->child(i)->data(3).toString() != "NONE")
            {
                res.append(item->child(i)->data(3).toString());
            }

            res.append(QString("\n"));

            if(item->child(i)->data(4).toString() == state)
            {
               res.append(QString("                return "));
               res.append(name);
               res.append(QString("_Handled;\n"));
            }
            else
            {
                res.append(QString("                "));
                res.append(name.toLower());
                res.append(QString("->state = ("));
                res.append(name);
                res.append(QString("StateHandle)("));
                res.append(name.toUpper());
                res.append(QString("_"));
                res.append(item->child(i)->data(4).toString());
                res.append("State);\n");

                res.append(QString("                return "));
                res.append(name);
                res.append(QString("_Tran;\n"));
            }

            res.append("            }\n");
       }
    }

    return res;
}

QString CodeGenerate::generateEventSwitch(QString name, QString state, TreeItem *item)
{
   int i, eventCount = item->childCount();

   QString res;

   res = "    switch(event->signal)\n    {\n";

   for(i = 0; i < eventCount; i++)
   {
      res.append(QString("        case "));
      res.append(name);
      res.append("_");
      res.append(item->child(i)->data(1).toString());
      res.append(QString(":\n"));
      res.append(this->generateCondition(name, state, item->child(i)));
      res.append(QString("            break;\n\n"));
   }

   res.append(QString("    }\n"));

   return res;
}

QString CodeGenerate::generateStateFunction(QString name, TreeItem *item)
{
    int i, stateCount = item->childCount();

    QString res, state_head;

    for(i = 0; i < stateCount; i++)
    {
       res.append(this->generateStateFunctionHead(name, (item->child(i)->data(0).toString())));
       res.append(QString("{\n\n"));
       res.append(this->generateEventSwitch(name, item->child(i)->data(0).toString(), item->child(i)));
       res.append(QString("    return "));
       res.append(name);
       res.append("_Ignore;\n}\n\n");
    }

    return res;
}

QString CodeGenerate::generateDispatchFunction(QString name)
{
    QString res;
    QString parameter_type = name+"EventTypeDef*";
    QString parameter_name = "event";
    QString parameter_fsm = name+"FSMTypeDef* "+name.toLower();

    res = "void "+name.toUpper()+ \
            "_StateMachineDispatch("+parameter_fsm+ ", " +parameter_type+" "+parameter_name+")\n{\n";

    res.append("    ");
    res.append(name);
    res.append("StateTypeDef res;\n");

    res.append("    ");
    res.append(name);
    res.append("StateHandle s;\n\n");

    res.append(QString("    s = "));
    res.append(name.toLower());
    res.append("->state;\n");

    res.append(QString("    res = ("));
    res.append(name.toLower());
    res.append("->state)(");
    res.append(name.toLower());
    res.append(", event);\n\n");

    res.append("    if(res == ");
    res.append(name);
    res.append("_Tran)\n    {\n");

    res.append("        event->signal = ");
    res.append(name);
    res.append("_Exit;\n");

    res.append("        (s)(");
    res.append(name.toLower());
    res.append(", event);\n\n");

    res.append("        event->signal = ");
    res.append(name);
    res.append("_Enter;\n");

    res.append(QString("        ("));
    res.append(name.toLower());
    res.append("->state)(");
    res.append(name.toLower());
    res.append(", event);\n");

    res.append("    }\n}\n\n");

    return res;
}

QString CodeGenerate::generateInitFunction(QString name)
{
    QString res;
    QString parameter_type = name+"EventTypeDef*";
    QString parameter_name = "event";
    QString parameter_fsm = name+"FSMTypeDef* "+name.toLower();

    res = "void "+name.toUpper()+"_StateMachineInit("+parameter_fsm+ ", " +parameter_type+" "+parameter_name+")\n";
    res.append("{\n");

    res.append(QString("    ("));
    res.append(name.toLower());
    res.append("->state)(");
    res.append(name.toLower());
    res.append(", event);\n");

    res.append("}\n\n");

    return res;
}

void CodeGenerate::generateSourceFile(QFile *file, QString name, StateModel *model)
{
   QString res;
   TreeItem* root = model->getRootItem();

    if(file->isOpen())
    {
        /* include file */
        res = "#include \""+name.toLower()+".h\"\n#include <stdio.h>\n\n";
        file->write(res.toLocal8Bit());

        /* state function */
        res = this->generateStateFunction(name, root);
        file->write(res.toLocal8Bit());

        res = this->generateInitFunction(name);
        file->write(res.toLocal8Bit());

        res = this->generateDispatchFunction(name);
        file->write(res.toLocal8Bit());
    }
}
