#ifndef DEV_TOOLS_CPP_LIBRARIES_LIB_MATH_NUMBER_NUMBER_H
#define DEV_TOOLS_CPP_LIBRARIES_LIB_MATH_NUMBER_NUMBER_H

namespace Math { 
    template <class NUM>
    void MinMax(NUM* data, int dmim, NUM& min, NUM& max);

    template <class NUM>
    void MinMax2D(NUM** data, int xdim, int ydmim, NUM& min, NUM& max);

    template <class NUM>
    void Fill(NUM* data, int count, NUM start, NUM step );
}

#include "number.hpp"
#endif
