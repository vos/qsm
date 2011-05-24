#ifndef QSM_H
#define QSM_H

#include <Qt>

namespace Qsm
{
    enum SortFlag {
        Name = 0x1, Date = 0x2, Unsorted = 0x4, // sort order
        Reversed = 0x8,                         // reverse sort order
        NameReversed = Name | Reversed,         // combinations
        DateReversed = Date | Reversed
    };
    Q_DECLARE_FLAGS(SortFlags, SortFlag)
}

Q_DECLARE_OPERATORS_FOR_FLAGS(Qsm::SortFlags)

#endif // QSM_H
