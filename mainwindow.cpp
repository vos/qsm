#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <climits>

#include <QLabel>
#include <QSettings>
#include <QMessageBox>
#include <QFileSystemModel>
#include <QThreadPool>
#include <QInputDialog>
#include <QDesktopServices>
#include <QUrl>
#include <QClipboard>
#include <QWheelEvent>

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
    ui(new Ui::MainWindow),
    m_currentSlideshow(NULL),
    m_currentSlideshowImage(NULL)
{
    ui->setupUi(this);
    ui->menuView->insertAction(ui->actionStatusbar, ui->mainToolBar->toggleViewAction());
    QMenu *menuSidebar = new QMenu(tr("&Sidebar"), ui->menuView);
    menuSidebar->addAction(ui->folderBrowserDockWidget->toggleViewAction());
    menuSidebar->addAction(ui->imageListDockWidget->toggleViewAction());
    menuSidebar->addAction(ui->slideshowListDockWidget->toggleViewAction());
    menuSidebar->addAction(ui->slideshowImageListDockWidget->toggleViewAction());
    ui->menuView->insertMenu(ui->actionStatusbar, menuSidebar);
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
    ui->imageListView->addAction(ui->actionOpenFileLocation);
    ui->imageListView->addAction(ui->actionExportImages);
    ui->imageListView->addAction(ui->actionPreloadAllImages);

    m_slideshowListModel = new SlideshowListModel(this);
    connect(m_slideshowListModel, SIGNAL(slideshowRenamed(QString,QString)), SLOT(slideshowListModel_slideshowRenamed(QString,QString)));
    ui->slideshowListView->setModel(m_slideshowListModel);
    ui->slideshowListView->addAction(ui->actionStartSlideshow);
    ui->slideshowListView->addAction(ui->actionSlideshowEditComment);
    ui->slideshowListView->addAction(ui->actionRenameSlideshow);
    ui->slideshowListView->addAction(ui->actionRemoveSlideshow);
    ui->slideshowListView->addAction(ui->actionCopyImagesToSlideshow);
    ui->slideshowListView->addAction(ui->actionOpenFileLocation);
    ui->slideshowListView->addAction(ui->actionExportAllImages);
    ui->slideshowListView->addAction(ui->actionReloadSlideshow);
    ui->slideshowListView->addAction(ui->actionSaveSlideshow);

    m_slideshowImageListModel = new ImageListModel(this);
    connect(m_slideshowImageListModel, SIGNAL(changed()), SLOT(slideshowImageListModel_changed()));
    connect(m_slideshowImageListModel, SIGNAL(imageRenamed(QModelIndex,QString)), SLOT(slideshowImageListModel_imageRenamed(QModelIndex,QString)));
    ui->slideshowImageListView->setModel(m_slideshowImageListModel);
    ui->slideshowImageListView->addAction(ui->actionImageEditComment);
    ui->slideshowImageListView->addAction(ui->actionRenameImage);
    ui->slideshowImageListView->addAction(ui->actionCutImage);
    ui->slideshowImageListView->addAction(ui->actionCopyImage);
    ui->slideshowImageListView->addAction(ui->actionPasteImage);
    ui->slideshowImageListView->addAction(ui->actionRemoveImage);
    ui->slideshowImageListView->addAction(ui->actionRemoveImageFromDisk);
    ui->slideshowImageListView->addAction(ui->actionCopyPath);
    ui->slideshowImageListView->addAction(ui->actionOpenFileLocation);
    ui->slideshowImageListView->addAction(ui->actionExportImages);
    ui->slideshowImageListView->addAction(ui->actionRemoveAllCorruptedImages);
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
    ui->imageWidget->addAction(ui->actionOpenFileLocation);
    ui->imageWidget->addAction(ui->actionExportImages);
    ui->imageWidget->addAction(ui->actionShowComments);

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

    connect(ui->multipleUpPushButton, SIGNAL(customContextMenuRequested(QPoint)), SLOT(multipleMoveCountDialog()));
    connect(ui->multipleDownPushButton, SIGNAL(customContextMenuRequested(QPoint)), SLOT(multipleMoveCountDialog()));

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

    // image settings
    ui->imageListSortComboBox->setCurrentIndex(settings.value("imageSort", 0).toInt());

    // slideshow options
    ui->intervalSpinBox->setValue(settings.value("interval", 5).toInt());
    ui->randomCheckBox->setChecked(settings.value("random", false).toBool());
    ui->repeatCheckBox->setChecked(settings.value("repeat", false).toBool());
    m_multipleMoveCount = settings.value("multipleMoveCount", 5).toInt();

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
    ui->actionShowComments->setChecked(settings.value("showComments", true).toBool());

    // slideshows and images directory
    m_slideshowsDirectory = settings.value("slideshowsDirectory", OptionsDialog::DEFAULT_SLIDESHOWS_DIRECTORY).toString();
    OptionsDialog::checkDirectory(m_slideshowsDirectory);
    m_imagesDirectory = settings.value("imagesDirectory", OptionsDialog::DEFAULT_IMAGES_DIRECTORY).toString();
    OptionsDialog::checkDirectory(m_imagesDirectory, true, false);

    // window shortcuts
    OptionsDialog::DefaultShortcuts.insert(ui->actionExit, QKeySequence::Quit);
    OptionsDialog::DefaultShortcuts.insert(ui->actionOptions, QKeySequence::Preferences);
    //OptionsDialog::DefaultShortcuts.insert(ui->actionOpenSlideshowsDirectory, 0);
    //OptionsDialog::DefaultShortcuts.insert(ui->actionOpenImagesDirectory, 0);
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
    //OptionsDialog::DefaultShortcuts.insert(ui->actionOpenFileLocation, 0);
    OptionsDialog::DefaultShortcuts.insert(ui->actionExportImages, Qt::CTRL + Qt::Key_E);
    //OptionsDialog::DefaultShortcuts.insert(ui->actionRemoveAllCorruptedImages, 0);
    //OptionsDialog::DefaultShortcuts.insert(ui->actionPreloadAllImages, 0);

    // slideshow shortcuts
    OptionsDialog::DefaultShortcuts.insert(ui->actionNewSlideshow,QKeySequence::New);
    OptionsDialog::DefaultShortcuts.insert(ui->actionReloadAllSlideshows, Qt::CTRL + Qt::SHIFT + Qt::Key_F5);
    OptionsDialog::DefaultShortcuts.insert(ui->actionSaveAllSlideshows, Qt::CTRL + Qt::SHIFT + Qt::Key_S);
    OptionsDialog::DefaultShortcuts.insert(ui->actionStartSlideshow, Qt::Key_MediaPlay);
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
    //OptionsDialog::DefaultShortcuts.insert(ui->actionShowComments, 0);

    OptionsDialog::loadAllShortcuts(settings);
}

void MainWindow::saveSettings()
{
    QSettings settings;
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
    settings.setValue("imageSort", ui->imageListSortComboBox->currentIndex());
    settings.setValue("interval", ui->intervalSpinBox->value());
    settings.setValue("random", ui->randomCheckBox->isChecked());
    settings.setValue("repeat", ui->repeatCheckBox->isChecked());
    settings.setValue("multipleMoveCount", m_multipleMoveCount);
    settings.setValue("imageListViewThumbnailSize", ui->imageListView->iconSize().width());
    settings.setValue("slideshowImageListViewThumbnailSize", ui->slideshowImageListView->iconSize().width());
    settings.setValue("showComments", ui->actionShowComments->isChecked());
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
    QDir::SortFlags sort;
    switch (ui->imageListSortComboBox->currentIndex()) {
    case 0: sort = QDir::Name; break;
    case 1: sort = QDir::Name | QDir::Reversed; break;
    case 2: sort = QDir::Time; break;
    case 3: sort = QDir::Time | QDir::Reversed; break;
    }
    m_scanFolderThread->setFolder(m_folderBrowserModel->filePath(index), sort, includeSubfolders);
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

    // enable image view actions if the first image gets set
    if (m_currentImagePath.isEmpty()) {
        ui->imageWidget->setEnabled(true);
        ui->actionRotateClockwise->setEnabled(true);
        ui->actionRotateCounterclockwise->setEnabled(true);
        ui->actionZoomFit->setEnabled(true);
        ui->actionZoomOriginal->setEnabled(true);
        ui->actionZoomIn->setEnabled(true);
        ui->actionZoomOut->setEnabled(true);
    }

    m_currentImagePath = imagePath;
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
    if (!index.isValid())
        return;

    prepareImage(m_imageListModel, index);
    ui->imageWidget->setText();
    m_currentSlideshow = NULL;
    m_currentSlideshowImage = NULL;
}

void MainWindow::on_imageListView_doubleClicked(const QModelIndex &index)
{
    if (!index.isValid() || !m_slideshowListModel->currentSlideshow())
        return;

    int i = m_slideshowListModel->addImage(SlideshowImage(m_imageListModel->imagePath(index)));
    m_slideshowImageListModel->addImage(m_imageListModel->imageInfo(index), i);
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
        menu.addAction(ui->actionOpenFileLocation);
        menu.addAction(ui->actionExportImages);
        menu.addSeparator();
    } else if (ui->actionPasteImage->isEnabled() && ui->folderBrowserTreeView->currentIndex().isValid()) {
        menu.addAction(ui->actionPasteImage);
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
    if (!index.isValid() || !m_slideshowListModel->setCurrentSlideshow(index))
        return;

    Slideshow *slideshow = m_slideshowListModel->currentSlideshow();

    // prepare slideshow image list
    m_slideshowImageListModel->clear();
    QFileInfoList imageInfoList;
    foreach (const SlideshowImage &image, slideshow->images())
        imageInfoList.append(image.fileInfo());
    m_slideshowImageListModel->addImages(imageInfoList);

    // update sort combo box
    int sortIndex = 4; // Unsorted
    switch (slideshow->sortFlags()) {
    case Qsm::Name: sortIndex = 0; break;
    case Qsm::NameReversed: sortIndex = 1; break;
    case Qsm::Date: sortIndex = 2; break;
    case Qsm::DateReversed: sortIndex = 3; break;
    }
    ui->slideshowSortComboBox->setCurrentIndex(sortIndex);
}

void MainWindow::on_slideshowListView_customContextMenuRequested(const QPoint &pos)
{
    QMenu menu(ui->slideshowListView);
    if (ui->slideshowListView->currentIndex().isValid()) {
        menu.addAction(ui->actionStartSlideshow);
        menu.addAction(ui->actionSlideshowEditComment);
        menu.addAction(ui->actionRenameSlideshow);
        menu.addAction(ui->actionRemoveSlideshow);
        bool imagesAvailable = m_slideshowListModel->currentSlideshow()->imageCount() > 0;
        ui->actionCopyImagesToSlideshow->setEnabled(imagesAvailable);
        menu.addAction(ui->actionCopyImagesToSlideshow);
        menu.addAction(ui->actionOpenFileLocation);
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

void MainWindow::slideshowListModel_slideshowRenamed(const QString &oldName, const QString &newName)
{
    QDir slideshowsDirectory(m_slideshowsDirectory);
    QString oldPath = slideshowsDirectory.filePath(oldName) + Slideshow::FILE_EXTENSION;
    QString newPath = slideshowsDirectory.filePath(newName) + Slideshow::FILE_EXTENSION;
    if (!QFile::rename(oldPath, newPath))
        qWarning("File \"%s\" could not be renamed to \"%s\"", qPrintable(oldPath), qPrintable(newPath));
}

void MainWindow::on_slideshowImageListView_clicked(const QModelIndex &index)
{
    if (!index.isValid())
        return;

    m_currentSlideshow = m_slideshowListModel->currentSlideshow();
    Q_ASSERT(m_currentSlideshow);
    m_currentSlideshowImage = m_currentSlideshow->image(index.row());
    Q_ASSERT(m_currentSlideshowImage);
    prepareImage(m_slideshowImageListModel, index);
    ui->imageWidget->setText(m_currentSlideshowImage->comment());
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
        static const int randomFactors[] = {0, 1, 2, 3, 4, 5, 10, 25, 50, 100};
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
        menu.addAction(ui->actionOpenFileLocation);
        menu.addAction(ui->actionExportImages);
        menu.addSeparator();
    } else if (ui->actionPasteImage->isEnabled() && m_slideshowListModel->currentSlideshow()) {
        menu.addAction(ui->actionPasteImage);
        menu.addSeparator();
    }
    QMenu *thumbnailSizeMenu = createThumbnailSizeMenu(&menu, ui->slideshowImageListView->iconSize().width());
    menu.addMenu(thumbnailSizeMenu);
    if (slideshow && slideshow->hasCorruptedImages())
        menu.addAction(ui->actionRemoveAllCorruptedImages);
    if (!m_slideshowImageListModel->allThumbnailsLoaded())
        menu.addAction(ui->actionPreloadAllImages);
    QAction *action = menu.exec(ui->slideshowImageListView->mapToGlobal(pos));
    if (action && action->parent() == &randomFactorMenu && image) {
        int randomFactor = action->data().toInt();
        foreach (const QModelIndex &index, ui->slideshowImageListView->selectedIndexes())
            slideshow->image(index.row())->setRandomFactor(randomFactor);
        slideshow->setChanged();
    }
}

void MainWindow::slideshowImageListModel_changed()
{
    ui->slideshowImageListDockWidget->setWindowTitle(tr("Slideshow Image List %1/%2")
                                                     .arg(m_slideshowImageListModel->rowCount())
                                                     .arg(m_slideshowImageListModel->imageCount()));
}

void MainWindow::slideshowImageListModel_imageRenamed(const QModelIndex &index, const QString &newPath)
{
    qDebug() << newPath;
    Slideshow *slideshow = m_slideshowListModel->currentSlideshow();
    if (!slideshow) return;
    SlideshowImage *image = slideshow->image(index.row());
    if (!image) return;
    image->setPath(newPath);
    slideshow->setChanged();
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
    if (ui->actionZoomFit->isEnabled())
        statusBar()->showMessage(tr("Zoom: %1%").arg(int(ui->imageWidget->zoomFactor() * 100)));
}

void MainWindow::on_imageWidget_doubleClicked()
{
    Slideshow *slideshow = m_slideshowListModel->currentSlideshow();
    if (m_currentImagePath.isEmpty() || !slideshow)
        return;

    int i = m_slideshowListModel->addImage(m_currentImagePath);
    m_slideshowImageListModel->addImage(ImageInfo(m_currentImagePath), i);
    statusBar()->showMessage(tr("Image added to slideshow \"%1\"").arg(slideshow->name()));
}

void MainWindow::on_imageWidget_customContextMenuRequested(const QPoint &pos)
{
    QMenu menu(ui->imageWidget);
    if (QFile::exists(m_currentImagePath)) {
        ui->actionAddToSlideshow->setEnabled(m_slideshowListModel->currentSlideshow());
        menu.addAction(ui->actionAddToSlideshow);
        menu.addSeparator();
        menu.addAction(ui->actionRotateClockwise);
        menu.addAction(ui->actionRotateCounterclockwise);
        menu.addMenu(ui->menuZoom);
        menu.addSeparator();
        if (m_currentSlideshowImage)
            menu.addAction(ui->actionImageEditComment);
        menu.addAction(ui->actionCutImage);
        menu.addAction(ui->actionCopyImage);
        menu.addAction(ui->actionRemoveImageFromDisk);
        menu.addAction(ui->actionCopyPath);
        menu.addAction(ui->actionOpenFileLocation);
        menu.addAction(ui->actionExportImages);
        menu.addSeparator();
    }
    menu.addAction(ui->actionShowComments);
    menu.exec(ui->imageWidget->mapToGlobal(pos));
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
    windowActions << ui->actionExit << ui->actionOptions << ui->actionOpenSlideshowsDirectory
                  << ui->actionOpenImagesDirectory << ui->actionQsmHelp;
    actions.append(qMakePair(tr("Window"), windowActions));

    // image shortcuts
    QList<QAction*> imageActions;
    imageActions << ui->actionAddToSlideshow << ui->actionImageEditComment << ui->actionRenameImage
                 << ui->actionCutImage << ui->actionCopyImage << ui->actionPasteImage
                 << ui->actionRemoveImage <<  ui->actionRemoveImageFromDisk << ui->actionCopyPath
                 << ui->actionOpenFileLocation << ui->actionExportImages << ui->actionRemoveAllCorruptedImages
                 << ui->actionPreloadAllImages;
    actions.append(qMakePair(tr("Image"), imageActions));

    // slideshow shortcuts
    QList<QAction*> slideshowActions;
    slideshowActions << ui->actionNewSlideshow << ui->actionReloadAllSlideshows << ui->actionSaveAllSlideshows
                     << ui->actionStartSlideshow << ui->actionSlideshowEditComment << ui->actionRenameSlideshow
                     << ui->actionRemoveSlideshow << ui->actionCopyImagesToSlideshow << ui->actionExportAllImages
                     << ui->actionReloadSlideshow << ui->actionSaveSlideshow;
    actions.append(qMakePair(tr("Slideshow"), slideshowActions));

    // image view shortcuts
    QList<QAction*> imageViewActions;
    imageViewActions << ui->actionRotateClockwise << ui->actionRotateCounterclockwise << ui->actionZoomFit
                     << ui->actionZoomOriginal << ui->actionZoomIn << ui->actionZoomOut
                     << ui->actionShowComments;
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

void MainWindow::on_actionOpenSlideshowsDirectory_triggered()
{
    QDesktopServices::openUrl(QUrl("file:///" + m_slideshowsDirectory, QUrl::TolerantMode));
}

void MainWindow::on_actionOpenImagesDirectory_triggered()
{
    QDesktopServices::openUrl(QUrl("file:///" + m_imagesDirectory, QUrl::TolerantMode));
}

void MainWindow::on_actionQsmHelp_triggered()
{
}

void MainWindow::on_actionAboutQsm_triggered()
{
    QMessageBox::about(this, tr("About QSM"),
            tr("<b>Qt SlideShow Manager</b> v0.1 beta<br/><br/>"
               "Copyright &copy; 2011 Fachhochschule S&uuml;dwestfalen<br/>"
               "Written by Alexander Vos and Till Althaus"));
}

void MainWindow::on_actionAddToSlideshow_triggered()
{
    Slideshow *slideshow = m_slideshowListModel->currentSlideshow();
    if (!slideshow) return;

    QWidget *widget = activeWidget(ui->actionAddToSlideshow);
    if (!widget) return;

    if (widget == ui->imageListView) {
        QModelIndexList indexList = ui->imageListView->selectedIndexes();
        foreach (const QModelIndex &index, indexList) {
            int i = m_slideshowListModel->addImage(m_imageListModel->imagePath(index));
            m_slideshowImageListModel->addImage(m_imageListModel->imageInfo(index), i);
        }
        statusBar()->showMessage(tr("%1 image(s) added to slideshow \"%2\"").arg(indexList.count()).arg(slideshow->name()));
    } else if (widget == ui->imageWidget)
        on_imageWidget_doubleClicked();
}

void MainWindow::on_actionImageEditComment_triggered()
{
    QWidget *widget = activeWidget(ui->actionImageEditComment);
    if (!widget) return;

    Slideshow *slideshow = NULL;
    QList<SlideshowImage*> images;
    if (widget == ui->slideshowImageListView) {
        slideshow = m_slideshowListModel->currentSlideshow();
        if (!slideshow) return;
        foreach (const QModelIndex &index, ui->slideshowImageListView->selectedIndexes())
            images.append(slideshow->image(index.row()));
    } else if (widget == ui->imageWidget && m_currentSlideshowImage) {
        slideshow = m_currentSlideshow;
        images.append(m_currentSlideshowImage);
    }
    if (!slideshow || !images.count()) return;

    bool ok;
    QString currentComment = slideshow->image(ui->slideshowImageListView->currentIndex().row())->comment();
    QString comment = QInputDialog::getText(this, tr("Edit Comment"), tr("Comment:"),
                                            QLineEdit::Normal, currentComment, &ok);
    if (ok) {
        foreach (SlideshowImage *image, images) {
            image->setComment(comment);
            if (image == m_currentSlideshowImage)
                ui->imageWidget->setText(comment);
        }
        slideshow->setChanged();
    }
}

void MainWindow::on_actionRenameImage_triggered()
{
    QWidget *widget = activeWidget(ui->actionRenameImage);
    if (!widget) return;
    ImageListView *view = qobject_cast<ImageListView*>(widget);
    if (!view) return;
    ImageListModel* model = qobject_cast<ImageListModel*>(view->model());
    if (!model) return;
    QModelIndexList indexList = view->selectedIndexes();
    if (indexList.isEmpty())
        return;
    else if (indexList.count() == 1) // inline rename
        view->edit(view->currentIndex());
    else // show dialog to rename multiple images
        ;
}

void MainWindow::on_actionCutImage_triggered()
{
    m_copyMode = Cut;
    m_clipboard = imagePaths(ui->actionCutImage);
    if (m_clipboard.isEmpty())
        return;
    statusBar()->showMessage(tr("%1 image(s) cut out").arg(m_clipboard.count()));
    ui->actionPasteImage->setEnabled(true);
}

void MainWindow::on_actionCopyImage_triggered()
{
    m_copyMode = Copy;
    m_clipboard = imagePaths(ui->actionCopyImage);
    if (m_clipboard.isEmpty())
        return;
    statusBar()->showMessage(tr("%1 image(s) copied").arg(m_clipboard.count()));
    ui->actionPasteImage->setEnabled(true);
}

void MainWindow::on_actionPasteImage_triggered()
{
    QWidget *widget = activeWidget(ui->actionPasteImage);
    if (!widget || m_clipboard.isEmpty()) return;
    // insert images from "clipboard"
    int count = 0;
    if (widget == ui->imageListView) {
        QModelIndex folderIndex = ui->folderBrowserTreeView->currentIndex();
        if (!folderIndex.isValid()) return;
        QDir dir = m_folderBrowserModel->filePath(folderIndex);
        foreach (const QString &src, m_clipboard) {
            QString dst = dir.filePath(QFileInfo(src).fileName());
            if (QFile::copy(src, dst)) {
                count++;
                if (!QFile::remove(src))
                    qWarning("File \"%s\" could not be removed", qPrintable(src));
            } else
                qWarning("File \"%s\" could not be copied to \"%s\"", qPrintable(src), qPrintable(dst));
        }
        if (count > 0) // refresh view
            scanFolder(folderIndex, ui->includeSubfoldersCheckBox->isChecked());
    } else if (widget == ui->slideshowImageListView) {
        Slideshow *slideshow = m_slideshowListModel->currentSlideshow();
        if (!slideshow) return;
        foreach (const QString &path, m_clipboard) {
            int i = slideshow->addImage(path);
            m_slideshowImageListModel->addImage(ImageInfo(path), i);
            count++;
        }
        // refresh view
        ui->slideshowListView->update(m_slideshowListModel->currentSlideshowIndex());
        //on_slideshowListView_selectionChanged(m_slideshowListModel->currentSlideshowIndex());
    } else
        return;
    // clean the clipboard and disable the paste action
    if (m_copyMode == Cut) {
        m_clipboard.clear();
        ui->actionPasteImage->setEnabled(false);
    }
    statusBar()->showMessage(tr("%1 image(s) inserted").arg(count));
}

void MainWindow::on_actionRemoveImage_triggered()
{
    Slideshow *slideshow = m_slideshowListModel->currentSlideshow();
    if (!slideshow) return;
    QModelIndexList indexList = ui->slideshowImageListView->selectedIndexes();
    foreach (const QModelIndex &index, indexList) {
        m_slideshowListModel->removeImage(index);
        m_slideshowImageListModel->removeImage(index);
        if (slideshow->image(index.row()) == m_currentSlideshowImage)
            m_currentSlideshowImage = NULL;
    }
    statusBar()->showMessage(tr("%1 image(s) removed from slideshow \"%2\"").arg(indexList.count()).arg(slideshow->name()));
}

QStringList MainWindow::imagePaths(QAction *action)
{
    Q_ASSERT(action);
    QWidget *widget = activeWidget(action);
    if (!widget) return QStringList();

    QStringList paths;
    if (widget == ui->imageListView)
        foreach (const QModelIndex &index, ui->imageListView->selectedIndexes())
            paths.append(m_imageListModel->imagePath(index));
    else if (widget == ui->slideshowImageListView)
        foreach (const QModelIndex &index, ui->slideshowImageListView->selectedIndexes())
            paths.append(m_slideshowImageListModel->imagePath(index));
    else if (widget == ui->imageWidget)
        paths.append(m_currentImagePath);

    return paths;
}

void MainWindow::on_actionRemoveImageFromDisk_triggered()
{
    QStringList paths = imagePaths(ui->actionRemoveImageFromDisk);
    if (QMessageBox::question(this, tr("Remove Image(s)"), tr("Are you sure you want to remove the %1 image(s) from disk?")
            .arg(paths.count()), QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes) {
        QWidget *widget = activeWidget(ui->actionRemoveImageFromDisk);
        if (widget == ui->slideshowImageListView) // remove images also from slideshow
            on_actionRemoveImage_triggered();
        // remove images from disk
        foreach (const QString &path, paths)
            if (!QFile::remove(path))
                qWarning("File \"%s\" could not be removed", qPrintable(path));
        // update view
        if (widget == ui->imageListView)
            foreach (const QModelIndex &index, ui->imageListView->selectedIndexes())
                m_imageListModel->removeImage(index);
        statusBar()->showMessage(tr("%1 image(s) removed from disk").arg(paths.count()));
    }
}

void MainWindow::on_actionCopyPath_triggered()
{
    QStringList paths = imagePaths(ui->actionCopyPath);

#ifdef Q_WS_WIN
    // convert to native paths on windows
    for (QStringList::iterator path = paths.begin(); path != paths.end(); ++path)
        *path = QDir::toNativeSeparators(*path);
#endif

    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(paths.join("\n"));

    statusBar()->showMessage(tr("%1 image path(s) copied to the clipboard").arg(paths.count()));
}

void MainWindow::on_actionOpenFileLocation_triggered()
{
    QWidget *widget = activeWidget(ui->actionOpenFileLocation);
    if (!widget) return;
    QString path;
    if (widget == ui->imageListView)
        path = m_imageListModel->imagePath(ui->imageListView->currentIndex());
    else if (widget == ui->slideshowListView) {
        if (m_slideshowListModel->currentSlideshow())
            path = m_slideshowListModel->currentSlideshow()->path(m_slideshowsDirectory);
    } else if (widget == ui->slideshowImageListView)
        path = m_slideshowImageListModel->imagePath(ui->slideshowImageListView->currentIndex());
    else if (widget == ui->imageWidget)
        path = m_currentImagePath;
    QDir dir = QDir(path);
    dir.cdUp();
    path = dir.path();
    QDesktopServices::openUrl(QUrl("file:///" + path, QUrl::TolerantMode));
}

void MainWindow::on_actionExportImages_triggered()
{
    if (!OptionsDialog::checkDirectory(m_imagesDirectory)) return;
    QStringList paths = imagePaths(ui->actionExportImages);
    if (paths.isEmpty()) return;
    int count = 0;
    foreach (const QString &originalPath, paths) {
        QString exportPath = QDir(m_imagesDirectory).filePath(QFileInfo(originalPath).fileName());
        if (QFile::copy(originalPath, exportPath))
            count++;
        else
            qWarning("File \"%s\" could not be copied to \"%s\"", qPrintable(originalPath), qPrintable(exportPath));
    }
    statusBar()->showMessage(tr("%1 of %2 image(s) copied to \"%3\"")
            .arg(count).arg(paths.count()).arg(QDir::toNativeSeparators(m_imagesDirectory)));
}

void MainWindow::on_actionRemoveAllCorruptedImages_triggered()
{
    Slideshow *slideshow = m_slideshowListModel->currentSlideshow();
    if (slideshow) {
        slideshow->removeAllCorruptedImages();
        // refresh view
        on_slideshowListView_selectionChanged(ui->slideshowListView->currentIndex());
    }
}

void MainWindow::on_actionPreloadAllImages_triggered()
{
    QWidget *widget = activeWidget(ui->actionPreloadAllImages);
    if (!widget) return;
    ImageListView *view = qobject_cast<ImageListView*>(widget);
    if (view) {
        ImageListModel* model = qobject_cast<ImageListModel*>(view->model());
        if (model)
            model->preloadAllImages();
    }
}

void MainWindow::on_actionStartSlideshow_triggered()
{
    Slideshow *slideshow = m_slideshowListModel->currentSlideshow();
    if (!slideshow || !slideshow->imageCount())
        return;

    SlideshowWindow *slideshowWindow = new SlideshowWindow(slideshow, ui->intervalSpinBox->value(),
            ui->randomCheckBox->isChecked(), ui->repeatCheckBox->isChecked());
    ImageWidget *imageWidget = slideshowWindow->imageWidget();
    imageWidget->setBackgroundColor(ui->imageWidget->backgroundColor());
    imageWidget->setTextBackgroundColor(ui->imageWidget->textBackgroundColor());
    imageWidget->setTextFont(ui->imageWidget->textFont());
    imageWidget->setTextColor(ui->imageWidget->textColor());
    imageWidget->setTextVisibility(ui->imageWidget->isTextVisible());
    slideshowWindow->showFullScreen();
}

void MainWindow::on_actionSlideshowEditComment_triggered()
{
    Slideshow *slideshow = m_slideshowListModel->currentSlideshow();
    if (!slideshow) return;

    bool ok;
    QString comment = QInputDialog::getText(this, tr("Edit Comment"), tr("Comment:"),
                                            QLineEdit::Normal, slideshow->comment(), &ok);
    if (ok)
        slideshow->setComment(comment);
}

void MainWindow::on_actionRenameSlideshow_triggered()
{
    ui->slideshowListView->edit(ui->slideshowListView->currentIndex());
}

void MainWindow::on_actionRemoveSlideshow_triggered()
{
    Slideshow *slideshow = m_slideshowListModel->currentSlideshow();
    if (!slideshow) return;
    if (QMessageBox::question(this, tr("Remove Slideshow"), tr("Are you sure you want to remove the slideshow \"%1\"?")
            .arg(slideshow->name()), QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes) {
        if (slideshow == m_currentSlideshow) {
            m_currentSlideshow = NULL;
            m_currentSlideshowImage = NULL;
        }
        QString path = slideshow->path(m_slideshowsDirectory);
        if (m_slideshowListModel->removeSlideshow(ui->slideshowListView->currentIndex()) && !QFile::remove(path))
            qWarning("File \"%s\" could not be removed", qPrintable(path));
    }
}

void MainWindow::on_actionCopyImagesToSlideshow_triggered()
{
    Slideshow *slideshowFrom = m_slideshowListModel->currentSlideshow();
    if (!slideshowFrom) return;
    QStringList slideshowList;
    foreach (const Slideshow &slideshow, m_slideshowListModel->slideshowList())
        slideshowList.append(slideshow.name());
    bool ok;
    QString slideshowString = QInputDialog::getItem(this, tr("Select Slideshow"),
            tr("Copy all images to slideshow:"), slideshowList, 0, true, &ok);
    if (ok && !slideshowString.isEmpty()) {
        Slideshow *slideshowTo = m_slideshowListModel->slideshow(slideshowString);
        if (!slideshowTo) { // slideshow not found, create new one
            QModelIndex slideshowIndex = m_slideshowListModel->addSlideshow(slideshowString);
            slideshowTo = m_slideshowListModel->slideshow(slideshowIndex);
            if (!slideshowTo) return; // should never happen, just in case
        }
        // copy all images
        foreach (const SlideshowImage &image, slideshowFrom->images())
            slideshowTo->addImage(image);
        statusBar()->showMessage(tr("%1 image(s) copied from slideshow \"%2\" to \"%3\"")
                .arg(slideshowFrom->imageCount()).arg(slideshowFrom->name()).arg(slideshowTo->name()));
    }
}

void MainWindow::on_actionExportAllImages_triggered()
{
    if (!OptionsDialog::checkDirectory(m_imagesDirectory)) return;
    Slideshow *slideshow = m_slideshowListModel->currentSlideshow();
    if (!slideshow) return;
    int count = 0;
    foreach (const SlideshowImage &image, slideshow->images()) {
        QString originalPath = image.path();
        QString exportPath = QDir(m_imagesDirectory).filePath(image.name());
        if (QFile::copy(originalPath, exportPath))
            count++;
        else
            qWarning("File \"%s\" could not be copied to \"%s\"", qPrintable(originalPath), qPrintable(exportPath));
    }
    statusBar()->showMessage(tr("%1 of %2 image(s) copied to \"%3\"")
            .arg(count).arg(slideshow->imageCount()).arg(QDir::toNativeSeparators(m_imagesDirectory)));
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

void MainWindow::on_imageListSortComboBox_currentIndexChanged(int)
{
    // rescan folder
    scanFolder(ui->folderBrowserTreeView->currentIndex(), ui->includeSubfoldersCheckBox->isChecked());
}

void MainWindow::on_includeSubfoldersCheckBox_toggled(bool checked)
{
    QModelIndex index = ui->folderBrowserTreeView->currentIndex();
    if (index.isValid())
        scanFolder(index, checked);
}

void MainWindow::on_slideshowSortComboBox_currentIndexChanged(int index)
{
    Slideshow *slideshow = m_slideshowListModel->currentSlideshow();
    if (!slideshow) return;

    Qsm::SortFlags sort;
    switch (index) {
    case 0: sort = Qsm::Name; break;
    case 1: sort = Qsm::NameReversed; break;
    case 2: sort = Qsm::Date; break;
    case 3: sort = Qsm::DateReversed; break;
    case 4: sort = Qsm::Unsorted; break;
    }

    if (slideshow->setSortFlags(sort) && !(sort & Qsm::Unsorted))
        on_slideshowListView_selectionChanged(ui->slideshowListView->currentIndex()); // refresh view
}

void MainWindow::moveImages(int delta)
{
    Slideshow *slideshow = m_slideshowListModel->currentSlideshow();
    if (!slideshow || slideshow->imageCount() < 2) return;

    QModelIndexList indexList = ui->slideshowImageListView->selectedIndexes();
    if (indexList.isEmpty()) return;

    // set to unsorted
    if (!(slideshow->sortFlags() & Qsm::Unsorted))
        ui->slideshowSortComboBox->setCurrentIndex(4);

    // move images
    QModelIndexList::const_iterator begin;
    QModelIndexList::const_iterator end;
    int direction;
    if (delta < 0) { // move up, begin with smallest index
        begin = indexList.constBegin();
        end = indexList.constEnd() - 1;
        direction = 1;
    } else { // move down, begin with largest index
        begin = indexList.constEnd() - 1;
        end = indexList.constBegin();
        direction = -1;
    }
    end += direction;
    for (QModelIndexList::const_iterator i = begin; i != end; i += direction) {
        slideshow->moveImage((*i).row(), delta);
        //m_slideshowImageListModel->moveImage(*i, delta); // BUG
    }
    // refresh view
    on_slideshowListView_selectionChanged(ui->slideshowListView->currentIndex());
}

void MainWindow::on_beginPushButton_clicked()
{
    moveImages(INT_MIN >> 1);
}

void MainWindow::on_multipleUpPushButton_clicked()
{
    moveImages(-m_multipleMoveCount);
}

void MainWindow::on_upPushButton_clicked()
{
    moveImages(-1);
}

void MainWindow::on_downPushButton_clicked()
{
    moveImages(1);
}

void MainWindow::on_multipleDownPushButton_clicked()
{
    moveImages(m_multipleMoveCount);
}

void MainWindow::on_endPushButton_clicked()
{
    moveImages(INT_MAX >> 1);
}

void MainWindow::multipleMoveCountDialog()
{
    bool ok;
    int moveCount = QInputDialog::getInt(this, tr("Select Multiple Move Count"),
            tr("Move Count:"), m_multipleMoveCount, 2, INT_MAX / 2, 1, &ok);
    if (ok)
        m_multipleMoveCount = moveCount;
}

void MainWindow::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() & Qt::ControlModifier) {
        QFont font = qApp->font();
        int delta = event->delta() / 120;
        int pointSize = qBound(6, font.pointSize() + delta, 72);
        font.setPointSize(pointSize);
        qApp->setFont(font);
        statusBar()->showMessage(tr("Changed font size to %1 points").arg(pointSize));
        event->accept();
    }
}
