#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QGLWidget>
#include <QImage>

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

    inline double zoomFactor() const { return m_zoom; }
    inline double rotation() const { return m_rotate; }

signals:
    void viewportChanged();

public slots:
    void setBackgroundColor(const QColor &color = Qt::black);
    void setImage(const QImage &image);
    void setText(const QString &text = QString());
    void setTextBackgroundColor(const QColor &color = QColor(0, 0, 0, 127));
    void setTextColor(const QColor &color = Qt::white);
    void setTextVisible(bool visible = true);
    void setImageMode(ImageMode mode = FitToWidget);
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
    int m_currentImageWidth;
    int m_currentImageHeight;
    QString m_text;
    QColor m_textBackgroundColor;
    QColor m_textColor;
    bool m_textVisible;
    ImageMode m_imageMode;
    QPoint m_lastPos;
    QPoint m_translate;
    double m_zoom;
    double m_rotate;

    void initializeGL();
    void setupViewport(int width, int height);
    void paintEvent(QPaintEvent *event);
    void drawText(QPainter *painter, const QString &text);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
};

#endif // IMAGEWIDGET_H
