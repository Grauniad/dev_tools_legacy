#include "plot.h"
#include <cmath> 
#include "tester.h"
#include "number.h"


int CheckMinMax(testLogger& log);
int CheckFill(testLogger& log);
int PlotTest (testLogger& log);
int PlotLine (testLogger& log);

using namespace std;
using namespace Plot;
using namespace Math;

int main (int argc, char**argv) {

    Test("Checking MinMax2D...", CheckMinMax).RunTest();
    Test("Checking Fill...", CheckFill).RunTest();
    Test("Making something pretty...", PlotTest).RunTest();
    Test("Making a line...", PlotLine).RunTest();

    /*
     * All seems good...lets plot!
     */
    return 0;
}

int PlotLine ( testLogger& log ) {
    Plot_2D plot_config;
    plot_config.xmin = 0;
    plot_config.ymin = 0;
    plot_config.xmax = 10;
    plot_config.ymax = 100;
    plot_config.fname = "plot2.svg";
    plot_config.xlabel = "Hello World2 !";
    plot_config.ylabel = "Hello World!";
    plot_config.title = "Hello World!";

    const int NUM = 500;
    double xstep = 10.0/NUM;

    double x[NUM], y[NUM];
    for ( int i =0; i< NUM; i++ ) {
        x[i] = 1 + i*xstep;
        y[i] = x[i]*x[i];
        log << x[i] << y[i] << endl;
    }

    Data_1D plot_data(x, y, NUM);
    Line(plot_config, plot_data);
    Done();

    return 0;
}

int PlotTest (testLogger& log) {
    double**  data = new double*[10];
    for ( int i =0; i < 10; i++ ) {
        data[i] = new double[10];

        for ( int j =0; j < 10; j++ ) {
            data[i][j] = i;
        }
    }

    Plot_2D plot_config;
    plot_config.xmax = 100;
    plot_config.ymax = 100;
    Data_2D plot_data(data,10,10, plot_config);

    HeatMap(plot_config, plot_data);
    Done();

    /*
     * Clean up
     */
    for ( int i =0; i < 10; i++ ) {
        delete [] data[i];
    }
    delete [] data;

    return 0;
}

int CheckMinMax ( testLogger& log ) {
    double**  data = new double*[10];
    for ( int i =0; i < 10; i++ ) {
        data[i] = new double[10];

        for ( int j =0; j < 10; j++ ) {
            data[i][j] = fabs(i-5) + fabs(j-5);
        }
    }

    double min, max;
    MinMax2D(data, 10,10,min , max);

    if ( min != 0 || max != 10) {
        log << "min: " << min;
        log << "max: " << max;
        return 1;
    }

    /*
     * Clean up
     */
    for ( int i =0; i < 10; i++ ) {
        delete [] data[i];
    }
    delete [] data;

    return 0;
}

int CheckFill ( testLogger& log ) {
    const int levels = 10;
    double min = 0;
    double max = 5;
    double  data[levels];

    double step = (max - min) / levels;
    Math::Fill(data, levels, min, step);

    for ( int i=0; i<levels; i++ ) {
        log  << i << ": " << data[i] << endl;;
        if ( data[i] != (min + i*step) ) {
            return 1;
        }
    }
    return 0;


}
