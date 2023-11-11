#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_GENERATIONS 100

// Структура для представления игрового поля
typedef struct {
    int width;
    int height;
    int** cells;
} GameOfLife;

// Функция для инициализации игрового поля
void initialize(GameOfLife* game) {
    game->cells = (int**)malloc(game->width * sizeof(int*));
    for (int i = 0; i < game->width; i++) {
        game->cells[i] = (int*)malloc(game->height * sizeof(int));
        for (int j = 0; j < game->height; j++) {
            game->cells[i][j] = rand() % 2;
        }
    }
}

// Функция для отображения текущего состояния игрового поля
void display(const GameOfLife* game) {
    for (int y = 0; y < game->height; y++) {
        for (int x = 0; x < game->width; x++) {
            if (game->cells[x][y] == 1) {
                printf("*");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
}

// Функция для обновления состояния игрового поля на следующем поколении
void update(GameOfLife* game) {
    int** newCells = (int**)malloc(game->width * sizeof(int*));

    for (int i = 0; i < game->width; i++) {
        newCells[i] = (int*)malloc(game->height * sizeof(int));
        for (int j = 0; j < game->height; j++) {
            int neighbors = 0;

            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    if (dx == 0 && dy == 0) continue;

                    int nx = i + dx;
                    int ny = j + dy;

                    if (nx >= 0 && nx < game->width && ny >= 0 && ny < game->height) {
                        neighbors += game->cells[nx][ny];
                    }
                }
            }

            if (game->cells[i][j] == 1) {
                newCells[i][j] = (neighbors == 2 || neighbors == 3);
            } else {
                newCells[i][j] = (neighbors == 3);
            }
        }
    }

    // Освобождаем память от старых клеток и обновляем указатель
    for (int i = 0; i < game->width; i++) {
        free(game->cells[i]);
    }
    free(game->cells);

    game->cells = newCells;
}

// Функция для освобождения памяти, занятой игровым полем
void cleanup(GameOfLife* game) {
    for (int i = 0; i < game->width; i++) {
        free(game->cells[i]);
    }
    free(game->cells);
}

int main() {
    GameOfLife game;

    // Запрос параметров у пользователя
    printf("Введите ширину игрового поля: ");
    scanf("%d", &game.width);
    printf("Введите высоту игрового поля: ");
    scanf("%d", &game.height);

    // Инициализация игрового поля
    initialize(&game);

    // Вычисление и отображение нескольких поколений
    for (int generation = 0; generation < MAX_GENERATIONS; generation++) {
        printf("Поколение %d:\n", generation);
        display(&game);
        update(&game);
        usleep(100000); // Задержка для визуального эффекта
        system("clear"); // Очистка экрана (для UNIX-подобных систем)
    }

    // Освобождение памяти
    cleanup(&game);

    return 0;
}
