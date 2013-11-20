#include "tester.h"
#include "subset.h"

int Range1(testLogger& log );
int Range2(testLogger& log );
int Range3(testLogger& log );
int Sum(testLogger& log );
int Values(testLogger& log );
int ForEach(testLogger& log );
int VectorConstructor(testLogger& log );
int VectorMove(testLogger& log );
int ArrayConstructor(testLogger& log );
int Sort(testLogger& log );
int Intersection(testLogger& log );
int Union(testLogger& log );
int XOR(testLogger& log );
int Diff(testLogger& log );

int main(int argc, const char *argv[])
{
    Test("Checking range(end)...",Range1).RunTest();
    Test("Checking range(start, end)...",Range2).RunTest();
    Test("Checking range(start, end,step)...",Range3).RunTest();
    Test("Checking Sum...",Sum).RunTest();
    Test("Checking Values...",Values).RunTest();
    Test("Checking construction from moved vector...",VectorMove).RunTest();
    Test("Checking construction from size_t* ...",ArrayConstructor).RunTest();
    Test("Checking construction Sort...",Sort).RunTest();
    Test("Checking intersection...",Intersection).RunTest();
    Test("Checking union...",Union).RunTest();
    Test("Checking XOR...",XOR).RunTest();
    Test("Checking difference...",Diff).RunTest();
    return 0;
}


int Range1(testLogger& log ) {
    Subset s(Subset::Range(10)); 

    size_t size = s.Size();
    log << "Size of set is: " << size;
    if ( s.Size() != 10 ) {
        return 1;
    }

    for ( size_t i=0; i< 10; i++ ) { 
        size_t j = s.Get(i);  
        log << "Element at " << i << " is " << j << endl;
        if ( j != i ) {
            return 1;
        }
    }
    return 0;
}

int Range2(testLogger& log ) {
    Subset s(Subset::Range(2, 10)); 

    size_t size = s.Size();
    log << "Size of set is: " << size;
    if ( s.Size() != 8 ) {
        return 1;
    }
    for ( size_t i=0; i< 8; i++ ) { 
        size_t j = s.Get(i);  
        log << "Element at " << i << " is " << j << endl;
        if ( j != i +2 ) {
            return 1;
        }
    }
    return 0;
}

int Range3(testLogger& log ) {
    Subset s(Subset::Range(2, 10,3)); 

    size_t size = s.Size();
    log << "Size of set is: " << size;
    if ( s.Size() != 3 ) {
        return 1;
    }
    for ( size_t i=0; i< 3; i++) { 
        size_t j = s.Get(i);  
        log << "Element at " << i << " is " << j << endl;
        if ( j != 3*i +2 ) {
            return 1;
        }
    }
    return 0;
}

int Sum(testLogger& log ) {
    Subset s(Subset::Range(2, 10,3)); 
    vector<int> data({0,100,200,300,400,500,600,700,800,9000});

    long sum = s.Accumulate(data,0);
    long esum = 200 + 500 + 800 ;
    log << "Expected sum: " << esum << endl;
    log << "Got sum: " << sum << endl;
    if (sum != esum ) { 
        return 1;
    }
    return 0;
}

int Values(testLogger& log ) {
    Subset s(Subset::Range(2, 10,3)); 

    vector<int> data({0,100,200,300,400,500,600,700,800,9000});
    vector<int> vals(s.Values<int>(data));

    size_t esize = 3;
    size_t size = vals.size();
    log << "Size of set is: " << size;
    if ( size != esize ) {
        return 1;
    }
    if (vals[0] != 200 ) {
        log << "Exepcted 0 to be 200, but got " << vals[0] << endl;
        return 1;
    }
    if (vals[1] != 500 ) {
        log << "Exepcted 0 to be 500, but got " << vals[1] << endl;
        return 1;
    }
    if (vals[2] != 800 ) {
        log << "Exepcted 0 to be 800, but got " << vals[2] << endl;
        return 1;
    }
    return 0;
}

int ForEach(testLogger& log ) {
    Subset s(Subset::Range(10)); 
    vector<int> data({0,100,200,300,400,500,600,700,800,900});

    vector<int> vals(s.Values<int>(data, [] (int& i) -> int {
        return i/50;
    }));

    size_t esize = 10;
    size_t size = vals.size();
    log << "Size of set is: " << size;
    if ( size != esize ) {
        return 1;
    }

    for ( size_t i=0; i< 10; i++ ) { 
        size_t j = vals[i];  
        log << "Element at " << i << " is " << j << endl;
        if ( j != 2*i ) {
            return 1;
        }
    }

    return 0;
}

int VectorConstructor(testLogger& log ) {
    vector<int> data({0,100,200,300,400,500,600,700,800,900});

    vector<size_t> idxs = Subset::Range(10).Values<size_t>(data);
    Subset s(idxs); 

    size_t size = s.Size();
    log << "Size of set is: " << size;
    if ( s.Size() != 10 ) {
        return 1;
    }

    for ( size_t i=0; i< 10; i++ ) { 
        size_t j = s.Get(i);  
        log << "Element at " << i << " is " << j << endl;
        if ( j != 100*i ) {
            return 1;
        }
    }
    return 0;
}

int VectorMove(testLogger& log ) {
    vector<int> data({0,100,200,300,400,500,600,700,800,900});

    Subset s(Subset::Range(10).Values<size_t>(data)); 

    size_t size = s.Size();
    log << "Size of set is: " << size;
    if ( s.Size() != 10 ) {
        return 1;
    }

    for ( size_t i=0; i< 10; i++ ) { 
        size_t j = s.Get(i);  
        log << "Element at " << i << " is " << j << endl;
        if ( j != 100*i ) {
            return 1;
        }
    }
    return 0;
}

int ArrayConstructor(testLogger& log ) {
    vector<int> data({0,100,200,300,400,500,600,700,800,900});

    vector<size_t> idxs = Subset::Range(10).Values<size_t>(data);
    Subset s(idxs.data(),idxs.size()); 

    size_t size = s.Size();
    log << "Size of set is: " << size;
    if ( s.Size() != 10 ) {
        return 1;
    }

    for ( size_t i=0; i< 10; i++ ) { 
        size_t j = s.Get(i);  
        log << "Element at " << i << " is " << j << endl;
        if ( j != 100*i ) {
            return 1;
        }
    }
    return 0;
}

int Sort(testLogger& log ) {
    Subset s({5,42,3,1}); 
    if (    s.Get(0) != 1 
         || s.Get(1) != 3
         || s.Get(2) != 5 
         || s.Get(3) != 42 ) 
    {
        return 1;
    }
    return 0;
}

int Intersection(testLogger& log ) {
    Subset s1 = Subset::Range(10);
    Subset s2 = Subset::Range(5,15);
    Subset s3 = s1 && s2;

    size_t size = s3.Size();
    log << "Size of set is: " << size;
    if ( s3.Size() != 5 ) {
        return 1;
    }

    for ( size_t i=0; i< 5; i++ ) { 
        size_t j = s3.Get(i);  
        log << "Element at " << i << " is " << j << endl;
        if ( j != i+5 ) {
            return 1;
        }
    }

    return 0;
}

int Union(testLogger& log ) {
    Subset s1 = Subset::Range(10);
    Subset s2 = Subset::Range(5,15);
    Subset s3 = s1 || s2;

    size_t size = s3.Size();
    log << "Size of set is: " << size;
    if ( s3.Size() != 15 ) {
        return 1;
    }

    for ( size_t i=0; i< 15; i++ ) { 
        size_t j = s3.Get(i);  
        log << "Element at " << i << " is " << j << endl;
        if ( j != i ) {
            return 1;
        }
    }

    return 0;
}

int XOR(testLogger& log ) {
    Subset s1 = Subset::Range(10);
    Subset s2 = Subset::Range(5,15);
    Subset s3 = s1 ^ s2;

    size_t size = s3.Size();
    log << "Size of set is: " << size;
    if ( s3.Size() != 10 ) {
        return 1;
    }

    size_t v;
    for ( size_t i=0; i< 10; i++ ) { 
        if ( i < 5) {
            v = i;
        } else {
            v = i+5;
        }
        size_t j = s3.Get(i);  
        log << "Element at " << i << " is " << j << endl;
        if ( j != v ) {
            return 1;
        }
    }

    return 0;
}

int Diff(testLogger& log ) {
    Subset s1 = Subset::Range(10);
    Subset s2 = Subset::Range(5,15);
    Subset s3 = s1 - s2;

    size_t size = s3.Size();
    log << "Size of set is: " << size;
    if ( s3.Size() != 5 ) {
        return 1;
    }

    for ( size_t i=0; i< 5; i++ ) { 
        size_t j = s3.Get(i);  
        log << "Element at " << i << " is " << j << endl;
        if ( j != i ) {
            return 1;
        }
    }

    return 0;
}
