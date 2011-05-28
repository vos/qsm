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

/*!
  \brief The ImageWidget class provides a OpenGL rendered view to display a
         single image and text with various settings.
 */
class ImageWidget : public QGLWidget
{
    Q_OBJECT

public:

    /*!
      \brief This enum type defines whether the image should be automatically
             adjusted to the widget or not.
     */
    enum ImageMode {

        /*!
          \brief The image will be automatically adjusted to the widget,
                 keeping the acpect ratio in mind.
         */
        FitToWidget,

        /*!
          \brief The image transformations like translation, rotation and scale
                 will not be adjusted automatically.
         */
        ManualAdjustment

    };

    /*!
      \brief Constructs a new ImageWidget with the given \a parent.
      \param parent The parent widget.
      \note The OpenGL context needs to be initialized before the view can be
            displayed or an image can be set.
      \sa initialized()
     */
    explicit ImageWidget(QWidget *parent = 0);

    /*!
      \brief Destroys the ImageWidget and frees all used OpenGL resources.
     */
    ~ImageWidget();

    /*!
      \brief Returns the background color of this widget.
      \return The background color.
      \sa setBackgroundColor()
     */
    inline const QColor& backgroundColor() const { return m_backgroundColor; }

    /*!
      \brief Returns the text which is displayed at the bottom of this widget.
      \return The text.
      \sa setText(), textBackgroundColor(), textFont(), textColor(), isTextVisible()
     */
    inline const QString& text() const { return m_text; }

    /*!
      \brief Returns the background color of the text.
      \return The background color of the text.
      \note The color can be transparent (see alpha value).
      \sa setTextBackgroundColor(), text(), textFont(), textColor(), isTextVisible()
     */
    inline const QColor& textBackgroundColor() const { return m_textBackgroundColor; }

    /*!
      \brief Returns the font which is used for the text at the bottom of the widget.
      \return The text font.
      \sa setTextFont(), text(), textBackgroundColor(), textColor(), isTextVisible()
     */
    inline const QFont& textFont() const { return m_textFont; }

    /*!
      \brief Returns the text color.
      \return The text color.
      \sa setTextFont(), text(), textBackgroundColor(), textColor(), isTextVisible()
     */
    inline const QColor& textColor() const { return m_textColor; }

    /*!
      \brief Returns \c true if the text is visible; otherwise returns \c false.
      \return \c True if the text is visible; otherwise \c false.
      \sa setTextVisibility(), toggleTextVisibility(), text(),
          textBackgroundColor(), textFont(), textColor()
     */
    inline bool isTextVisible() const { return m_textVisible; }

    /*!
      \brief Returns the image mode.
      \return The image mode.
      \sa setImageMode()
     */
    inline ImageMode imageMode() const { return m_imageMode; }

    /*!
      \brief Returns the translation of the image in pixels.
      \return The translation of the image.
      \sa translateTo(), translate()
     */
    inline const QPoint& translation() const { return m_translate; }

    /*!
      \brief Returns the zoom factor of the image.
      \return The zoom factor of the image.
      \note \c 1.0 means original image size (100 percent).
      \sa zoomTo(), zoomIn(), zoomOut()
     */
    inline double zoomFactor() const { return m_zoom; }

    /*!
      \brief Returns the rotation angle of the image in degrees within the
             interval [0, 360).
      \return The rotation angle of the image.
      \sa rotateTo(), rotateClockwise(), rotateCounterclockwise()
     */
    inline double rotation() const { return m_rotate; }

signals:

    /*!
      \brief This signal is emitted after the OpenGL context has been initialized.
     */
    void initialized();

    /*!
      \brief This signal is emitted whenever the view changes.
     */
    void viewChanged();

    /*!
      \brief This signal is emitted whenever the double click event on this
             widget is received.
     */
    void doubleClicked();

public slots:

    /*!
      \brief Sets the background \a color of this widget.
      \param color The color.
      \sa backgroundColor()
     */
    void setBackgroundColor(const QColor &color = Qt::black);

    /*!
      \brief Sets the \a image and displays it immediately. The image mode will
             be reset to \c ImageMode::FitToWidget.
      \param image The image.
      \note The OpenGL context needs to be initialized before an image can be set.
      \sa initialized(), setImageMode()
     */
    void setImage(const QImage &image);

    /*!
      \brief Sets the \a text which is displayed at the bottom of this widget.
      \param text The text.
      \sa text(), textBackgroundColor(), textFont(), textColor(),
          isTextVisible(), setOverlayText()
     */
    void setText(const QString &text = QString());

    /*!
      \brief Sets the overlay \a text which is displayed in the center of this widget.
      \param text The text.
      \param timeout The timeout in milliseconds after the text will fade out.
      \note If the timeout is set to \c 0 or any negative value the text will
            not disappear by itself.
     */
    void setOverlayText(const QString &text = QString(), int timeout = 1000);

    /*!
      \brief Sets the background \a color of the text.
      \param color The background color of the text.
      \note The color can be transparent (see alpha value).
      \sa textBackgroundColor(), text(), textFont(), textColor(), isTextVisible()
     */
    void setTextBackgroundColor(const QColor &color = QColor(0, 0, 0, 127));

    /*!
      \brief Sets the \a font which is used for the text at the bottom of the widget.
      \param font The text font.
      \sa textFont(), text(), textBackgroundColor(), textColor(), isTextVisible()
     */
    void setTextFont(const QFont &font);

    /*!
      \brief Sets the text \a color.
      \param color The text color.
      \sa textColor(), text(), textBackgroundColor(), textFont(), isTextVisible()
     */
    void setTextColor(const QColor &color = Qt::white);

    /*!
      \brief Sets the text visibility to \a visible.
      \param visible \c True if the text should be visible; otherwise \c false.
      \sa isTextVisible(), toggleTextVisibility(), text(),
          textBackgroundColor(), textFont(), textColor()
     */
    void setTextVisibility(bool visible = true);

    /*!
      \brief Toggles the text visibility.
      \return \c True if the text is visible; otherwise \c false.
      \sa isTextVisible(), setTextVisibility(), text(),
          textBackgroundColor(), textFont(), textColor()
     */
    bool toggleTextVisibility();

    /*!
      \brief Sets the image \a mode.
      \param mode The image mode.
      \note If the \a mode is set to \c ImageMode::FitToWidget all
            image transformations will be reset.
      \sa ImageMode, imageMode()
     */
    void setImageMode(ImageMode mode = FitToWidget);

    /*!
      \brief Translates the image to the given position \a pos.
      \param pos The position in pixels to shift the image to.
      \note The origin is the bottom-left corner of the widget.
      \sa translation(), translate()
     */
    void translateTo(const QPoint &pos = QPoint());

    /*!
      \brief Translates the image relative by \a delta pixels.
      \param delta The point containing the relative translation in pixels.
      \sa translation(), translateTo()
     */
    inline void translate(const QPoint &delta) { translateTo(m_translate + delta); }

    /*!
      \brief Translates the image relative by \a dx and \a dy pixels.
      \param dx The translation in horizontal direction in pixels.
      \param dy The translation in vertical direction in pixels.
      \sa translation(), translateTo()
     */
    inline void translate(int dx, int dy) { translate(QPoint(dx, dy)); }

    /*!
      \brief Zoomes the image to the given \a factor.
      \param factor The absolute zoom factor.
      \note \c 1.0 means original image size (100 percent).
      \note The origin is the center of the image.
      \sa zoomFactor(), zoomIn(), zoomOut()
     */
    void zoomTo(double factor = 1.0);

    /*!
      \brief Zoomes the image in by the given \a factor.
      \param factor The relative zoom factor.
      \note The origin is the center of the image.
      \sa zoomFactor(), zoomTo(), zoomOut()
     */
    inline void zoomIn(double factor = 1.15) { zoomTo(m_zoom * factor); }

    /*!
      \brief Zoomes the image out by th given \a factor.
      \param factor The relative zoom factor.
      \note The origin is the center of the image.
      \sa zoomFactor(), zoomTo(), zoomIn()
     */
    inline void zoomOut(double factor = 1.15) { zoomIn(1.0 / factor); }

    /*!
      \brief Rotates the image to the given \a angle in degrees.
      \param angle The rotation in degrees within the interval [0, 360).
      \note The origin is the center of the image.
      \sa rotation(), rotateClockwise(), rotateCounterclockwise()
     */
    void rotateTo(double angle = 0.0);

    /*!
      \brief Rotates the image clockwise by the given \a angle in degrees.
      \param angle The rotation in degrees within the interval [0, 360).
      \note The origin is the center of the image.
      \sa rotation(), rotateTo(), rotateCounterclockwise()
     */
    inline void rotateClockwise(double angle = 90.0) { rotateTo(m_rotate + angle); }

    /*!
      \brief Rotates the image counterclockwise by the given \a angle in degrees.
      \param angle The rotation in degrees within the interval [0, 360).
      \note The origin is the center of the image.
      \sa rotation(), rotateTo(), rotateClockwise()
     */
    inline void rotateCounterclockwise(double angle = 90.0) { rotateClockwise(-angle); }

private:

    QColor m_backgroundColor;     //!< The background color of this widget.
    GLuint m_textureId;           //!< The OpenGL texture ID of the current image.
    int m_imageWidth;             //!< The real image width of the current image.
    int m_imageHeight;            //!< The real image height of the current image.
    int m_actualImageWidth;       //!< The actual image width of the current image.
    int m_actualImageHeight;      //!< The actual image height of the current image.
    QString m_text;               //!< The text which is displayed at the bottom of this widget.
    QString m_overlayText;        //!< The overlay text which is displayed in the center of this widget.
    QColor m_textBackgroundColor; //!< The background color of the text.
    QFont m_textFont;             //!< The font which is used for the text at the bottom of the widget
    QColor m_textColor;           //!< The text color.
    bool m_textVisible;           //!< Indicates if the text should be drawn or not.
    ImageMode m_imageMode;        //!< The image mode.
    QPoint m_lastPos;             //!< The last position of the mouse cursor.
    QPoint m_translate;           //!< The translation of the image in pixels.
    double m_zoom;                //!< The zoom factor of the image (1.0 means 100 percent).
    double m_rotate;              //!< The rotation angle of the image in degrees within the interval [0, 360).
    Qt::CursorShape m_cursor;     //!< The shape of the cursor before the override.
    QTimer m_overlayTimer;        //!< The single shot timer to hide the overlay text after a certain interval.

    /*!
      \brief Initializes the OpenGL context.
      \sa initialized()
     */
    void initializeGL();

    /*!
      \brief Helper method to set up the OpenGL viewport and perspective.
      \param width The width of the widget.
      \param height The height of the widget.
     */
    void setupViewport(int width, int height);

    /*!
      \brief Draws the image to a texture and overpaint it with the text.
      \param event The paint event.
     */
    void paintEvent(QPaintEvent *event);

    /*!
      \brief Helper method to draw the text at the bottom of this widget
      \param painter The painter to draw the text.
     */
    void drawText(QPainter *painter);

    /*!
      \brief Helper method to draw the text in the center of this widget
      \param painter The painter to draw the text.
     */
    void drawOverlay(QPainter *painter);

    /*!
      \brief The mouse press event sets the initial state of the mouse events.
      \param event The mouse event.
      \sa mouseReleaseEvent(), mouseMoveEvent()
     */
    void mousePressEvent(QMouseEvent *event);

    /*!
      \brief The mouse double click event emits the \c doubleClicked() signal.
      \param event The mouse event.
     */
    void mouseDoubleClickEvent(QMouseEvent *event);

    /*!
      \brief The mouse move event handles the image pan to translate the image.
      \param event The mouse event.
      \sa mousePressEvent(), mouseReleaseEvent()
     */
    void mouseMoveEvent(QMouseEvent *event);

    /*!
      \brief The mouse release event resets the cursor to the previous state.
      \param event The mouse event.
      \sa mousePressEvent()
     */
    void mouseReleaseEvent(QMouseEvent *event);

    /*!
      \brief The wheel event handles zoom in and out.
      \param event The wheel event.
     */
    void wheelEvent(QWheelEvent *event);

};

#endif // IMAGEWIDGET_H
