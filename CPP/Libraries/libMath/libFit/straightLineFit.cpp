#include "straightLineFit.h"
#include "gsl/gsl_fit.h"
#include "logger.h"

StraightLineFit::StraightLineFit(double *x, double *y, size_t n): m(1), c(1), sumq(0) {
    double tmp[2][2];
    IF_LOG( LOG_VERY_VERBOSE, 
        std::stringstream& s = LogFactory::Buf();
        s << "Fitting data for " << n << " data points: " << endl;
        for ( size_t i=0; i< n; i++ ) {
            s << x[i] << ", " << y[i] << endl;
        }
        Logger::Instance().LogMessage(s.str(), LOG_VERY_VERBOSE, "StraightLineFit::StraightLineFit");
    )
    gsl_fit_linear(x,1,y,1,n, /* in data */
                   &c,&m,     /* fit coeficients */
                   &(tmp[0][0]), &(tmp[0][1]), &(tmp[1][1]), /* covariance matrix */
                   &sumq /* sum of squares */
                   );


}
