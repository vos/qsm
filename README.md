# Qt Slideshow Manager (QSM)

Qt Slideshow Manager (QSM) is a free slideshow manager for Windows, Linux and Mac OS X.  QSM is build on the [Qt framework](http://qt.nokia.com) from Nokia to provide cross-platform support.


## Installation

If you want to build QSM from the [source files](http://github.com/vos/qsm), you will need to make several preparations.

### Dependencies

QSM requires the following external libraries:

 * [Qt](http://qt.nokia.com) 4.7 or greater

Install the Qt library (there may be some precompiled binary files available for your platform, if you don't want to compile all the packages by yourself you might want to check that first).

### Compilation

If you have installed all the required libraries from above, the easiest way to compile QSM itself is to download the complete [Qt SDK](http://qt.nokia.com/downloads) (if you haven't already done so). Then you can start up the provided Qt Creator IDE and open the QSM project file **qsm.pro** (the one inside the root folder of QSM) and hit the **Build All** button.  Alternatively you can use `qmake` on the command line to generate the necessary make files for your compiler (see the [Qt documentation](http://doc.qt.nokia.com/latest/qmake-manual.html) for further information).


## Usage

To use QSM you just need to run the compiled binary file called **qsm** or **qsm.exe** on windows systems.  QSM uses a rich GUI (graphical user interface) with intuitive usability for most use cases. If you need any detailed instuctions take a look at the complete user guide (press the help key inside the program).

## Documentation

The generated HTML-documentation of the source code can be viewed online at http://vos.github.com/qsm/.


## License

Copyright (C) 2011 Alexander Vos <alexander.vos@stud.fh-swf.de>, Till Althaus <till.althaus@stud.fh-swf.de>

Distributed under the GNU General Public License.  See
the COPYING file for more information.
