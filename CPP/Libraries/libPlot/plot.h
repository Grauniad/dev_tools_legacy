#ifndef DEVTOOLS_CPP_LIBRARIES_PLOT_H
#define DEVTOOLS_CPP_LIBRARIES_PLOT_H

#include <string>
#include  "plplot/plplot.h"

namespace Plot {

    enum AXIS_STYLE {
        NONE = -2,
        BOX_ONLY = -1,
        BOX_AND_VALUES = 0,
        BOX_AND_VALUES_AND_AXES = 1,
        BOX_AND_VALUES_AND_MAJOR = 2,
        LOG_X_LIN_Y = 10,
        LOG_X_LIN_Y_LINE_Y= 11,
        LIN_X_LOG_Y = 20,
        LIN_X_LOG_Y_LINE_X = 21,
        LOG_X_LOG_Y = 30
    };

    struct Plot_2D {
        Plot_2D( double _xmin=0, 
                 double _xmax=1,
                 double _ymin=0, 
                 double _ymax=1,
                 std::string _fname="plot.ps",
                 std::string _device="psc",
                 bool   _forceSquare = false,
                 AXIS_STYLE _axis = BOX_AND_VALUES);

        double         xmin;
        double         xmax;
        double         ymin;
        double         ymax;
        std::string    fname;
        std::string    device;
        bool           forceSquare; // Scale x & Y the same?
        AXIS_STYLE     axis;
    };

    class Data_2D {
    public:
        Data_2D(double** _data, int _nx, int _ny, const Plot_2D& config);

        virtual ~Data_2D();

        double** Data() {
            return data;
        }

        int NX() { return nx; }
        int NY() { return ny; }

        PLcGrid* Grid();

    private:
        PLcGrid gridMap;
        double** data;
        int nx;
        int ny;
    };

    void HeatMap(const Plot_2D& config, Data_2D& data);
}

#endif
