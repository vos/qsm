#ifndef IMAGELISTVIEW_H
#define IMAGELISTVIEW_H

#include <QListView>

class ImageListView : public QListView
{
    Q_OBJECT

public:
    explicit ImageListView(QWidget *parent = 0);

    inline QModelIndexList selectedIndexes() const { return QListView::selectedIndexes(); }
};

#endif // IMAGELISTVIEW_H
