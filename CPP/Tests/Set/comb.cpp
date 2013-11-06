#include "tester.h"
#include "combination.h"
#include "multi_combination.h"

using namespace std;

int Combinations(testLogger& log );
int CustomCombinations(testLogger& log );
int Pairs(testLogger& log );
int GetAdjoint(testLogger& log );

int MultiCombForward(testLogger& log);
int MultiCombBackward(testLogger& log);

int main(void) {
    /*
     * Combinations
     */
    Test("Checking basic combinations",Combinations).RunTest();
    Test("Checking custom combinations",CustomCombinations).RunTest();
    Test("Checking Pairs",Pairs).RunTest();
    Test("Checking Adjoint pair count", GetAdjoint).RunTest();

    /*
     * Full Permutations (range^count type problems)
     */
    Test("Checking forward iteration through complete permutation", MultiCombForward).RunTest();
    Test("Checking backward iteration through complete permutation", MultiCombBackward).RunTest();
}

int Combinations(testLogger& log ) {
    Subsets s(Combination::AllCombinations(3,2));
    if ( s.Count() != 3 ) {
        return 1;
    }

    bool found[3][3];
    for ( int i =0; i< 3; i++) {
        for ( int j =0; i< 3; i++) {
            found[i][j] = false;
        }
    }

    for(Subset& set: s) {
        found[set.Get(0)][set.Get(1)] = true;
        log << "Have set: " << set.Get(0) << " , " << set.Get(1) << endl;
    }

    // Make sure we found what we needed
    if ( !found[0][1] || !found[0][2] || !found[1][2] ) {
        log << "Didn't fidn the right stuff :(" << endl;
        return 1;
    }
    return 0;
}

int CustomCombinations(testLogger& log ) {
    vector<size_t> data({5,4,3});
    Subsets s(Combination::AllCombinations(data,2));
    if ( s.Count() != 3 ) {
        return 1;
    }

    bool found[6][6];
    for ( int i =0; i< 6; i++) {
        for ( int j =0; i< 6; i++) {
            found[i][j] = false;
        }
    }

    for(Subset& set: s) {
        found[set.Get(0)][set.Get(1)] = true;
        log << "Have set: " << set.Get(0) << " , " << set.Get(1) << endl;
    }

    // Make sure we found what we needed
    if ( !found[3][4] || !found[3][5] || !found[4][5] ) {
        log << "Didn't fidn the right stuff :(" << endl;
        return 1;
    }
    return 0;
}
int Pairs(testLogger& log ) {
    SubsetPairs s(Combination::GetPairs(4,2,2));
    /*
     * 01 23 (0)
     * 02 13 (1) 
     * 03 12 (2) 
     */
    bool found[3] = { false, false, false};

    for(SubsetPair& pair: s) {
        size_t item[2][2];
        item[0][0] = pair.Set1().Get(0);
        item[0][1] = pair.Set1().Get(1);
        item[1][0] = pair.Set2().Get(0);
        item[1][1] = pair.Set2().Get(1);

        log << "Found: " << item[0][0] << item[0][1];
        log << "       " << item[1][0] << item[1][1];
        if ( item[0][0] == 0 && item[0][1] == 1 &&
             item[1][0] == 2 && item[1][1] == 3 ) {
            found[0] = true;
        }
        if ( item[0][0] == 0 && item[0][1] == 2 &&
             item[1][0] == 1 && item[1][1] == 3 ) {
            found[1] = true;
        }
        if ( item[0][0] == 0 && item[0][1] == 3 &&
             item[1][0] == 1 && item[1][1] == 2 ) {
            found[2] = true;
        }
    }

    if ( !found[0] || !found[1] || !found[2] ) {
        return 1;
    }
    return 0;
}

int GetAdjoint( testLogger& log ) {
    SubsetPairs s(Combination::AllAjointPairs(4));
    log << "got " << s.Count() << " pairs " << endl;
    if ( s.Count() != 25 ) {
        return 1;
    }
    return 0;
}

int MultiCombForward ( testLogger& log ) {

    const int RANGE = 3;

    MultiCombination set(RANGE,3);

    short data[RANGE][RANGE][RANGE];

    int i, j ,k;


    // Initialise the test
    for (i=0; i< RANGE; i++ ) {
        for (j=0; j< RANGE; j++ ) {
            for (k=0; k< RANGE; k++ ) {
                data[i][j][k] = 0;
            }
        }
    }

    // Loop through everything
    for ( bool OK = set.First(); OK; OK = set.Next() ) {
        i = set.Set()[0];
        j = set.Set()[1];
        k = set.Set()[2];

        data[i][j][k] += 1;

        log << i << ", " << j << ", " << k << endl;
    }

    // Check we got each cell eactly once
    // Initialise the test
    for (i=0; i< RANGE; i++ ) {
        for (j=0; j< RANGE; j++ ) {
            for (k=0; k< RANGE; k++ ) {
                if ( data[i][j][k] == 0 ) {
                    log << "Didn't get combo: "
                        << i << ", "
                        << j << ", "
                        << k << endl;
                    return 1;
                } else if (data[i][j][k] > 1) { 
                    log << "Called to many times ("
                        << data[i][j][k] << "): "
                        << i << ", "
                        << j << ", "
                        << k << endl;
                    return 1;

                }
            }
        }
    }

    return 0;
}

int MultiCombBackward ( testLogger& log ) {

    const int RANGE = 3;

    MultiCombination set(RANGE,3);

    short data[RANGE][RANGE][RANGE];

    int i, j ,k;


    // Initialise the test
    for (i=0; i< RANGE; i++ ) {
        for (j=0; j< RANGE; j++ ) {
            for (k=0; k< RANGE; k++ ) {
                data[i][j][k] = 0;
            }
        }
    }

    // Loop through everything
    for ( bool OK = set.Last(); OK; OK = set.Prev() ) {
        i = set.Set()[0];
        j = set.Set()[1];
        k = set.Set()[2];

        data[i][j][k] += 1;

        log << i << ", " << j << ", " << k << endl;
    }

    // Check we got each cell eactly once
    // Initialise the test
    for (i=0; i< RANGE; i++ ) {
        for (j=0; j< RANGE; j++ ) {
            for (k=0; k< RANGE; k++ ) {
                if ( data[i][j][k] == 0 ) {
                    log << "Didn't get combo: "
                        << i << ", "
                        << j << ", "
                        << k << endl;
                    return 1;
                } else if (data[i][j][k] > 1) { 
                    log << "Called to many times ("
                        << data[i][j][k] << "): "
                        << i << ", "
                        << j << ", "
                        << k << endl;
                    return 1;

                }
            }
        }
    }

    return 0;
}
