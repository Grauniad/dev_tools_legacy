#ifndef EULER_LIBRARIES_LIBSET_SUBSET_PAIR_H
#define EULER_LIBRARIES_LIBSET_SUBSET_PAIR_H

#include "subset.h"
#include <forward_list>

class SubsetPair {
public:
    SubsetPair( Subset&& s1, Subset&& s2): 
        set1(std::move(s1)), 
        set2(std::move(s2)){}

    SubsetPair( SubsetPair&& source ): 
        set1(std::move(source.set1)), 
        set2(std::move(source.set2)){}

    Subset& Set1() { return set1;}
    Subset& Set2() { return set2;}
private:
    Subset set1;
    Subset set2;
};

class SubsetPairs {
private:
    // We want rapid insertion / deletion at the current
    // location, but can do without random access
    std::forward_list<SubsetPair> pairs;
public:
    static SubsetPairs AllDisjointPairs(size_t n);
    static SubsetPairs Empty() {
        return SubsetPairs();
    };

    SubsetPairs(SubsetPairs&& source) :
        pairs(std::move(source.pairs))
    {}

    template<class PRED>
    SubsetPairs&  Filter(PRED f) {
        pairs.remove_if(f);
        return *this;
    }

    decltype(pairs.begin()) begin() {
        return pairs.begin();
    }
    decltype(pairs.end())   end() {
        return pairs.end();
    }

    size_t Count();

    SubsetPairs& Add(SubsetPair&&);

    SubsetPairs& Add(SubsetPairs&& other) {
        for ( SubsetPair& p: other.pairs) {
            pairs.emplace_front(std::move(p));
        }
        return *this;
    }
private:
    SubsetPairs(){}
};

#endif
