#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "scanfolderthread.h"

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

    m_folderBrowserModel = new QFileSystemModel;
    m_folderBrowserModel->setFilter(QDir::Drives | QDir::Dirs | QDir::NoDotAndDotDot);
    m_folderBrowserModel->setRootPath(m_folderBrowserModel->myComputer().toString());

    ui->folderBrowserTreeView->setModel(m_folderBrowserModel);
    ui->folderBrowserTreeView->expand(m_folderBrowserModel->index(QDir::rootPath()));
    ui->folderBrowserTreeView->setHeaderHidden(true);
    ui->folderBrowserTreeView->hideColumn(1);
    ui->folderBrowserTreeView->hideColumn(2);
    ui->folderBrowserTreeView->hideColumn(3);
    connect(ui->folderBrowserTreeView, SIGNAL(clicked(QModelIndex)), SLOT(folderBrowserTreeView_clicked(QModelIndex)));

    m_scanFolderThread = new ScanFolderThread(this);
    connect(m_scanFolderThread, SIGNAL(started()), SLOT(scanFolderThread_started()));
    connect(m_scanFolderThread, SIGNAL(folderScanned(QString, QFileInfoList)), SLOT(scanFolderThread_folderScanned(QString, QFileInfoList)));
    connect(m_scanFolderThread, SIGNAL(finished()), SLOT(scanFolderThread_finished()));

    statusBar()->showMessage(tr("Ready"));
}

MainWindow::~MainWindow()
{
    delete m_scanFolderThread;
    delete m_folderBrowserModel;
    delete ui;
}

void MainWindow::folderBrowserTreeView_clicked(const QModelIndex &index)
{
    if (m_scanFolderThread->isRunning()) {
        m_scanFolderThread->terminate();
        m_scanFolderThread->wait();
    }
    m_scanFolderThread->setFolder(m_folderBrowserModel->filePath(index), ui->includeSubfoldersCheckBox->isChecked());
    m_scanFolderThread->start(QThread::NormalPriority);
}

void MainWindow::scanFolderThread_started()
{
    qDebug("scanFolderThread_started");
}

void MainWindow::scanFolderThread_folderScanned(const QString folder, const QFileInfoList &files)
{
    qDebug() << "folderScanned: " << folder << ", file count = " << files.size();
}

void MainWindow::scanFolderThread_finished()
{
    qDebug("scanFolderThread_finished");
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
