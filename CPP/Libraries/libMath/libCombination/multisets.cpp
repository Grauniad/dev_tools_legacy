#include "multisets.h"


// Initialise the gsl_set
MultiSet::MultiSet(size_t range, size_t len): 
    gsl_set(gsl_multiset_calloc(range,len)) { }

// Clean up the GSL set
MultiSet::~MultiSet() {
    gsl_multiset_free(gsl_set);
}

/*
 * Tools to change the data
 */

bool MultiSet::First() {
    gsl_multiset_init_first(gsl_set);
    return true;
}

bool MultiSet::Last() {
    gsl_multiset_init_last(gsl_set);
    return true;
}

bool MultiSet::Next() {
    if ( gsl_multiset_next(gsl_set) == GSL_SUCCESS )  {
        return true;
    } else {
        return false;
    }
}

bool MultiSet::Prev() {
    if ( gsl_multiset_prev(gsl_set) == GSL_SUCCESS )  {
        return true;
    } else {
        return false;
    }
}

/*
 * Get the data...
 */

const size_t* MultiSet::Set() {
    return gsl_multiset_data(gsl_set);
}
