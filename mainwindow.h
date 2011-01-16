#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>

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
    QFileSystemModel *folderBrowserModel;

private slots:
    void on_actionStatusbar_triggered();
    void on_actionAboutQsm_triggered();

    void folderBrowserTreeView_activated(const QModelIndex &index);
};

#endif // MAINWINDOW_H
