#include "subset_pair.h"
#include <iterator>

SubsetPairs SubsetPairs::AllDisjointPairs(size_t n) {
    SubsetPairs s;
    for ( int i=0; i< 25; i++ ) {
        s.Add({Subset::Range(3), Subset::Range(5)});
    }
    return std::move(s);
}

SubsetPairs& SubsetPairs::Add(SubsetPair&& item) {
    pairs.emplace_front(std::move(item));
    return *this;
}

size_t SubsetPairs::Count() {
    return std::distance(begin(),end());
}
