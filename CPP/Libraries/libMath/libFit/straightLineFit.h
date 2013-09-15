#ifndef DEV_TOOLS_MATH_FIT_STRAIGHT_LINE
#define DEV_TOOLS_MATH_FIT_STRAIGHT_LINE

#include "csv.h"

class StraightLineFit {
public:
    StraightLineFit(double *x, double *y, size_t n);

    template<int x_col, int y_col, class CSV_TYPE>
    static StraightLineFit MakeFit(CSV_TYPE& csv ) {
         return StraightLineFit(
              csv.template GetColumn<x_col>().array(),
              csv.template GetColumn<y_col>().array(),
              csv.Rows()
         );
     }

    // Return the value of the function at a location
    double f(double x);

    // Populate an existing array with y values for each x value
    void f(double *x, double *y, int n);

    template<class CSV_TYPE, int x_col, int y_col>
    void f(CSV_TYPE& csv) { 
         f( csv.template GetColumn<x_col>().array(),
            csv.template GetColumn<y_col>().array(),
            csv.Rows() 
          );
    }

    // Generate data between a range
    CSV<double,double> Range(double start, double stop, double step=1.0);

    double M() { return m;}
    double C() { return c;}
    double SUM_RESIDUALS() { return sumq; }
private:
   double m;
   double c;
   double sumq;
};

#endif
