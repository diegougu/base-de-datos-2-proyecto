#include <iostream>
#include <vector>
#include <string>
using namespace std;

struct table {
    int index;
    string item;
    float cost;
    float tax;
    float total;
    table(int i, string it, float c, float t, float to) : index(i), item(it), cost(c), tax(t), total(to) {}
    void print_obj();
    bool operator==(const table& other);
};

void table::print_obj() {
    cout << index << " " << item << " " << cost << " " << tax << " " << total << endl;
}

bool table::operator==(const table& other) {
    return index == other.index && item == other.item && cost == other.cost && tax == other.tax && total == other.total;
}

template <class T>
struct comp {
    int sec_c, pista_c, superficie_c, plato_c, disco_c, bytes;
    vector<T> sec;
    comp<T>* next = nullptr;
    comp(int s_c, int p_c, int su_c, int pl_c, int d_c, int b, comp<T>* n = nullptr) : sec_c(s_c ? s_c : 1), pista_c(p_c ? p_c : 1), superficie_c(su_c ? su_c : 1), plato_c(pl_c ? pl_c : 1), disco_c(d_c ? d_c : 1), bytes(b ? b : 100), next(n) {}
    int bytes_ocupados = 0;
};

template <class T>
class sim {
private:
    comp<T>* head = nullptr;
    comp<T>* tail = nullptr;
    int sec_c, pista_c, superficie_c, plato_c, disco_c, bytes;
    void update_pos(comp<T>* node);
public:
    sim(int s_c, int p_c, int su_c, int pl_c, int d_c, int b) : sec_c(s_c ? s_c : 1), pista_c(p_c ? p_c : 1), superficie_c(su_c ? su_c : 1), plato_c(pl_c ? pl_c : 1), disco_c(d_c ? d_c : 1), bytes(b ? b : 100) {}
    void add(T x);
    bool find_print(T x);
    void print();
};

template <class T>
void sim<T>::update_pos(comp<T>* node) {
    if (++node->sec_c > sec_c) {
        node->sec_c = 1;
        if (++node->pista_c > pista_c) {
            node->pista_c = 1;
            if (++node->superficie_c > superficie_c) {
                node->superficie_c = 1;
                if (++node->plato_c > plato_c) {
                    node->plato_c = 1;
                    if (++node->disco_c > disco_c) {
                        node->disco_c = 1;
                    }
                }
            }
        }
    }
}

template <class T>
void sim<T>::add(T x) {
    if (head == nullptr) {
        comp<T>* newnodo = new comp<T>(1, 1, 1, 1, 1, bytes);
        head = newnodo;
        tail = newnodo;
    }
    if (tail->sec_c == sec_c && tail->pista_c == pista_c && tail->superficie_c == superficie_c && tail->plato_c == plato_c && tail->disco_c == disco_c && tail->bytes_ocupados + sizeof(x) > bytes) {
        return;
    }
    if (sizeof(x) > bytes || tail->bytes_ocupados + sizeof(x) > bytes) {
        comp<T>* temp = tail;
        int sum = sizeof(x);
        while (tail != nullptr && tail->bytes_ocupados + sizeof(x) > bytes) {
            if (tail->sec_c == sec_c && tail->pista_c == pista_c && tail->superficie_c == superficie_c && tail->plato_c == plato_c && tail->disco_c == disco_c) {
                tail = temp;
                if (head == tail) {
                    temp = head;
                    head = nullptr;
                    tail = nullptr;
                }
                else {
                    temp = tail->next;
                    tail->next = nullptr;
                }
                while (temp != nullptr) {
                    comp<T>* d = temp;
                    temp = temp->next;
                    delete d;
                }
            }
            else {
                if (sum > bytes) {
                    tail->sec.push_back(x);
                    tail->bytes_ocupados = tail->bytes;
                    comp<T>* newnodo = new comp<T>(tail->sec_c, tail->pista_c, tail->superficie_c, tail->plato_c, tail->disco_c, bytes);
                    tail->next = newnodo;
                    tail = tail->next;
                    update_pos(tail);
                    sum = sum - bytes;
                }
                else if (tail->bytes == tail->bytes_ocupados) {
                    comp<T>* newnodo = new comp<T>(tail->sec_c, tail->pista_c, tail->superficie_c, tail->plato_c, tail->disco_c, bytes);
                    newnodo->bytes_ocupados = sizeof(x);
                    tail->next = newnodo;
                    tail = tail->next;
                    update_pos(tail);
                    tail->sec.push_back(x);
                }
                else {
                    tail->sec.push_back(x);
                    int rest = tail->bytes - tail->bytes_ocupados;
                    tail->bytes_ocupados = tail->bytes;
                    comp<T>* newnodo = new comp<T>(tail->sec_c, tail->pista_c, tail->superficie_c, tail->plato_c, tail->disco_c, bytes);
                    newnodo->bytes_ocupados += rest;
                    tail->next = newnodo;
                    tail = tail->next;
                    update_pos(tail);
                    tail->sec.push_back(x);
                }
            }
        }
    }
    else if (tail->bytes == tail->bytes_ocupados) {
        comp<T>* newnodo = new comp<T>(tail->sec_c, tail->pista_c, tail->superficie_c, tail->plato_c, tail->disco_c, bytes);
        newnodo->bytes_ocupados += sizeof(x);
        tail->next = newnodo;
        tail = tail->next;
        update_pos(tail);
        tail->sec.push_back(x);
    }
    else {
        tail->sec.push_back(x);
        tail->bytes_ocupados += sizeof(x);
    }
}

template <class T>
void sim<T>::print() {
    comp<T>* temp = head;
    while (temp != nullptr) {
        cout << "Nodo en posición: " << "sector N: " << temp->sec_c << " pista N: " << temp->pista_c << " superficie N: " << temp->superficie_c << " plato N: " << temp->plato_c << " disco: N " << temp->disco_c << endl;
        for (int i = 0; i < temp->sec.size(); i++) {
            T obj = temp->sec[i];
            obj.print_obj();
        }
        temp = temp->next;
    }

    cout << endl;
}

template <class T>
bool sim<T>::find_print(T x) {
    vector<comp<T>*> bloque;
    comp<T>* temp = head;
    while (temp != nullptr) {
        for (int i = 0; i < temp->sec.size(); i++) {
            T obj = temp->sec[i];
            if (obj == x) {
                bloque.push_back(temp);
            }
        }
        temp = temp->next;
    }
    if (head == nullptr || bloque.empty()) {
        cout << "no se encontro " << endl;
        return false;
    }
    else {
        for (int i = 0; i < bloque.size(); i++) {
            comp<T>* act = bloque[i];
            for (int i = 0; i < act->sec.size(); i++) {
                T obj = act->sec[i];
                obj.print_obj();
            }
        }
        cout << endl;
        return true;
    }
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
