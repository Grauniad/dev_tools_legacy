// Do a sum
template<class T,class S>
T Subset::Accumulate( S data, T&& init) {
    for( size_t& idx:  idxs) {
        init+=data[idx];
    }
    return std::move(init);
}

template<class T, class S>
std::vector<T> Subset::Values(S source) {
    std::vector<T> v;
    v.resize(Size());
    for ( size_t i = 0; i < Size(); ++i ) {
        v[i] = source[Get(i)];
    }
    return std::move(v);
}
// Apply f
template<class T, class S, class F>
std::vector<T> Subset::Values(S source, F f) {
    std::vector<T> v;
    v.resize(Size());
    for ( size_t i = 0; i < Size(); ++i ) {
        v[i] = f(source[Get(i)]);
    }
    return std::move(v);
}
