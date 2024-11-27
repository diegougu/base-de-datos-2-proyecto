#ifndef COMP_HPP
#define COMP_HPP
#include <vector>


//Implementación de la clase 'comp'
template <class T>
struct comp
{
    int sec_c, pista_c, superficie_c, plato_c, disco_c, bytes;
    vector <T> sec;
    comp <T>* next = nullptr;
    comp(int s_c, int p_c, int su_c, int pl_c, int d_c, int b, comp<T>* n = nullptr)
    :   sec_c(s_c ? s_c : 1), 
        pista_c(p_c ? p_c : 1), 
        superficie_c(su_c ? su_c : 1), 
        plato_c(pl_c ? pl_c : 1), 
        disco_c(d_c ? d_c : 1), 
        bytes(b ? b : 100), 
        next(n) {}
    int bytes_ocupados = 0;
};
#endif