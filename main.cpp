#include <iostream>
#include <random>
#include <time.h>
using namespace std;

//enum class Directions { Up, Down, Left, Right };
    
char matrix[10][10];
    
int snakeHeadX = 5; //голова змії з'являється в центрі поля
int snakeHeadY = 5;
int snakeTailY[100];
int snakeTailX[100];

int tailLength = 1;

int player_score = 0;

bool GameOver = false;


void GameLogic() {
    srand(time(0));
    int fruitX = 1 + (rand() % 8);
    int fruitY = 1 + (rand() % 8);
    

    while (!GameOver){
        cout << "\nSnake Game\n";
        cout << player_score << "\n";
        
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
        
        // cout << "\033[2J\033[1;1H"; // очищення терміналу в Linux
        
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

        char user_inp;
        cout << "\nТвій хід: \n";
        cin >> user_inp;
            
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
        
        // збільшення хвоста при з'їданні фрукта
        if (snakeHeadX == fruitX && snakeHeadY == fruitY) {
            fruitX = 1 + (rand() % 8);
            fruitY = 1 + (rand() % 8);
            tailLength++;
            player_score += 10;
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
    }
}

int main() {
    GameLogic();
    return 0;
}