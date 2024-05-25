#include <iostream>
#include <conio.h> // para _kbhit() y _getch()
#include <windows.h> // para Sleep() y SetConsoleCursorPosition()
#include <vector>
#include <chrono>

using namespace std;

const int width = 50;
const int height = 20;
int x, y; // posición del jugador
vector<int> obstacles; // posiciones de los obstáculos
bool gameOver;

void gotoXY(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void setup() {
    gameOver = false;
    x = 10;
    y = height - 1;
    obstacles.push_back(width - 1);
    // Establecer modo de la consola
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}

void draw() {
    gotoXY(0, 0); // mover el cursor al principio
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (i == y && j == x) {
                cout << "O"; // el jugador
            }
            else if (find(obstacles.begin(), obstacles.end(), j) != obstacles.end() && i == height - 1) {
                cout << "#"; // obstáculo
            }
            else {
                cout << " ";
            }
        }
        cout << endl;
    }
}

void input() {
    if (_kbhit()) {
        switch (_getch()) {
        case ' ':
            if (y == height - 1) { // sólo puede saltar si está en el suelo
                y -= 3; // salta
            }
            break;
        }
    }
}

void logic() {
    // mueve el jugador hacia abajo (gravedad)
    if (y < height - 1) {
        y++;
    }

    // mueve los obstáculos hacia la izquierda
    for (int i = 0; i < obstacles.size(); i++) {
        obstacles[i]--;
        if (obstacles[i] < 0) {
            obstacles[i] = width - 1;
        }
    }

    // verificar colisión
    if (find(obstacles.begin(), obstacles.end(), x) != obstacles.end() && y == height - 1) {
        gameOver = true;
    }
}

int main() {
    setup();
    auto start = chrono::steady_clock::now();
    while (!gameOver) {
        draw();
        input();
        logic();
        Sleep(50); // reducir el tiempo de espera para mejorar la fluidez

        auto end = chrono::steady_clock::now();
        chrono::duration<double> elapsed_seconds = end - start;
        if (elapsed_seconds.count() > 2.0) { // añadir un obstáculo cada 2 segundos
            obstacles.push_back(width - 1);
            start = chrono::steady_clock::now();
        }
    }

    cout << "Game Over!" << endl;
    return 0;
}
