#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp> 
#include <iomanip>
using namespace std;

class objx {
public:
    vector<pair<string, int>> tipos; //txt
    vector<string> valores; //csv
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
    Nodo* tail = nullptr;
    int sec_c, pista_c, superficie_c, plato_c, disco_c, bytes;
    void update_pos(Nodo* node);
    Nodo* head = nullptr;
public:
    Gest(int s_c, int p_c, int su_c, int pl_c, int d_c, int b) : sec_c(s_c ? s_c : 1), pista_c(p_c ? p_c : 1), superficie_c(su_c ? su_c : 1), plato_c(pl_c ? pl_c : 1), disco_c(d_c ? d_c : 1), bytes(b ? b : 100) {}
    void add(objx x);
    bool find_print(objx x, vector<Nodo*>& pos);
    void print(float startx, float starty, float linesSpacing, vector<sf::Text>& lines, sf::Font& font);
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

bool Gest::find_print(objx x, vector<Nodo*>& pos) {

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

    return true;
}


void Gest::print(float startx, float starty, float linesSpacing, vector<sf::Text>& lines, sf::Font& font) {
    lines.clear();

    for (Nodo* current = head; current; current = current->next) {
        for (int i = 0; i < current->sec.size(); i++) {
            if (current->sec[i].valores.empty()) {
                continue;
            }

            for (int j = 0; j < current->sec[i].valores.size(); j++) {
                sf::Text lineText;
                lineText.setFont(font);
                lineText.setString(current->sec[i].valores[j]);
                lineText.setCharacterSize(15);
                lineText.setFillColor(sf::Color::Black);

                lineText.setPosition(startx, starty + lines.size() * linesSpacing);

                lines.push_back(lineText);
            }
        }
    }
}

void newdato(objx& dato) {
    dato.tipos = todo.tipos;
    dato.peso = todo.peso;
    string concatenated;
    cout << "ingresa los valores del dato: " << endl;
    for (int i = 0; i < dato.tipos.size(); i++) {
        string all;
        if (dato.tipos[i].first == "INTEGER") {
            cin >> all;
            concatenated += all;
            concatenated += " ";

        }
        else if (dato.tipos[i].first == "VARCHAR") {
            do {
                cin >> all;
            } while (all.size() > dato.tipos[i].second);
            concatenated += all;
            concatenated += " ";
        }
        else if (dato.tipos[i].first == "DECIMAL") {
            cin >> all;
            concatenated += all;
            concatenated += " ";
        }
    }
    dato.valores.push_back(concatenated);
}

void menu(Gest* gestor, const vector<objx>& objects) {
    int sectores, pistas, superficie, plato, disco, bytes;

    cout << "Ingrese la cantidad de sectores, pistas, superficie, plato, disco y cantidad de bytes que soporta cada sector: " << endl;
    cin >> sectores >> pistas >> superficie >> plato >> disco >> bytes;

    gestor = new Gest(sectores, pistas, superficie, plato, disco, bytes);

    for (int i = 0; i < objects.size(); i++) {
        gestor->add(objects[i]);
    }

    sf::RenderWindow window(sf::VideoMode(1000, 600), "Menu SFML");
    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        cerr << "No se pudo cargar la fuente arial.ttf desde C:/Windows/Fonts/" << endl;
        return;
    }

    sf::RectangleShape insertButton(sf::Vector2f(100, 30));
    insertButton.setFillColor(sf::Color::White);
    insertButton.setOutlineColor(sf::Color::Black);
    insertButton.setOutlineThickness(2);
    insertButton.setPosition(50, 50);

    sf::Text insertText("INSERT", font, 15);
    insertText.setFillColor(sf::Color::Black);
    insertText.setPosition(60, 55);

    sf::RectangleShape findButton(sf::Vector2f(100, 30));
    findButton.setFillColor(sf::Color::White);
    findButton.setOutlineColor(sf::Color::Black);
    findButton.setOutlineThickness(2);
    findButton.setPosition(50, 100);

    sf::Text findText("FIND", font, 15);
    findText.setFillColor(sf::Color::Black);
    findText.setPosition(60, 105);

    sf::RectangleShape backButton(sf::Vector2f(100, 30));
    backButton.setFillColor(sf::Color::White);
    backButton.setOutlineColor(sf::Color::Black);
    backButton.setOutlineThickness(2);
    backButton.setPosition(50, 150);

    sf::Text backText("BACK", font, 15);
    backText.setFillColor(sf::Color::Black);
    backText.setPosition(60, 155);

    vector<sf::Text> lines;
    float startX = 200.0f;
    float startY = 50.0f;
    float lineSpacing = 20.0f;
    vector<sf::Text> findResults;
    bool isFindView = false;
    gestor->print(startX, startY, lineSpacing, lines, font);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::Vector2f mousePosF((float)mousePos.x, (float)mousePos.y);

                if (insertButton.getGlobalBounds().contains(mousePosF) && !isFindView) {
                    cout << "Botón INSERT presionado" << endl;

                    objx nuevoObjeto;

                    newdato(nuevoObjeto);
                    gestor->add(nuevoObjeto);
                    lines.clear();
                    gestor->print(startX, startY, lineSpacing, lines, font);
                }

                if (findButton.getGlobalBounds().contains(mousePosF) && !isFindView) {
                    cout << "Boton FIND presionado" << endl;
                    findResults.clear();
                    float resultStartY = 50.0f;
                    objx objetoBuscar;
                    bool veri = false;
                    cout << "ingresa 1 si es para buscar un elemento de csv o ingresa 0 si es para buscar un elemento ya insertado: " << endl;
                    cin >> veri;
                    if (veri) {
                        int pos = -1;
                        do {
                            cout << "ingresa la posicion del elemento dentro del csv (1 a " << objects.size() << "): " << endl;
                            cin >> pos;
                            if (pos < 1 || pos > objects.size()) {
                                cout << "Posicion invalida. Inténtalo de nuevo." << endl;
                            }
                        } while (pos < 1 || pos > objects.size());
                        objetoBuscar.valores = objects[pos - 1].valores;
                        objetoBuscar.tipos = objects[pos - 1].tipos;
                    }
                    else {
                        newdato(objetoBuscar);
                    }

                    vector<Nodo*> pos;
                    if (gestor->find_print(objetoBuscar, pos)) {
                        for (Nodo* nodo : pos) {
                            sf::Text resultText;
                            resultText.setFont(font);

                            string ubicacion = "Sector: " + to_string(nodo->sec_c) +
                                " Pista: " + to_string(nodo->pista_c) +
                                " Superficie: " + to_string(nodo->superficie_c) +
                                " Plato: " + to_string(nodo->plato_c) +
                                " Disco: " + to_string(nodo->disco_c);

                            string valores = "Valores: \n";
                            for (const auto& sector : nodo->sec) {
                                for (const auto& valor : sector.valores) {
                                    valores += valor + " ";
                                }
                                valores += "\n";
                            }

                            resultText.setString(ubicacion + "\n" + valores);
                            resultText.setCharacterSize(15);
                            resultText.setFillColor(sf::Color::Black);
                            resultText.setPosition(startX, resultStartY);

                            findResults.push_back(resultText);
                            resultStartY += lineSpacing * 2;
                        }
                        isFindView = true;
                    }
                    else {
                        cout << "Objeto no encontrado." << endl;
                    }
                }

                if (backButton.getGlobalBounds().contains(mousePosF) && isFindView) {
                    cout << "Boton BACK presionado" << endl;
                    isFindView = false;
                }
            }
        }

        window.clear(sf::Color::White);

        if (isFindView) {
            window.draw(backButton);
            window.draw(backText);

            for (auto& result : findResults) {
                window.draw(result);
            }
        }
        else {
            window.draw(insertButton);
            window.draw(insertText);
            window.draw(findButton);
            window.draw(findText);

            for (auto& l : lines) {
                window.draw(l);
            }
        }

        window.display();
    }
}


int main() {
    string filePath = "C:\\Users\\anthony\\Proyecto_Base_Datos_II\\proyecto_bd2_codigo\\data\\taxables.csv";
    string filename = "C:\\Users\\anthony\\Proyecto_Base_Datos_II\\proyecto_bd2_codigo\\data\\struct_table.txt";
    vector<objx> objects;
    Gest* gestor = nullptr;

    readTXT(filename);
    readCSV(filePath, objects);
    menu(gestor, objects);
}