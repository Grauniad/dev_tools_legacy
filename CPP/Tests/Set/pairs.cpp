#include "tester.h"
#include "subset.h"
#include "subset_pair.h"

int Filter(testLogger& log );
int Empty(testLogger& log );
int Count(testLogger& log );
int Add(testLogger& log );

int main(int argc, const char *argv[])
{
    Test("Checking Empty...",Empty).RunTest();
    Test("Checking Count...",Count).RunTest();
    Test("Checking Filter...",Filter).RunTest();
    Test("Checking Add...",Add).RunTest();
    return 0;
}


int Empty(testLogger& log ) {
    SubsetPairs pairs = SubsetPairs::Empty();
    log << "Expected count to be 0, got: " << pairs.Count() << endl;;
    if ( pairs.Count() != 0 ) { 
        return 1;
    }
    return 0;
}

int Count(testLogger& log ) {
    SubsetPairs pairs = SubsetPairs::Empty();
    for ( int i=0; i< 5; ++i) {
        pairs.Add({Subset::Range(i), Subset::Range(5)});
    }

    log << "Expected count to be 5, got: " << pairs.Count() << endl;;
    if ( pairs.Count() != 5 ) { 
        return 1;
    }
    return 0;
}

int Filter(testLogger& log ) {
    SubsetPairs pairs1 = SubsetPairs::Empty();

    for ( int i=1; i< 5; ++i) {
        pairs1.Add({Subset::Range(i), Subset::Range(5)});
    }

    pairs1.Filter([] (SubsetPair& p) -> bool {
        return p.Set1().Size() > 3 || p.Set1().Size() < 1;
    });

    log << "Expected count to be 3, got: " << pairs1.Count() << endl;;
    if ( pairs1.Count() != 3 ) { 
        return 1;
    }
    size_t next=3;
    for( SubsetPair& p: pairs1) {
        log << "Next item should have size " << next;
        log << ", it has size " << p.Set1().Size() << endl;
        if (p.Set1().Size() != next) { 
            return 1; 
        } else {
            next--;
        }
    }
    return 0;
}

int Add(testLogger& log ) {
    SubsetPairs pairs1 = SubsetPairs::Empty();
    SubsetPairs pairs2 = SubsetPairs::Empty();

    for ( int i=1; i< 5; ++i) {
        pairs1.Add({Subset::Range(i), Subset::Range(5)});
    }

    for ( int i=5; i< 10; ++i) {
        pairs2.Add({Subset::Range(i), Subset::Range(5)});
    }

    pairs1.Add(std::move(pairs2));

    
    log << "Expected count to be 9, got: " << pairs1.Count() << endl;;
    if ( pairs1.Count() != 9 ) { 
        return 1;
    }
    return 0;
}
