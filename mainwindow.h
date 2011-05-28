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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>
#include <QFileInfoList>
#include <QImage>

#include "slideshow.h"

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QFileSystemModel;
QT_END_NAMESPACE

class ScanFolderThread;
class ImageListModel;
class ImageWidget;
class SlideshowListModel;
class SlideshowFileManager;

/*!
  \brief The UI namespace.
 */
namespace Ui {
    class MainWindow;
}

/*!
  \brief The MainWindow class defines the main application window.

  The MainWindow is used to display the main working areas of the application
  like the image and slideshow lists and an instance of the image widget to show
  a preview image of the currently selected image to the user.

  \sa OptionsDialog, SlideshowWindow
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    static const int MAX_THUMBNAIL_SIZE = 128; //!< The maximum size of the thumbnail icon.

    /*!
      \brief Constructs a new MainWindow with the given \a parent.

      The MainWindow constructor sets up the UI and connects all necessary
      signals for the user interaction. Also the persistent program settings
      will be loaded and the asynchronous slideshow loading started.

      \param parent The parent widget.
      \sa loadSettings(), on_actionReloadAllSlideshows_triggered()
     */
    explicit MainWindow(QWidget *parent = 0);

    /*!
      \brief Destroys the MainWindow and frees all UI resources.
     */
    ~MainWindow();

private slots:

    /*!
      \brief Starts scanning the selected folder for images.
      \param index The model index of the clicked folder item.
      \sa scanFolder()
     */
    void on_folderBrowserTreeView_clicked(const QModelIndex &index);

    /*!
      \brief Enables the button to abort the folder scanning and displayes a
             message to the user, which folder will be scanned for images.
      \sa scanFolderThread_folderScanned(), scanFolderThread_finished(),
          scanFolderAbortButton_clicked()
     */
    void scanFolderThread_started();

    /*!
      \brief Adds all found images to the image list and displayes a
             message to the user, which folder was scanned for images.
      \param folder The scanned folder.
      \param files The list of found image files.
      \sa scanFolderThread_started(), scanFolderThread_finished(),
          scanFolderAbortButton_clicked()
     */
    void scanFolderThread_folderScanned(const QString &folder, const QFileInfoList &files);

    /*!
      \brief Disables the button to abort the folder scanning and displayes a
             message to the user, how many folders has been scanned and how
             many images has been found.
      \sa scanFolderThread_folderScanned(), scanFolderThread_folderScanned(),
          scanFolderAbortButton_clicked()
     */
    void scanFolderThread_finished();

    /*!
      \brief Aborts the running folder scan.
      \sa scanFolderThread_started(), scanFolderThread_folderScanned(),
          scanFolderThread_finished()
     */
    void scanFolderAbortButton_clicked();

    /*!
      \brief Prepares the clicked image to be displayed.
      \param index The model index of the clicked image item.
      \sa prepareImage()
     */
    void on_imageListView_clicked(const QModelIndex &index);

    /*!
      \brief Adds the double clicked image to the current slideshow.
      \param index The model index of the double clicked image item.
     */
    void on_imageListView_doubleClicked(const QModelIndex &index);

    /*!
      \brief Shows the context menu of the image list view to the user.
      \param pos The position where the context menu should pop up.
     */
    void on_imageListView_customContextMenuRequested(const QPoint &pos);

    /*!
      \brief Shows the found and already loaded image count to the user.
     */
    void imageListModel_changed();

    /*!
      \brief Changes the currently active slideshow and prepares the image list.
      \param index The model index of the selected slideshow.
     */
    void on_slideshowListView_selectionChanged(const QModelIndex &index);

    /*!
      \brief Shows the context menu of the slideshow list view to the user.
      \param pos The position where the context menu should pop up.
     */
    void on_slideshowListView_customContextMenuRequested(const QPoint &pos);

    /*!
      \brief Renames the actual slideshow file on the disk.
      \param oldName The name of the slideshow before the rename.
      \param newName The name of the slideshow after the rename.
     */
    void slideshowListModel_slideshowRenamed(const QString &oldName, const QString &newName);

    void on_slideshowImageListView_clicked(const QModelIndex &index);

    /*!
      \brief Shows the context menu of the slideshow image list view to the user.
      \param pos The position where the context menu should pop up.
     */
    void on_slideshowImageListView_customContextMenuRequested(const QPoint &pos);

    /*!
      \brief Shows the found and already loaded image count to the user.
     */
    void slideshowImageListModel_changed();

    /*!
      \brief Updates the reference of the renamed image path inside the current slideshow.
      \param index The model index of the renamed slideshow image.
      \param newPath The path of the slideshow image after the rename.
     */
    void slideshowImageListModel_imageRenamed(const QModelIndex &index, const QString &newPath);

    /*!
      \brief Redirects the loaded image to the image widget.
      \param image The loaded image.
      \param width The width of the loaded image.
      \param height The height of the loaded image.
      \param index The model index of the loaded image.
     */
    void imageLoaded(const QImage &image, int width, int height, int index);

    /*!
      \brief Disables all save and load actions until all slideshows are loaded.
      \sa slideshowFileManager_finished()
     */
    void slideshowFileManager_started();

    /*!
      \brief Enables all save and load actions.
      \sa slideshowFileManager_started()
     */
    void slideshowFileManager_finished();

    /*!
      \brief Shows the zoom factor of the current image to the user.
     */
    void on_imageWidget_viewChanged();

    /*!
      \brief Adds the current image to the current slideshow.
     */
    void on_imageWidget_doubleClicked();

    /*!
      \brief Shows the context menu of the image widget to the user.
      \param pos The position where the context menu should pop up.
     */
    void on_imageWidget_customContextMenuRequested(const QPoint &pos);

    /*!
      \brief Creates a new Slideshow and sets it as current slideshow.
     */
    void on_actionNewSlideshow_triggered();

    /*!
      \brief Reloads all Slideshows.
      \sa SlideshowFileManager::loadAllSlideshows()
     */
    void on_actionReloadAllSlideshows_triggered();

    /*!
      \brief Saves all Slideshows.
      \sa SlideshowFileManager::saveAllSlideshows()
     */
    void on_actionSaveAllSlideshows_triggered();

    /*!
      \brief Toggles the visibility of the statusbar.
     */
    void on_actionStatusbar_triggered();

    /*!
      \brief Opens the options window as a modal dialog.
     */
    void on_actionOptions_triggered();

    /*!
      \brief Opens the slideshows directory with the native file browser.
     */
    void on_actionOpenSlideshowsDirectory_triggered();

    /*!
      \brief Opens the images directory with the native file browser.
     */
    void on_actionOpenImagesDirectory_triggered();

    /*!
      \brief Opens the user guide.
     */
    void on_actionQsmHelp_triggered();

    /*!
      \brief Shows the about box.
     */
    void on_actionAboutQsm_triggered();

    /*!
      \brief Adds the selected images to the current slideshow.
     */
    void on_actionAddToSlideshow_triggered();

    /*!
      \brief Opens a modal dialog to edit the comment of one or multiple image(s).
     */
    void on_actionImageEditComment_triggered();

    /*!
      \brief Activates the inline edit function if one image is selected, if
             multiple images are selected a modal rename dialog will be shown.
     */
    void on_actionRenameImage_triggered();

    /*!
      \brief Cuts the selected images to the internal clipboard.
     */
    void on_actionCutImage_triggered();

    /*!
      \brief Copies the selected images to the internal clipboard.
     */
    void on_actionCopyImage_triggered();

    /*!
      \brief Inserts all images from the internal clipboard.
      \note If the images has been copied with the \b cut action, the original
            images will be removed after the insertion.
     */
    void on_actionPasteImage_triggered();

    /*!
      \brief Removes the selected images from the slideshow.
     */
    void on_actionRemoveImage_triggered();

    /*!
      \brief Removes the selected images from the disk.
     */
    void on_actionRemoveImageFromDisk_triggered();

    /*!
      \brief Copies the selected image paths to the system clipboard.
     */
    void on_actionCopyPath_triggered();

    /*!
      \brief Opens the directory of the selected image or slideshow with the
             system file browser.
     */
    void on_actionOpenFileLocation_triggered();

    /*!
      \brief Exports the selected images to the predefined images directory.
     */
    void on_actionExportImages_triggered();

    /*!
      \brief Removes all corrupted images from the active slideshow.
      \sa Slideshow::removeAllCorruptedImages()
     */
    void on_actionRemoveAllCorruptedImages_triggered();

    /*!
      \brief Schedules the preload of all thumbnail icons at once.
      \sa ImageListModel::preloadAllImages()
     */
    void on_actionPreloadAllImages_triggered();

    /*!
      \brief Starts the active slideshow in full-screen mode.
      \sa SlideshowWindow
     */
    void on_actionStartSlideshow_triggered();

    /*!
      \brief Opens a modal dialog to edit the comment of the selected slideshow.
     */
    void on_actionSlideshowEditComment_triggered();

    /*!
      \brief Activates the inline rename function of the slected slideshow.
     */
    void on_actionRenameSlideshow_triggered();

    /*!
      \brief Removes the selected slideshow from disk.
     */
    void on_actionRemoveSlideshow_triggered();

    /*!
      \brief Opens a modal dialog to select an existing slideshow or enter a
             new one and then copies all images from the current slideshow to
             the selected/new one.
     */
    void on_actionCopyImagesToSlideshow_triggered();

    /*!
      \brief Exports all images from the selected slideshow to the predefined
             images directory.
     */
    void on_actionExportAllImages_triggered();

    /*!
      \brief Reloads the selected slideshow from disk.
     */
    void on_actionReloadSlideshow_triggered();

    /*!
      \brief Saves the selected slideshow to disk.
     */
    void on_actionSaveSlideshow_triggered();

    /*!
      \brief Reorders the image list according to the selected sort order.
      \param index The selected index of the sort combo box.
     */
    void on_imageListSortComboBox_currentIndexChanged(int index);

    /*!
      \brief Sets if the selected folder scan should also include all subfolders.
      \param checked \c True if the checkbox is checked; otherwise \c false.
     */
    void on_includeSubfoldersCheckBox_toggled(bool checked);

    /*!
      \brief Sets the thumbnail size.
      \param action The action which triggered the signal.
      \sa createThumbnailSizeMenu()
     */
    void thumbnailSizeMenu_triggered(QAction *action);

    /*!
      \brief Reorders the image list according to the selected sort order.
      \param index The selected index of the sort combo box.
      \sa Slideshow::sort()
     */
    void on_slideshowSortComboBox_currentIndexChanged(int index);

    /*!
      \brief Moves the selected images to the begin of the list.
      \sa moveImages()
     */
    void on_beginPushButton_clicked();

    /*!
      \brief Moves the selected images multiple positions up in the list.
      \sa moveImages(), multipleMoveCountDialog()
     */
    void on_multipleUpPushButton_clicked();

    /*!
      \brief Moves the selected images to the begin of the list.
      \sa moveImages()
     */
    void on_upPushButton_clicked();

    /*!
      \brief Moves the selected images one position down in the list.
      \sa moveImages()
     */
    void on_downPushButton_clicked();

    /*!
      \brief Moves the selected images multiple positions down in the list.
      \sa moveImages(), multipleMoveCountDialog()
     */
    void on_multipleDownPushButton_clicked();

    /*!
      \brief Moves the selected images to the end of the list.
      \sa moveImages()
     */
    void on_endPushButton_clicked();

    /*!
      \brief Shows a modal dialog to select the multiple move count.
      \sa moveImages(), on_multipleUpPushButton_clicked(),
          on_multipleDownPushButton_clicked()
     */
    void multipleMoveCountDialog();

private:

    /*!
      \brief This enum type defines the copy mode.
     */
    enum CopyMode {
        Copy, //!< The copy mode.
        Cut   //!< The cut mode.
    };

    Ui::MainWindow *ui;                           //!< The pointer to the UI.
    QLabel *m_scanFolderLabel;                    //!< The scan folder label to display the current folder being scanned.
    QPushButton *m_scanFolderAbortButton;         //!< The button to abort the folder scan.
    QFileSystemModel *m_folderBrowserModel;       //!< The file system model.
    ScanFolderThread *m_scanFolderThread;         //!< The thread to scan the folders in the background.
    ImageListModel *m_imageListModel;             //!< The image list model.
    SlideshowListModel *m_slideshowListModel;     //!< The slideshow list model.
    ImageListModel *m_slideshowImageListModel;    //!< The slideshow image list model.
    SlideshowFileManager *m_slideshowFileManager; //!< The slideshow file manager to load and save slideshows.

    int m_multipleMoveCount;                      //!< The amount of positions to move the image(s).
    QString m_slideshowsDirectory;                //!< The directory where the slideshows are stored.
    QString m_imagesDirectory;                    //!< The directory where the exported images are stored.
    QString m_currentImagePath;                   //!< The path of the current image.
    Slideshow *m_currentSlideshow;                //!< The associated slidehsow of the current image.
    SlideshowImage *m_currentSlideshowImage;      //!< The associated slidehsow image of the current image.

    CopyMode m_copyMode;                          //!< The copy mode of the last copy action.
    QStringList m_clipboard;                      //!< The path list of the copied images.

    /*!
      \brief Loads all user specific settings and shortcuts from the file system.
     */
    void loadSettings();

    /*!
      \brief Saves all user specific settings to the file system.
     */
    void saveSettings();

    /*!
      \brief Helper method to start the asynchronous folder scan.
      \param index The model index of the selected folder item.
      \param includeSubfolders \c True if all included subfolder should also
             be scanned; otherwise \c false.
      \sa ScanFolderThread
     */
    void scanFolder(const QModelIndex &index, bool includeSubfolders = false);

    /*!
      \brief Helper method to prepare the asynchronous image loading and set up
             a preview image from the thumbnail icon while loading the full
             image form disk.
      \param model The model index of the selected folder item.
      \param index The model index of the image.
     */
    void prepareImage(const ImageListModel *model, const QModelIndex &index);

    /*!
      \brief Helper method to determine the active widget for the given \a action.
      \param action The action which triggered the signal.
     */
    QWidget* activeWidget(QAction *action);

    /*!
      \brief Helper method to create a menu for selecting the thumbnail size.
      \param parent The parent widget.
      \param selectedSize The preselected thumbnail size in the menu.
     */
    QMenu* createThumbnailSizeMenu(QWidget *parent = 0, int selectedSize = 64);

    /*!
      \brief Helper method to get all image paths for the given \a action.
      \param action The action which triggered the signal.
     */
    QStringList imagePaths(QAction *action);

    /*!
      \brief Helper method to move images in a slideshow by \a delta positions.
      \param delta The amount of position to move the selected images.
      \sa Slideshow::moveImage(), ImageListModel::moveImage()
     */
    void moveImages(int delta);

    /*!
      \brief Changes the font size of the application.
      \param event The wheel event.
     */
    void wheelEvent(QWheelEvent *event);

    /*!
      \brief Saves all changed slideshows and user settings to the disk.
      \param event The close event.
      \sa on_actionSaveAllSlideshows_triggered(), saveSettings()
     */
    void closeEvent(QCloseEvent *event);

};

#endif // MAINWINDOW_H
