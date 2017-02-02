#ifndef VARIABLEDIALOG_H
#define VARIABLEDIALOG_H

#include <QDialog>

namespace Ui {
class VariableDialog;
}

class VariableDialog : public QDialog
{
    Q_OBJECT

public:
    explicit VariableDialog(QWidget *parent = 0);
    ~VariableDialog();
    QVector<QStringList> stateVariable();

private slots:
    void on_pushButton_clicked();


private:
    Ui::VariableDialog *ui;

    QStringList getStateVariableItem(int index);
    int         stateVariableCount(void);
};

#endif // VARIABLEDIALOG_H
