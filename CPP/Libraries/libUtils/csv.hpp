#include "csv.h"
template<class...Types>
int CSV<Types...>::Rows() {
    return std::get<0>(this->columns).size();
}

template<class...Types>
CSV<Types...>&& CSV<Types...>::LoadCSV(BinaryReader reader) {
    CSV<Types...> csv;

    for ( std::string l = reader.ReadString();
          reader.Offset() < reader.End();
          reader+= l.length()) 
    {
        NewRow(Tokeniser(l));
    }

    return std::move(csv);
}

//*********************************
// template to add from tokens
//*********************************
template<class...Types>
void CSV<Types...>::NewRow(const Tokeniser& tok) {
    Tokeniser::iter it = tok.begin();
    AddCell<ncols-1>(this->data,it);
}

template<class...Types>
template< int index>
inline typename std::enable_if< index!=0,void>::type
CSV<Types...>::AddCell(Tokeniser::iter& it) {
    std::get<index>(this->columns).NewRow(*it);
    it++;
    this->AddCell<index -1>(it);
}

template<class...Types>
template< int index>
inline typename std::enable_if< index==0,void>::type
CSV<Types...>::AddCell(Tokeniser::iter& it) {
    std::get<index>(this->columns).NewRow(*it);
}
//*********************************

//*********************************
// template to add from values
//*********************************
template<class...Types>
void CSV<Types...>::AddRow(Types&&...args) { 
    this->AddCell<ncols-1>(forward_as_tuple(args...));
}

template<class...Types>
template<int index>
inline typename std::enable_if< index!=0,void>::type
CSV<Types...>::AddCell(forwardRowType&& row) {
    std::get<index>(this->columns).emplace_back(std::get<index>(row));
    this->AddCell<index-1>(row);
}

template<class...Types>
template<int index>
inline typename std::enable_if< index==0,void>::type
CSV<Types...>::AddCell(forwardRowType&& row) {
    std::get<index>(this->columns).emplace_back(std::get<index>(row));
}
//*********************************

//*********************************
// template to get a row
//*********************************
template<class...Types>
typename CSV<Types...>::rowType&& 
CSV<Types...>::GetRow(int i) {
    return this->GetCell<ncols-1>(make_tuple(),i);
}

template<class...Types>
template< int index, class tupleType, class extendedTupleType>
inline typename std::enable_if< index!=0,extendedTupleType>::type&&
CSV<Types...>::GetCell(tupleType&& t, int row) {
    extendedTupleType&& cells =
         tuple_cat(t, tie(std::get<index>(this->columns)[row]));
    return this->GetCell<index -1>(cells,row);
}

template<class...Types>
template< int index, class tupleType, class extendedTupleType>
inline typename std::enable_if< index==0,extendedTupleType>::type&&
CSV<Types...>::GetCell(tupleType&& t, int row) {
    extendedTupleType&& cells =
         tuple_cat(t, tie(std::get<index>(this->columns)[row]));
    return cells;
}
//*********************************
  
//*********************************
// template to print a row
//*********************************
template<class...Types>
std::string&& CSV<Types...>::PrintRow(int i) {
    std::stringstream s;
    this->PrintCell<ncols-1>(s,i);
    return std::move(s.str());
}

template<class...Types>
template<int index>
inline typename std::enable_if< index!=0,void>::type&&
CSV<Types...>::PrintCell(std::stringstream& s, int row) {
    std::get<index>(this->columns).Print(s,row);
    this->PrintCell<index-1>(s,row);
}

template<class...Types>
template<int index>
inline typename std::enable_if< index==0,void>::type&&
CSV<Types...>::PrintCell(std::stringstream& s, int row) {
    std::get<index>(this->columns).Print(s,row);
}
//*********************************


//*********************************
// template to remove a row
//*********************************
template<class...Types>
void CSV<Types...>::RemoveRow(int i) {
    this->RemoveCell<ncols-1>(i);
}

template<class...Types>
template< int index>
inline typename std::enable_if< index!=0,void>::type
CSV<Types...>::RemoveCell(int row) {
    std::get<index>(this->columns).remove(row);
    this->GetCell<index -1>(row);
}

template<class...Types>
template< int index>
inline typename std::enable_if< index==0,void>::type
CSV<Types...>::RemoveCell(int row) {
    std::get<index>(this->columns).remove(row);
}
//*********************************
  
