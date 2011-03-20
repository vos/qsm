#ifndef QSM_H
#define QSM_H

#include <Qt>

namespace Qsm
{
    enum SortFlag {
        Name = 0x1, Date = 0x2, Unsorted = 0x3, // sort order
        Reversed = 0x4                          // reverse sort order
    };
    Q_DECLARE_FLAGS(SortFlags, SortFlag)
}

#endif // QSM_H
