#include "plot.h"
#include "number.h"

using namespace std;
using namespace Plot;

namespace {
    static bool initialised = false;

    void PlotInit(const Plot_2D& params) {

        if ( !initialised) {
            const char* plargv[] = { "plotter",  /* argv0 dummy */
                                      "-o",   params.fname.c_str(), 
                                      "-dev", params.device.c_str(),
                                      "-mar", "0.04",
                                      "-freeaspect"
                                   };
            int plargc = 8;

            // Setup the device
            plparseopts(&plargc, plargv, PL_PARSE_FULL);

            // Start the plot
            plinit();

            // Setup the page
            plenv( params.xmin, params.xmax, 
                   params.ymin, params.ymax, 
                   params.forceSquare? 1 : 0, 
                   params.axis);

            pllab( params.xlabel.c_str(), 
                   params.ylabel.c_str(), 
                   params.title.c_str());
            initialised = true;
        }
    }
    void PlotDone () {
        if ( initialised) {
            plend();
            initialised = false;
        }
    }
}

Plot::Plot_2D::Plot_2D( double _xmin, double _xmax,
                                double _ymin, double _ymax,
                                string _fname,
                                string _device,
                                bool   _forceSquare,
                                AXIS_STYLE _axis,
                                std::string _xlabel,
                                std::string _ylabel,
                                std::string _title)
    : xmin(_xmin), xmax(_xmax), ymin(_ymin), ymax(_ymax),
      fname(_fname), device(_device),
      forceSquare(_forceSquare),
      axis(_axis),
      xlabel(_xlabel), ylabel(_ylabel), title(_title)
{
}

Plot::Data_1D::Data_1D( double* _x, double* _y, int _n):
                        x(_x), y(_y), n(_n)
{
}

Plot::Data_2D::Data_2D( double** _data, int _nx, int _ny, 
                        const Plot_2D& config):
    data(_data), nx(_nx), ny(_ny) 
{
    gridMap.nx = nx;
    gridMap.ny = ny;
    gridMap.nz = 0;
    gridMap.xg = new double[nx];
    gridMap.yg = new double[ny];

    for ( int i =0 ; i < nx; i++ ) {
        gridMap.xg[i] = float(i) * (config.xmax-config.xmin) / (nx-1);
    }

    for ( int i =0 ; i < ny; i++ ) {
        gridMap.yg[i] = float(i) * (config.ymax-config.ymin) / (ny-1);
    }
}

PLcGrid* Plot::Data_2D::Grid() {
    return &gridMap;
}

Plot::Data_2D::~Data_2D() {
    delete [] gridMap.xg;
    delete [] gridMap.yg;
}

void Plot::Line(const Plot_2D& config, Data_1D& data) {
    PlotInit(config);

    plline(data.N(), data.X(), data.Y());
}

void Plot::HeatMap(const Plot_2D& config, Data_2D& data) {
    PlotInit(config);

    /*
     * First we need to work out our scale
     */
    const int clevels = 500;
    double  level_edges[clevels+1];  

    double min, max;
    Math::MinMax2D(data.Data(),data.NX(), data.NY(), min, max);

    double step = (max - min) / clevels;
    Math::Fill(level_edges, clevels, min, step);

    /*
     * Now we hard code some values
     */
    int fill_width = 1; 

    int cont_color = 0; // set to 0 for none
    int cont_width = 0;
    auto fillFunc = plfill;
    bool rectangular_grid = true;

    // The map from ind in array to cord on graph
    auto mapFunc = pltr1;
    void* userData = data.Grid();

    /*
     * use plplot's types...
     */
    const PLFLT** pl_data = static_cast<const PLFLT**>(
                               const_cast<const double**>(data.Data())
                            );

    /*
     * Let's plot!
     */
    plshades(pl_data, data.NX(), data.NY(), 
             NULL, /*func to filter data*/
             config.xmin, config.xmax, config.ymin, config.ymax,
             level_edges, clevels, 
             fill_width, cont_color, cont_width, 
             fillFunc,  
             rectangular_grid, mapFunc, userData);

}

void Plot::Done() {
    PlotDone();
}
