#include "multi_combination.h"

MultiCombination::MultiCombination(size_t range, size_t len) {
    this->range = range;
    this->len = len;

    values.resize(len);
    First();

}

bool MultiCombination::First ( ) {
    for ( size_t& i : values ) {
        i = 0;
    }
    return true;
}

bool MultiCombination::Last ( ) {
    for ( size_t& i : values ) {
        i = range-1;
    }
    return true;
}

bool MultiCombination::Next ( ) {
    // Look for the first non-max item
    short incrIdx = -1;
    for ( int i =len-1; i >= 0; i-- ) {

        // Check if we can increment this number
        if ( values[i] < (range-1) )  {
            incrIdx = i;
            break;
        }
    }

    if ( incrIdx != -1 ) {
        values[incrIdx]++;

        // Everything else must be reset to 0
        for ( size_t i = incrIdx+1; i< len; i++ ) {
            values[i] = 0;
        }
        
        return true;
    } else {
        // Already at MAX
        return false;
    }
}

bool MultiCombination::Prev ( ) {
    // Look for the first non-zero item
    short decrIdx = -1;
    for ( int i =len-1; i >= 0; i-- ) {

        // Check if we can decrement this number
        if ( values[i] > 0 )  {
            decrIdx = i;
            break;
        }
    }

    if ( decrIdx != -1 ) {
        values[decrIdx]--;

        // Everything else must be reset to MAX
        for ( size_t i = decrIdx+1; i< len; i++ ) {
            values[i] = range-1;
        }
        
        return true;
    } else {
        // Already at MAX
        return false;
    }
}
