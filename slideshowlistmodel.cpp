#include "slideshowlistmodel.h"

#include <QBrush>

SlideshowListModel::SlideshowListModel(QObject *parent) :
    QAbstractListModel(parent), m_currentSlideshow(NULL)
{
}

Qt::ItemFlags SlideshowListModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
}

int SlideshowListModel::rowCount(const QModelIndex &) const
{
    return m_slideshowList.count();
}

QVariant SlideshowListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_slideshowList.count())
        return QVariant();

    if (role == Qt::DisplayRole) {
        const Slideshow *slideshow = &m_slideshowList.at(index.row());
        return QString("%1 (%2 images)").arg(slideshow->name()).arg(slideshow->imageCount());
    }
    else if (role == Qt::ToolTipRole)
        return m_slideshowList.at(index.row()).comment();
    else if (role == Qt::EditRole)
        return m_slideshowList.at(index.row()).name();

    return QVariant();
}

bool SlideshowListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || index.row() >= m_slideshowList.count())
        return false;

    if (role == Qt::EditRole) {
        m_slideshowList[index.row()].setName(value.toString());
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

Slideshow* SlideshowListModel::slideshow(const QModelIndex &index) const
{
    if (!index.isValid() || index.row() >= m_slideshowList.count())
        return NULL;

    return const_cast<Slideshow*>(&m_slideshowList[index.row()]);
}

QModelIndex SlideshowListModel::addSlideshow(const Slideshow &slideshow)
{
    beginInsertRows(QModelIndex(), m_slideshowList.count(), m_slideshowList.count());
    m_slideshowList.append(slideshow);
    endInsertRows();
    return index(m_slideshowList.count() - 1);
}

bool SlideshowListModel::removeSlideshow(const QModelIndex &index)
{
    if (!index.isValid() || index.row() >= m_slideshowList.count())
        return false;

    beginRemoveRows(QModelIndex(), index.row(), index.row());
    m_slideshowList.removeAt(index.row());
    if (index == m_currentSlideshowIndex) {
        m_currentSlideshow = NULL;
        m_currentSlideshowIndex = QModelIndex();
    }
    endRemoveRows();

    return true;
}

void SlideshowListModel::clear()
{
    beginResetModel();
    m_slideshowList.clear();
    m_currentSlideshow = NULL;
    m_currentSlideshowIndex = QModelIndex();
    endResetModel();
}

void SlideshowListModel::addImage(const SlideshowImage &image)
{
    if (!m_currentSlideshow)
        return;

    m_currentSlideshow->addImage(image);
    emit dataChanged(m_currentSlideshowIndex, m_currentSlideshowIndex);
}

void SlideshowListModel::removeImage(const QModelIndex &index)
{
    if (!m_currentSlideshow)
        return;

    m_currentSlideshow->removeImage(index.row());
    emit dataChanged(m_currentSlideshowIndex, m_currentSlideshowIndex);
}

bool SlideshowListModel::setCurrentSlideshow(const QModelIndex &index)
{
    if (!index.isValid() || index.row() >= m_slideshowList.count())
        return false;

    m_currentSlideshow = &m_slideshowList[index.row()];
    m_currentSlideshowIndex = index;
    return true;
}
