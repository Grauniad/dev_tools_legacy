#ifndef EULER_LIBRARIES_LIB_COMBINATION_MULTISET_h
#define EULER_LIBRARIES_LIB_COMBINATION_MULTISET_h

#include <cstdlib>
#include "gsl/gsl_multiset.h"
/*
 * 0 0 0
 * 0 0 1
 * 0 0 2
 * 0 0 3
 * 0 1 1
 * 0 1 2
 * 0 1 3
 * 0 2 2
 * 0 2 3
 * 0 3 3
 * 1 1 1
 * etc
 */

class MultiSet {
public:
    MultiSet(size_t range, size_t len);
    ~MultiSet();

    bool Next();
    bool Prev();
    bool First();
    bool Last();
    const size_t* Set();
private:
    // data
    gsl_multiset* gsl_set;

    // prevent
    MultiSet(const MultiSet& rhs) = delete;
    MultiSet& operator=(const MultiSet&) = delete;

};

#endif
