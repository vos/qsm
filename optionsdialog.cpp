#include "optionsdialog.h"
#include "ui_optionsdialog.h"

#include <QKeyEvent>
#include <QDebug>

OptionsDialog::OptionsDialog(QList<QPair<QString, QList<QAction*> > > *actions, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OptionsDialog),
    m_actions(actions)
{
    Q_ASSERT(actions);
    ui->setupUi(this);

    // set shortcuts
    QList<QTreeWidgetItem*> items;
    for (int i = 0; i < actions->count(); ++i) {
        QPair<QString, QList<QAction*> > actionGroup = actions->at(i);
        QTreeWidgetItem *itemGroup = new QTreeWidgetItem(ui->shortcutsTreeWidget, QStringList(actionGroup.first));
        QFont font = itemGroup->font(0);
        font.setBold(true);
        itemGroup->setFont(0, font);
        items.append(itemGroup);
        for (int j = 0; j < actionGroup.second.count(); ++j) {
            QAction *action = actionGroup.second.at(j);
            QStringList itemText(action->text().replace('&', ""));
            itemText.append(action->toolTip());
            itemText.append(action->shortcut().toString(QKeySequence::NativeText));
            QTreeWidgetItem *item = new QTreeWidgetItem(itemGroup, itemText);
            item->setIcon(0, action->icon());
            items.append(item);
        }
    }
    ui->shortcutsTreeWidget->addTopLevelItems(items);
    ui->shortcutsTreeWidget->expandAll();

    checkShortcuts();
    ui->keyLineEdit->installEventFilter(this);
}

OptionsDialog::~OptionsDialog()
{
    delete ui;
}

void OptionsDialog::on_shortcutsTreeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *)
{
    ui->shortcutGroupBox->setEnabled(current->childCount() == 0);
    ui->keyLineEdit->setText(current->text(2));
}

void OptionsDialog::on_resetAllPushButton_clicked()
{
    for (int i = 0; i < m_actions->count(); ++i) {
        QPair<QString, QList<QAction*> > actionGroup = m_actions->at(i);
        QTreeWidgetItem *item = ui->shortcutsTreeWidget->topLevelItem(i);
        for (int j = 0; j < actionGroup.second.count(); ++j) {
            actionGroup.second[j]->setShortcut(QKeySequence());
            item->child(j)->setText(2, "");
        }
    }
    ui->keyLineEdit->clear();
}

void OptionsDialog::on_resetPushButton_clicked()
{
    QTreeWidgetItem *item = ui->shortcutsTreeWidget->currentItem();
    if (item) {
        QModelIndex index = ui->shortcutsTreeWidget->currentIndex();
        if (index.parent().isValid())
            m_actions->value(index.parent().row()).second.value(index.row())->setShortcut(QKeySequence());
        item->setText(2, "");
    }
    ui->keyLineEdit->clear();
}

bool OptionsDialog::checkShortcuts()
{
    bool multipleUsage = false;
    for (int i = 0; i < m_actions->count(); ++i) {
        QPair<QString, QList<QAction*> > actionGroup = m_actions->at(i);
        for (int j = 0; j < actionGroup.second.count(); ++j) {
            ui->shortcutsTreeWidget->topLevelItem(i)->child(j)->setForeground(2, Qt::black); // reset color
            for (int k = 0; k < m_actions->count(); ++k) {
                QPair<QString, QList<QAction*> > actionGroup2 = m_actions->at(k);
                for (int l = 0; l < actionGroup2.second.count(); ++l) {
                    if (i == k && j == l)
                        continue;
                    if (actionGroup.second.at(j)->shortcut() == actionGroup2.second.at(l)->shortcut()) {
                        ui->shortcutsTreeWidget->topLevelItem(i)->child(j)->setForeground(2, Qt::red);
                        multipleUsage = true;
                    }
                }
            }
        }
    }
    return multipleUsage;
}

bool OptionsDialog::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->keyLineEdit) {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            QModelIndex index = ui->shortcutsTreeWidget->currentIndex();
            if (index.parent().isValid()) {
                int key = keyEvent->key();
                if (key < Qt::Key_Shift || key > Qt::Key_ScrollLock) {
                    QKeySequence keySequence(key + keyEvent->modifiers());
                    QString keySequenceString = keySequence.toString(QKeySequence::NativeText);
                    m_actions->value(index.parent().row()).second.value(index.row())->setShortcut(keySequence);
                    ui->shortcutsTreeWidget->topLevelItem(index.parent().row())->child(index.row())->setText(2, keySequenceString);
                    ui->keyLineEdit->setText(keySequenceString);
                    checkShortcuts();
                }
            }
            return true;
        } else
            return false;
    } else
        return QDialog::eventFilter(obj, event);
}
