#include <iostream>
#include <conio.h>
#include <windows.h>
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
    obstacles.clear();
    obstacles.push_back(width - 1);
}

void draw(char screen[height][width]) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (i == y && j == x) {
                screen[i][j] = 'O'; // el jugador
            }
            else if (find(obstacles.begin(), obstacles.end(), j) != obstacles.end() && i == height - 1) {
                screen[i][j] = '#'; // obstáculo
            }
            else {
                screen[i][j] = ' ';
            }
        }
    }
}

void render(char screen[height][width]) {
    gotoXY(0, 0);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            cout << screen[i][j];
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

void menu() {
    char choice;
    do {
        system("cls");
        cout << "=== Geometry Dash by Andreuu2k ===\n";
        cout << "1. Jugar\n";
        cout << "2. Visitar canal de YouTube\n";
        cout << "3. Salir\n";
        cout << "Elige una opcion: ";
        choice = _getch();

        switch (choice) {
        case '1': {
            setup();
            auto start = chrono::steady_clock::now();
            char screen[height][width];
            while (!gameOver) {
                draw(screen);
                render(screen);
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
            system("cls");
            cout << "Game Over!" << endl;
            Sleep(2000);
            break;
        }
        case '2':
            ShellExecute(0, 0, L"https://www.youtube.com/@Andreuu2k", 0, 0, SW_SHOW);
            break;
        case '3':
            break;
        default:
            cout << "Opcion no valida, intentalo de nuevo." << endl;
            Sleep(1000);
            break;
        }
    } while (choice != '3');
}

int main() {
    SetConsoleTitleA("Geometry Dash by Andreuu2k");
    menu();
    return 0;
}
