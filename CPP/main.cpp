// codigo de prueba, proyecto.cpp es codigo condicionado hasta ahora

#include "comp.hpp"
#include "sim.hpp"
#include "table.hpp"
#include <iostream>
using namespace std;

// Implementación del método 'add' de 'sim'
template <class T>
void sim<T>::add(T x) {
    comp<T>* new_node = new comp<T>(sec_c, pista_c, superficie_c, plato_c, disco_c, bytes);
    if (!head) {
        head = tail = new_node;
    } else {
        tail->next = new_node;
        tail = new_node;
    }
}

// Implementación del método 'find_print' de 'sim'
template <class T>
bool sim<T>::find_print(T x) {
    comp<T>* current = head;
    while (current) {
        if (*current == x) {
            current->print_obj();
            return true;
        }
        current = current->next;
    }
    return false;
}

// Implementación del método 'print' de 'sim'
template <class T>
void sim<T>::print() {
    comp<T>* current = head;
    while (current) {
        current->print_obj();
        current = current->next;
    }
}

// Implementación del método 'update_pos' de 'sim'
template <class T>
void sim<T>::update_pos(comp<T>* node) {
    if (++node->sec_c > sec_c) { // Si el número de sector supera el máximo
        node->sec_c = 1; // Reinicia el sector
        if (++node->pista_c > pista_c) { // Si el número de pista supera el máximo
            node->pista_c = 1; // Reinicia la pista
            if (++node->superficie_c > superficie_c) { // Si la superficie supera el máximo
                node->superficie_c = 1; // Reinicia la superficie
                if (++node->plato_c > plato_c) { // Si el plato supera el máximo
                    node->plato_c = 1; // Reinicia el plato
                    if (++node->disco_c > disco_c) { // Si el disco supera el máximo
                        node->disco_c = 1; // Reinicia el disco
                    }
                }
            }
        }
    }
}

// Implementación de los métodos 'print_obj' de 'table'
void table::print_obj() {
    cout << "Index: " << index << ", Item: " << item << ", Cost: " << cost << ", Tax: " << tax << ", Total: " << total << endl;
}

// Implementación del operador '==' de 'table'
bool table::operator==(const table& other) {
    return index == other.index;
}

int main() {
    sim<table> simulacion(2, 2, 2, 2, 2, 200);

    table o1(1, "Fruit of the Loom Girls Socks", 7.97, 0.6, 8.57);
    table o2(2, "Rawlings Little League Baseball", 2.97, 0.22, 3.19);
    table o3(3, "Secret Antiperspirant", 1.29, 0.1, 1.39);
    table o4(4, "Deadpool DVD", 14.96, 1.12, 16.08);
    table o5(5, "Maxwell House Coffee", 7.28, 0.55, 7.83);
    table o6(6, "Banana Boat Sunscreen", 6.68, 0.5, 7.18);
    table o7(7, "Wrench Set, 18 pieces", 10.00, 0.75, 10.75);
    table o8(8, "M and M, 42 oz", 8.98, 0.67, 9.65);
    table o9(9, "Bertolli Alfredo Sauce", 2.12, 0.16, 2.28);
    table o10(10, "Large Paperclips, 10", 6.19, 0.46, 6.65);

    simulacion.add(o1);
    simulacion.add(o2);
    simulacion.add(o3);
    simulacion.add(o4);
    simulacion.add(o5);
    simulacion.add(o6);
    simulacion.add(o7);
    simulacion.add(o8);
    simulacion.add(o9);
    simulacion.add(o10);


    simulacion.print();

    simulacion.find_print(o1);
    simulacion.find_print(o4);
    simulacion.find_print(o8);
}