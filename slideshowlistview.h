#ifndef SLIDESHOWLISTVIEW_H
#define SLIDESHOWLISTVIEW_H

#include <QListView>

class SlideshowListView : public QListView
{
    Q_OBJECT

public:
    explicit SlideshowListView(QWidget *parent = 0);

signals:
    void selectionChanged(const QModelIndex &index);

protected slots:
    void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

};

#endif // SLIDESHOWLISTVIEW_H
