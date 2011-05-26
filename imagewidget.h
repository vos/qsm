/****************************************************************************
**
** Copyright (C) 2011 Alexander Vos <alexander.vos@stud.fh-swf.de>,
**                    Till Althaus <till.althaus@stud.fh-swf.de>
**
** This file is part of Qt Slideshow Manager (QSM).
**
** QSM is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** QSM is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with QSM.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QGLWidget>
#include <QImage>
#include <QTimer>

class ImageWidget : public QGLWidget
{
    Q_OBJECT

public:
    enum ImageMode {
        FitToWidget,
        ManualAdjustment
    };

    explicit ImageWidget(QWidget *parent = 0);
    ~ImageWidget();

    inline const QColor& backgroundColor() const { return m_backgroundColor; }
    inline const QString& text() const { return m_text; }
    inline const QColor& textBackgroundColor() const { return m_textBackgroundColor; }
    inline const QFont& textFont() const { return m_textFont; }
    inline const QColor& textColor() const { return m_textColor; }
    inline bool isTextVisible() const { return m_textVisible; }
    inline ImageMode imageMode() const { return m_imageMode; }
    inline const QPoint& translation() const { return m_translate; }
    inline double zoomFactor() const { return m_zoom; }
    inline double rotation() const { return m_rotate; }

signals:
    void initialized();
    void viewChanged();
    void doubleClicked();

public slots:
    void setBackgroundColor(const QColor &color = Qt::black);
    void setImage(const QImage &image);
    void setText(const QString &text = QString());
    void setOverlayText(const QString &text = QString(), int timeout = 1000);
    void setTextBackgroundColor(const QColor &color = QColor(0, 0, 0, 127));
    void setTextFont(const QFont &font);
    void setTextColor(const QColor &color = Qt::white);
    void setTextVisibility(bool visible = true);
    bool toggleTextVisibility();
    void setImageMode(ImageMode mode = FitToWidget);
    void translateTo(const QPoint &pos = QPoint());
    inline void translate(const QPoint &delta) { translateTo(m_translate + delta); }
    inline void translate(int dx, int dy) { translate(QPoint(dx, dy)); }
    void zoomTo(double factor = 1.0);
    inline void zoomIn(double factor = 1.15) { zoomTo(m_zoom * factor); }
    inline void zoomOut(double factor = 1.15) { zoomIn(1.0 / factor); }
    void rotateTo(double angle = 0.0);
    inline void rotateClockwise(double angle = 90.0) { rotateTo(m_rotate + angle); }
    inline void rotateCounterclockwise(double angle = 90.0) { rotateClockwise(-angle); }

private:
    QColor m_backgroundColor;
    GLuint m_textureId;
    int m_imageWidth;
    int m_imageHeight;
    int m_actualImageWidth;
    int m_actualImageHeight;
    QString m_text;
    QString m_overlayText;
    QColor m_textBackgroundColor;
    QFont m_textFont;
    QColor m_textColor;
    bool m_textVisible;
    ImageMode m_imageMode;
    QPoint m_lastPos;
    QPoint m_translate;
    double m_zoom;
    double m_rotate;
    Qt::CursorShape m_cursor;
    QTimer m_overlayTimer;

    void initializeGL();
    void setupViewport(int width, int height);
    void paintEvent(QPaintEvent *event);
    void drawText(QPainter *painter);
    void drawOverlay(QPainter *painter);
    void mousePressEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
};

#endif // IMAGEWIDGET_H
