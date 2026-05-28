#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define WIDTH 40
#define HEIGHT 20
#define MAX_ENEMIES 5
#define MAX_BULLETS 10
#define MAX_PARTICLES 50

typedef struct {
    int x, y;
    int alive;
} Enemy;

typedef struct {
    int x, y;
    int active;
} Bullet;

typedef struct {
    int x, y;
    int vx, vy;
    int life;
    char symbol;
} Particle;

int playerX = WIDTH / 2;
int playerY = HEIGHT - 2;
int score = 0;
Enemy enemies[MAX_ENEMIES];
Bullet bullets[MAX_BULLETS];
Particle particles[MAX_PARTICLES];

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void hideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 100;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void initEnemies() {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        enemies[i].x = rand() % (WIDTH - 2) + 1;
        enemies[i].y = rand() % 5 + 1;
        enemies[i].alive = 1;
    }
}

void initBullets() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        bullets[i].active = 0;
    }
}

void initParticles() {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        particles[i].life = 0;
    }
}

void spawnExplosion(int x, int y) {
    char symbols[] = {'*', '+', 'o', '.', '@', '#'};
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (particles[i].life == 0) {
            particles[i].x = x;
            particles[i].y = y;
            particles[i].vx = (rand() % 5) - 2;
            particles[i].vy = (rand() % 5) - 2;
            particles[i].life = 8 + rand() % 8;
            particles[i].symbol = symbols[rand() % 6];
            break;
        }
    }
}

void updateParticles() {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (particles[i].life > 0) {
            particles[i].x += particles[i].vx;
            particles[i].y += particles[i].vy;
            particles[i].life--;
        }
    }
}

void drawParticles() {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (particles[i].life > 0) {
            gotoxy(particles[i].x, particles[i].y);
            printf("%c", particles[i].symbol);
        }
    }
}

void moveEnemies() {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].alive) {
            enemies[i].y++;
            if (enemies[i].y >= HEIGHT) {
                enemies[i].x = rand() % (WIDTH - 2) + 1;
                enemies[i].y = 1;
            }
        }
    }
}

void moveBullets() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            bullets[i].y--;
            if (bullets[i].y < 0) {
                bullets[i].active = 0;
            }
        }
    }
}

int checkCollision(int bx, int by, int ex, int ey) {
    int bulletWidth = 1;
    int bulletHeight = 1;
    int enemyWidth = 2;
    int enemyHeight = 1;
    
    int collisionLeft = ex - enemyWidth / 2;
    int collisionRight = ex + enemyWidth / 2;
    int collisionTop = ey - enemyHeight / 2;
    int collisionBottom = ey + enemyHeight / 2;
    
    if (bx >= collisionLeft && bx <= collisionRight &&
        by >= collisionTop && by <= collisionBottom) {
        return 1;
    }
    return 0;
}

void checkCollisions() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            for (int j = 0; j < MAX_ENEMIES; j++) {
                if (enemies[j].alive) {
                    if (checkCollision(bullets[i].x, bullets[i].y,
                                      enemies[j].x, enemies[j].y)) {
                        bullets[i].active = 0;
                        enemies[j].alive = 0;
                        spawnExplosion(enemies[j].x, enemies[j].y);
                        score += 10;
                        
                        enemies[j].x = rand() % (WIDTH - 2) + 1;
                        enemies[j].y = 1;
                        enemies[j].alive = 1;
                    }
                }
            }
        }
    }
}

void drawGame() {
    gotoxy(0, 0);
    for (int y = 0; y < HEIGHT; y++) {
        gotoxy(0, y);
        for (int x = 0; x < WIDTH; x++) {
            if (x == 0 || x == WIDTH - 1 || y == 0 || y == HEIGHT - 1) {
                printf("#");
            } else {
                printf(" ");
            }
        }
    }
    
    gotoxy(playerX, playerY);
    printf("A");
    
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].alive) {
            gotoxy(enemies[i].x, enemies[i].y);
            printf("W");
        }
    }
    
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            gotoxy(bullets[i].x, bullets[i].y);
            printf("|");
        }
    }
    
    drawParticles();
    
    gotoxy(0, HEIGHT);
    printf("Score: %d", score);
}

void fireBullet() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) {
            bullets[i].x = playerX;
            bullets[i].y = playerY - 1;
            bullets[i].active = 1;
            break;
        }
    }
}

int main() {
    srand(time(NULL));
    hideCursor();
    initEnemies();
    initBullets();
    initParticles();
    
    while (1) {
        if (_kbhit()) {
            char ch = _getch();
            if (ch == 'a' || ch == 'A') {
                if (playerX > 1) playerX--;
            } else if (ch == 'd' || ch == 'D') {
                if (playerX < WIDTH - 2) playerX++;
            } else if (ch == ' ') {
                fireBullet();
            }
        }
        
        moveEnemies();
        moveBullets();
        updateParticles();
        checkCollisions();
        drawGame();
        
        Sleep(100);
    }
    
    return 0;
}
