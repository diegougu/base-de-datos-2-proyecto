#ifndef TABLE_HPP
#define TABLE_HPP
#include <string>
using namespace std;

struct table{
    int index;
    string item;
    float cost, tax, total;
    table(int i, string it, float c, float t, float to):
        index(i), 
        item(it), 
        cost(c), 
        tax(t), 
        total(to) {}
    void print_obj();
    bool operator==(const table& other);    
};
#endif