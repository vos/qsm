#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QTreeWidgetItem;
QT_END_NAMESPACE

namespace Ui {
    class OptionsDialog;
}

class OptionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OptionsDialog(QList<QPair<QString, QList<QAction*> > > *actions, QWidget *parent = 0);
    ~OptionsDialog();

private slots:
    void on_shortcutsTreeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

    void on_resetAllPushButton_clicked();
    void on_resetPushButton_clicked();

private:
    Ui::OptionsDialog *ui;
    QList<QPair<QString, QList<QAction*> > > *m_actions;

    bool checkShortcuts();
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif // OPTIONSDIALOG_H
