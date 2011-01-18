#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>
#include <QFileInfoList>

class QFileSystemModel;
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

private:
    Ui::MainWindow *ui;
    QFileSystemModel *m_folderBrowserModel;
    ScanFolderThread *m_scanFolderThread;
    ImageListModel *m_imageListModel;
    ImageWidget *m_imageWidget;
    SlideshowListModel *m_slideshowListModel;

    void loadImage(const QString &path);

private slots:
    void folderBrowserTreeView_clicked(const QModelIndex &index);
    void scanFolderThread_started();
    void scanFolderThread_folderScanned(const QString &folder, const QFileInfoList &files);
    void scanFolderThread_finished();
    void on_imageListListView_clicked(QModelIndex index);
    void imageListListView_customContextMenuRequested(const QPoint &pos);

    void on_actionStatusbar_triggered();
    void on_actionAboutQsm_triggered();

};

#endif // MAINWINDOW_H
