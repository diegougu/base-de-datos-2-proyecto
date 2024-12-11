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
    bool quest(string& ox);
};

bool objx::quest(string& subcadena) {
    for (int i = 0; i < valores.size(); i++) {
        if (valores[i].find(subcadena) != std::string::npos) {
            return true;
        }
    }
    return false;
}

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
    bool find_print(string subcadena, vector<Nodo*>& pos);
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

bool Gest::find_print(string subcadena, vector<Nodo*>& pos) {

    for (Nodo* current = head; current; current = current->next) {
        for (int i = 0; i < current->sec.size(); i++) {
            if (current->sec[i].quest(subcadena)) {
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

void newquest(string& palabra, sf::RenderWindow& window, sf::Font& font) {
    sf::Text inputLabel("Ingrese una palabra:", font, 20);
    inputLabel.setPosition(50, 50);

    sf::RectangleShape inputBox(sf::Vector2f(400, 30));
    inputBox.setPosition(50, 100);
    inputBox.setFillColor(sf::Color::White);
    inputBox.setOutlineColor(sf::Color::Black);
    inputBox.setOutlineThickness(1);

    sf::Text inputText("", font, 20);
    inputText.setPosition(55, 105);
    inputText.setFillColor(sf::Color::Black);

    sf::RectangleShape button(sf::Vector2f(100, 40));
    button.setPosition(200, 200);
    button.setFillColor(sf::Color::Green);

    sf::Text buttonText("Aceptar", font, 20);
    buttonText.setPosition(215, 205);

    string inputString;
    bool submitted = false;

    while (window.isOpen() && !submitted) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == '\b' && !inputString.empty()) {
                    inputString.pop_back();
                }
                else if (event.text.unicode < 128) {
                    inputString += static_cast<char>(event.text.unicode);
                }
                inputText.setString(inputString);
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (button.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    palabra = inputString;
                    submitted = true;
                }
            }
        }

        window.clear(sf::Color::White);
        window.draw(inputLabel);
        window.draw(inputBox);
        window.draw(inputText);
        window.draw(button);
        window.draw(buttonText);
        window.display();
    }
}

void newdato(objx& dato, sf::RenderWindow& window, sf::Font& font) {
    dato.tipos = todo.tipos;
    dato.peso = todo.peso;
    string concatenated;

    sf::Text inputLabel("Ingrese valores del dato:", font, 20);
    inputLabel.setPosition(50, 50);

    sf::RectangleShape inputBox(sf::Vector2f(400, 30));
    inputBox.setFillColor(sf::Color::White);
    inputBox.setOutlineColor(sf::Color::Black);
    inputBox.setOutlineThickness(1);

    sf::Text inputText("", font, 20);
    inputText.setFillColor(sf::Color::Black);

    sf::RectangleShape button(sf::Vector2f(100, 40));
    button.setFillColor(sf::Color::Green);
    sf::Text buttonText("Siguiente", font, 20);
    buttonText.setFillColor(sf::Color::Black);

    for (int i = 0; i < dato.tipos.size(); i++) {
        string inputString;
        bool submitted = false;

        inputLabel.setString("Ingrese " + dato.tipos[i].first + " (max " + to_string(dato.tipos[i].second) + "):");
        inputBox.setPosition(50, 100);
        inputText.setPosition(55, 105);
        button.setPosition(200, 200);
        buttonText.setPosition(210, 205);

        while (window.isOpen() && !submitted) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();

                if (event.type == sf::Event::TextEntered) {
                    if (event.text.unicode == '\b' && !inputString.empty()) {
                        inputString.pop_back();
                    }
                    else if (event.text.unicode < 128) {
                        inputString += static_cast<char>(event.text.unicode);
                    }

                    inputText.setString(inputString);
                }

                if (event.type == sf::Event::MouseButtonPressed) {
                    if (button.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        if (dato.tipos[i].first == "VARCHAR" && inputString.size() <= dato.tipos[i].second) {
                            concatenated += inputString + " ";
                            submitted = true;
                        }
                        else if (dato.tipos[i].first != "VARCHAR") {
                            concatenated += inputString + " ";
                            submitted = true;
                        }
                    }
                }
            }

            window.clear(sf::Color::White);
            window.draw(inputLabel);
            window.draw(inputBox);
            window.draw(inputText);
            window.draw(button);
            window.draw(buttonText);
            window.display();
        }
    }

    dato.valores.push_back(concatenated);
}

void getInput(sf::RenderWindow& window, sf::Font& font, vector<int>& values) {
    vector<sf::Text> labels = {
        sf::Text("Sectores:", font, 20),
        sf::Text("Pistas:", font, 20),
        sf::Text("Superficie:", font, 20),
        sf::Text("Plato:", font, 20),
        sf::Text("Disco:", font, 20),
        sf::Text("Bytes:", font, 20)
    };

    vector<sf::RectangleShape> inputBoxes(labels.size(), sf::RectangleShape(sf::Vector2f(200, 30)));
    vector<string> inputStrings(labels.size(), "");

    for (size_t i = 0; i < labels.size(); ++i) {
        labels[i].setPosition(50, 50 + i * 60);
        labels[i].setFillColor(sf::Color::Black);
        inputBoxes[i].setFillColor(sf::Color::White);
        inputBoxes[i].setOutlineColor(sf::Color::Black);
        inputBoxes[i].setOutlineThickness(2);
        inputBoxes[i].setPosition(200, 50 + i * 60);
    }

    sf::Text submitText("Submit", font, 20);
    sf::RectangleShape submitButton(sf::Vector2f(100, 40));
    submitButton.setFillColor(sf::Color::White);
    submitButton.setOutlineColor(sf::Color::Black);
    submitButton.setOutlineThickness(2);
    submitButton.setPosition(200, 50 + labels.size() * 60);
    submitText.setFillColor(sf::Color::Black);
    submitText.setPosition(215, 55 + labels.size() * 60);

    size_t activeBox = 0;
    bool submitted = false;

    while (window.isOpen() && !submitted) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::TextEntered) {
                if (activeBox < inputBoxes.size()) {
                    if (event.text.unicode == '\b' && !inputStrings[activeBox].empty()) {
                        inputStrings[activeBox].pop_back();
                    }
                    else if (isdigit(event.text.unicode)) {
                        inputStrings[activeBox] += static_cast<char>(event.text.unicode);
                    }
                }
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

                for (size_t i = 0; i < inputBoxes.size(); ++i) {
                    if (inputBoxes[i].getGlobalBounds().contains(mousePosF)) {
                        activeBox = i;
                        break;
                    }
                }

                if (submitButton.getGlobalBounds().contains(mousePosF)) {
                    submitted = true;
                }
            }
        }

        window.clear(sf::Color::White);

        for (size_t i = 0; i < labels.size(); ++i) {
            window.draw(labels[i]);
            window.draw(inputBoxes[i]);

            sf::Text inputText(inputStrings[i], font, 20);
            inputText.setFillColor(sf::Color::Black);
            inputText.setPosition(inputBoxes[i].getPosition().x + 5, inputBoxes[i].getPosition().y + 5);
            window.draw(inputText);
        }

        window.draw(submitButton);
        window.draw(submitText);
        window.display();
    }

    values.clear();
    for (const auto& str : inputStrings) {
        values.push_back(stoi(str));
    }
}

void menu(Gest* gestor, const vector<objx>& objects) {
    sf::RenderWindow window(sf::VideoMode(1000, 600), "Menu SFML");
    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        cerr << "No se pudo cargar la fuente arial.ttf desde C:/Windows/Fonts/" << endl;
        return;
    }

    vector<int> values;
    getInput(window, font, values);

    int sectores = values[0];
    int pistas = values[1];
    int superficie = values[2];
    int plato = values[3];
    int disco = values[4];
    int bytes = values[5];

    gestor = new Gest(sectores, pistas, superficie, plato, disco, bytes);

    for (int i = 0; i < objects.size(); i++) {
        gestor->add(objects[i]);
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

                    newdato(nuevoObjeto, window, font);
                    gestor->add(nuevoObjeto);
                    lines.clear();
                    gestor->print(startX, startY, lineSpacing, lines, font);
                }

                if (findButton.getGlobalBounds().contains(mousePosF) && !isFindView) {
                    findResults.clear();
                    float resultStartY = 50.0f;
                    objx objetoBuscar;
                    bool veri = false;
                    string palabra;
                    newquest(palabra,window,font);
                    vector<Nodo*> pos;
                    if (gestor->find_print(palabra, pos)) {
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
                    cout << "Botón BACK presionado" << endl;
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

void getInputFromUser(string& filePath, string& fileName) {
    sf::RenderWindow window(sf::VideoMode(1000, 600), "Input de Archivo");

    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cerr << "No se pudo cargar la fuente." << std::endl;
        return;
    }

    sf::Text filePathLabel("filePath:", font, 20);
    filePathLabel.setPosition(50, 50);

    sf::Text fileNameLabel("fileName:", font, 20);
    fileNameLabel.setPosition(50, 150);

    sf::RectangleShape filePathBox(sf::Vector2f(400, 30));
    filePathBox.setPosition(50, 90);
    filePathBox.setFillColor(sf::Color::White);
    filePathBox.setOutlineColor(sf::Color::Black);
    filePathBox.setOutlineThickness(1);

    sf::RectangleShape fileNameBox(sf::Vector2f(400, 30));
    fileNameBox.setPosition(50, 190);
    fileNameBox.setFillColor(sf::Color::White);
    fileNameBox.setOutlineColor(sf::Color::Black);
    fileNameBox.setOutlineThickness(1);

    sf::RectangleShape acceptButton(sf::Vector2f(100, 40));
    acceptButton.setPosition(250, 300);
    acceptButton.setFillColor(sf::Color::Green);

    sf::Text acceptText("Aceptar", font, 20);
    acceptText.setPosition(260, 305);

    sf::Text filePathText("", font, 18);
    filePathText.setPosition(55, 95);
    filePathText.setFillColor(sf::Color::Black);

    sf::Text fileNameText("", font, 18);
    fileNameText.setPosition(55, 195);
    fileNameText.setFillColor(sf::Color::Black);

    std::string filePathInput;
    std::string fileNameInput;
    bool isFilePathActive = true;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::TextEntered) {
                if (isFilePathActive) {
                    if (event.text.unicode == '\b' && !filePathInput.empty()) {
                        filePathInput.pop_back();
                    }
                    else if (event.text.unicode < 128 && event.text.unicode != '\b') {
                        filePathInput += static_cast<char>(event.text.unicode);
                    }
                    filePathText.setString(filePathInput);
                }
                else {
                    if (event.text.unicode == '\b' && !fileNameInput.empty()) {
                        fileNameInput.pop_back();
                    }
                    else if (event.text.unicode < 128 && event.text.unicode != '\b') {
                        fileNameInput += static_cast<char>(event.text.unicode);
                    }
                    fileNameText.setString(fileNameInput);
                }
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                if (filePathBox.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    isFilePathActive = true;
                }
                else if (fileNameBox.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    isFilePathActive = false;
                }
                else if (acceptButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    filePath = filePathInput;
                    fileName = fileNameInput;
                    window.close();
                }
            }
        }

        window.clear(sf::Color::Cyan);
        window.draw(filePathLabel);
        window.draw(fileNameLabel);
        window.draw(filePathBox);
        window.draw(fileNameBox);
        window.draw(filePathText);
        window.draw(fileNameText);
        window.draw(acceptButton);
        window.draw(acceptText);
        window.display();
    }
}

int main() {
    //primero csv 
    // luego txt
    string filePath;
    string filename;
    vector<objx> objects;
    getInputFromUser(filePath, filename);
    Gest* gestor = nullptr;

    readTXT(filename);
    readCSV(filePath, objects);
    menu(gestor, objects);
}
