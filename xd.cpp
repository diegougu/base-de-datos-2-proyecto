#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
using namespace std;

class objx {
public:
    vector<pair<string, int>> tipos;
    vector<string> valores;
    int peso = 0;
    void add(string tipo, string tam);
    bool operator==(const objx& other);
};

void objx::add(string tipo, string tam) {
    tipos.push_back(make_pair(tipo, stoi(tam)));
    peso += stoi(tam);
}

bool objx::operator==(const objx& other) {
    return tipos == other.tipos && valores == other.valores;
}

objx todo;

void readTXT(const string& file_name) {
    ifstream file(file_name);
    if (!file) {
        cerr << "Error: No se puede abrir el archivo " << file_name << endl;
        return;
    }

    string line;
    int line_count = 0;
    while (getline(file, line)) {
        line_count++;

        if (line.find_first_not_of(" \t") != string::npos) { //para borrar espacios simples o de tabulacion
            line = line.substr(line.find_first_not_of(" \t"),
                line.find_last_not_of(" \t") + 1);
        }

        if (line.empty()) {
            continue;
        }


        if (line.find("CREATE TABLE") != string::npos) { //npos es como un MAX_DEFINED para strings

            string nombreobjeto = line.substr(12 + 1, line.find('('));

        }
        else if (line.find(";") != string::npos) {
            break;
        }
        else {
            string nombre;
            string tipo;
            string tam;
            string otros;

            int i = 0;
            for (; i < line.size(); i++) {
                if (line[i] == ' ') {
                    i++;
                    break;
                }
                else {
                    nombre.push_back(line[i]);
                }
            }

            for (; i < line.size(); i++) {
                if (line[i] == '(') {
                    i++;
                    break;
                }
                else {
                    tipo.push_back(line[i]);
                }
            }

            for (; i < line.size(); i++) {
                if (line[i] == ')') {
                    i++;
                    break;
                }
                else {
                    tam.push_back(line[i]);
                }
            }

            for (; i < line.size(); i++) {
                if (line[i] == ',') {
                    i++;
                    break;
                }
                else {
                    otros.push_back(line[i]);
                }
            }
            todo.add(tipo, tam);
        }
    }
    file.close();
}



void readCSV(const string& filePath, vector<objx>& objects) {
    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "Error al abrir el archivo: " << filePath << endl;
        return;
    }

    string line;
    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        string data;
        objx obj;
        obj.tipos = todo.tipos;
        obj.peso = todo.peso;
        string concatenated;

        while (getline(ss, data, ',')) {
            if (!concatenated.empty()) {
                concatenated += " ";
            }
            concatenated += data;
        }

        obj.valores.push_back(concatenated);

        objects.push_back(obj);
    }
    objects.pop_back();
    file.close();
}


struct Nodo {
    int sec_c, pista_c, superficie_c, plato_c, disco_c, bytes;
    vector<objx> sec;
    Nodo* next = nullptr;
    Nodo(int s_c, int p_c, int su_c, int pl_c, int d_c, int b, Nodo* n = nullptr) : sec_c(s_c ? s_c : 1), pista_c(p_c ? p_c : 1), superficie_c(su_c ? su_c : 1), plato_c(pl_c ? pl_c : 1), disco_c(d_c ? d_c : 1), bytes(b ? b : 100), next(n) {}
    int bytes_ocupados = sizeof(Nodo);
};

class Gest {
private:
    Nodo* head = nullptr;
    Nodo* tail = nullptr;
    int sec_c, pista_c, superficie_c, plato_c, disco_c, bytes;
    void update_pos(Nodo* node);
public:
    Gest(int s_c, int p_c, int su_c, int pl_c, int d_c, int b) : sec_c(s_c ? s_c : 1), pista_c(p_c ? p_c : 1), superficie_c(su_c ? su_c : 1), plato_c(pl_c ? pl_c : 1), disco_c(d_c ? d_c : 1), bytes(b ? b : 100) {}
    void add(objx x);
    bool find_print(objx x);
};


void Gest::update_pos(Nodo* node) {
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

void Gest::add(objx x) {
    if (head == nullptr) {
        Nodo* newnodo = new Nodo(1, 1, 1, 1, 1, bytes);
        head = newnodo;
        tail = newnodo;
    }
    else if (tail->sec_c == sec_c && tail->pista_c == pista_c && tail->superficie_c == superficie_c && tail->plato_c == plato_c && tail->disco_c == disco_c && tail->bytes_ocupados + sizeof(x) > bytes) {
        return;
    }
    else if (tail->bytes_ocupados + x.peso > bytes) {
        Nodo* newnodo = new Nodo(tail->sec_c, tail->pista_c, tail->superficie_c, tail->plato_c, tail->disco_c, bytes);
        update_pos(newnodo);
        tail->next = newnodo;
        tail = tail->next;
    }
    tail->sec.push_back(x);
    tail->bytes_ocupados += x.peso;
}

bool Gest::find_print(objx x) {

    vector<Nodo*> pos;
    for (Nodo* current = head; current; current = current->next) {
        for (int i = 0; i < current->sec.size(); i++) {
            if (current->sec[i] == x) {
                pos.push_back(current);
            }
        }
    }

    if (pos.empty() || head == nullptr) {
        return false;
    }
    for (int i = 0; i < pos.size(); i++) {
        cout << "sector N: " << pos[i]->sec_c << " pista N: " << pos[i]->pista_c << " superficie N: " << pos[i]->superficie_c << " plato N: " << pos[i]->plato_c << " disco: N " << pos[i]->disco_c << endl;
        for (int j = 0; j < pos[i]->sec.size(); j++) {
            for (int k = 0; k < pos[i]->sec[j].valores.size(); k++) {
                cout << pos[i]->sec[j].valores[k] << endl;
            }
        }
    }
    cout << endl;
    return true;
}

void menu(Gest* x) {
    
}



int main() {
    string filePath = "C:\\Users\\diego\\source\\repos\\ConsoleApplication205\\ConsoleApplication205\\taxables.csv";
    string filename = "C:\\Users\\diego\\source\\repos\\ConsoleApplication205\\ConsoleApplication205\\struct_table.txt";
    vector<objx> objects;


    readTXT(filename);
    readCSV(filePath, objects);

    for (int i = 0; i < todo.tipos.size(); i++) {
        cout << todo.tipos[i].first << " " << todo.tipos[i].second << endl;
    }
    cout << endl;

    for (const auto& obj : objects) {
        for (const auto& val : obj.valores) {
            cout << "Valores: " << val << endl;
        }
    }


    Gest x(2, 2, 2, 2, 2, 500);
    for (int i = 0; i < objects.size(); i++) {
        x.add(objects[i]);
    }

    for (int i = 0; i < objects.size(); i++) {
        x.find_print(objects[i]);
    }

}
