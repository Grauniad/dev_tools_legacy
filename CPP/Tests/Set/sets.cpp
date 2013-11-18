#include "tester.h"
#include "subset.h"
#include "subsets.h"

int Filter(testLogger& log );
int Empty(testLogger& log );
int Count(testLogger& log );

int main(int argc, const char *argv[])
{
    Test("Checking Empty...",Empty).RunTest();
    Test("Checking Count / Add...",Count).RunTest();
    Test("Checking Filter...",Filter).RunTest();
    return 0;
}

int Empty(testLogger& log ) {
    Subsets sets = Subsets::Empty();
    log << "Expected count to be 0, got: " << sets.Count() << endl;;
    if ( sets.Count() != 0 ) { 
        return 1;
    }
    return 0;
}

int Count(testLogger& log ) {
    Subsets sets = Subsets::Empty();
    for ( int i=0; i< 5; ++i) {
        sets.Add(Subset::Range(i));
    }

    log << "Expected count to be 5, got: " << sets.Count() << endl;;
    if ( sets.Count() != 5 ) { 
        return 1;
    }
    return 0;
}

int Filter(testLogger& log ) {
    Subsets sets1 = Subsets::Empty();

    for ( int i=1; i< 5; ++i) {
        sets1.Add(Subset::Range(i));
    }

    sets1.Filter([] (Subset& p) -> bool {
        return p.Size() > 3 || p.Size() < 1;
    });

    log << "Expected count to be 3, got: " << sets1.Count() << endl;;
    if ( sets1.Count() != 3 ) { 
        return 1;
    }
    size_t next=3;
    for( Subset& p: sets1) {
        log << "Next item should have size " << next;
        log << ", it has size " << p.Size() << endl;
        if (p.Size() != next) { 
            return 1; 
        } else {
            next--;
        }
    }
    return 0;
}

