#include "slideshowlistview.h"

SlideshowListView::SlideshowListView(QWidget *parent) :
    QListView(parent)
{
}

void SlideshowListView::selectionChanged(const QItemSelection &selected, const QItemSelection &)
{
    if (selected.isEmpty())
        return;

    emit selectionChanged(selected.indexes().at(0));
}
