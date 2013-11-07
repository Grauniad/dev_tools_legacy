#include <algorithm>

template <class NUM>
void Math::MinMax2D(NUM** data, int xdim, int ydim, NUM& min, NUM& max) {

    NUM local_min;
    NUM local_max;

    MinMax(data[0], ydim, min, max);

    for ( int i = 1; i< xdim; i++ ) {
        MinMax(data[i], ydim, local_min, local_max);
        if ( local_min < min ) 
            min = local_min;
        if ( local_max > max ) 
            max = local_max;
    }
}

template <class NUM>
void Math::MinMax(NUM* data, int dim, NUM& min, NUM& max) {
    auto result = std::minmax_element(data,data+dim);
    min = *(result.first);
    max = *(result.second);
}

template <class NUM>
void Math::Fill(NUM* data, int count, NUM start, NUM step ) {
    NUM working = start;
    for ( int i =0; i< count; i++) {
        data[i] = working;
        working += step;
    }
}
