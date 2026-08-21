#include <iostream>
using namespace std;

int main() {
    char matrix[10][10];

    for (int y = 0; y < 10; y++) {
        for (int x = 0; x < 10; x++) {
            if (y == 0 || y == 9 || x == 0 || x == 9) {
                matrix[y][x] = '#';
            } else {
                matrix[y][x] = ' ';
            }
        }
    }

    for (int y = 0; y < 10; y++) {
        for (int x = 0; x < 10; x++) {
            cout << matrix[y][x] << " ";
        }
        cout << endl;
    }

    return 0;
}