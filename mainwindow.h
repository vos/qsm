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
class Slideshow;

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
    void on_imageListListView_clicked(const QModelIndex &index);
    void on_imageListListView_doubleClicked(const QModelIndex &index);
    void imageListModel_changed();
    void on_slideshowListListView_activated(const QModelIndex &index);
    void on_slideshowImageListListView_clicked(const QModelIndex &index);
    void slideshowImageListModel_changed();
    void imageLoaded(const QImage &image, int, int, int);
    void on_imageListListView_customContextMenuRequested(const QPoint &pos);
    void scanFolderCancelButton_clicked();

    void on_actionNewSlideshow_triggered();
    void on_actionStatusbar_triggered();
    void on_actionAboutQsm_triggered();

private:
    Ui::MainWindow *ui;
    QLabel *m_scanFolderLabel;
    QPushButton *m_scanFolderAbortButton;
    QFileSystemModel *m_folderBrowserModel;
    ScanFolderThread *m_scanFolderThread;
    ImageListModel *m_imageListModel;
    SlideshowListModel *m_slideshowListModel;
    ImageListModel *m_slideshowImageListModel;
    ImageWidget *m_imageWidget;

    QString m_currentImagePath;
    Slideshow *m_currentSlideshow;

    void prepareImage(const ImageListModel *model, const QModelIndex &index);
};

#endif // MAINWINDOW_H
