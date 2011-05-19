#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QLabel>
#include <QSettings>
#include <QMessageBox>
#include <QFileSystemModel>
#include <QThreadPool>

#include <QDebug>

#include "scanfolderthread.h"
#include "imagelistmodel.h"
#include "slideshowlistmodel.h"
#include "slideshowfilemanager.h"
#include "imagewidget.h"
#include "slideshowwindow.h"
#include "optionsdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->menuView->addAction(ui->mainToolBar->toggleViewAction());
    ui->menuView->addAction(ui->actionStatusbar);
    QMenu *menuSidebar = new QMenu(tr("&Sidebar"), ui->menuBar);
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
    ui->imageListView->addAction(ui->actionAddToSlideshow);
    ui->imageListView->addAction(ui->actionRenameImage);
    ui->imageListView->addAction(ui->actionCutImage);
    ui->imageListView->addAction(ui->actionCopyImage);
    ui->imageListView->addAction(ui->actionPasteImage);
    ui->imageListView->addAction(ui->actionRemoveImageFromDisk);
    ui->imageListView->addAction(ui->actionCopyPath);
    ui->imageListView->addAction(ui->actionExportImages);
    ui->imageListView->addAction(ui->actionPreloadAllImages);

    m_slideshowListModel = new SlideshowListModel(this);
    ui->slideshowListView->setModel(m_slideshowListModel);
    ui->slideshowListView->addAction(ui->actionSlideshowEditComment);
    ui->slideshowListView->addAction(ui->actionRenameSlideshow);
    ui->slideshowListView->addAction(ui->actionRemoveSlideshow);
    ui->slideshowListView->addAction(ui->actionCopyImagesToSlideshow);
    ui->slideshowListView->addAction(ui->actionExportAllImages);
    ui->slideshowListView->addAction(ui->actionReloadSlideshow);
    ui->slideshowListView->addAction(ui->actionSaveSlideshow);

    m_slideshowImageListModel = new ImageListModel(this);
    connect(m_slideshowImageListModel, SIGNAL(changed()), SLOT(slideshowImageListModel_changed()));
    ui->slideshowImageListView->setModel(m_slideshowImageListModel);
    ui->slideshowImageListView->addAction(ui->actionImageEditComment);
    ui->slideshowImageListView->addAction(ui->actionRenameImage);
    ui->slideshowImageListView->addAction(ui->actionCutImage);
    ui->slideshowImageListView->addAction(ui->actionCopyImage);
    ui->slideshowImageListView->addAction(ui->actionPasteImage);
    ui->slideshowImageListView->addAction(ui->actionRemoveImage);
    ui->slideshowImageListView->addAction(ui->actionRemoveImageFromDisk);
    ui->slideshowImageListView->addAction(ui->actionCopyPath);
    ui->slideshowImageListView->addAction(ui->actionExportImages);
    ui->slideshowImageListView->addAction(ui->actionPreloadAllImages);

    ui->imageWidget->addAction(ui->actionAddToSlideshow);
    ui->imageWidget->addAction(ui->actionRotateClockwise);
    ui->imageWidget->addAction(ui->actionRotateCounterclockwise);
    ui->imageWidget->addAction(ui->actionZoomFit);
    ui->imageWidget->addAction(ui->actionZoomOriginal);
    ui->imageWidget->addAction(ui->actionZoomIn);
    ui->imageWidget->addAction(ui->actionZoomOut);
    ui->imageWidget->addAction(ui->actionImageEditComment);
    ui->imageWidget->addAction(ui->actionCutImage);
    ui->imageWidget->addAction(ui->actionCopyImage);
    ui->imageWidget->addAction(ui->actionRemoveImageFromDisk);
    ui->imageWidget->addAction(ui->actionCopyPath);
    ui->imageWidget->addAction(ui->actionExportImages);

    m_slideshowFileManager = new SlideshowFileManager(this);
    connect(m_slideshowFileManager, SIGNAL(started()), SLOT(slideshowFileManager_started()));
    connect(m_slideshowFileManager, SIGNAL(slideshowLoaded(Slideshow)), m_slideshowListModel, SLOT(addSlideshow(Slideshow)));
    connect(m_slideshowFileManager, SIGNAL(finished()), SLOT(slideshowFileManager_finished()));

    m_scanFolderLabel = new QLabel(this);
    m_scanFolderLabel->setVisible(false);
    statusBar()->addPermanentWidget(m_scanFolderLabel);
    m_scanFolderAbortButton = new QPushButton(tr("Abort"), this);
    m_scanFolderAbortButton->setVisible(false);
    connect(m_scanFolderAbortButton, SIGNAL(clicked()), SLOT(scanFolderAbortButton_clicked()));
    statusBar()->addPermanentWidget(m_scanFolderAbortButton);

    loadSettings();
    on_actionReloadAllSlideshows_triggered(); // start slideshow loading

    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::loadSettings()
{
    QSettings settings;

    // window geometry and state
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());

    // thumbnail sizes
    int imageListViewThumbnailSize = settings.value("imageListViewThumbnailSize", 64).toInt();
    ui->imageListView->setIconSize(QSize(imageListViewThumbnailSize, imageListViewThumbnailSize));
    int slideshowImageListViewThumbnailSize = settings.value("slideshowImageListViewThumbnailSize", 64).toInt();
    ui->slideshowImageListView->setIconSize(QSize(slideshowImageListViewThumbnailSize, slideshowImageListViewThumbnailSize));

    // image view
    ui->imageWidget->setBackgroundColor(settings.value("backgroundColor", OptionsDialog::DEFAULT_BACKGROUND_COLOR).value<QColor>());
    ui->imageWidget->setTextBackgroundColor(settings.value("textBackgroundColor", OptionsDialog::DEFAULT_TEXT_BACKGROUND_COLOR).value<QColor>());
    ui->imageWidget->setTextFont(settings.value("textFont", OptionsDialog::DEFAULT_TEXT_FONT).value<QFont>());
    ui->imageWidget->setTextColor(settings.value("textColor", OptionsDialog::DEFAULT_TEXT_COLOR).value<QColor>());

    // slideshows and images directory
    m_slideshowsDirectory = settings.value("slideshowsDirectory", OptionsDialog::DEFAULT_SLIDESHOWS_DIRECTORY).toString();
    OptionsDialog::checkDirectory(m_slideshowsDirectory);
    m_imagesDirectory = settings.value("imagesDirectory", OptionsDialog::DEFAULT_IMAGES_DIRECTORY).toString();
    OptionsDialog::checkDirectory(m_imagesDirectory, true, false);

    // window shortcuts
    OptionsDialog::DefaultShortcuts.insert(ui->actionExit, QKeySequence::Quit);
    OptionsDialog::DefaultShortcuts.insert(ui->actionOptions, QKeySequence::Preferences);
    OptionsDialog::DefaultShortcuts.insert(ui->actionQsmHelp, QKeySequence::HelpContents);

    // image shortcuts
    OptionsDialog::DefaultShortcuts.insert(ui->actionAddToSlideshow, Qt::Key_Space);
    OptionsDialog::DefaultShortcuts.insert(ui->actionImageEditComment, Qt::Key_Insert);
    OptionsDialog::DefaultShortcuts.insert(ui->actionRenameImage, Qt::Key_F2);
    OptionsDialog::DefaultShortcuts.insert(ui->actionCutImage, QKeySequence::Cut);
    OptionsDialog::DefaultShortcuts.insert(ui->actionCopyImage, QKeySequence::Copy);
    OptionsDialog::DefaultShortcuts.insert(ui->actionPasteImage, QKeySequence::Paste);
    OptionsDialog::DefaultShortcuts.insert(ui->actionRemoveImage, QKeySequence::Delete);
    OptionsDialog::DefaultShortcuts.insert(ui->actionRemoveImageFromDisk, Qt::ALT + Qt::Key_Delete);
    OptionsDialog::DefaultShortcuts.insert(ui->actionCopyPath, Qt::CTRL + Qt::SHIFT + Qt::Key_C);
    OptionsDialog::DefaultShortcuts.insert(ui->actionExportImages, Qt::CTRL + Qt::Key_E);
    OptionsDialog::DefaultShortcuts.insert(ui->actionPreloadAllImages, QKeySequence::Print);

    // slideshow shortcuts
    OptionsDialog::DefaultShortcuts.insert(ui->actionNewSlideshow,QKeySequence::New);
    OptionsDialog::DefaultShortcuts.insert(ui->actionReloadAllSlideshows, Qt::CTRL + Qt::SHIFT + Qt::Key_F5);
    OptionsDialog::DefaultShortcuts.insert(ui->actionSaveAllSlideshows, Qt::CTRL + Qt::SHIFT + Qt::Key_S);
    OptionsDialog::DefaultShortcuts.insert(ui->actionSlideshowEditComment, Qt::Key_Insert);
    OptionsDialog::DefaultShortcuts.insert(ui->actionRenameSlideshow, Qt::Key_F2);
    OptionsDialog::DefaultShortcuts.insert(ui->actionRemoveSlideshow, QKeySequence::Delete);
    OptionsDialog::DefaultShortcuts.insert(ui->actionCopyImagesToSlideshow, QKeySequence::Copy);
    OptionsDialog::DefaultShortcuts.insert(ui->actionExportAllImages, Qt::CTRL + Qt::Key_E);
    OptionsDialog::DefaultShortcuts.insert(ui->actionReloadSlideshow, QKeySequence::Refresh);
    OptionsDialog::DefaultShortcuts.insert(ui->actionSaveSlideshow, QKeySequence::Save);

    // image view shortcuts
    OptionsDialog::DefaultShortcuts.insert(ui->actionRotateClockwise, Qt::CTRL + Qt::Key_R);
    OptionsDialog::DefaultShortcuts.insert(ui->actionRotateCounterclockwise, Qt::CTRL + Qt::SHIFT + Qt::Key_R);
    OptionsDialog::DefaultShortcuts.insert(ui->actionZoomFit, Qt::CTRL + Qt::Key_0);
    OptionsDialog::DefaultShortcuts.insert(ui->actionZoomOriginal, Qt::CTRL + Qt::Key_1);
    OptionsDialog::DefaultShortcuts.insert(ui->actionZoomIn, QKeySequence::ZoomIn);
    OptionsDialog::DefaultShortcuts.insert(ui->actionZoomOut, QKeySequence::ZoomOut);

    OptionsDialog::loadAllShortcuts(settings);
}

void MainWindow::saveSettings()
{
    QSettings settings;
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
    settings.setValue("imageListViewThumbnailSize", ui->imageListView->iconSize().width());
    settings.setValue("slideshowImageListViewThumbnailSize", ui->slideshowImageListView->iconSize().width());
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    on_actionSaveAllSlideshows_triggered();
    saveSettings();
    qApp->quit();
    QMainWindow::closeEvent(event);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::scanFolder(const QModelIndex &index, bool includeSubfolders)
{
    if (m_scanFolderThread->isRunning()) {
        scanFolderAbortButton_clicked();
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
    statusBar()->showMessage(tr("%1 images found, %2 folders scanned")
                             .arg(m_imageListModel->imageCount())
                             .arg(m_scanFolderThread->count()));
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
        ui->imageWidget->setImage(icon.pixmap(MainWindow::MAX_THUMBNAIL_SIZE).toImage());
    }

    // load the actual image inside a separate thread
    ImageLoader *imageLoader = new ImageLoader(imagePath);
    connect(imageLoader, SIGNAL(imageLoaded(QImage, int, int, int)), SLOT(imageLoaded(QImage, int, int, int)));
    QThreadPool::globalInstance()->start(imageLoader, 1);
}

QWidget* MainWindow::activeWidget(QAction *action)
{
    Q_ASSERT(action);
    foreach (QWidget* widget, action->associatedWidgets())
        if (widget->hasFocus())
            return widget;
    return NULL;
}

QMenu* MainWindow::createThumbnailSizeMenu(QWidget *parent, int selectedSize)
{
    QMenu *menu = new QMenu(tr("Thumbnail Size"), parent);
    QActionGroup *actionGroup = new QActionGroup(menu);
    QAction *disabledAction = menu->addAction(tr("Disabled"));
    disabledAction->setCheckable(true);
    disabledAction->setData(0);
    actionGroup->addAction(disabledAction);
    if (selectedSize == 0)
        disabledAction->setChecked(true);
    static const int sizes[] = {16, 24, 32, 48, 64, 96, 128};
    for (int i = 0; i < int(sizeof(sizes) / sizeof(int)); ++i) {
        int size = sizes[i];
        QAction *sizeAction = menu->addAction(tr("%1 Pixels").arg(size));
        sizeAction->setCheckable(true);
        sizeAction->setData(size);
        actionGroup->addAction(sizeAction);
        if (size == selectedSize)
            sizeAction->setChecked(true);
    }
    connect(menu, SIGNAL(triggered(QAction*)), SLOT(thumbnailSizeMenu_triggered(QAction*)));
    return menu;
}

void MainWindow::thumbnailSizeMenu_triggered(QAction *action)
{
    int size = action->data().toInt();
    if (sender()->parent()->parent() == ui->imageListView)
        ui->imageListView->setIconSize(QSize(size, size));
    else
        ui->slideshowImageListView->setIconSize(QSize(size, size));
}

void MainWindow::on_imageListView_clicked(const QModelIndex &index)
{
    prepareImage(m_imageListModel, index);
    ui->imageWidget->setText();
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
    if (ui->imageListView->currentIndex().isValid()) {
        ui->actionAddToSlideshow->setEnabled(m_slideshowListModel->currentSlideshow());
        menu.addAction(ui->actionAddToSlideshow);
        menu.addAction(ui->actionRenameImage);
        menu.addAction(ui->actionCutImage);
        menu.addAction(ui->actionCopyImage);
        menu.addAction(ui->actionPasteImage);
        menu.addAction(ui->actionRemoveImageFromDisk);
        menu.addAction(ui->actionCopyPath);
        menu.addAction(ui->actionExportImages);
        menu.addSeparator();
    }
    QMenu *thumbnailSizeMenu = createThumbnailSizeMenu(&menu, ui->imageListView->iconSize().width());
    menu.addMenu(thumbnailSizeMenu);
    if (!m_imageListModel->allThumbnailsLoaded())
        menu.addAction(ui->actionPreloadAllImages);
    menu.exec(ui->imageListView->mapToGlobal(pos));
}

void MainWindow::imageListModel_changed()
{
    ui->imageListDockWidget->setWindowTitle(tr("Image List %1/%2")
                                            .arg(m_imageListModel->rowCount())
                                            .arg(m_imageListModel->imageCount()));
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
        menu.addAction(ui->actionSlideshowEditComment);
        menu.addAction(ui->actionRenameSlideshow);
        menu.addAction(ui->actionRemoveSlideshow);
        bool imagesAvailable = m_slideshowListModel->currentSlideshow()->imageCount() > 0;
        ui->actionCopyImagesToSlideshow->setEnabled(imagesAvailable);
        menu.addAction(ui->actionCopyImagesToSlideshow);
        ui->actionExportAllImages->setEnabled(imagesAvailable);
        menu.addAction(ui->actionExportAllImages);
        menu.addAction(ui->actionReloadSlideshow);
        ui->actionSaveSlideshow->setEnabled(m_slideshowListModel->currentSlideshow()->hasChanged());
        menu.addAction(ui->actionSaveSlideshow);
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
    ui->imageWidget->setText(m_slideshowListModel->currentSlideshow()->image(index.row())->comment());
}

void MainWindow::on_slideshowImageListView_customContextMenuRequested(const QPoint &pos)
{
    QMenu menu(ui->slideshowImageListView);
    QMenu randomFactorMenu(tr("Random Factor"), &menu);
    QActionGroup randomFactorGroup(&menu);
    Slideshow *slideshow = m_slideshowListModel->currentSlideshow();
    SlideshowImage *image = slideshow ? slideshow->image(ui->slideshowImageListView->currentIndex().row()) : NULL;
    if (ui->slideshowImageListView->currentIndex().isValid()) {
        menu.addAction(ui->actionImageEditComment);
        static const int randomFactors[] = {1, 2, 3, 4, 5, 10, 25, 50, 100};
        for (int i = 0; i < int(sizeof(randomFactors) / sizeof(int)); ++i) {
            int randomFactor = randomFactors[i];
            QAction *randomFactorAction = randomFactorMenu.addAction(QString::number(randomFactor));
            randomFactorAction->setCheckable(true);
            randomFactorAction->setData(randomFactor);
            randomFactorGroup.addAction(randomFactorAction);
            if (image && image->randomFactor() == randomFactor)
                randomFactorAction->setChecked(true);
        }
        //randomFactorMenu.addAction(...); // TODO: custom random factor
        menu.addMenu(&randomFactorMenu);
        menu.addAction(ui->actionRenameImage);
        menu.addAction(ui->actionCutImage);
        menu.addAction(ui->actionCopyImage);
        menu.addAction(ui->actionPasteImage);
        menu.addAction(ui->actionRemoveImage);
        menu.addAction(ui->actionRemoveImageFromDisk);
        menu.addAction(ui->actionCopyPath);
        menu.addAction(ui->actionExportImages);
        menu.addSeparator();
    }
    QMenu *thumbnailSizeMenu = createThumbnailSizeMenu(&menu, ui->slideshowImageListView->iconSize().width());
    menu.addMenu(thumbnailSizeMenu);
    if (!m_slideshowImageListModel->allThumbnailsLoaded())
        menu.addAction(ui->actionPreloadAllImages);
    QAction *action = menu.exec(ui->slideshowImageListView->mapToGlobal(pos));
    if (action && action->parent() == &randomFactorMenu && image) {
        image->setRandomFactor(action->data().toInt());
        slideshow->setChanged();
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
    ui->imageWidget->setImage(image);
}

void MainWindow::slideshowFileManager_started()
{
    ui->actionReloadAllSlideshows->setEnabled(false);
    ui->actionSaveAllSlideshows->setEnabled(false);
    ui->actionReloadSlideshow->setEnabled(false);
    ui->actionSaveSlideshow->setEnabled(false);
}

void MainWindow::slideshowFileManager_finished()
{
    ui->actionReloadAllSlideshows->setEnabled(true);
    ui->actionSaveAllSlideshows->setEnabled(true);
    ui->actionReloadSlideshow->setEnabled(true);
    ui->actionSaveSlideshow->setEnabled(true);
}

void MainWindow::on_imageWidget_viewChanged()
{
    statusBar()->showMessage(tr("Zoom: %1 %").arg(ui->imageWidget->zoomFactor() * 100, 0, 'f', 0));
}

void MainWindow::on_imageWidget_customContextMenuRequested(const QPoint &pos)
{
    if (QFile::exists(m_currentImagePath)) {
        QMenu menu(ui->imageWidget);
        ui->actionAddToSlideshow->setEnabled(m_slideshowListModel->currentSlideshow());
        menu.addAction(ui->actionAddToSlideshow);
        menu.addSeparator();
        menu.addAction(ui->actionRotateClockwise);
        menu.addAction(ui->actionRotateCounterclockwise);
        QMenu zoomMenu(tr("Zoom"), &menu);
        zoomMenu.addAction(ui->actionZoomFit);
        zoomMenu.addAction(ui->actionZoomOriginal);
        zoomMenu.addAction(ui->actionZoomIn);
        zoomMenu.addAction(ui->actionZoomOut);
        menu.addMenu(&zoomMenu);
        menu.addSeparator();
        if (ui->slideshowImageListView->currentIndex().isValid()) // TODO: check for slideshow image
            menu.addAction(ui->actionImageEditComment);
        menu.addAction(ui->actionCutImage);
        menu.addAction(ui->actionCopyImage);
        menu.addAction(ui->actionRemoveImageFromDisk);
        menu.addAction(ui->actionCopyPath);
        menu.addAction(ui->actionExportImages);
        menu.exec(ui->imageWidget->mapToGlobal(pos));
    }
}

void MainWindow::scanFolderAbortButton_clicked()
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

void MainWindow::on_actionReloadAllSlideshows_triggered()
{
    m_slideshowFileManager->loadAllSlideshows(m_slideshowsDirectory);
}

void MainWindow::on_actionSaveAllSlideshows_triggered()
{
    m_slideshowFileManager->saveAllSlideshows(m_slideshowListModel->slideshowList(), m_slideshowsDirectory);
}

void MainWindow::on_actionStatusbar_triggered()
{
    ui->statusBar->setVisible(ui->actionStatusbar->isChecked());
}

void MainWindow::on_actionOptions_triggered()
{
    QList<QPair<QString, QList<QAction*> > > actions;

    // window shortcuts
    QList<QAction*> windowActions;
    windowActions << ui->actionExit << ui->actionOptions << ui->actionQsmHelp;
    actions.append(qMakePair(tr("Window"), windowActions));

    // image shortcuts
    QList<QAction*> imageActions;
    imageActions << ui->actionAddToSlideshow << ui->actionImageEditComment << ui->actionRenameImage
                 << ui->actionCutImage << ui->actionCopyImage << ui->actionPasteImage
                 << ui->actionRemoveImage <<  ui->actionRemoveImageFromDisk << ui->actionCopyPath
                 << ui->actionExportImages << ui->actionPreloadAllImages;
    actions.append(qMakePair(tr("Image"), imageActions));

    // slideshow shortcuts
    QList<QAction*> slideshowActions;
    slideshowActions << ui->actionNewSlideshow << ui->actionReloadAllSlideshows << ui->actionSaveAllSlideshows
                     << ui->actionSlideshowEditComment << ui->actionRenameSlideshow << ui->actionRemoveSlideshow
                     << ui->actionCopyImagesToSlideshow << ui->actionExportAllImages << ui->actionReloadSlideshow
                     << ui->actionSaveSlideshow;
    actions.append(qMakePair(tr("Slideshow"), slideshowActions));

    // image view shortcuts
    QList<QAction*> imageViewActions;
    imageViewActions << ui->actionRotateClockwise << ui->actionRotateCounterclockwise << ui->actionZoomFit
                     << ui->actionZoomOriginal << ui->actionZoomIn << ui->actionZoomOut;
    actions.append(qMakePair(tr("Image View"), imageViewActions));

    // open options dialog
    OptionsDialog options(&actions, this);
    if (options.exec() == QDialog::Accepted) {
        ui->imageWidget->setBackgroundColor(options.backgroundColor());
        ui->imageWidget->setTextBackgroundColor(options.textBackgroundColor());
        ui->imageWidget->setTextFont(options.textFont());
        ui->imageWidget->setTextColor(options.textColor());
        QString slideshowsDirectory = options.slideshowsDirectory();
        if (slideshowsDirectory != m_slideshowsDirectory) {
            m_slideshowsDirectory = slideshowsDirectory;
            m_slideshowListModel->clear();
            on_actionReloadAllSlideshows_triggered();
        }
        m_imagesDirectory = options.imagesDirectory();
    }
}

void MainWindow::on_actionQsmHelp_triggered()
{
}

void MainWindow::on_actionAboutQsm_triggered()
{
    QMessageBox::about(this, tr("About QSM"),
            tr("<b>Qt SlideShow Manager</b> v0.1 alpha<br/><br/>"
               "Copyright &copy; 2011 Fachhochschule S&uuml;dwestfalen<br/>"
               "Written by Alexander Vos and Till Althaus"));
}

void MainWindow::on_actionAddToSlideshow_triggered()
{
    QWidget *widget = activeWidget(ui->actionAddToSlideshow);
    if (!widget) return;

    Slideshow *slideshow = m_slideshowListModel->currentSlideshow();
    if (!slideshow) return;

    if (widget == ui->imageListView) {
        QString path = m_imageListModel->imagePath(ui->imageListView->currentIndex());
        m_slideshowListModel->addImage(path);
        m_slideshowImageListModel->addImage(ImageInfo(path));
    } else if (widget == ui->imageWidget) {
        m_slideshowListModel->addImage(m_currentImagePath);
        m_slideshowImageListModel->addImage(ImageInfo(m_currentImagePath));
    }
}

void MainWindow::on_actionImageEditComment_triggered()
{
}

void MainWindow::on_actionRenameImage_triggered()
{
}

void MainWindow::on_actionCutImage_triggered()
{
}

void MainWindow::on_actionCopyImage_triggered()
{
}

void MainWindow::on_actionPasteImage_triggered()
{
}

void MainWindow::on_actionRemoveImage_triggered()
{
    m_slideshowListModel->removeImage(ui->slideshowImageListView->currentIndex());
    m_slideshowImageListModel->removeImage(ui->slideshowImageListView->currentIndex());
}

void MainWindow::on_actionRemoveImageFromDisk_triggered()
{
    QWidget *widget = activeWidget(ui->actionRemoveImageFromDisk);
    if (!widget) return;

    QString path;
    if (widget == ui->imageListView)
        path = m_imageListModel->imagePath(ui->imageListView->currentIndex());
    else if (widget == ui->slideshowImageListView)
        path = m_slideshowImageListModel->imagePath(ui->slideshowImageListView->currentIndex());
    else if (widget == ui->imageWidget)
        path = m_currentImagePath;

    qDebug() << path;
}

void MainWindow::on_actionCopyPath_triggered()
{
}

void MainWindow::on_actionPreloadAllImages_triggered()
{
}

void MainWindow::on_actionSlideshowEditComment_triggered()
{
}

void MainWindow::on_actionRenameSlideshow_triggered()
{
    ui->slideshowListView->edit(ui->slideshowListView->currentIndex());
}

void MainWindow::on_actionRemoveSlideshow_triggered()
{
    m_slideshowListModel->removeSlideshow(ui->slideshowListView->currentIndex());
}

void MainWindow::on_actionCopyImagesToSlideshow_triggered()
{
}

void MainWindow::on_actionReloadSlideshow_triggered()
{
    Slideshow *slideshow = m_slideshowListModel->currentSlideshow();
    if (slideshow)
        m_slideshowFileManager->loadSlideshow(slideshow->path(m_slideshowsDirectory));
}

void MainWindow::on_actionSaveSlideshow_triggered()
{
    Slideshow *slideshow = m_slideshowListModel->currentSlideshow();
    if (slideshow)
        m_slideshowFileManager->saveSlideshow(*slideshow, m_slideshowsDirectory);
}

void MainWindow::on_includeSubfoldersCheckBox_toggled(bool checked)
{
    QModelIndex index = ui->folderBrowserTreeView->currentIndex();
    if (index.isValid())
        scanFolder(index, checked);
}

void MainWindow::on_slideshowStartPushButton_clicked()
{
    Slideshow *slideshow = m_slideshowListModel->currentSlideshow();
    if (!slideshow || !slideshow->imageCount())
        return;

    SlideshowWindow *slideshowWindow = new SlideshowWindow(slideshow);
    ImageWidget *imageWidget = slideshowWindow->imageWidget();
    imageWidget->setBackgroundColor(ui->imageWidget->backgroundColor());
    imageWidget->setTextBackgroundColor(ui->imageWidget->textBackgroundColor());
    imageWidget->setTextFont(ui->imageWidget->textFont());
    imageWidget->setTextColor(ui->imageWidget->textColor());
    slideshowWindow->showFullScreen();
}
