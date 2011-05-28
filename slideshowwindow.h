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

#ifndef SLIDESHOWWINDOW_H
#define SLIDESHOWWINDOW_H

#include <QWidget>
#include <QTimer>

#include "slideshow.h"

class ImageWidget;

/*!
  \brief The UI namespace.
 */
namespace Ui {
    class SlideshowWindow;
}

/*!
  \brief The SlideshowWindow class provides an encapsulated ImageWidget to play
             a single Slideshow in automatic or manual mode.

   The SlideshowWindow uses the weighted random algorithm to play back the
   images in random order.  Also in automatic play back mode the next image
   will be buffered for consistent transition time between all images.

  \sa Slideshow, ImageWidget, MainWindow
 */
class SlideshowWindow : public QWidget
{
    Q_OBJECT

public:

    /*!
      \brief Constructs a new SlideshowWindow with the given \a slideshow.

      The SlideshowWindow constructor sets up the UI and connects all necessary
      signals. Also if \a random is set to \c true, the random generator will
      be initialized and the random factors normalized for quicker look up times
      of the next random image of large slideshows.

      \param slideshow The slideshow associated with this window.
      \param interval The interval in milliseconds between the transition of
             two images.
      \param random \c True if the slideshow should be played in random order;
             otherwise \c false.
      \param repeat \c True if the slideshow should be repeated and start again
             at the beginning after the last image.
      \param parent The parent widget.
      \sa imageWidget()
     */
    explicit SlideshowWindow(Slideshow *slideshow,
                             int interval = 5,
                             bool random = false,
                             bool repeat = true,
                             QWidget *parent = 0);

    /*!
      \brief Destroys the SlideshowWindow and frees all UI resources.
     */
    ~SlideshowWindow();

    /*!
      \brief Returns a pointer to the encapsulated image widget.
      \return A pointer to the image widget
     */
    ImageWidget* imageWidget() const;

private slots:

    /*!
      \brief Displays the first image and name of the slideshow and starts
             the timer for automatic play back.
      \sa ImageWidget::initialized()
     */
    void on_imageWidget_initialized();

    /*!
      \brief Toggles between full screen and normal window mode.
     */
    void on_imageWidget_doubleClicked();

    /*!
      \brief Shows the next image and starts the asynchronous preparation of
             the following image using a ImageLoader inside the global thread
             pool.
      \sa ImageLoader, timer_stop()
     */
    void timer_timeout();

    /*!
      \brief Stops the automatic slideshow play back.
      \sa timer_timeout()
     */
    void timer_stop();

    /*!
      \brief Redirects the loaded image to the image widget.
      \param image The loaded image.
      \param width The width of the loaded image.
      \param height The height of the loaded image.
      \param index The slideshow index of the loaded image.
      \sa prepareNextImage()
     */
    void imageLoaded(const QImage &image, int width, int height, int index);

private:

    Ui::SlideshowWindow *ui;         //!< The pointer to the UI.
    Slideshow *m_slideshow;          //!< The slideshow associated with this window
    bool m_random;                   //!< Indicates if the slideshow should be played in random order.
    bool m_repeat;                   //!< Indicates if the slideshow should repeat itself.
    QVector<double> m_randomFactors; //!< Cache for the normalized random factors.
    SlideshowImage *m_currentImage;  //!< Pointer to the current image.
    SlideshowImage *m_nextImage;     //!< Pointer to the next image.
    QImage m_imageBuffer;            //!< The buffer for the next image.
    int m_slideshowIndex;            //!< The current index in the slideshow.
    QTimer m_timer;                  //!< The timer for automatic play back.

    /*!
      \brief Helper method to check if there is a following image available.
      \return \c True if there is a next image available; otherwise \c false.
     */
    inline bool hasNextImage() const { return (m_nextImage && m_nextImage != m_currentImage); }

    /*!
      \brief Helper method to prepare the next image.
      \param delta The index difference to the current image.
      \param synchronous \c True if the image should be loaded synchronously;
             \c false if the image should be loaded asynchronously inside
             a separate thread using a ImageLoader.
      \return \c True if there is a next image; otherwise \c false.
      \sa showNextImage(), hasNextImage(), imageLoaded()
     */
    bool prepareNextImage(int delta = 1, bool synchronous = false);

    /*!
      \brief Displays the next image and comment if available and sets the next
             image as the current one.
      \sa hasNextImage(), prepareNextImage()
     */
    void showNextImage();

    /*!
      \brief Handles all keyboard shortcuts for the interaction with the user.

      Keyboard shortcuts:
        - \b Escape  : Terminates the slideshow and closes the window.
        - \b 0       : Zooms the image to fit the screen/widget.
        - \b 1       : Zooms the image to its original size (100 %).
        - \b R       : Rotates the image 90 degrees clockwise.
        - \b Shift+R : Rotates the image 90 degrees counterclockwise.
        - \b +       : Zooms the image in by 15 %.
        - \b -       : Zooms the image out by 15 %.
        - \b C       : Toggles the visibility of the image' comment.
        - \b Space   : Stops or resumes the slideshow play back.
        - \b Right   : Go to the next image (stops the automatic play back)
        - \b Left    : Go to the previous image (stops the automatic play back)
        - \b F/F11   : Toggles between full screen and normal window mode.

      \param event The key event.
      \sa ImageWidget
     */
    void keyPressEvent(QKeyEvent *event);

};

#endif // SLIDESHOWWINDOW_H
