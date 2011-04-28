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
#include "slideshowlistmodel.h"
#include "imagewidget.h"

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
    ui->imageListView->setModel(m_imageListModel);
    ui->imageListView->setIconSize(QSize(64, 64));

    m_slideshowListModel = new SlideshowListModel(this);
    ui->slideshowListView->setModel(m_slideshowListModel);
    ui->slideshowListView->addAction(ui->actionRemoveSlideshow);
    m_slideshowImageListModel = new ImageListModel(this);
    connect(m_slideshowImageListModel, SIGNAL(changed()), SLOT(slideshowImageListModel_changed()));
    ui->slideshowImageListView->setModel(m_slideshowImageListModel);
    ui->slideshowImageListView->setIconSize(QSize(64, 64));
    ui->slideshowImageListView->addAction(ui->actionRemoveImage);

    m_imageWidget = new ImageWidget(this);
    setCentralWidget(m_imageWidget);

    m_scanFolderLabel = new QLabel;
    m_scanFolderLabel->setVisible(false);
    statusBar()->addPermanentWidget(m_scanFolderLabel);
    m_scanFolderAbortButton = new QPushButton(tr("Abort"));
    m_scanFolderAbortButton->setVisible(false);
    connect(m_scanFolderAbortButton, SIGNAL(clicked()), SLOT(scanFolderCancelButton_clicked()));
    statusBar()->addPermanentWidget(m_scanFolderAbortButton);

    setShortcuts();
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::setShortcuts()
{
    // window shortcuts
    ui->actionNewSlideshow->setShortcut(QKeySequence::New);
    ui->actionReloadAllSlideshows->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_F5);
    ui->actionSaveAllSlideshows->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_S);
    ui->actionExit->setShortcut(QKeySequence::Quit);
    ui->actionOptions->setShortcut(QKeySequence::Preferences);
    ui->actionQsmHelp->setShortcut(QKeySequence::HelpContents);

    // slideshow shortcuts
    ui->actionRenameSlideshow->setShortcut(Qt::Key_F2);
    ui->actionRemoveSlideshow->setShortcut(QKeySequence::Delete);
    ui->actionReloadSlideshow->setShortcut(QKeySequence::Refresh);
    ui->actionSaveSlideshow->setShortcut(QKeySequence::Save);
    ui->actionCopyImagesToSlideshow->setShortcut(QKeySequence::Copy);

    // image shortcuts
    ui->actionRemoveImage->setShortcut(QKeySequence::Delete);
    ui->actionRemoveImageFromFileSystem->setShortcut(Qt::ALT + Qt::Key_Delete);
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

void MainWindow::scanFolder(const QModelIndex &index, bool includeSubfolders)
{
    if (m_scanFolderThread->isRunning()) {
        scanFolderCancelButton_clicked();
        m_scanFolderThread->wait();
    }
    m_scanFolderThread->setFolder(m_folderBrowserModel->filePath(index), includeSubfolders);
    m_scanFolderThread->start(QThread::NormalPriority);
}

void MainWindow::on_folderBrowserTreeView_clicked(const QModelIndex &index)
{
    if (index.isValid())
        scanFolder(index, ui->includeSubfoldersCheckBox->isChecked());
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
    m_scanFolderLabel->setText(tr("Scanning folder %1 ...").arg(folder.left(100)));
    m_imageListModel->addImages(files);
}

void MainWindow::scanFolderThread_finished()
{
    qDebug("scanFolderThread_finished: %d images found", m_imageListModel->imageCount());
    statusBar()->showMessage(tr("%1 images found, %2 folders scanned").arg(m_imageListModel->imageCount()).arg(m_scanFolderThread->count()));
    m_scanFolderLabel->setVisible(false);
    m_scanFolderAbortButton->setVisible(false);
}

void MainWindow::prepareImage(const ImageListModel *model, const QModelIndex &index)
{
    Q_ASSERT(model);

    QString imagePath = model->imagePath(index);
    if (imagePath == m_currentImagePath)
        return;

    m_currentImagePath = imagePath;

    QVariant var = model->data(index, Qt::DecorationRole);
    if (var.canConvert<QIcon>()) {
        // set up preview image
        QIcon icon = var.value<QIcon>();
        m_imageWidget->setImage(icon.pixmap(64, 64).toImage());
    }

    // load the actual image inside a separate thread
    ImageLoader *imageLoader = new ImageLoader(imagePath);
    connect(imageLoader, SIGNAL(imageLoaded(QImage, int, int, int)), SLOT(imageLoaded(QImage, int, int, int)));
    QThreadPool::globalInstance()->start(imageLoader);
}

void MainWindow::on_imageListView_clicked(const QModelIndex &index)
{
    prepareImage(m_imageListModel, index);
}

void MainWindow::on_imageListView_doubleClicked(const QModelIndex &index)
{
    if (!m_slideshowListModel->currentSlideshow())
        return;

    m_slideshowListModel->addImage(SlideshowImage(m_imageListModel->imagePath(index)));
    m_slideshowImageListModel->addImage(m_imageListModel->imageInfo(index));
}

void MainWindow::on_imageListView_customContextMenuRequested(const QPoint &pos)
{
    QMenu menu(ui->imageListView);
    menu.addAction(ui->actionAboutQsm);
    menu.exec(ui->imageListView->mapToGlobal(pos));
}

void MainWindow::imageListModel_changed()
{
    ui->imageListDockWidget->setWindowTitle(tr("Image List %1/%2").arg(m_imageListModel->rowCount()).arg(m_imageListModel->imageCount()));
}

void MainWindow::on_slideshowListView_selectionChanged(const QModelIndex &index)
{
    if (!m_slideshowListModel->setCurrentSlideshow(index))
        return;

    // prepare slideshow image list
    m_slideshowImageListModel->clear();
    QFileInfoList imageInfoList;
    foreach (const SlideshowImage &image, m_slideshowListModel->currentSlideshow()->images())
        imageInfoList.append(image.fileInfo());
    m_slideshowImageListModel->addImages(imageInfoList);
}

void MainWindow::on_slideshowListView_customContextMenuRequested(const QPoint &pos)
{
    QMenu menu(ui->slideshowListView);
    if (ui->slideshowListView->currentIndex().isValid()) {
        menu.addAction(ui->actionRenameSlideshow);
        menu.addAction(ui->actionRemoveSlideshow);
        menu.addAction(ui->actionReloadSlideshow);
        menu.addAction(ui->actionSaveSlideshow);
        ui->actionCopyImagesToSlideshow->setEnabled(m_slideshowListModel->currentSlideshow()->imageCount() > 0);
        menu.addAction(ui->actionCopyImagesToSlideshow);
    }
    menu.addSeparator();
    menu.addAction(ui->actionNewSlideshow);
    menu.addSeparator();
    menu.addAction(ui->actionReloadAllSlideshows);
    menu.addAction(ui->actionSaveAllSlideshows);
    menu.exec(ui->slideshowListView->mapToGlobal(pos));
}

void MainWindow::on_slideshowImageListView_clicked(const QModelIndex &index)
{
    prepareImage(m_slideshowImageListModel, index);
}

void MainWindow::on_slideshowImageListView_customContextMenuRequested(const QPoint &pos)
{
    if (ui->slideshowImageListView->currentIndex().isValid()) {
        QMenu menu(ui->slideshowImageListView);
        menu.addAction(ui->actionRemoveImage);
        menu.addAction(ui->actionRemoveImageFromFileSystem);
        menu.exec(ui->slideshowImageListView->mapToGlobal(pos));
    }
}

void MainWindow::slideshowImageListModel_changed()
{
    ui->slideshowImageListDockWidget->setWindowTitle(tr("Slideshow Image List %1/%2")
                                                     .arg(m_slideshowImageListModel->rowCount())
                                                     .arg(m_slideshowImageListModel->imageCount()));
}

void MainWindow::imageLoaded(const QImage &image, int, int, int)
{
    m_imageWidget->setImage(image);
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
    ui->slideshowListView->setCurrentIndex(m_slideshowListModel->addSlideshow(Slideshow(tr("New Slideshow"))));
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

void MainWindow::on_actionRenameSlideshow_triggered()
{
    ui->slideshowListView->edit(ui->slideshowListView->currentIndex());
}

void MainWindow::on_actionRemoveSlideshow_triggered()
{
    m_slideshowListModel->removeSlideshow(ui->slideshowListView->currentIndex());
}

void MainWindow::on_actionRemoveImage_triggered()
{
    m_slideshowListModel->removeImage(ui->slideshowImageListView->currentIndex());
    m_slideshowImageListModel->removeImage(ui->slideshowImageListView->currentIndex());
}

void MainWindow::on_includeSubfoldersCheckBox_toggled(bool checked)
{
    QModelIndex index = ui->folderBrowserTreeView->currentIndex();
    if (index.isValid())
        scanFolder(index, checked);
}
