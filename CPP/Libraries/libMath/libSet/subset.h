#ifndef EULER_LIBRARIES_LIBSET_SUBSET_H
#define EULER_LIBRARIES_LIBSET_SUBSET_H

#include <vector>
#include <cstddef> /* size_t definition*/
#include <functional>
#include <string>
/*
 * Subset
 *
 * Represents indexes into a container
 * (Can be used to created a subset of elements)
 */
class Subset { 
public:
    Subset(Subset&& other) = default; // Construct from temp
    Subset& operator=(Subset&& other) = default; // assign from temp

    Subset(const Subset& other) = default;

    // Construct from values
    Subset(const std::vector<size_t >& values): idxs(values) {
        Sort();
    }
    Subset(std::vector<size_t >&& values): idxs(values) {
        Sort();
    } 
    Subset(size_t* values, size_t n) { 
        idxs.resize(n);
        for ( size_t i=0; i<n; ++i ) {
            idxs[i] = values[i];
        }
        Sort();
    }

    // return 0 -> end -1
    static Subset Range(size_t end) {
        return Range(0,end); 
    }
    // return start -> end -1
    static Subset Range(size_t start, size_t end); 
    // return start -> end -1 (incr: step)
    static Subset Range(size_t start, size_t end, size_t step); 
 
    // Do a sum
    template<class T,class S>
    T Accumulate( S data, T&& init);

    // Return the values
    template<class T, class S>
    std::vector<T> Values(S source);

    // Apply f
    template<class T, class S, class F>
    std::vector<T> Values(S source, F f);

    Subset operator|| (const Subset& other); // return element in either
    Subset operator&& (const Subset& other); // return element in both
    Subset operator^  (const Subset& other); // XOR (symmetric difference)
    Subset operator-  (const Subset& other); // difference
 
    // Return the indexes
    size_t Size() { 
        return idxs.size();
    } 
    
    const std::vector<size_t>& Indexes() {
        return idxs;
    }

    std::string Description();
 
 
    // Get the ith index
    size_t Get(size_t i) { return idxs[i]; } 
private:
    // Sort the indexes
    Subset& Sort(); 
    Subset(){}
    std::vector<size_t> idxs;
};

#include "subset.hpp"
#endif
