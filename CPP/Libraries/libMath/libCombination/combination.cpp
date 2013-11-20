#include "combination.h"
#include "gsl/gsl_combination.h"
#include "logger.h"

using namespace std;


Subsets Combination::AllCombinations(size_t n, size_t groupSize)
{
    Subsets s(Subsets::Empty());
    gsl_combination* comb = gsl_combination_calloc(n, groupSize);
    do
    {
        s.Add(Subset(comb->data,groupSize));
    } while(gsl_combination_next(comb) == GSL_SUCCESS);

    if ( Logger::Instance().LogEnabled(LOG_VERY_VERBOSE)) {
        stringstream& msg = LogFactory::Buf();
        msg << "Returning all groups of size " << groupSize;
        msg << " from a set of size " << n;
        msg << endl;
        for ( Subset& set: s) {
            for (size_t i =0; i<set.Size(); ++i) {
                msg << set.Get(i) << ", ";
            }
            msg << endl;
        }

        Logger::Instance().LogMessage( 
            msg.str(),
            LOG_VERY_VERBOSE,
            "AllCombinations");
    }
    gsl_combination_free(comb);

    return std::move(s);
}

Subsets Combination::AllCombinations(vector<size_t> data, size_t groupSize)
{
    // Get the basic subsets
    Subsets raw(AllCombinations(data.size(), groupSize));
    Subsets s(Subsets::Empty());

    for (Subset& set: raw) {
        s.Add(set.Values<size_t>(data));
    }

    if ( Logger::Instance().LogEnabled(LOG_VERY_VERBOSE)) {
        stringstream& msg = LogFactory::Buf();
        msg << "Returning all groups of size " << groupSize << " from: ";
        msg << endl;
        for (size_t& i : data) {
            msg << i << ", ";
        }
        msg << endl;
        for ( Subset& set: s) {
            for (size_t i =0; i<set.Size(); ++i) {
                msg << set.Get(i) << ", ";
            }
            msg << endl;
        }
        Logger::Instance().LogMessage( 
            msg.str(),
            LOG_VERY_VERBOSE,
            "AllCombinations - with vector data");
    }
    return std::move(s);
}

SubsetPairs Combination::GetPairs( size_t n, 
                                   size_t groupSize1, 
                                   size_t groupSize2) 
{
    SLOG_FROM(LOG_VERY_VERBOSE, "GetPairs",    "n: " << n 
                                            << ", g1: " << groupSize1 
                                            << ", g2: " << groupSize2)

    SubsetPairs pairs(SubsetPairs::Empty());
    Subsets groups_1(AllCombinations(n,groupSize1) );
    for (Subset& s: groups_1 ) {
        SLOG_FROM(
            LOG_VERY_VERBOSE, 
            "GetPairs",
            " Group1: " << s.Description()
        )

        Subset toChoose(Subset::Range(n) - s);


        Subsets groups_2(AllCombinations(toChoose.Indexes(),groupSize2));
        for (Subset&t: groups_2) {
            SLOG_FROM(
                LOG_VERY_VERBOSE, 
                "GetPairs",
                   " Group1: " << s.Description() << endl
                << " Group2: " << t.Description()
            )
            // only want one of ({a,b},{c,d}) and ({c,d},{a,b})
            if (    (t.Size() == s.Size() &&  t.Get(0) > s.Get(0)) 
                 || (t.Size() < s.Size()) 
               )
            {
                SLOG_FROM(
                    LOG_VERY_VERBOSE, 
                    "GetPairs",
                    "Added: " << s.Description() 
                              << ", " 
                              << t.Description()
                )
                pairs.Add(SubsetPair(Subset(s),std::move(t)));
            }
        }
    }
    return std::move(pairs);
}

SubsetPairs Combination::AllAjointPairs(size_t n) {
    SubsetPairs s(SubsetPairs::Empty());
    for ( size_t i = 1; i<n; ++i) {
        for ( size_t j = 1; j<=i && (i+j)<=n; ++j) {
            s.Add(GetPairs(n,i,j));
        }
    }
    return std::move(s);
}
