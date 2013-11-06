#ifndef EULER_LIBRARIES_LIB_COMBINATION_COMBINATION_h
#define EULER_LIBRARIES_LIB_COMBINATION_COMBINATION_h

#include "subsets.h"
#include "subset_pair.h"
#include <vector>
#include <string>
#include "gsl/gsl_combination.h"
#include <sstream>

namespace Combination {
    Subsets AllCombinations(size_t n, size_t groupSize); 
    Subsets AllCombinations(std::vector<size_t> data, size_t groupSize); 
    SubsetPairs GetPairs(size_t n, size_t groupsize1, size_t groupsize2);
    SubsetPairs AllAjointPairs(size_t n);
}

template<class T>
class SingleCombination {
public:
    SingleCombination(std::vector<T> data, size_t toPick) : 
        groupSize(toPick),
        comb(gsl_combination_calloc(data.size(),toPick)),
        refData(data),
        indxs(comb->data, groupSize)
    { }

    bool Next() {
        if ( gsl_combination_next(comb) == GSL_SUCCESS ) {
            indxs = std::move(Subset(comb->data, groupSize));
            return true;
        } else {
            return false;
        }
    }

    bool Prev() {
        if ( gsl_combination_prev(comb) == GSL_SUCCESS ) {
            indxs = std::move(Subset(comb->data, groupSize));
            return true;
        } else {
            return false;
        }
    }

    void Last() {
        gsl_combination_init_last (comb);
    }

    std::vector<T> Set() {
        return std::move(indxs.Values<size_t>(refData));
    }

    T Sum(T&& init) {
        return std::move(
            indxs.Accumulate(refData,std::move(init)));
    }

    std::string SetString() {
        std::stringstream s;
        s << "{";
        for ( size_t i=0; i<groupSize; i++ ) {
            s << refData[indxs.Get(i)];
            if ( i==groupSize-1 ) {
                s << "}";
            } else {
                s << ",";
            }
        }
        return std::move(s.str());
    }

    virtual ~SingleCombination() {
        gsl_combination_free(comb);
    }
private:
    size_t groupSize;
    gsl_combination* comb;
    std::vector<T> refData;
    Subset         indxs;
};
#endif
