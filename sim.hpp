#ifndef SIM_HPP
#define SIM_HPP

template <class T>
class sim{
    private:
        comp<T>* head = nullptr;
        comp<T>* tail = nullptr;   
        int sec_c, pista_c, superficie_c, plato_c, disco_c, bytes;  
        void update_pos(comp<T>* node);
    public:
        sim (int s_c,int p_c,int su_c,int pl_c,int d_c, int b);
        void add(T x);
        bool find_print(T x);
        void print();
};
#endif