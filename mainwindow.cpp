#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileSystemModel>

#include <QDebug>
#include <QElapsedTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    folderBrowserModel = new QFileSystemModel;
    folderBrowserModel->setFilter(QDir::Drives | QDir::Dirs | QDir::NoDotAndDotDot);
    folderBrowserModel->setRootPath(folderBrowserModel->myComputer().toString());

    ui->folderBrowserTreeView->setModel(folderBrowserModel);
    ui->folderBrowserTreeView->expand(folderBrowserModel->index(QDir::rootPath()));
    ui->folderBrowserTreeView->setHeaderHidden(true);
    ui->folderBrowserTreeView->hideColumn(1);
    ui->folderBrowserTreeView->hideColumn(2);
    ui->folderBrowserTreeView->hideColumn(3);

    connect(ui->folderBrowserTreeView, SIGNAL(clicked(QModelIndex)), SLOT(folderBrowserTreeView_clicked(QModelIndex)));
}

MainWindow::~MainWindow()
{
    delete folderBrowserModel;
    delete ui;
}

void MainWindow::folderBrowserTreeView_clicked(QModelIndex index)
{
    // TODO separate thread for folder scanning
    QStringList filters;
    filters << "*.jpg" << "*.jpeg" << "*.png" << "*.bmp" << "*.tiff" << "*.gif" << "*.ico";
    QFileInfoList fileList = QDir(folderBrowserModel->filePath(index)).entryInfoList(filters, QDir::Files | QDir::Readable, QDir::Name);

    qDebug() << fileList.size();
}
