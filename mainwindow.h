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

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    static const int MAX_THUMBNAIL_SIZE = 128;

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_folderBrowserTreeView_clicked(const QModelIndex &index);
    void scanFolderThread_started();
    void scanFolderThread_folderScanned(const QString &folder, const QFileInfoList &files);
    void scanFolderThread_finished();
    void on_imageListView_clicked(const QModelIndex &index);
    void on_imageListView_doubleClicked(const QModelIndex &index);
    void on_imageListView_customContextMenuRequested(const QPoint &pos);
    void imageListModel_changed();
    void on_slideshowListView_selectionChanged(const QModelIndex &index);
    void on_slideshowListView_customContextMenuRequested(const QPoint &pos);
    void on_slideshowImageListView_clicked(const QModelIndex &index);
    void on_slideshowImageListView_customContextMenuRequested(const QPoint &pos);
    void slideshowImageListModel_changed();
    void imageLoaded(const QImage &image, int width, int height, int index);
    void slideshowFileManager_started();
    void slideshowFileManager_finished();
    void on_imageWidget_viewChanged();
    void on_imageWidget_doubleClicked();
    void on_imageWidget_customContextMenuRequested(const QPoint &pos);
    void scanFolderAbortButton_clicked();

    void on_actionNewSlideshow_triggered();
    void on_actionReloadAllSlideshows_triggered();
    void on_actionSaveAllSlideshows_triggered();
    void on_actionStatusbar_triggered();
    void on_actionOptions_triggered();
    void on_actionQsmHelp_triggered();
    void on_actionAboutQsm_triggered();

    void on_actionAddToSlideshow_triggered();
    void on_actionImageEditComment_triggered();
    void on_actionRenameImage_triggered();
    void on_actionCutImage_triggered();
    void on_actionCopyImage_triggered();
    void on_actionPasteImage_triggered();
    void on_actionRemoveImage_triggered();
    void on_actionRemoveImageFromDisk_triggered();
    void on_actionCopyPath_triggered();
    void on_actionPreloadAllImages_triggered();

    void on_actionStartSlideshow_triggered();
    void on_actionSlideshowEditComment_triggered();
    void on_actionRenameSlideshow_triggered();
    void on_actionRemoveSlideshow_triggered();
    void on_actionCopyImagesToSlideshow_triggered();
    void on_actionReloadSlideshow_triggered();
    void on_actionSaveSlideshow_triggered();

    void on_includeSubfoldersCheckBox_toggled(bool checked);
    void thumbnailSizeMenu_triggered(QAction *action);

private:
    Ui::MainWindow *ui;
    QLabel *m_scanFolderLabel;
    QPushButton *m_scanFolderAbortButton;
    QFileSystemModel *m_folderBrowserModel;
    ScanFolderThread *m_scanFolderThread;
    ImageListModel *m_imageListModel;
    SlideshowListModel *m_slideshowListModel;
    ImageListModel *m_slideshowImageListModel;
    SlideshowFileManager *m_slideshowFileManager;

    QString m_slideshowsDirectory;
    QString m_imagesDirectory;
    QString m_currentImagePath;
    Slideshow *m_currentSlideshow;
    SlideshowImage *m_currentSlideshowImage;

    void loadSettings();
    void saveSettings();

    void scanFolder(const QModelIndex &index, bool includeSubfolders = false);
    void prepareImage(const ImageListModel *model, const QModelIndex &index);

    QWidget* activeWidget(QAction *action);
    QMenu* createThumbnailSizeMenu(QWidget *parent = 0, int selectedSize = 64);
    QStringList imagePaths(QAction *action);

    void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
