#include <iostream>
#include <conio.h> // Incluir la biblioteca conio.h para _getch()
#include <windows.h>
#include <vector>
#include <chrono>
#include <fstream>

using namespace std;

const int width = 50;
const int height = 20;
int x, y; // posición del jugador
vector<int> obstacles; // posiciones de los obstáculos
bool gameOver;
int score;
int level;
int highScore;

void gotoXY(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void clearScreen() {
    system("cls");
}

void setup() {
    gameOver = false;
    x = 10;
    y = height - 1;
    obstacles.clear();
    obstacles.push_back(width - 1);
    score = 0;
    level = 1;
    ifstream file("highscore.txt");
    if (file.is_open()) {
        file >> highScore;
        file.close();
    }
    else {
        highScore = 0;
    }
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
    clearScreen();
    cout << "Nivel: " << level << "   Puntuacion: " << score << "   Puntuacion mas alta: " << highScore << endl;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            cout << screen[i][j];
        }
        cout << endl;
    }
}

void input() {
    if (GetAsyncKeyState(VK_SPACE) && y == height - 1) {
        y -= 3; // salta
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
    for (int i = 0; i < obstacles.size(); i++) {
        if (obstacles[i] == x && y == height - 1) {
            gameOver = true;
            break; // salimos del bucle tan pronto como encontramos una colisión
        }
    }

    // aumentar puntaje
    score++;

    // pasar al siguiente nivel si el puntaje alcanza cierto límite
    if (score >= level * 100) {
        level++;
        // incrementar la velocidad de los obstáculos
        for (int i = 0; i < obstacles.size(); i++) {
            obstacles[i] -= level;
        }
    }
}

void menu() {
    char choice;
    do {
        clearScreen();
        cout << "=== Geometry Dash by Andreuu2k ===\n";
        cout << "1. Jugar\n";
        cout << "2. Visitar canal de YouTube\n";
        cout << "3. Puntuacion mas alta\n";
        cout << "4. Salir\n";
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
                Sleep(50); // reducir el tiempo de espera entre fotogramas para mejorar la fluidez
            }
            clearScreen();
            cout << "Game Over!" << endl;
            cout << "Tu puntuacion final es: " << score << endl;
            if (score > highScore) {
                ofstream file("highscore.txt");
                if (file.is_open()) {
                    file << score;
                    file.close();
                    cout << "¡Nueva puntuacion mas alta!" << endl;
                }
            }
            Sleep(2000);
            break;
        }
        case '2':
            ShellExecute(0, 0, L"https://www.youtube.com/@Andreuu2k", 0, 0, SW_SHOW);
            break;
        case '3':
            cout << "Puntuacion mas alta: " << highScore << endl;
            system("pause");
            break;
        case '4':
            break;
        default:
            cout << "Opcion no valida, intentalo de nuevo." << endl;
            Sleep(1000);
            break;
        }
    } while (choice != '4');
}

int main() {
    SetConsoleTitleA("Geometry Dash by Andreuu2k");
    menu();
    return 0;
}
