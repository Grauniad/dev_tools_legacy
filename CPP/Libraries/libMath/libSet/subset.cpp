#include "subset.h"
#include <sstream>
#include <algorithm>

Subset Subset::Range(size_t start, size_t end) {
    Subset s;
    for ( size_t i =start ; i<end; ++i ) {
        s.idxs.push_back(i);
    }
    return std::move(s);
}

Subset Subset::Range(size_t start, size_t end, size_t step) {
    Subset s;
    for ( size_t i =start ; i<end; i+=step ) {
        s.idxs.push_back(i);
    }
    return std::move(s);
}

Subset& Subset::Sort() {
    sort(idxs.begin(),idxs.end());
    return *this;
}

Subset Subset::operator&& (const Subset& other) {
    Subset s;
    s.idxs.reserve(idxs.size() + other.idxs.size());
    
    std::set_intersection(idxs.begin(), idxs.end(),
                          other.idxs.begin(), other.idxs.end(),
                          std::back_inserter(s.idxs));
    return std::move(s);
}

Subset Subset::operator|| (const Subset& other) {
    Subset s;
    s.idxs.reserve(idxs.size() + other.idxs.size());
    
    std::set_union(idxs.begin(), idxs.end(),
                          other.idxs.begin(), other.idxs.end(),
                          std::back_inserter(s.idxs));
    return std::move(s);
}

Subset Subset::operator^ (const Subset& other) {
    Subset s;
    s.idxs.reserve(idxs.size() + other.idxs.size());
    
    std::set_symmetric_difference(idxs.begin(), idxs.end(),
                          other.idxs.begin(), other.idxs.end(),
                          std::back_inserter(s.idxs));
    return std::move(s);
}

Subset Subset::operator- (const Subset& other) {
    Subset s;
    s.idxs.reserve(idxs.size() + other.idxs.size());
    
    std::set_difference(idxs.begin(), idxs.end(),
                          other.idxs.begin(), other.idxs.end(),
                          std::back_inserter(s.idxs));
    return std::move(s);
}

std::string Subset::Description() {
    std::stringstream s;
    s << "{";
    for ( size_t i=0; i<Size(); i++ ) {
        s << Get(i);
        if ( i==Size()-1 ) {
            s << "}";
        } else {
            s << ",";
        }
    }
    return std::move(s.str());
}
