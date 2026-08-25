#include <iostream>
#include <random>
#include <time.h>

#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

using namespace std;

//enum class Directions { Up, Down, Left, Right };
    
char matrix[10][10];
    
int snakeHeadX = 5; //голова змії з'являється в центрі поля
int snakeHeadY = 5;
int snakeTailY[100];
int snakeTailX[100];

int tailLength = 0;

int player_score = 0;

bool GameOver = false;

struct termios orig_termios;

void disableRawMode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enableRawMode() {
    tcgetattr(STDIN_FILENO, &orig_termios);
    // автоматично викликає ф-ю для виходу з raw mode перед виходом
    atexit(disableRawMode);
    
    struct termios raw = orig_termios;
    
    // затримка перед рухом
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 10;

    // вихід з canon mode та заборона виводу введеного користувачем в термінал
    raw.c_lflag &= ~(ECHO | ICANON);

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);

    // термінал не буде чекати на read, а продовжить роботу
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
}



void GameLogic() {
    srand(time(0));
    int fruitX = 1 + (rand() % 8);
    int fruitY = 1 + (rand() % 8);
    
    // задання базового напрямку руху, для того щоб на початку гри
    // рух був навіть якщо користувач його не задав
    char user_inp = 'w';

    bool isFruitInSnake;
    
    while (!GameOver){
        // cout << "\033[2J\033[1;1H"; // очищення терміналу в Linux
        cout << "\nSnake Game\n";
        cout << player_score << "\n";

        char last_key = '\0'; // останній введений елемент, на випадок переповнення буферу
        char temp_inp;
        
        // зчитування вводу користувача замість cin для raw mode
        // якщо користувач не задав напрямок, то рух продовжується в
        // останньому заданому напрямку
        while(read(STDIN_FILENO, &temp_inp, 1) == 1) { 
            last_key = temp_inp;
        }

        // якщо був ввід
        if (last_key != '\0') {
            // заборона розвороту на 180 градусів
            if (last_key == 'w' && user_inp != 's') user_inp = last_key;
            else if (last_key == 's' && user_inp != 'w') user_inp = last_key;
            else if (last_key == 'a' && user_inp != 'd') user_inp = last_key;
            else if (last_key == 'd' && user_inp != 'a') user_inp = last_key;
        }

        
        // зсув сегментів хвоста починаючи з останнього
        for (int i = tailLength - 1; i > 0; i--) {
            snakeTailX[i] = snakeTailX[i - 1];
            snakeTailY[i] = snakeTailY[i - 1];
        }
        
        // перший сегмент хвоста займає місце голови
        if (tailLength > 0) {
            snakeTailX[0] = snakeHeadX;
            snakeTailY[0] = snakeHeadY;
        }

        switch (user_inp) {
            case 'w':
            case 'W':
                snakeHeadY--;
                break;
                
            case 's':
            case 'S':
                snakeHeadY++;
                break;
                
            case 'a':
            case 'A':
                snakeHeadX--;
                break;
                
            case 'd':
            case 'D':
                snakeHeadX++;
                break;
            case 'q':
            case 'Q':
                GameOver = true;
                break;
                
            default:
                break;
        }

        // збільшення хвоста при з'їданні фрукта та створення нового фрукта
        if (snakeHeadX == fruitX && snakeHeadY == fruitY) {
            isFruitInSnake = true;

            tailLength++;
            player_score += 10;
            // перевірка чи не з'явився фрукт в тілі змійки

            while (isFruitInSnake) {
                isFruitInSnake = false;
                fruitX = 1 + (rand() % 8);
                fruitY = 1 + (rand() % 8);
                for (int i = 0; i < tailLength; i++) {
                    if (fruitX == snakeTailX[i] && fruitY == snakeTailY[i]) {
                        isFruitInSnake = true;
                        break;
                    }
                }
            }
        }

        // закінчення гри якщо голова вийшла за межі поля
        if (snakeHeadX <= 0 || snakeHeadX >= 9 || snakeHeadY <= 0 || snakeHeadY >= 9) {
            cout << "Кінець гри!\n";
            GameOver = true;
        }
            
        // закінчення гри якщо голова торкнулась хвоста
        for (int i = 0; i < tailLength; i++) {
            if (snakeHeadX == snakeTailX[i] && snakeHeadY == snakeTailY[i]) {
                cout << "Кінець гри! Не можна торкатись свого хвоста\n";
                GameOver = true;
            }
        }

        // створення поля та меж
        for (int y = 0; y < 10; y++) {
            for (int x = 0; x < 10; x++) {
                if (y == 0 || y == 9 || x == 0 || x == 9) {
                    matrix[y][x] = '#';
                } else {
                   matrix[y][x] = ' ';
                }
            }
        }
        matrix[fruitY][fruitX] = '$';
        
        // виведення хвоста
        for (int i = 0; i < tailLength; i++) {
            matrix[snakeTailY[i]][snakeTailX[i]] = 'o';
        }
        
        matrix[snakeHeadY][snakeHeadX] = '0';
        
        // виведення поля
        for (int y = 0; y < 10; y++) {
            for (int x = 0; x < 10; x++) {
                cout << matrix[y][x] << " ";
            }
            cout << endl;
        }
        cout << "\nТвій хід: \n";
        
        if (tailLength == 63) { cout << "\nПеремога!\n"; break; }
        
        usleep(500000); // час задається в мікросекундах
    }
}

int main() {
    enableRawMode();
    GameLogic();
    return 0;
}