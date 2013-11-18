#ifndef EULER_LIBRARIES_LIBSET_SUBSETS_H
#define EULER_LIBRARIES_LIBSET_SUBSETS_H

#include "subset.h"
#include <forward_list>

class Subsets {
private:
    // We want rapid insertion / deletion at the current
    // location, but can do without random access
    std::forward_list<Subset> sets;
public:
    static Subsets Empty() {
        return Subsets();
    }

    Subsets(Subsets&& source) :
        sets(std::move(source.sets))
    {}

    template<class PRED>
    Subsets&  Filter(PRED f) {
        sets.remove_if(f);
        return *this;
    }

    decltype(sets.begin()) begin() {
        return sets.begin();
    }

    decltype(sets.end())   end() {
        return sets.end();
    }

    size_t Count();

    Subsets& Add(Subset&&);

private:
    Subsets(){}
};

#endif
