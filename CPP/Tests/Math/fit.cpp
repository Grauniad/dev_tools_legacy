#include "straightLineFit.h"
#include "tester.h"
#include "stdReader.h"

int FitData(testLogger& log );
using DATA_FILE = CSV<double,double,double,double,double,double>;

int main(int argc, const char *argv[])
{
    Test("Checking basic fit...",FitData).RunTest();
    return 0;
}

int FitData(testLogger& log ) {
    double m = 12.4447656531;
    double c = -2.4475625251;
    double sumq = 244.9580528016;

    double error = 0.0000001;

    // Load the test data
    IFStreamReader in_f("GNU.csv");
    DATA_FILE csv(DATA_FILE::LoadCSV(in_f));

    // Do the fit
    StraightLineFit fit(StraightLineFit::MakeFit<0,5>(csv));

    // check the results
    if ( fit.M() >  (m + error) || fit.M() < (m - error) ) {
        log << "Expected fit to be: " << m
            << " but got: " << fit.M() << endl;
        return 1;
    }

    if ( fit.C() >  (c + error) || fit.C() < (c - error) ) {
        log << "Expected fit to be: " << c
            << " but got: " << fit.C() << endl;
        return 1;
    }

    if ( fit.SUM_RESIDUALS() >  (sumq + error) || fit.SUM_RESIDUALS() < (sumq - error) ) {
        log << "Expected RSQ to be: " << sumq
            << " but got: " << fit.SUM_RESIDUALS() << endl;
        return 1;
    }
    return 0;
}
