#include "optionsdialog.h"
#include "ui_optionsdialog.h"

#include <QColorDialog>
#include <QFileDialog>
#include <QKeyEvent>
#include <QMessageBox>

const QLocale::Language OptionsDialog::DEFAULT_LANGUAGE = QLocale::C;
const QColor OptionsDialog::DEFAULT_BACKGROUND_COLOR = Qt::black;
const QString OptionsDialog::DEFAULT_APPLICATION_DIRECTORY = QDir::homePath() + "/.qsm";
const QString OptionsDialog::DEFAULT_SLIDESHOWS_DIRECTORY = OptionsDialog::DEFAULT_APPLICATION_DIRECTORY + "/slideshows";
const QString OptionsDialog::DEFAULT_IMAGES_DIRECTORY = OptionsDialog::DEFAULT_APPLICATION_DIRECTORY + "/images";

QHash<QAction*, QKeySequence> OptionsDialog::DefaultShortcuts;

void OptionsDialog::loadAllShortcuts(const QSettings &settings)
{
    foreach (QAction *action, OptionsDialog::DefaultShortcuts.keys()) {
        action->setShortcut(settings.value("shortcuts/" + action->objectName(),
            OptionsDialog::DefaultShortcuts.value(action)).value<QKeySequence>());
    }
}

OptionsDialog::OptionsDialog(QList<QPair<QString, QList<QAction*> > > *actions, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OptionsDialog),
    m_actions(actions)
{
    Q_ASSERT(actions);
    ui->setupUi(this);

    loadSettings();

    // set shortcuts
    QList<QTreeWidgetItem*> items;
    static const QString windowContext(tr("Window Context"));
    static const QString widgetContext(tr("Widget Context"));
    for (int i = 0; i < actions->count(); ++i) {
        const QPair<QString, QList<QAction*> > actionGroup = actions->at(i);
        QTreeWidgetItem *itemGroup = new QTreeWidgetItem(ui->shortcutsTreeWidget, QStringList(actionGroup.first));
        QFont font = itemGroup->font(0);
        font.setBold(true);
        itemGroup->setFont(0, font);
        items.append(itemGroup);
        for (int j = 0; j < actionGroup.second.count(); ++j) {
            const QAction *action = actionGroup.second.at(j);
            QStringList itemText(action->text().replace('&', ""));
            itemText.append(action->toolTip());
            itemText.append(action->shortcut().toString(QKeySequence::NativeText));
            QTreeWidgetItem *item = new QTreeWidgetItem(itemGroup, itemText);
            item->setIcon(0, action->icon());
            item->setToolTip(2, action->shortcutContext() == Qt::WindowShortcut ? windowContext : widgetContext);
            items.append(item);
        }
    }
    ui->shortcutsTreeWidget->addTopLevelItems(items);
    ui->shortcutsTreeWidget->expandAll();

    checkShortcuts();
    ui->keyLineEdit->installEventFilter(this);
}

void OptionsDialog::loadSettings()
{
    QSettings settings;

    QLocale::Language language = (QLocale::Language)settings.value("language", (int)OptionsDialog::DEFAULT_LANGUAGE).toInt();
    switch (language) {
    case QLocale::English:
        ui->languageComboBox->setCurrentIndex(1);
        break;
    case QLocale::German:
        ui->languageComboBox->setCurrentIndex(2);
        break;
    default:
        ui->languageComboBox->setCurrentIndex(0);
    }

    QColor backgroundColor = settings.value("backgroundColor", OptionsDialog::DEFAULT_BACKGROUND_COLOR).value<QColor>();
    QPalette palette = ui->backgroundColorPushButton->palette();
    palette.setColor(QPalette::Button, backgroundColor);
    ui->backgroundColorPushButton->setPalette(palette);

    QString slideshowsDirectory = settings.value("slideshowsDirectory", OptionsDialog::DEFAULT_SLIDESHOWS_DIRECTORY).toString();
    ui->slideshowsDirectoryLineEdit->setText(QDir::toNativeSeparators(slideshowsDirectory));

    QString imagesDirectory = settings.value("imagesDirectory", OptionsDialog::DEFAULT_IMAGES_DIRECTORY).toString();
    ui->imagesDirectoryLineEdit->setText(QDir::toNativeSeparators(imagesDirectory));
}

void OptionsDialog::saveSettings()
{
    QSettings settings;

    QLocale::Language lang = language();
    if (lang != (QLocale::Language)settings.value("language", (int)OptionsDialog::DEFAULT_LANGUAGE).toInt()) {
        if (lang == OptionsDialog::DEFAULT_LANGUAGE)
            settings.remove("language");
        else
            settings.setValue("language", (int)lang);
        QMessageBox::information(this, tr("Restart required"),
            tr("The language change will take effect after a restart of Qt SlideShow Manager."));
    }
    settings.setValue("backgroundColor", backgroundColor());
    settings.setValue("slideshowsDirectory", slideshowsDirectory());
    settings.setValue("imagesDirectory", imagesDirectory());

    settings.beginGroup("shortcuts");
    for (int i = 0; i < m_actions->count(); ++i) {
        const QTreeWidgetItem *shortcutGroup = ui->shortcutsTreeWidget->topLevelItem(i);
        QPair<QString, QList<QAction*> > actionGroup = m_actions->at(i);
        for (int j = 0; j < actionGroup.second.count(); ++j) {
            const QTreeWidgetItem *shortcutItem = shortcutGroup->child(j);
            QAction *action = actionGroup.second.at(j);
            action->setShortcut(shortcutItem->text(2));
            settings.setValue(action->objectName(), action->shortcut());
        }
    }
    settings.endGroup();
}

OptionsDialog::~OptionsDialog()
{
    delete ui;
}

void OptionsDialog::on_backgroundColorPushButton_clicked()
{
    QPalette palette = ui->backgroundColorPushButton->palette();
    QColor color = QColorDialog::getColor(palette.button().color(), this, tr("Select Color"));
    if (color.isValid()) {
        palette.setColor(QPalette::Button, color);
        ui->backgroundColorPushButton->setPalette(palette);
    }
}

void OptionsDialog::on_backgroundColorResetPushButton_clicked()
{
    QPalette palette = ui->backgroundColorPushButton->palette();
    palette.setColor(QPalette::Button, OptionsDialog::DEFAULT_BACKGROUND_COLOR);
    ui->backgroundColorPushButton->setPalette(palette);
}

void OptionsDialog::chooseDirectory(QLineEdit *lineEdit, const QString &defaultDirectory)
{
    Q_ASSERT(lineEdit);
    QDir currentDir(lineEdit->text());
    if (!currentDir.exists())
        currentDir = defaultDirectory;
    QString dir = QFileDialog::getExistingDirectory(this, tr("Choose Directory"), currentDir.path());
    if (!dir.isEmpty())
        lineEdit->setText(dir);
}

void OptionsDialog::on_slideshowsDirectoryPushButton_clicked()
{
    chooseDirectory(ui->slideshowsDirectoryLineEdit, OptionsDialog::DEFAULT_SLIDESHOWS_DIRECTORY);
}

void OptionsDialog::on_imagesDirectoryPushButton_clicked()
{
    chooseDirectory(ui->imagesDirectoryLineEdit, OptionsDialog::DEFAULT_IMAGES_DIRECTORY);
}

void OptionsDialog::on_shortcutsTreeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *)
{
    ui->shortcutGroupBox->setEnabled(current->parent());
    ui->keyLineEdit->setText(current->text(2));
}

void OptionsDialog::on_resetAllPushButton_clicked()
{
    for (int i = 0; i < ui->shortcutsTreeWidget->topLevelItemCount(); ++i) {
        QTreeWidgetItem *item = ui->shortcutsTreeWidget->topLevelItem(i);
        for (int j = 0; j < item->childCount(); ++j) {
            QAction *action = m_actions->at(i).second.at(j);
            QString keySequence = OptionsDialog::DefaultShortcuts.value(action).toString(QKeySequence::NativeText);
            item->child(j)->setText(2, keySequence);
        }
    }
    QTreeWidgetItem *item = ui->shortcutsTreeWidget->currentItem();
    if (item && item->parent())
        ui->keyLineEdit->setText(item->text(2));
    checkShortcuts();
}

void OptionsDialog::on_resetPushButton_clicked()
{
    QTreeWidgetItem *item = ui->shortcutsTreeWidget->currentItem();
    if (item && item->parent()) {
        QModelIndex index = ui->shortcutsTreeWidget->currentIndex();
        QAction *action = m_actions->at(index.parent().row()).second.at(index.row());
        QString keySequence = OptionsDialog::DefaultShortcuts.value(action).toString(QKeySequence::NativeText);
        item->setText(2, keySequence);
        ui->keyLineEdit->setText(keySequence);
        checkShortcuts();
    }
}

void OptionsDialog::on_removePushButton_clicked()
{
    QTreeWidgetItem *item = ui->shortcutsTreeWidget->currentItem();
    if (item && item->parent()) {
        item->setText(2, "");
        ui->keyLineEdit->clear();
        checkShortcuts();
    }
}

bool OptionsDialog::checkShortcuts()
{
    bool multipleUsage = false;
    for (int i = 0; i < ui->shortcutsTreeWidget->topLevelItemCount(); ++i) {
        QTreeWidgetItem *shortcutGroup = ui->shortcutsTreeWidget->topLevelItem(i);
        for (int j = 0; j < shortcutGroup->childCount(); ++j) {
            QTreeWidgetItem *shortcutItem = shortcutGroup->child(j);
            shortcutItem->setForeground(2, Qt::black); // reset color
            for (int k = 0; k < ui->shortcutsTreeWidget->topLevelItemCount(); ++k) {
                QTreeWidgetItem *shortcutGroup2 = ui->shortcutsTreeWidget->topLevelItem(k);
                for (int l = 0; l < shortcutGroup2->childCount(); ++l) {
                    QTreeWidgetItem *shortcutItem2 = shortcutGroup2->child(l);
                    if (shortcutItem != shortcutItem2 && shortcutItem->text(2) == shortcutItem2->text(2)) {
                        shortcutItem->setForeground(2, Qt::red);
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

QLocale::Language OptionsDialog::language() const
{
    QLocale::Language language = OptionsDialog::DEFAULT_LANGUAGE;
    switch (ui->languageComboBox->currentIndex()) {
    case 1:
        language = QLocale::English;
        break;
    case 2:
        language = QLocale::German;
        break;
    }
    return language;
}

QColor OptionsDialog::backgroundColor() const
{
    return ui->backgroundColorPushButton->palette().button().color();
}

QString OptionsDialog::slideshowsDirectory() const
{
    return ui->slideshowsDirectoryLineEdit->text();
}

QString OptionsDialog::imagesDirectory() const
{
    return ui->imagesDirectoryLineEdit->text();
}

void OptionsDialog::on_buttonBox_accepted()
{
    // TODO: check for valid slideshows and images directories
    saveSettings();
    accept();
}
