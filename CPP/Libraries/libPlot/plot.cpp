#include "plot.h"
#include  "plplot/plplot.h"

using namespace std;
using namespace Plot;

namespace {
    void PlotInit(const Plot_2D& params) {
        const char* plargv[] = { "plotter",  /* argv0 dummy */
                                  "-o",   params.fname.c_str(), 
                                  "-dev", params.device.c_str()
                               };
        int plargc = 5;

        // Setup the device
        plparseopts(&plargc, plargv, PL_PARSE_FULL);

        // Start the plot
        plinit();

        // Setup the page
        plenv( params.xmin, params.xmax, 
               params.ymin, params.ymax, 
               params.forceSquare, 
               params.axis);
    }
}

Plot::Plot_2D::Plot_2D( double _xmin, double _xmax,
                                double _ymin, double _ymax,
                                string _fname,
                                string _device,
                                bool   _forceSquare,
                                AXIS_STYLE _axis)
    : xmin(_xmin), xmax(_xmax), ymin(_ymin), ymax(_ymax),
      fname(_fname), device(_device),
      forceSquare(_forceSquare),
      axis(_axis)
{
}

void Plot::HeatMap(const Plot_2D& config, double ** data) {
    PlotInit(config);
}
