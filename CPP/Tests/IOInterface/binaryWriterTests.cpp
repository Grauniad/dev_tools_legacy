#include "tester.h"
#include <algorithm>
#include <vector>
#include <iostream>
#include "dataVector.h"
#include "dataLump.h"
#include "binaryWriter.h"
#include <sstream>
#include <string>
#include <cstring>

/*
 * Tests should be run after the datavector object has been verified by the
 * test suite
 *
 */ 

const int DATA_SIZE=128;
char c_data[DATA_SIZE];
unsigned char io_lump[DATA_SIZE];
DataVector data(DATA_SIZE);
DataLump<DATA_SIZE> lump;

class TestError {
public:
    TestError (string s, long i) {
        this->err = s;
        this->errCode = i;
    };
    int Error( testLogger& log) { 
        log << err << endl;
        return errCode==0? 255: errCode;
    }

private:
    string err;
    long errCode;
};

int VerifyPositionFinders( testLogger& log);
int VerifyAssignments( testLogger& log);
int VerifyArithmetic( testLogger& log);

int VerifyWrites_DataVector( testLogger& log);
int VerifyWrites_DataLump( testLogger& log);
int VerifyWrites_DataIO( testLogger& log);

int VerifyStringWrites_DataVector( testLogger& log);
int VerifyStringWrites_DataLump( testLogger& log);
int VerifyStringWrites_DataIO( testLogger& log);

int VerifyBoundraryFinder_DataVector( testLogger& log);
int VerifyBoundraryFinder_DataLump( testLogger& log);
int VerifyBoundraryFinder_DataIO( testLogger& log);

int VerifyStringPushes_DataVector( testLogger& log);
int VerifyStringPushes_DataLump( testLogger& log);
int VerifyStringPushes_DataIO( testLogger& log);

int VerifyPODPush_DataVector( testLogger& log);
int VerifyPODPush_DataLump( testLogger& log);
int VerifyPODPush_DataIO( testLogger& log);

using namespace std;
int main(int argc, const char *argv[])
{
    // Set up the data vectory
    for (int i = 0; i < DATA_SIZE; i++) {
        c_data[i] = char(i);
        io_lump[i] = char(i);
        data[i] = char(i);
        lump.Put(i,char(i));
    }
    Test("Return positions in file",  (loggedTest)VerifyPositionFinders).RunTest();
    Test("Verify Assignment Operators",  (loggedTest)VerifyAssignments).RunTest();
    Test("Verify Arithmetic Operators",  (loggedTest)VerifyArithmetic).RunTest();

    Test("Verify Writes (DataVector)",  (loggedTest)VerifyWrites_DataVector).RunTest();
    Test("Verify Writes (DataLump)",  (loggedTest)VerifyWrites_DataLump).RunTest();
    Test("Verify Writes (DataIO)",  (loggedTest)VerifyWrites_DataIO).RunTest();

    Test("Verify String Writes (DataVector)",  (loggedTest)VerifyStringWrites_DataVector).RunTest();
    Test("Verify String Writes (DataLump)",  (loggedTest)VerifyStringWrites_DataLump).RunTest();
    Test("Verify String Writes (DataIO)",  (loggedTest)VerifyStringWrites_DataIO).RunTest();

    Test("Verify Alignment (DataVector)",  (loggedTest)VerifyBoundraryFinder_DataVector).RunTest();
    Test("Verify Alignment (DataLump)",  (loggedTest)VerifyBoundraryFinder_DataLump).RunTest();
    Test("Verify Alignment (DataIO)",  (loggedTest)VerifyBoundraryFinder_DataIO).RunTest();

    Test("Verify String Pushes (DataVector)",  (loggedTest)VerifyStringPushes_DataVector).RunTest();
    Test("Verify String Pushes (DataLump)",  (loggedTest)VerifyStringPushes_DataLump).RunTest();
    Test("Verify String Pushes (DataIO)",  (loggedTest)VerifyStringPushes_DataIO).RunTest();

    Test("Verify POD Pushes (DataVector)",  (loggedTest)VerifyPODPush_DataVector).RunTest();
    Test("Verify POD Pushes (DataLump)",  (loggedTest)VerifyPODPush_DataLump).RunTest();
    Test("Verify POD Pushes (DataIO)",  (loggedTest)VerifyPODPush_DataIO).RunTest();
    return 0;
}

template<class T>
void Filler(T& container, int size, char filler) {
    for (int i = 0; i < size; i++) {
        container.Put(i,filler);
    }
}

long VerifyOffset(BinaryWriter& writer, long offset, 
                                              string msg,
                                              char c) 
{
    long statedOffset = writer.Offset();
    long castedOffset = (long)writer;
    if ( statedOffset != offset ) {
        ostringstream err;
        err << msg;
        err << "\nreader.Offset returned an unexpected value: ";
        err << statedOffset;
        err << " ( I expected: ";
        err << offset;
        err << " )";
        throw TestError(err.str(),statedOffset);
    }
    if ( castedOffset != offset ) {
        ostringstream err;
        err << msg;
        err << "\n(long)writer returned an unexpected value: ";
        err << castedOffset;
        err << " ( I expected: ";
        err << offset;
        err << " )";
        throw TestError(err.str(),castedOffset);
    }
    if ( offset < DATA_SIZE  ) {
        char old = data[offset];
        writer.Write("c",1);
        char got = data[offset];
        if ( got != 'c' ) {
            ostringstream err;
            err << msg;
            err << "\n[] returned an unexpected character: ";
            err << got;
            err << " ( I expected: ";
            err << 'c';
            err << " )";
            throw TestError(err.str(),castedOffset);
        }
        writer.Write(&old,1);
    }
    return 0;
}

long VerifyOffset(const BinaryReader& reader, long offset, 
                                              string msg,
                                              char c) 
{
    long statedOffset = reader.Offset();
    long castedOffset = (long)reader;
    if ( statedOffset != offset ) {
        ostringstream err;
        err << msg;
        err << "\nreader.Offset returned an unexpected value: ";
        err << statedOffset;
        err << " ( I expected: ";
        err << offset;
        err << " )";
        throw TestError(err.str(),statedOffset);
    }
    if ( castedOffset != offset ) {
        ostringstream err;
        err << msg;
        err << "\n(long)reader returned an unexpected value: ";
        err << castedOffset;
        err << " ( I expected: ";
        err << offset;
        err << " )";
        throw TestError(err.str(),castedOffset);
    }
    if ( offset < reader.End() ) {
        char got = reader.Get();
        if ( got != c ) {
            ostringstream err;
            err << msg;
            err << "\nGet returned an unexpected character: ";
            err << got;
            err << " ( I expected: ";
            err << c;
            err << " )";
            throw TestError(err.str(),castedOffset);
        }
    }
    return 0;
}

long VerifyData(const char* d1, const char *d2, long size) {
    for (int i = 0; i < size; i++) {
        if ( d1[i] != d2[i] ) {
            ostringstream err;
            err << "Missmatch in data streams at position ";
            err << i << ": " << d1[i];
            err << " ( " << int(d1[i]) << " ) ";
            err << " and " << d2[i];
            err << " ( " << int(d2[i]) << " ) " << endl;

            char *str1 = new char[i+1];
            char *str2 = new char[i+1];
            char *ptr = new char[i+1];
            for ( int j=0; j<=i; j++ ) {
                if ( d1[j] == '\0' ) { 
                    str1[j] = '`';
                } else {
                    str1[j] = d1[j];
                }

                if ( d2[j] == '\0' ) { 
                    str2[j] = '`';
                } else {
                    str2[j] = d2[j];
                }
                ptr[j] = ' ';
            }

            if ( d1[i] == '\0' ) { 
                str1[i] = '`';
            } else {
                str1[i] = d1[i];
            }

            if ( d2[i] == '\0' ) { 
                str2[i] = '`';
            } else {
                str2[i] = d2[i];
            }
            ptr[i] = '^';
            str1[i+1] = '\0';
            str2[i+1] = '\0';
            ptr[i+1] = '\0';
            err << endl << str1 << endl;
            err << str2 << endl;
            err << ptr << endl;
            delete [] str1;
            delete [] str2;
            delete [] ptr;

            throw TestError(err.str(),i);
        }
    }
    return 0;
}

long VerifyContents(FileLikeReader& d1, long start1, const char* data2, long size) {
    char* data1 = new char[size];

    d1.Read(start1,data1,size);

    long ret =  VerifyData(data1,data2,size);

    delete [] data1;

    return ret;
}

long VerifyContents(FileLikeReader& d1, long start1, FileLikeReader& d2, long start2, long size) {
    char* data1 = new char[size];
    char* data2 = new char[size];

    d1.Read(start1,data1,size);
    d2.Read(start2,data2,size);

    long ret =  VerifyData(data1,data2,size);

    delete [] data1;
    delete [] data2;

    return ret;
}

long VerifyOffset(BinaryWriter& writer, long offset, string msg) {
    return VerifyOffset(writer,offset,msg,char(offset));
}

int VerifyPositionFinders( testLogger& log) {
    try {
        BinaryWriter pos(data);
        BinaryWriter other = pos.operator+(DATA_SIZE);
        VerifyOffset(pos,0, "Initialisation");
        BinaryWriter b = other.Begin();
        VerifyOffset(b,0,"Begin()");
        for (int i = 0; i < DATA_SIZE; i++) {
            BinaryWriter newReader = other.Pos(i);
            pos = i;
            VerifyOffset(newReader,i,"= Pos(i)");
            VerifyOffset(pos,i, " = i");
        }

    } catch (TestError& e) {
        return e.Error(log);
    }
    return 0;
}

template<class OBJECT>
int VerifyBoundraryFinder(OBJECT& data, testLogger& log) {
    BinaryWriter pos(data,50);
    if (   pos.NextBoundrary(10) != pos
        || pos.NextBoundrary(2) != pos
        || pos.NextBoundrary(1) != pos
        || pos.NextBoundrary(50) != pos ) 
    {
        log << "moved off a valid boundrary!" << endl;
        return 1;
    }
    if ( pos.NextBoundrary(3) != pos.operator+(1) ) {
        log << "Failed to move to the correct boundrary";
        log << " ( " << pos.NextBoundrary(3).Offset() << endl;
        return 2;
    }
    BinaryWriter pos0(data,0);
    if (   pos0.NextBoundrary(10) != pos0
        || pos0.NextBoundrary(2) != pos0
        || pos0.NextBoundrary(1) != pos0
        || pos0.NextBoundrary(50) != pos0 ) 
    {
        log << "moved off a valid boundrary!" << endl;
        return 3;
    }
    BinaryWriter pos2(data,2);
    if ( pos2.NextBoundrary(4) != pos2.Pos(4)) {
        log << "Failed to move to the correct boundrary";
        log << " ( " << pos2.NextBoundrary(3).Offset() << endl;
        return 4;
    }
    BinaryWriter pos12(data,12);
    if ( pos12.NextBoundrary(8) != pos12.Pos(16)) {
        log << "Failed to move to the correct boundrary";
        log << " ( " << pos12.NextBoundrary(8).Offset() << endl;
        return 5;
    }
    return 0;
}

int VerifyAssignments( testLogger& log) {
    try {
        BinaryWriter pos(data);
        BinaryWriter other = pos.Pos(5);
        pos+=5;
        VerifyOffset(pos,5,"+= int");
        pos+= other;
        VerifyOffset(pos,10,"+= other");
        pos-= other;
        VerifyOffset(pos,5,"-= other");
        pos-= 1;
        VerifyOffset(pos,4,"-= int");
        pos = 123;
        VerifyOffset(pos,123,"= int");
    } catch (TestError& e) {
        return e.Error(log);
    }
    return 0;
}


int VerifyArithmetic( testLogger& log) {
    try {
        BinaryWriter pos(data);
        pos+=1;
        BinaryWriter other = pos.Pos(5);
        BinaryWriter p0 = pos.operator+(5);
        VerifyOffset(p0,6,"+ int");
        VerifyOffset(pos,1,"+ int (no change)");
        BinaryWriter p = pos + other;
        VerifyOffset(p,6,"+ other");
        VerifyOffset(pos,1,"+ other (no change)");
        BinaryWriter p2 = other.operator-(3);
        VerifyOffset(p2,2,"- int");
        VerifyOffset(other,5,"- int (no change)");
        BinaryWriter p3 = other - pos;
        VerifyOffset(p3,4,"- other");
        VerifyOffset(other,5,"- int (no change)");
    } catch (TestError& e) {
        return e.Error(log);
    }
    return 0;
}

template <class T> 
int VerifyWrites(T& data, T& out, T& zeroes, T& twos, T& cs, testLogger& log) {
    try {
        char c_zeroes[100];
        char c_twos[100];
        char c_cs[100];

        Filler(zeroes,100,0);
        Filler(twos,100,2);
        Filler(cs,100,'c');
        FillContainer(c_zeroes,100,0);
        FillContainer(c_twos,100,2);
        FillContainer(c_cs,100,'c');

        // Fill
        for (int i = 0; i < 100; i++) {
            out.Put(i,data.Get(i));
        }
        BinaryWriter beg(out);
        BinaryWriter off = beg.operator+(50);
        BinaryWriter off2 = beg.operator+(75);
        beg.Fill(20);
        off.Fill(20,2);
        off2.FillTo(off.Begin() + BinaryReader(out).Find(95),'c');
        
        log << "front" <<endl;
        VerifyContents(out,0,zeroes,0,20);
        log << "data" << endl;
        VerifyContents(out,20,data,20,20);
        log << "twos" << endl;
        VerifyContents(out,50,twos,0,20);
        log << "padding" <<endl;
        VerifyContents(out,70,data,70,5);
        log << "FillTo" <<endl;
        VerifyContents(out,75,cs,75,20);
        log << "back" <<endl;
        VerifyContents(out,95,data,95,5);

        // Write
        for (int i = 0; i < 100; i++) {
            out.Put(i,0);
        }
        BinaryWriter wbeg(out);
        BinaryWriter woff = beg.operator+(50);
        BinaryWriter woff2 = beg.operator+(75);


        wbeg.Write(c_data,100);
        woff.Write(c_zeroes,20);
        woff2.Write(BinaryReader(cs).Begin(),20);

        log << "write" <<endl;
        VerifyContents(out,0,c_data,50);
        log << "offset write" << endl;
        VerifyContents(out,50,c_zeroes,20);
        log << "Write reader" << endl;
        VerifyContents(out,75,c_cs,20);
        log << "back" << endl;
        VerifyContents(out,95,c_data+95,5);

    } catch (TestError& e) {
        return e.Error(log);
    }
    return 0;
}


template <class T>
int VerifyStringWrites(T& data, T& out, T& out2, testLogger& log ) {
    try {
        char str[] = "Hello World!\0++IGNORED++";
        //don't forget the null char
        int len = strlen(str) + 1;
        string stds = "some other string";
        for (int i = 0; i < 100; i++) {
            out.Put(i,data.Get(i));
            out2.Put(i,data.Get(i));
        }

        BinaryWriter w(out);
        BinaryWriter pos = w.operator+(50);
        BinaryWriter w2(out2);
        w2+=10;

        w.WriteString(str);
        pos.WriteString(stds);
        w2.WriteString(BinaryReader(out).Pos(50));

        log << "array write" << endl;
        VerifyContents(out,0,str,len);
        log << "left alone" << endl;
        VerifyContents(out,len,data,len,50-len);
        log << "string write" << endl;
        VerifyContents(out,50,stds.c_str(),stds.length()+1);
        log << "back" << endl;
        VerifyContents(out,50 +stds.length() +1,
                       data,50 + stds.length() + 1,
                       50 - 1 - stds.length());
        log << "skipped" << endl;
        VerifyContents(out2,0,data,0,10);
        log << "reader write" << endl;
        VerifyContents(out2,10,stds.c_str(),stds.length());
        log << "back (2) " << endl;
        VerifyContents( out2, 10 + stds.length() + 1,
                        data, 10 + stds.length() + 1,
                        100 - 10 - stds.length() -1 );

    } catch (TestError& e) {
        return e.Error(log);
    }
    return 0;
}

template <class T>
int VerifyStringPushes(T& out, T& out2, T& out3, testLogger& log ) {
    try {
        const char *str = "Hello World!\0++IGNORED++";
        //don't forget the null char
        string stds = "some other string";

        DataVector out(100), out2(100), out3(100);
        out.Fill(0,'*',100);
        out2.Fill(0,'*',100);
        out3.Fill(0,'*',100);

        BinaryWriter w(out);
        BinaryWriter w2(out2);
        BinaryWriter w3(out3);

        w.WriteString(str);
        w+=strlen(str) + 1;
        w.WriteString(stds);

        w2 << str;
        w2 << stds.c_str();

        w3 << str << stds.c_str();

        log << "Checking separate pushes" << endl;
        VerifyContents(out,0,out2,0,100);
        log << "Checking single push" << endl;
        VerifyContents(out,0,out3,0,100);

    } catch (TestError& e) {
        return e.Error(log);
    }
    return 0;
}

template <class OBJECT>
int VerifyPODPush( OBJECT& dv, testLogger& log) {
    
    dv.Fill(0,'*',100);
    BinaryWriter w(dv);
    BinaryReader r(dv);
    
    int i = 1;
    long l = 10;
    float f = 100;
    double d = 1000;
    
    struct POD1 {
            int i;
            long l;
            float f;
            double d;
            long sum() { return i + l + f +d; }
            bool operator== (POD1& rhs) { 
                return memcpy(this,&rhs,sizeof(POD1)) != 0;
            }
    };

    struct POD2 {
            POD1 p1;
            int ints[5];
            POD1 p3;
            long sum () {
               long sum = 0;
               for (int i = 0; i < 5; i++) {
                   sum+=ints[i];
               }
               return p1.sum() + p3.sum() + sum;
            }
            bool operator== (POD2& rhs) { 
                return memcpy(this,&rhs,sizeof(POD2)) != 0;
            }
    };
    
    
    struct POD3 {
            POD1 p1s[5];
            POD2 p2s[5];
            long sum () {
                long sum = 0;
                for (int i = 0; i < 5; i++) {
                    sum+=p1s[i].sum() + p2s[i].sum();
                }
                return sum;
            }
            bool operator== (POD3& rhs) { 
                return memcpy(this,&rhs,sizeof(POD3)) != 0;
            }
    };
    
    POD1 p1a = {i,l,f,d};
    POD1 p1b = {2*i,2*l,2*f,2*d};
    POD2 p2 = {p1a,{1,2,3,4,5},p1b};
    POD3 p3 = { {p1a,p1b,p1a,p1b,p1a}, { p2,p2,p2,p2,p2} };

    w << i << l << f << d << p1a << p1b << p2 << p3;

    // We've alrady independantly validated the binaryReader:
    int ri = 30;
    long rl = 30000;
    float rf = 34345.45345;
    double rd = 3453453434.34534534;
    POD1 rp1a;
    POD1 rp1b;
    POD2 rp2;
    POD3 rp3;

    r >> ri;
    r >> rl;
    r >> rf;
    r >> rd;
    r >> rp1a;
    r >> rp1b;
    r >> rp2;
    r >> rp3;

    if ( ri != i ) {
        log << "i missmatch, " << ri << endl;
        return 1;
    }

    if ( rl != l ) {
        log << "l missmatch, " << rl << endl;
        return 1;
    }

    if ( rf != f ) {
        log << "f missmatch, " << rf << endl;
        return 1;
    }

    if ( rd != d ) {
        log << "d missmatch,  " << rd << endl;
        return 1;
    }

    if ( rp1a.sum() != p1a.sum() || !(rp1a == p1a) ) {
        log << "p1a missmatch, " << endl;;
        return 1;
    }

    if ( rp1b.sum() != p1b.sum() || !(rp1b == p1b) ) {
        log << "p1b missmatch, "<< endl;;
        return 1;
    }

    if ( rp2.sum() != p2.sum() || !(rp2 == p2) ) {
        log << "p2 missmatch, "<< endl;;
        return 1;
    }

    if ( rp3.sum() != p3.sum() || !(rp3 == p3) ) {
        log << "p3 missmatch, "<< endl;;
        return 1;
    }

    return 0;
}

int VerifyWrites_DataVector(testLogger& log ) {
    DataVector d1(100), d2(100), d3(100), d4(100);
    return VerifyWrites(data,d1,d2,d3,d4,log);
}
int VerifyWrites_DataLump(testLogger& log ) {
    DataLump<128> d1, d2, d3, d4;
    return VerifyWrites(lump,d1,d2,d3,d4,log);
}
int VerifyWrites_DataIO(testLogger& log ) {
    unsigned char d1[128], d2[128], d3[128], d4[128];

    DataIO dio_lump(io_lump,128);
    DataIO dio_1(d1,128);
    DataIO dio_2(d2,128);
    DataIO dio_3(d3,128);
    DataIO dio_4(d4,128);

    return VerifyWrites(dio_lump,dio_1,dio_2,dio_3,dio_4,log);
}

int VerifyStringWrites_DataVector(testLogger& log ) {
    DataVector dv(100), dv2(100);
    return VerifyStringWrites(data,dv,dv2,log);
}
int VerifyStringWrites_DataLump(testLogger& log ) {
    DataLump<128> dl, dl2;
    return VerifyStringWrites(lump,dl,dl2,log);
}
int VerifyStringWrites_DataIO(testLogger& log ) {
    unsigned char d1[128], d2[128];

    DataIO dio_lump(io_lump,128);
    DataIO dio_1(d1,128);
    DataIO dio_2(d2,128);

    return VerifyStringWrites(dio_lump,dio_1,dio_2,log);
}

int VerifyBoundraryFinder_DataVector( testLogger& log) {
    return VerifyBoundraryFinder(data,log);
}
int VerifyBoundraryFinder_DataLump( testLogger& log) {
    return VerifyBoundraryFinder(lump,log);
}
int VerifyBoundraryFinder_DataIO( testLogger& log) {
    DataIO dio_lump(io_lump,128);
    return VerifyBoundraryFinder(dio_lump,log);
}

int VerifyStringPushes_DataVector( testLogger& log) {
    DataVector dv1(100), dv2(100), dv3(100);
    return VerifyStringPushes(dv1,dv2,dv3,log);
}
int VerifyStringPushes_DataLump( testLogger& log) {
    DataLump<100> dl1, dl2, dl3;
    return VerifyStringPushes(dl1,dl2,dl3,log);
}
int VerifyStringPushes_DataIO( testLogger& log) {
    unsigned char d1[128], d2[128], d3[128];

    DataIO dio_1(d1,128);
    DataIO dio_2(d2,128);
    DataIO dio_3(d3,128);

    return VerifyStringPushes(dio_1,dio_2,dio_3,log);
}

int VerifyPODPush_DataVector(testLogger& log ) {
    DataVector dv(100);
    return VerifyPODPush(dv,log);
}

int VerifyPODPush_DataLump(testLogger& log ) {
    DataLump<10000> dl;
    return VerifyPODPush(dl,log);
}
int VerifyPODPush_DataIO(testLogger& log ) {
    unsigned char d1[10000];

    DataIO dio_1(d1,10000);
    return VerifyPODPush(dio_1,log);
}
