#include "slideshowlistmodel.h"

SlideshowListModel::SlideshowListModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

int SlideshowListModel::rowCount(const QModelIndex &) const
{
    return m_slideshowList.count();
}

QVariant SlideshowListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_slideshowList.count())
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::ToolTipRole || role == Qt::EditRole)
        return QString("%1: ").arg(index.row()) + m_slideshowList.at(index.row()).name();

    return QVariant();
}

void SlideshowListModel::addSlideshow(const Slideshow &slideshow)
{
    beginInsertRows(QModelIndex(), m_slideshowList.count(), m_slideshowList.count());
    m_slideshowList.append(slideshow);
    endInsertRows();
}

Slideshow* SlideshowListModel::slideshow(const QModelIndex &index) const
{
    if (!index.isValid() || index.row() >= m_slideshowList.count())
        return NULL;

    return const_cast<Slideshow*>(&m_slideshowList[index.row()]);
}

bool SlideshowListModel::removeSlideshow(const QModelIndex &index)
{
    if (!index.isValid() || index.row() >= m_slideshowList.count())
        return false;

    beginRemoveRows(QModelIndex(), index.row(), index.row());
    m_slideshowList.removeAt(index.row());
    endRemoveRows();

    return true;
}

void SlideshowListModel::clear()
{
    beginResetModel();
    m_slideshowList.clear();
    endResetModel();
}
