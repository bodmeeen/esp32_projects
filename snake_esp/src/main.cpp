#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define GRID 8 // Розмір однієї клітинки 8х8 пікселів (сітка 16 на 8)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Піни кнопок керування
#define BTN_UP 12
#define BTN_DOWN 14
#define BTN_LEFT 27
#define BTN_RIGHT 26

// Змінні гри
int snakeHeadX = 5;
int snakeHeadY = 5;
int snakeTailX[100];
int snakeTailY[100];
int tailLength = 0;
int fruitX, fruitY;
int player_score = 0;
bool gameOver = false;

char currentDir = 'd'; // Початковий рух вправо (d)

// Функція спавну фрукта
void spawnFruit() {
    bool isFruitInSnake;
    do {
        isFruitInSnake = false;
        fruitX = random(0, SCREEN_WIDTH / GRID);
        fruitY = random(0, SCREEN_HEIGHT / GRID);
        
        // Перевірка голови
        if (fruitX == snakeHeadX && fruitY == snakeHeadY) isFruitInSnake = true;
        
        // Перевірка хвоста
        for (int i = 0; i < tailLength; i++) {
            if (fruitX == snakeTailX[i] && fruitY == snakeTailY[i]) {
                isFruitInSnake = true;
                break;
            }
        }
    } while (isFruitInSnake);
}

void setup() {
    Serial.begin(115200);
    
    // Ввімкнення дисплея
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("Помилка дисплея"));
        for(;;);
    }
    
    // Налаштування пінів кнопок на вхід з внутрішньою підтяжкою до плюса
    pinMode(BTN_UP, INPUT_PULLUP);
    pinMode(BTN_DOWN, INPUT_PULLUP);
    pinMode(BTN_LEFT, INPUT_PULLUP);
    pinMode(BTN_RIGHT, INPUT_PULLUP);
    
    // Хардварний рандом для генератора (щоб яблука завжди були в різних місцях)
    randomSeed(analogRead(34)); 
    spawnFruit();
}

void loop() {
    // Екран програшу
    if (gameOver) {
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(30, 20);
        display.println("GAME OVER!");
        display.setCursor(35, 40);
        display.print("Score: ");
        display.print(player_score);
        display.display();
        
        // Якщо натиснули будь-яку кнопку (сигнал падає в LOW) - рестарт гри
        if (digitalRead(BTN_UP) == LOW || digitalRead(BTN_DOWN) == LOW || 
            digitalRead(BTN_LEFT) == LOW || digitalRead(BTN_RIGHT) == LOW) {
            
            gameOver = false;
            tailLength = 0;
            player_score = 0;
            snakeHeadX = 5;
            snakeHeadY = 5;
            currentDir = 'd';
            spawnFruit();
            delay(300); // Захист від того, щоб гра одразу не зчитала натискання як рух
        }
        return; // Виіхд з loop, поки гравець не натисне кнопку
    }

    // Читання кнопок
    // Кнопка замикає на землю (GND), тому натиснутий стан — це LOW (0)
    if (digitalRead(BTN_UP) == LOW && currentDir != 's') currentDir = 'w';
    if (digitalRead(BTN_DOWN) == LOW && currentDir != 'w') currentDir = 's';
    if (digitalRead(BTN_LEFT) == LOW && currentDir != 'd') currentDir = 'a';
    if (digitalRead(BTN_RIGHT) == LOW && currentDir != 'a') currentDir = 'd';

    // Зсув хвоста
    for (int i = tailLength - 1; i > 0; i--) {
        snakeTailX[i] = snakeTailX[i - 1];
        snakeTailY[i] = snakeTailY[i - 1];
    }
    if (tailLength > 0) {
        snakeTailX[0] = snakeHeadX;
        snakeTailY[0] = snakeHeadY;
    }

    // Рух голови
    switch (currentDir) {
        case 'w': snakeHeadY--; break;
        case 's': snakeHeadY++; break;
        case 'a': snakeHeadX--; break;
        case 'd': snakeHeadX++; break;
    }

    // Перевірка меж
    int maxWidth = SCREEN_WIDTH / GRID;
    int maxHeight = SCREEN_HEIGHT / GRID;
    
    if (snakeHeadX < 0 || snakeHeadX >= maxWidth || snakeHeadY < 0 || snakeHeadY >= maxHeight) {
        gameOver = true;
    }

    // Перевірка хвоста
    for (int i = 0; i < tailLength; i++) {
        if (snakeHeadX == snakeTailX[i] && snakeHeadY == snakeTailY[i]) {
            gameOver = true;
        }
    }

    // З'їдання фрукта 
    if (snakeHeadX == fruitX && snakeHeadY == fruitY) {
        tailLength++;
        player_score += 10;
        spawnFruit();
    }

    // Малювання
    display.clearDisplay();
    
    // Фрукт
    display.fillRect(fruitX * GRID, fruitY * GRID, GRID, GRID, SSD1306_WHITE);
    
    // Голова
    display.fillRect(snakeHeadX * GRID, snakeHeadY * GRID, GRID, GRID, SSD1306_WHITE);
    
    // Хвіст
    for (int i = 0; i < tailLength; i++) {
        display.fillRect(snakeTailX[i] * GRID + 1, snakeTailY[i] * GRID + 1, GRID - 2, GRID - 2, SSD1306_WHITE);
    }

    display.display();

    // Швидкість гри в мілісекундах
    delay(1000); 
}