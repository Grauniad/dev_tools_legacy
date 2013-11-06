#ifndef EULER_LIBRARIES_LIB_COMBINATION_MULTISET_h
#define EULER_LIBRARIES_LIB_COMBINATION_MULTISET_h

#include <cstdlib>
#include <vector>

/*
 * 0 0 0
 * 0 0 1
 * 0 0 1
 * 0 0 2
 * 0 0 3
 * 0 1 0
 * 0 1 1
 * 0 1 2
 * 0 1 3
 * 0 2 0
 * 0 2 1
 * 0 2 2
 * 0 2 3
 * 0 3 0
 * 0 3 1
 * 0 3 2
 * 0 3 3
 * 1 1 1
 * etc
 */

class MultiCombination {
public:
    MultiCombination(size_t range, size_t len);

    bool Next();
    bool Prev();
    bool First();
    bool Last();

    const std::vector<size_t>& Set() {
        return values;
    }


private:
    size_t range;
    size_t len;

    std::vector<size_t> values;
};
#endif
