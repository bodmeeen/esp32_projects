#include <iostream>
using namespace std;

int main() {
    // enum class Directions { Up, Down, Left, Right };

    char matrix[10][10];
    
    int snakeX = 5; //голова змії з'являється в центрі поля
    int snakeY = 5;
    
    bool GameOver = false;
    while (!GameOver){
        cout << "Snake Game\n";
        int count = 0;
        for (int y = 0; y < 10; y++) {
            for (int x = 0; x < 10; x++) {
                if (y == 0 || y == 9 || x == 0 || x == 9) {
                    matrix[y][x] = '#';
                } else {
                    matrix[y][x] = ' ';
                }
            }
        }
        matrix[snakeY][snakeX] = '@';

        // cout << "\033[2J\033[1;1H"; // очищення терміналу в Linux
        
        for (int y = 0; y < 10; y++) {
            for (int x = 0; x < 10; x++) {
                cout << matrix[y][x] << " ";
                count++;
            }
            cout << endl;
        }
        cout << count;

        char user_inp;
        cout << "\nТвій хід: \n";
        cin >> user_inp;

        switch (user_inp) {
            case 'w':
            case 'W':
                snakeY--;
                break;
            
            case 's':
            case 'S':
                snakeY++;
                break;
            
            case 'a':
            case 'A':
                snakeX--;
                break;
            
            case 'd':
            case 'D':
                snakeX++;
                break;


            default:
                break;
        }

        if (snakeX <= 0 || snakeX >= 9 || snakeY <= 0 || snakeY >= 9) {
            cout << "Кінець гри!\n";
            GameOver = true;
        }
    }
    return 0;
}