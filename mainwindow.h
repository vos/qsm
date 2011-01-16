#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>
#include <QFileInfoList>

class ScanFolderThread;
class QFileSystemModel;

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

private slots:
    void folderBrowserTreeView_clicked(const QModelIndex &index);
    void scanFolderThread_started();
    void scanFolderThread_folderScanned(const QString folder, const QFileInfoList &files);
    void scanFolderThread_finished();

    void on_actionStatusbar_triggered();
    void on_actionAboutQsm_triggered();

};

#endif // MAINWINDOW_H
