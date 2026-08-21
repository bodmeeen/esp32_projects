#include <iostream>
using namespace std;

int main() {
    char matrix[10][10];

    for (int y = 0; y < 10; y++) {
        for (int x = 0; x < 10; x++) {
            matrix[y][x] = '#';
        }
    }


    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            cout << matrix[i][j] << " ";
        }
    cout << endl;
    }

    cout << sizeof(matrix) << " bytes\n";
    
    
    
    return 0;
}