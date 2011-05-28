/****************************************************************************
**
** Copyright (C) 2011 Alexander Vos <alexander.vos@stud.fh-swf.de>,
**                    Till Althaus <till.althaus@stud.fh-swf.de>
**
** This file is part of Qt Slideshow Manager (QSM).
**
** QSM is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** QSM is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with QSM.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>
#include <QSettings>
#include <QLocale>
#include <QColorDialog>

QT_BEGIN_NAMESPACE
class QTreeWidgetItem;
class QLineEdit;
QT_END_NAMESPACE

/*!
  \brief The UI namespace.
 */
namespace Ui {
    class OptionsDialog;
}

/*!
  \brief The OptionsDialog class defines the preferences window for user customization.
  \sa MainWindow
 */
class OptionsDialog : public QDialog
{
    Q_OBJECT

public:

    static const QLocale::Language DEFAULT_LANGUAGE;    //!< The default language for the user interface.
    static const QColor DEFAULT_BACKGROUND_COLOR;       //!< The default background color of the image widget.
    static const QColor DEFAULT_TEXT_BACKGROUND_COLOR;  //!< The default text background color of the image widget.
    static const QFont DEFAULT_TEXT_FONT;               //!< The default text font of the image widget.
    static const QColor DEFAULT_TEXT_COLOR;             //!< The default text color of the image widget.
    static const QString DEFAULT_APPLICATION_DIRECTORY; //!< The default application directory to store slideshows and images.
    static const QString DEFAULT_SLIDESHOWS_DIRECTORY;  //!< The default slideshows directory to store all slideshows.
    static const QString DEFAULT_IMAGES_DIRECTORY;      //!< The default images directory to store all exported images.

    static QHash<QAction*, QKeySequence> DefaultShortcuts; //!< Stores all default shortcuts.

    /*!
      \brief Loads all shortcuts from the user's settings file.
      \param settings The settings configuration to load the shortcuts from.
     */
    static void loadAllShortcuts(const QSettings &settings);

    /*!
      \brief Checks if a directory exists or creates it if \a create is set to \c true.
      \param directory The directory to check.
      \param create \c True if the directory should be ceated if it does not exists yet; otherwise \c false.
      \param warningOnFailure Shows a warning message to the user if the creation of the directory fails.
     */
    static bool checkDirectory(const QString &directory, bool create = true, bool warningOnFailure = true);

    /*!
      \brief Constructs a new OptionsDialog with the given \a parent.

      The OptionsDialog constructor sets up the UI and connects all necessary
      signals for the user interaction. Also the persistent program settings
      will be loaded and shortcuts will be prepared.

      \param actions The list of action grouped by topics.
      \param parent The parent widget.
      \sa loadSettings()
     */
    explicit OptionsDialog(QList<QPair<QString, QList<QAction*> > > *actions,
                           QWidget *parent = 0);

    /*!
      \brief Destroys the OptionsDialog and frees all UI resources.
     */
    ~OptionsDialog();

    /*!
      \brief Returns the selected language.
      \return The selected language.
     */
    QLocale::Language language() const;

    /*!
      \brief Returns the selected background color for the image widget.
      \return The selected background color.
     */
    QColor backgroundColor() const;

    /*!
      \brief Returns the selected text background color for the image widget.
      \return The selected text background color.
     */
    QColor textBackgroundColor() const;

    /*!
      \brief Returns the selected text font.
      \return The selected text font for the image widget.
     */
    QFont textFont() const;

    /*!
      \brief Returns the selected text color for the image widget.
      \return The selected text color.
     */
    QColor textColor() const;

    /*!
      \brief Returns the selected slideshows directory to store all slideshows.
      \return The selected slideshows directory.
     */
    QString slideshowsDirectory() const;

    /*!
      \brief Returns the selected images directory to stored all exported images.
      \return The selected images directory.
     */
    QString imagesDirectory() const;

private slots:

    /*!
      \brief Opens a modal dialog to select the background color for the image widget.
      \sa selectColor(), on_backgroundColorResetPushButton_clicked()
     */
    void on_backgroundColorPushButton_clicked();

    /*!
      \brief Resets the background color for the image widget to the default value.
      \sa on_backgroundColorPushButton_clicked()
     */
    void on_backgroundColorResetPushButton_clicked();

    /*!
      \brief Opens a modal dialog to select the text background color for the image widget.
      \sa selectColor(), on_textBackgroundColorResetPushButton_clicked()
     */
    void on_textBackgroundColorPushButton_clicked();

    /*!
      \brief Resets the text background color for the image widget to the default value.
      \sa on_textBackgroundColorPushButton_clicked()
     */
    void on_textBackgroundColorResetPushButton_clicked();

    /*!
      \brief Opens a modal dialog to select the text font for the image widget.
      \sa on_textFontResetPushButton_clicked()
     */
    void on_textFontPushButton_clicked();

    /*!
      \brief Resets the text font for the image widget to the default value.
      \sa on_textFontPushButton_clicked()
     */
    void on_textFontResetPushButton_clicked();

    /*!
      \brief Opens a modal dialog to select the text color for the image widget.
      \sa selectColor(), on_textColorResetPushButton_clicked()
     */
    void on_textColorPushButton_clicked();

    /*!
      \brief Resets the text text color for the image widget to the default value.
      \sa on_textColorPushButton_clicked()
     */
    void on_textColorResetPushButton_clicked();

    /*!
      \brief Opens the slideshows drectory with the system file explorer.
      \sa on_slideshowsDirectoryPushButton_clicked()
     */
    void on_openSlideshowsDirectoryPushButton_clicked();

    /*!
      \brief Opens a modal dialog to select the slideshows drectory.
      \sa chooseDirectory(), on_openSlideshowsDirectoryPushButton_clicked()
     */
    void on_slideshowsDirectoryPushButton_clicked();

    /*!
      \brief Opens the images drectory with the system file explorer.
      \sa on_imagesDirectoryPushButton_clicked()
     */
    void on_openImagesDirectoryPushButton_clicked();

    /*!
      \brief Opens a modal dialog to select the images drectory.
      \sa chooseDirectory(), on_openImagesDirectoryPushButton_clicked()
     */
    void on_imagesDirectoryPushButton_clicked();

    /*!
      \brief Checks if the entered text is a valid directory and if not changes
             the text color to red.
      \param text The changed text.
     */
    void directoryLineEdit_textChanged(const QString &text);

    /*!
      \brief Shows the selected shortcut at the bottom line edit.
      \param current The current selected item.
      \param previous The previous selected item.
     */
    void on_shortcutsTreeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

    /*!
      \brief Resets all shortcuts to their default values.
      \sa on_resetPushButton_clicked(), on_removePushButton_clicked()
     */
    void on_resetAllPushButton_clicked();

    /*!
      \brief Resets the selected shortcut to the default value.
      \sa on_resetAllPushButton_clicked(), on_removePushButton_clicked()
     */
    void on_resetPushButton_clicked();

    /*!
      \brief Removes the shortcut from the selected item.
      \sa on_resetPushButton_clicked()
     */
    void on_removePushButton_clicked();

    /*!
      \brief Closes this options dialog if the slideshows and images directories
             are valid and saves all changed settings to the disk.
      \sa OptionsDialog::checkDirectory(), saveSettings()
     */
    void on_buttonBox_accepted();

private:

    Ui::OptionsDialog *ui;                               //!< The pointer to the UI.
    QList<QPair<QString, QList<QAction*> > > *m_actions; //!< The list of action grouped by topics.

    /*!
      \brief Helper method to load all user settings form the file system.
     */
    void loadSettings();

    /*!
      \brief Helper method to save all user settings to the file system.
     */
    void saveSettings();

    /*!
      \brief Helper method to select a color.

      This method will open a modal dialog to select a color.

      \param pushButton The button which displays the selected color.
      \param options The options for the color dialog.
     */
    void selectColor(QPushButton *pushButton, QColorDialog::ColorDialogOptions options = 0);

    /*!
      \brief Helper method to choose a directory.

      This method will open a modal dialog to choose a directory.

      \param lineEdit The lines edit which displays the choosen directory.
      \param defaultDirectory The default directory if the text from the
             \a lineEdit contains no valid path.
     */
    void chooseDirectory(QLineEdit *lineEdit, const QString &defaultDirectory);

    /*!
      \brief Check for multiple usage of the same shortcuts and displays them in red.
     */
    bool checkShortcuts();

    /*!
      \brief Filters all key events from the line edit out to set a new shortcut.
      \param obj The object which triggered the event.
      \param event The event.
     */
    bool eventFilter(QObject *obj, QEvent *event);

};

#endif // OPTIONSDIALOG_H
