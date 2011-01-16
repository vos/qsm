#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QFileSystemModel>

#include <QDebug>
#include <QElapsedTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->menuView->addAction(ui->mainToolBar->toggleViewAction());
    ui->menuView->addAction(ui->actionStatusbar);
    QMenu *menuSidebar = new QMenu(ui->menuBar);
    menuSidebar->setTitle(tr("&Sidebar"));
    menuSidebar->addAction(ui->folderBrowserDockWidget->toggleViewAction());
    menuSidebar->addAction(ui->imageListDockWidget->toggleViewAction());
    menuSidebar->addAction(ui->slideshowListDockWidget->toggleViewAction());
    menuSidebar->addAction(ui->slideshowImageListDockWidget->toggleViewAction());
    ui->menuView->addMenu(menuSidebar);
    connect(ui->actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    folderBrowserModel = new QFileSystemModel;
    folderBrowserModel->setFilter(QDir::Drives | QDir::Dirs | QDir::NoDotAndDotDot);
    folderBrowserModel->setRootPath(folderBrowserModel->myComputer().toString());

    ui->folderBrowserTreeView->setModel(folderBrowserModel);
    ui->folderBrowserTreeView->expand(folderBrowserModel->index(QDir::rootPath()));
    ui->folderBrowserTreeView->setHeaderHidden(true);
    ui->folderBrowserTreeView->hideColumn(1);
    ui->folderBrowserTreeView->hideColumn(2);
    ui->folderBrowserTreeView->hideColumn(3);

    connect(ui->folderBrowserTreeView, SIGNAL(activated(QModelIndex)), SLOT(folderBrowserTreeView_activated(QModelIndex)));

    statusBar()->showMessage(tr("Ready"));
}

MainWindow::~MainWindow()
{
    delete folderBrowserModel;
    delete ui;
}

void MainWindow::folderBrowserTreeView_activated(const QModelIndex &index)
{
    // TODO separate thread for folder scanning
    QStringList filters;
    filters << "*.jpg" << "*.jpeg" << "*.png" << "*.bmp" << "*.tiff" << "*.gif" << "*.ico";
    QFileInfoList fileList = QDir(folderBrowserModel->filePath(index)).entryInfoList(filters, QDir::Files | QDir::Readable, QDir::Name);

    qDebug() << fileList.size();
}

void MainWindow::on_actionStatusbar_triggered()
{
    ui->statusBar->setVisible(ui->actionStatusbar->isChecked());
}

void MainWindow::on_actionAboutQsm_triggered()
{
    QMessageBox::about(this, tr("About QSM"),
            tr("<b>Qt SlideShow Manager</b> v0.1 alpha<br/><br/>"
               "Copyright © 2011 Fachhochschule Südwestfalen<br/>"
               "Written by Alexander Vos and Till Althaus"));
}
