#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>
#include <QFileInfoList>
#include <QImage>

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QFileSystemModel;
QT_END_NAMESPACE

class ScanFolderThread;
class ImageListModel;
class ImageWidget;
class SlideshowListModel;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
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
    void imageLoaded(const QImage &image, int, int, int);
    void scanFolderCancelButton_clicked();

    void on_actionNewSlideshow_triggered();
    void on_actionStatusbar_triggered();
    void on_actionAboutQsm_triggered();

    void actionRemoveSlideshow_triggered();
    void actionRemoveSlideshowImage_triggered();

    void on_includeSubfoldersCheckBox_toggled(bool checked);

private:
    Ui::MainWindow *ui;
    QLabel *m_scanFolderLabel;
    QPushButton *m_scanFolderAbortButton;
    QFileSystemModel *m_folderBrowserModel;
    ScanFolderThread *m_scanFolderThread;
    ImageListModel *m_imageListModel;
    SlideshowListModel *m_slideshowListModel;
    QAction *m_actionRemoveSlideshow;
    ImageListModel *m_slideshowImageListModel;
    QAction *m_actionRemoveSlideshowImage;
    ImageWidget *m_imageWidget;

    QString m_currentImagePath;

    void scanFolder(const QModelIndex &index, bool includeSubfolders = false);
    void prepareImage(const ImageListModel *model, const QModelIndex &index);
};

#endif // MAINWINDOW_H
