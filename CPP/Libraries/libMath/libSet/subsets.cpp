#include "subsets.h"
#include <iterator>

Subsets& Subsets::Add(Subset&& item) {
    sets.emplace_front(std::move(item));
    return *this;
}

size_t Subsets::Count() {
    return std::distance(begin(),end());
}
