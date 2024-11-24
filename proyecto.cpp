#include <iostream>
#include <vector>
#include <string>
using namespace std;

struct obj {
    int id, dni;
    string primer_nombre, segundo_nombre, apellido_materno, apellido_paterno;
    obj(int _id, int _dni, string p_n, string s_n, string a_m, string a_p) : id(_id), dni(_dni), primer_nombre(p_n), segundo_nombre(s_n), apellido_materno(a_m), apellido_paterno(a_p) {}
    void print_obj();
};

void obj::print_obj() {
    cout << id << " " << dni << " " << primer_nombre << " " << segundo_nombre << " " << apellido_materno << " " << apellido_paterno << " " << endl;
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
    if (tail->sec_c == sec_c && tail->pista_c == pista_c && tail->superficie_c == superficie_c && tail->plato_c == plato_c && tail->disco_c == disco_c) {
        return;
    }
    if (sizeof(x) > bytes || tail->bytes_ocupados + sizeof(x) > bytes) {
        comp<T>* temp = tail;
        int sum = sizeof(x);
        while (tail->bytes_ocupados + sizeof(x) > bytes) {
            if (tail->sec_c == sec_c && tail->pista_c == pista_c && tail->superficie_c == superficie_c && tail->plato_c == plato_c && tail->disco_c == disco_c) {
                comp<T>* del = tail;
                tail = temp;
                while (del != nullptr) {
                    comp<T>* borr = del;
                    temp = temp->next;
                    delete borr;
                }
            }
            else {
                if (sum > bytes) {
                    tail->sec.push_back(x);
                    int rest = sum - bytes;
                    tail->bytes_ocupados = tail->bytes;
                    comp<T>* newnodo = new comp<T>(tail->sec_c, tail->pista_c, tail->superficie_c, tail->plato_c, tail->disco_c, bytes);
                    newnodo->bytes_ocupados += rest;
                    newnodo->bytes_ocupados += rest;
                    tail->next = newnodo;
                    tail = tail->next;
                    update_pos(tail);
                    tail->sec.push_back(x);
                    sum = sum - bytes;
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
            if (obj.id == x.id) {
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
    sim<obj> simulacion(2, 2, 2, 2, 2, 200);

    obj o1(1, 12345678, "Juan", "Carlos", "Perez", "Lopez");
    obj o2(2, 87654321, "Maria", "Elena", "Gomez", "Martinez");
    obj o3(3, 11223344, "Pedro", "Alberto", "Ramirez", "Diaz");
    obj o4(4, 55667788, "Ana", "Sofia", "Torres", "Gutierrez");
    obj o5(5, 99887766, "Luis", "Fernando", "Vargas", "Moreno");

    simulacion.add(o1);
    simulacion.add(o2);
    simulacion.add(o3);
    simulacion.add(o4);
    simulacion.add(o5);

    simulacion.print();

    simulacion.find_print(o1);
    simulacion.find_print(o2);
    simulacion.find_print(o4);

}
