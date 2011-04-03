#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSettings>
#include <QLabel>
#include <QMessageBox>
#include <QFileSystemModel>
#include <QThreadPool>

#include <QDebug>

#include "scanfolderthread.h"
#include "imagelistmodel.h"
#include "imagewidget.h"
#include "slideshowlistmodel.h"

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

    m_folderBrowserModel = new QFileSystemModel(this);
    m_folderBrowserModel->setFilter(QDir::Drives | QDir::Dirs | QDir::NoDotAndDotDot);
    m_folderBrowserModel->setRootPath(m_folderBrowserModel->myComputer().toString());

    ui->folderBrowserTreeView->setModel(m_folderBrowserModel);
    ui->folderBrowserTreeView->expand(m_folderBrowserModel->index(QDir::rootPath()));
    ui->folderBrowserTreeView->setHeaderHidden(true);
    ui->folderBrowserTreeView->hideColumn(1);
    ui->folderBrowserTreeView->hideColumn(2);
    ui->folderBrowserTreeView->hideColumn(3);

    m_scanFolderThread = new ScanFolderThread(this);
    connect(m_scanFolderThread, SIGNAL(started()), SLOT(scanFolderThread_started()));
    connect(m_scanFolderThread, SIGNAL(folderScanned(QString, QFileInfoList)), SLOT(scanFolderThread_folderScanned(QString, QFileInfoList)));
    connect(m_scanFolderThread, SIGNAL(finished()), SLOT(scanFolderThread_finished()));

    m_imageListModel = new ImageListModel(this);
    connect(m_imageListModel, SIGNAL(changed()), SLOT(imageListModel_changed()));
    ui->imageListListView->setModel(m_imageListModel);
    ui->imageListListView->setIconSize(QSize(64, 64));

    m_slideshowListModel = new SlideshowListModel(this);
    ui->slideshowListListView->setModel(m_slideshowListModel);
    m_slideshowImageListModel = new ImageListModel(this);
    connect(m_slideshowImageListModel, SIGNAL(changed()), SLOT(slideshowImageListModel_changed()));
    ui->slideshowImageListListView->setModel(m_slideshowImageListModel);

    m_imageWidget = new ImageWidget(this);
    setCentralWidget(m_imageWidget);

    m_scanFolderLabel = new QLabel;
    m_scanFolderLabel->setVisible(false);
    statusBar()->addPermanentWidget(m_scanFolderLabel);
    m_scanFolderAbortButton = new QPushButton(tr("Abort"));
    m_scanFolderAbortButton->setVisible(false);
    connect(m_scanFolderAbortButton, SIGNAL(clicked()), SLOT(scanFolderCancelButton_clicked()));
    statusBar()->addPermanentWidget(m_scanFolderAbortButton);
    statusBar()->showMessage(tr("Ready"));
}

MainWindow::~MainWindow()
{
    delete m_imageWidget;
    delete m_slideshowImageListModel;
    delete m_slideshowListModel;
    delete m_imageListModel;
    delete m_scanFolderThread;
    delete m_folderBrowserModel;
    delete m_scanFolderAbortButton;
    delete m_scanFolderLabel;
    delete ui;
}

void MainWindow::on_folderBrowserTreeView_clicked(const QModelIndex &index)
{
    if (m_scanFolderThread->isRunning()) {
        scanFolderCancelButton_clicked();
        m_scanFolderThread->wait();
    }
    m_scanFolderThread->setFolder(m_folderBrowserModel->filePath(index), ui->includeSubfoldersCheckBox->isChecked());
    m_scanFolderThread->start(QThread::NormalPriority);
}

void MainWindow::scanFolderThread_started()
{
    qDebug("scanFolderThread_started");
    m_imageListModel->clear();
    statusBar()->showMessage(tr("Scanning folder %1 ...").arg(m_scanFolderThread->getFolder()));
    m_scanFolderLabel->clear();
    m_scanFolderLabel->setVisible(true);
    m_scanFolderAbortButton->setText(tr("Abort"));
    m_scanFolderAbortButton->setEnabled(true);
    m_scanFolderAbortButton->setVisible(true);
}

void MainWindow::scanFolderThread_folderScanned(const QString &folder, const QFileInfoList &files)
{
    qDebug() << "folderScanned: " << folder << ", images found = " << files.size();
    m_scanFolderLabel->setText(tr("Scanning folder %1 ...").arg(folder));
    m_imageListModel->addImageFileInfoList(files);
}

void MainWindow::scanFolderThread_finished()
{
    qDebug("scanFolderThread_finished: %d images found", m_imageListModel->imageCount());
    statusBar()->showMessage(tr("%1 images found, %2 folders scanned").arg(m_imageListModel->imageCount()).arg(m_scanFolderThread->count()));
    m_scanFolderLabel->setVisible(false);
    m_scanFolderAbortButton->setVisible(false);
}

void MainWindow::on_imageListListView_clicked(const QModelIndex &index)
{
    QVariant var = m_imageListModel->data(index, Qt::DecorationRole);
    if (var.canConvert<QIcon>()) {
        // set up preview image
        QIcon icon = var.value<QIcon>();
        m_imageWidget->setImage(icon.pixmap(64, 64).toImage());
    }

    // load the actual image inside a separate thread
    QString imagePath = m_imageListModel->imagePath(index);
    ImageLoader *imageLoader = new ImageLoader(imagePath);
    connect(imageLoader, SIGNAL(imageLoaded(QImage, int, int, int)), SLOT(imageLoaded(QImage, int, int, int)));
    QThreadPool::globalInstance()->start(imageLoader);
}

void MainWindow::imageListModel_changed()
{
    ui->imageListDockWidget->setWindowTitle(tr("Image List %1/%2").arg(m_imageListModel->rowCount()).arg(m_imageListModel->imageCount()));
}

void MainWindow::slideshowImageListModel_changed()
{
    ui->slideshowImageListDockWidget->setWindowTitle(tr("Image List %1/%2").arg(m_slideshowImageListModel->rowCount()).arg(m_slideshowImageListModel->imageCount()));
}

void MainWindow::imageLoaded(const QImage &image, int, int, int)
{
    m_imageWidget->setImage(image);
}

void MainWindow::on_imageListListView_customContextMenuRequested(const QPoint &)
{
    QMenu menu(ui->imageListListView);
    menu.addAction(ui->actionAboutQsm);
    menu.exec(QCursor::pos());
}

void MainWindow::scanFolderCancelButton_clicked()
{
    if (!m_scanFolderThread->isRunning())
        return;

    m_scanFolderAbortButton->setText(tr("aborting..."));
    m_scanFolderAbortButton->setEnabled(false);
    m_scanFolderThread->abort();
}

void MainWindow::on_actionNewSlideshow_triggered()
{
    m_slideshowListModel->addSlideshow(Slideshow("New Slideshow"));
}

void MainWindow::on_actionStatusbar_triggered()
{
    ui->statusBar->setVisible(ui->actionStatusbar->isChecked());
}

void MainWindow::on_actionAboutQsm_triggered()
{
    QMessageBox::about(this, tr("About QSM"),
            tr("<b>Qt SlideShow Manager</b> v0.1 alpha<br/><br/>"
               "Copyright &copy; 2011 Fachhochschule S&uuml;dwestfalen<br/>"
               "Written by Alexander Vos and Till Althaus"));
}
