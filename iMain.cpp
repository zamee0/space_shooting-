#include "iGraphics.h"
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <windows.h>  // For PlaySound
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")

#define SCREEN_WIDTH 1344
#define SCREEN_HEIGHT 760

// Game states
enum GameState { LOADING, MENU, NAME_ENTRY, PLAYING, GAME_OVER };
GameState currentState = LOADING;

int loadingStartTime;

int playerX = SCREEN_WIDTH / 2, playerY = 50;
int bulletX[50], bulletY[50], bulletCount = 0;
int enemyX[5], enemyY[5], enemyAlive[5];
int enemyBulletX[50], enemyBulletY[50], enemyBulletCount = 0;
int meteorX[2], meteorY[2];
int fuel = 100, health = 100;
bool showFuelTank = false, isPaused = false, showControls = false, nameEntered = false;
int fuelTankX = 100, fuelTankY = SCREEN_HEIGHT;
int score = 0, fuelTimer = 0, fuelDrainTimer = 0;
char playerName[50] = "";
int nameIndex = 0;

void spawnEnemies() {
    for (int i = 0; i < 5; i++) {
        enemyX[i] = rand() % (SCREEN_WIDTH - 100);
        enemyY[i] = SCREEN_HEIGHT - rand() % 200;
        enemyAlive[i] = 6;
    }
}

void spawnMeteors() {
    for (int i = 0; i < 2; i++) {
        meteorX[i] = rand() % (SCREEN_WIDTH - 50);
        meteorY[i] = SCREEN_HEIGHT + rand() % 300;
    }
}

void drawControls() {
    int x = SCREEN_WIDTH - 300;
    int y = 700;
    iSetColor(255, 255, 255);
    iText(x, y, "CONTROLS:", GLUT_BITMAP_HELVETICA_18);
    iText(x, y - 25, "← → : Move Left/Right", GLUT_BITMAP_HELVETICA_12);
    iText(x, y - 45, "W : Shoot", GLUT_BITMAP_HELVETICA_12);
    iText(x, y - 65, "ESC : Pause / Resume", GLUT_BITMAP_HELVETICA_12);
    iText(x, y - 85, "H : Show/Hide Controls", GLUT_BITMAP_HELVETICA_12);
    iText(x, y - 105, "R : Restart Game", GLUT_BITMAP_HELVETICA_12);
    iText(x, y - 125, "M : Mute Sound", GLUT_BITMAP_HELVETICA_12);
}

void iDraw() {
    iClear();

    if (currentState == LOADING) {
        iShowImage(0, 0, "loadingscreen.png");
        return;
    }

    if (currentState == MENU) {
        iShowImage(0, 0, "menu.bmp");
        iSetColor(255, 255, 255);
        iText(SCREEN_WIDTH/2 - 100, 100, "Press ENTER to Start", GLUT_BITMAP_HELVETICA_18);
        return;
    }

    if (!nameEntered && currentState == NAME_ENTRY) {
        iSetColor(255, 255, 255);
        iText(500, 400, "Enter Your Name:", GLUT_BITMAP_HELVETICA_18);
        iText(500, 370, playerName, GLUT_BITMAP_HELVETICA_18);
        return;
    }

    if (currentState == PLAYING || currentState == GAME_OVER) {
        // Game Background
        iShowImage(0, 0, "background (2).png");

        // Player
        iShowImage(playerX, playerY, "player.png");

        // Player Bullets
        for (int i = 0; i < bulletCount; i++) {
            iSetColor(0, 255, 0);
            iFilledRectangle(bulletX[i], bulletY[i], 6, 15);
        }

        // Enemies
        for (int i = 0; i < 5; i++) {
            if (enemyAlive[i] > 0)
                iShowImage(enemyX[i], enemyY[i], "enemy.png");
        }

        // Enemy Bullets
        for (int i = 0; i < enemyBulletCount; i++) {
            iSetColor(255, 0, 0);
            iFilledCircle(enemyBulletX[i], enemyBulletY[i], 4);
        }

        // Meteors
        for (int i = 0; i < 2; i++) {
            iShowImage(meteorX[i], meteorY[i], "meteor.png");
        }

        // Fuel tank
        if (showFuelTank) {
            iShowImage(fuelTankX, fuelTankY, "fuel.png");
        }

        // Health bar
        iSetColor(255, 0, 0);
        iFilledRectangle(20, 720, health * 2, 20);
        iSetColor(255, 255, 255);
        iRectangle(20, 720, 200, 20);
        iText(20, 745, "Health", GLUT_BITMAP_HELVETICA_12);

        // Fuel bar
        iSetColor(0, 255, 0);
        iFilledRectangle(20, 690, fuel, 20);
        iSetColor(255, 255, 255);
        iRectangle(20, 690, 100, 20);
        iText(20, 665, "Fuel", GLUT_BITMAP_HELVETICA_12);

        // Score
        char scoreStr[50];
        sprintf(scoreStr, "Score: %d", score);
        iText(SCREEN_WIDTH - 200, 740, scoreStr, GLUT_BITMAP_HELVETICA_12);

        if (showControls) drawControls();

        if (currentState == GAME_OVER) {
            iSetColor(255, 0, 0);
            iText(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 20, "GAME OVER", GLUT_BITMAP_TIMES_ROMAN_24);

            char scoreMsg[100];
            sprintf(scoreMsg, "Your Score: %d", score);
            iText(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 10, scoreMsg, GLUT_BITMAP_HELVETICA_18);
        }
    }
}

void iMouseMove(int mx, int my) {
    if (currentState == PLAYING && !isPaused) {
        playerX = mx - 50;
        playerY = my - 50;
    }
}

void iMouse(int button, int state, int mx, int my) {
    // Not used
}

void iMouseDrag(int mx, int my) {}

void iMouseWheel(int dir , int mx , int my) {}

void iKeyboard(unsigned char key) {
    if (currentState == MENU && key == '\r') {
        currentState = NAME_ENTRY;
        PlaySound(0, 0, 0); // stop menu music
        return;
    }

    if (currentState == NAME_ENTRY) {
        if (key == '\r') {
            if (nameIndex > 0) {  // Only proceed if name not empty
                nameEntered = true;
                currentState = PLAYING;
                PlaySound("ingame.wav", NULL, SND_ASYNC | SND_LOOP);
            }
        }
        else if (key == '\b' && nameIndex > 0) {
            playerName[--nameIndex] = '\0';
        }
        else if (nameIndex < 49 && key != '\r') {
            playerName[nameIndex++] = key;
            playerName[nameIndex] = '\0';
        }
        return;
    }

    if (currentState != PLAYING) return;

    if (key == 'r') {
        health = 100;
        fuel = 100;
        score = 0;
        bulletCount = 0;
        enemyBulletCount = 0;
        isPaused = false;
        spawnEnemies();
        spawnMeteors();
    } else if (key == 'w' && bulletCount < 50) {
        bulletX[bulletCount] = playerX + 45;
        bulletY[bulletCount] = playerY + 90;
        bulletCount++;
    } else if (key == 27) { // ESC pause/resume
        isPaused = !isPaused;
    } else if (key == 'h') {
        showControls = !showControls;
    }
}

void iSpecialKeyboard(unsigned char key) {
    if (currentState != PLAYING || isPaused) return;
    if (key == GLUT_KEY_RIGHT) playerX += 20;
    if (key == GLUT_KEY_LEFT) playerX -= 20;
}

void updateGame() {
    if (currentState != PLAYING || isPaused) return;

    // Move player bullets
    for (int i = 0; i < bulletCount; i++) {
        bulletY[i] += 15;
        if (bulletY[i] > SCREEN_HEIGHT)
            bulletY[i] = -100;
    }

    // Move enemy bullets
    for (int i = 0; i < enemyBulletCount; i++) {
        enemyBulletY[i] -= 5;
        if (enemyBulletY[i] < 0)
            enemyBulletY[i] = -100;
        if (enemyBulletX[i] > playerX && enemyBulletX[i] < playerX + 100 &&
            enemyBulletY[i] > playerY && enemyBulletY[i] < playerY + 100) {
            health -= 30;
            enemyBulletY[i] = -100;
        }
    }

    // Enemies move and shoot
    for (int i = 0; i < 5; i++) {
        if (enemyAlive[i] > 0) {
            enemyY[i] -= 1;
            if (enemyY[i] < 0) {
                enemyAlive[i] = 0;
                health -= 30;
                continue;
            }
            if (rand() % 60 == 0 && enemyBulletCount < 50) {
                enemyBulletX[enemyBulletCount] = enemyX[i] + 45;
                enemyBulletY[enemyBulletCount] = enemyY[i];
                enemyBulletCount++;
            }
            for (int j = 0; j < bulletCount; j++) {
                if (bulletX[j] >= enemyX[i] && bulletX[j] <= enemyX[i] + 100 &&
                    bulletY[j] >= enemyY[i] && bulletY[j] <= enemyY[i] + 100) {
                    enemyAlive[i]--;
                    bulletY[j] = -100;
                    if (enemyAlive[i] == 0) score += 10;
                }
            }
        }
    }

    // Meteors move and collide
    for (int i = 0; i < 2; i++) {
        meteorY[i] -= 5;
        if (meteorY[i] < 0) {
            meteorY[i] = SCREEN_HEIGHT + rand() % 300;
            meteorX[i] = rand() % (SCREEN_WIDTH - 50);
        }
        if (playerX + 80 > meteorX[i] && playerX < meteorX[i] + 50 &&
            playerY + 80 > meteorY[i] && playerY < meteorY[i] + 50) {
            health -= 10;
            meteorY[i] = SCREEN_HEIGHT + rand() % 300;
        }
    }

    // Fuel tank move & collide
    if (showFuelTank) {
        fuelTankY -= 2;
        if (fuelTankY < 0) showFuelTank = false;
        if (playerX + 80 > fuelTankX && playerX < fuelTankX + 50 &&
            playerY + 80 > fuelTankY && playerY < fuelTankY + 50) {
            fuel = 100;
            showFuelTank = false;
        }
    }

    // Fuel tank spawn timer
    fuelTimer++;
    if (fuelTimer > 3000) {
        showFuelTank = true;
        fuelTankX = rand() % (SCREEN_WIDTH - 50);
        fuelTankY = SCREEN_HEIGHT;
        fuelTimer = 0;
    }

    // Fuel drain timer
    fuelDrainTimer++;
    if (fuelDrainTimer > 6000) {
        fuel -= 10;
        fuelDrainTimer = 0;
    }

    // Check game over
    if (fuel <= 0 || health <= 0) {
        currentState = GAME_OVER;
        isPaused = true;
        PlaySound("missionpass.wav", NULL, SND_ASYNC);
    }
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    srand(time(0));
    loadingStartTime = clock();

    spawnEnemies();
    spawnMeteors();

    iSetTimer(100, []() {
        if (currentState == LOADING && (clock() - loadingStartTime) / CLOCKS_PER_SEC >= 5) {
            currentState = MENU;
            PlaySound("menu.wav", NULL, SND_ASYNC | SND_LOOP);
        }
    });

    iSetTimer(17, updateGame);

    iInitialize(SCREEN_WIDTH, SCREEN_HEIGHT, "Space Shooter Game");

    return 0;
}
