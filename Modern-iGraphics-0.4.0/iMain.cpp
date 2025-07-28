#include "iGraphics.h"
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <windows.h>
#include <mmsystem.h>



#define SCREEN_WIDTH 1344
#define SCREEN_HEIGHT 760

DWORD musicVolume = 0x50005000;

void applyMusicVolume()
{
    waveOutSetVolume(NULL, musicVolume);
}

bool showMusicSettings = false;
bool musicEnabled = true;



int musicOffBtnX = 510;
int musicOffBtnY = 105;
int musicBtnW = 145;
int musicBtnH = 95;


int musicOnBtnX = 690;
int musicOnBtnY = 105;

int hoveredButtonIndex = -1;
int buttonX = 395;
int buttonYStart = 585;
int buttonWidth = 440;
int buttonHeight = 70;
int buttonSpacing = 97;
int highScore = 0;
char highScorer[50] = "None";
int fuelTickTimer = 0;
int fuelTankSpawnTimer = 0;
int timeSinceFuelEmpty = 0;


int pauseBtnX = 390;
int pauseBtnYStart = 360;
int pauseBtnWidth = 500;
int pauseBtnHeight = 100;
int pauseBtnSpacing = 100;



struct PlayerScore
{
    char name[50];
    int score;
};

PlayerScore topScores[100];
int totalScores = 0;





enum GameState { LOADING, MENU, NAME_ENTRY, PLAYING, GAME_OVER, CONGRATS, LEADERBOARD };

GameState currentState = LOADING;

bool showHelp = false, showCredit = false;
bool showControls = false, isPaused = false, nameEntered = false;
int loadingStartTime;

int playerX = SCREEN_WIDTH / 2, playerY = 50;
int escPressed=false;
int bulletX[50], bulletY[50], bulletCount = 0;
int enemyX[5], enemyY[5], enemyAlive[5];
int enemyBulletX[200], enemyBulletY[200], enemyBulletCount = 0;

int meteorX[2], meteorY[2];

int fuel = 100, health = 100;
bool showFuelTank = false;
int fuelTankX = 100, fuelTankY = SCREEN_HEIGHT;
int score = 0, fuelTimer = 0, fuelDrainTimer = 0;

char playerName[50] = "";
int nameIndex = 0;

int currentWave = 1;

int maxWavesPerLevel[6] = {0, 6, 6, 5, 1, 1};

bool bossActive = false;
int bossX, bossY, bossHealth;


int bossShootPattern = 1;

bool galactusDead = false;

int currentLevel = 1;
const int maxLevel = 5;



int levelWaves[] = {6, 6, 5, 1, 1};
char *levelBackgrounds[] =
{
    "background (2).png",
    "bgm(2).png",
    "bgm(3).png",
    "bgm(4).png",
    "bgm(4).png"
};

char *enemyImages[] =
{
    "enemy.png",
    "enemy(2).png",
    "enemy(3).png",
    "enemy(3).png",
    "enemy(3).png"
};

char *bossImages[] =
{
    "boss.png",
    "boss(2).png",
    "boss(3).png",
    "megaboss.png",
    "galactus.png"
};

int bossMaxHealth[] = {50, 80, 120, 200, 500};

int bossBulletCount[] = {1, 1, 2, 4, 5};

int enemySpeed[] = {1, 2, 4, 6, 8};
int enemyDamage[] = {30, 90, 270, 300, 1000};
int bossDamage[]  = {40, 80, 150, 300, 10000};





void spawnEnemies()
{
    for (int i = 0; i < 5; i++)
    {
        enemyX[i] = rand() % (SCREEN_WIDTH - 100);
        enemyY[i] = SCREEN_HEIGHT - rand() % 200;
        enemyAlive[i] = 6;  // Enemy health
    }
}

void spawnMeteors()
{
    for (int i = 0; i < 2; i++)
    {
        meteorX[i] = rand() % (SCREEN_WIDTH - 50);
        meteorY[i] = SCREEN_HEIGHT + rand() % 300;
    }
}

void spawnBoss()
{
    bossX = SCREEN_WIDTH / 2 - 150;
    bossY = SCREEN_HEIGHT - 200;
    bossHealth = bossMaxHealth[currentLevel - 1];

    bossActive = true;
}



void drawControls()
{
    int x = SCREEN_WIDTH - 260;
    int y = 680;
    iSetColor(255, 0, 255);
    iText(x, y, "CONTROLS:", GLUT_BITMAP_HELVETICA_18);
    iText(x, y - 30, "W : Shoot", GLUT_BITMAP_HELVETICA_12);
    iText(x, y - 50, "ESC : Pause / Resume", GLUT_BITMAP_HELVETICA_12);
    iText(x, y - 70, "H : Show/Hide Controls", GLUT_BITMAP_HELVETICA_12);
    iText(x, y - 90, "R : Restart Game", GLUT_BITMAP_HELVETICA_12);
    iText(x, y - 110, "M : Mute Sound", GLUT_BITMAP_HELVETICA_12);
    iText(x, y - 130, "'+' or '-' to increase and decrease the volume", GLUT_BITMAP_HELVETICA_12);
}

void iDraw()
{
    iClear();

    if (currentState == LOADING)
    {
        iShowImage(0, 0, "loadingscreen.png");
        return;
    }
    if (currentState == MENU)
    {
        iShowImage(0, 0, "menu.png");


if (hoveredButtonIndex != -1)
{
    int expand = 6;
    int bx = buttonX - expand;
    int by = buttonYStart - hoveredButtonIndex * buttonSpacing - expand;
    int bw = buttonWidth + 2 * expand;
    int bh = buttonHeight + 2 * expand;

    iSetColor(0, 255, 250); // Neon cyan
    iSetLineWidth(3);

    int bevel = 15;

    double px[8] = {
        bx + bevel,
        bx + bw - bevel,
        bx + bw,
        bx + bw,
        bx + bw - bevel,
        bx + bevel,
        bx,
        bx,
    };

    double py[8] = {
        by + bh,
        by + bh,
        by + bh - bevel,
        by + bevel,
        by,
        by,
        by + bevel,
        by + bh - bevel
    };

    // Border line
    iSetColor(0, 255, 255);
    iPolygon(px, py, 8);
}


    if (showMusicSettings)
{
    iShowImage(0, 0, "music.png");

    iSetColor(0, 255, 255); // Cyan color
    iText(500, 50, "Press 'h' to get back to the menu page", GLUT_BITMAP_HELVETICA_18);



    return;
}


        if (showHelp)
        {
            iShowImage(0, 0, "help.png");
        }

        if (showCredit)
        {
            int x = (SCREEN_WIDTH - 800) / 2;
            int y = (SCREEN_HEIGHT - 600) / 2;
            iSetColor(255, 255, 255);
            iFilledRectangle(x - 10, y - 10, 820, 620);
            iShowImage(x, y, "credit(1).png");
        }

        return;
    }
    if (currentState == LEADERBOARD)
    {
     iShowImage(0, 0, "leaderboard_bg.png");

        iSetColor(255, 223, 0);
        iText(SCREEN_WIDTH / 2 - 60, SCREEN_HEIGHT - 100, "TOP 10 PLAYERS", GLUT_BITMAP_TIMES_ROMAN_24);

        for (int i = 0; i < totalScores; i++)
        {
            char entry[100];
            sprintf(entry, "%d. %s - %d", i + 1, topScores[i].name, topScores[i].score);
            iText(SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT - 150 - i * 30, entry, GLUT_BITMAP_HELVETICA_18);
        }

        iText(SCREEN_WIDTH / 2 - 100, 50, "Press 'B' to go back", GLUT_BITMAP_HELVETICA_12);
        return;
    }





    if (!nameEntered && currentState == NAME_ENTRY)
    {
           iShowImage(0, 0,"name.png");


    iSetColor(255,0,0);
    iText(375, 340, playerName, GLUT_BITMAP_HELVETICA_18);

    return;
    }

    if (currentState == PLAYING || currentState == GAME_OVER)
    {
        iShowImage(0, 0, levelBackgrounds[currentLevel - 1]);



        iShowImage(playerX, playerY, "ship(1).png");


        for (int i = 0; i < bulletCount; i++)
        {
            if (bulletY[i] >= 0)
            {
                iSetColor(0, 255, 0);
                iFilledRectangle(bulletX[i], bulletY[i], 6, 15);
            }
        }


        if (!bossActive)
        {
            for (int i = 0; i < 5; i++)
            {
                if (enemyAlive[i] > 0)
                    iShowImage(enemyX[i], enemyY[i], enemyImages[currentLevel - 1]);


            }
        }

        if (bossActive)
        {
            iShowImage(bossX, bossY, bossImages[currentLevel - 1]);


            iSetColor(255, 0, 0);
            iFilledRectangle(bossX, bossY + 140, bossHealth * 5, 20);
            iSetColor(255, 255, 255);
           // iRectangle(bossX, bossY + 140, 250, 20);
            iText(bossX, bossY + 165, "BOSS HEALTH", GLUT_BITMAP_HELVETICA_12);
        }


        for (int i = 0; i < enemyBulletCount; i++)
        {
            if (enemyBulletY[i] >= 0)
            {
                iSetColor(255, 0, 0);
                iFilledCircle(enemyBulletX[i], enemyBulletY[i], 4);
            }
        }


        for (int i = 0; i < 2; i++)
        {
            iShowImage(meteorX[i], meteorY[i], "meteor.png");
        }


        if (showFuelTank)
        {
            iShowImage(fuelTankX, fuelTankY, "fuel.png");
        }

        // Health bar
        iSetColor(255, 0, 0);
        iFilledRectangle(20, 720, health * 2, 20);
        iSetColor(255, 255, 255);
        //iRectangle(20, 720, 200, 20);
        iSetColor(255,0 ,0);
        iText(20, 745, "Health", GLUT_BITMAP_HELVETICA_12);

        // Fuel bar
        iSetColor(0, 255, 0);
        iFilledRectangle(20, 690, fuel, 20);
        iSetColor(255, 255, 255);
        //iRectangle(20, 690, 100, 20);
        iSetColor(0,255,0);
        iText(20, 675, "Fuel", GLUT_BITMAP_HELVETICA_12);


        char scoreStr[50];
        sprintf(scoreStr, "Score: %d", score);
        iText(SCREEN_WIDTH - 200, 740, scoreStr, GLUT_BITMAP_HELVETICA_12);

        char waveStr[50];
        sprintf(waveStr, "Wave: %d%s", currentWave, (bossActive ? " (Boss)" : ""));
        iText(SCREEN_WIDTH - 200, 710, waveStr, GLUT_BITMAP_HELVETICA_12);

        if (showControls) drawControls();

        if (currentState == GAME_OVER)
        {
              if (currentLevel == 5 && !bossActive)
    {
        iShowImage(0, 0, "winscreen.png");
    }

            else
            {
                iShowImage(0, 0, "deathpage.png");
                iSetColor(255, 0, 0);


                char scoreMsg[100];
                sprintf(scoreMsg, "Your Score: %d", score);
                iText(600,200, scoreMsg, GLUT_BITMAP_TIMES_ROMAN_24);
            }


            iText(550,130, "Press 'L' to return to the Main Menu", GLUT_BITMAP_HELVETICA_18);




        }

        if (isPaused)
        {

            iShowImage(0, 0, "pause.png");




        }



    }
}

void iKeyboard(unsigned char key,int state)
{
    if (currentState == MENU)
    {
        if (key == 'h' || key == 'H')
        {
            showHelp = false;
            showCredit = false;
            showMusicSettings = false;
        }
        else if ( (key == 'i' || key == 'I') && state == GLUT_DOWN)
        {
            showCredit = !showCredit;
            showHelp = false;
        }
        else if (key == '\r')
        {
            currentState = NAME_ENTRY;
            PlaySound(0, 0, 0);
        }
        else if (key == 'e' || key == 'E')
        {
            exit(0);
        }
        return;
    }


    if (currentState == NAME_ENTRY && state == GLUT_DOWN)
    {
        if (key == '\r' && nameIndex > 0)
        {
            nameEntered = true;
            currentState = PLAYING;
            PlaySound("ingame.wav", NULL, SND_ASYNC | SND_LOOP);
            applyMusicVolume();

        }
        else if (key == '\b' && nameIndex > 0)
        {
            playerName[--nameIndex] = '\0';
        }
        else if (nameIndex < 49 && key != '\r')
        {
            playerName[nameIndex++] = key;
            playerName[nameIndex] = '\0';
        }
        return;
    }

    if (currentState == PLAYING)
    {

        if (key == 'r')
        {
            health = 100;
            fuel = 100;
            score = 0;
            bulletCount = 0;
            enemyBulletCount = 0;
            isPaused = false;
            bossActive = false;
            currentWave = 1;
            spawnEnemies();
            spawnMeteors();
        }
         else if (key == 's' && state == GLUT_DOWN)
    {
        if (currentLevel < maxLevel)
        {
            currentLevel++;
            currentWave = 1;
            bossActive = false;
            spawnEnemies();
            spawnMeteors();

        }
        else
        {

            currentState = GAME_OVER;
            PlaySound("missionpass.wav", NULL, SND_ASYNC);
            applyMusicVolume();

        }
    }
        else if (key == 'w' && bulletCount < 50 && state == GLUT_DOWN)
        {
            bulletX[bulletCount] = playerX + 57;
            bulletY[bulletCount] = playerY + 90;
        bulletCount++;

        PlaySound("fire.wav", NULL , SND_ASYNC | SND_FILENAME | SND_NOSTOP);


        }
        else if (key == 27 && state == GLUT_DOWN)
        {
            isPaused = !isPaused;
        }
        else if (key == 'h' && state == GLUT_DOWN)
        {
            showControls = !showControls;
        }
        else if (key == 'm')
        {
            PlaySound(0, 0, 0);
        }
    }
    if (currentState == GAME_OVER)
    {
        if (key == 'l' || key == 'L')
        {
            currentState = MENU;
            isPaused = false;
            nameEntered = false;
            fuel = 100;
            health = 100;
            score = 0;
            bulletCount = 0;
            enemyBulletCount = 0;
            currentWave = 1;
            bossActive = false;
            PlaySound("menu.wav", NULL, SND_ASYNC | SND_LOOP);
            applyMusicVolume();

        }
    }

    if (currentState == LEADERBOARD)
    {
        if (key == 'b' || key == 'B')
        {
            currentState = MENU;
        }


        return;
    }

else if (key == '=' || key == '+')
{
    DWORD left = musicVolume & 0xFFFF;
    DWORD right = (musicVolume >> 16) & 0xFFFF;

    left += 0x1000;
    right += 0x1000;

    if (left > 0xFFFF) left = 0xFFFF;
    if (right > 0xFFFF) right = 0xFFFF;

    musicVolume = (right << 16) | left;
    applyMusicVolume();
}
else if (key == '-')
{
    DWORD left = musicVolume & 0xFFFF;
    DWORD right = (musicVolume >> 16) & 0xFFFF;

    if (left >= 0x1000) left -= 0x1000;
    if (right >= 0x1000) right -= 0x1000;

    musicVolume = (right << 16) | left;
    applyMusicVolume();
}



}

void iSpecialKeyboard(unsigned char key,int state)
{

    if (currentState != PLAYING || isPaused) return;

    if (key == GLUT_KEY_RIGHT)
    {
        playerX += 20;
        if (playerX > SCREEN_WIDTH - 100) playerX = SCREEN_WIDTH - 100;
    }
    else if (key == GLUT_KEY_LEFT)
    {
        playerX -= 20;
        if (playerX < 0) playerX = 0;
    }
}

void loadLeaderboard()
{
    FILE *file = fopen("leaderboard.txt", "r");
    totalScores = 0;

    if (file)
    {
        while (fscanf(file, "%d %[^\n]", &topScores[totalScores].score, topScores[totalScores].name) == 2)
        {
            totalScores++;
            if (totalScores == 100) break;
        }
        fclose(file);
    }

    for (int i = 0; i < totalScores - 1; i++)
    {
        for (int j = i + 1; j < totalScores; j++)
        {
            if (topScores[j].score > topScores[i].score)
            {
                PlayerScore temp = topScores[i];
                topScores[i] = topScores[j];
                topScores[j] = temp;
            }
        }
    }


    if (totalScores > 10)
        totalScores = 10;
}



void loadHighScore()
{
    FILE *file = fopen("highscore.txt", "r");
    if (file)
    {
        fscanf(file, "%d %[^\n]", &highScore, highScorer);
        fclose(file);
    }
}


void saveHighScore(int score, const char *name)
{
    FILE *file = fopen("highscore.txt", "w");
    if (file)
    {
        fprintf(file, "%d %s", score, name);
        fclose(file);
    }
}
void saveGame()
{
    FILE *file = fopen("save.txt", "w");
    if (file)
    {
        fprintf(file, "%s\n", playerName);
        fprintf(file, "%d %d %d %d %d\n", score, health, fuel, currentLevel, currentWave);
        fclose(file);
        printf("Game saved.\n");
    }
}
void loadGame()
{
    FILE *file = fopen("save.txt", "r");
    if (file)
    {
        fscanf(file, "%[^\n]\n", playerName);
        fscanf(file, "%d %d %d %d %d", &score, &health, &fuel, &currentLevel, &currentWave);
        fclose(file);

        nameEntered = true;
        currentState = PLAYING;
        bossActive = false;
        bulletCount = 0;
        enemyBulletCount = 0;
        isPaused = false;

        spawnEnemies();
        spawnMeteors();

        PlaySound("ingame.wav", NULL, SND_ASYNC | SND_LOOP);
        applyMusicVolume();
    }
    else
    {
        printf("No saved game found.\n");
    }
}






void updateGame()
{
    if (currentState != PLAYING || isPaused) return;

    for (int i = 0; i < bulletCount; i++)
    {
        bulletY[i] += 20;
        if (bulletY[i] > SCREEN_HEIGHT)
        {
            for (int j = i; j < bulletCount - 1; j++)
            {
                bulletX[j] = bulletX[j + 1];
                bulletY[j] = bulletY[j + 1];
            }
            bulletCount--;
            i--;
        }
    }


    for (int i = 0; i < enemyBulletCount; i++)
    {
        enemyBulletY[i] -= 5 + currentLevel * 2;

        if (enemyBulletY[i] < 0)
        {
            for (int j = i; j < enemyBulletCount - 1; j++)
            {
                enemyBulletX[j] = enemyBulletX[j + 1];
                enemyBulletY[j] = enemyBulletY[j + 1];
            }
            enemyBulletCount--;
            i--;
            continue;
        }

        if (enemyBulletX[i] >=playerX && enemyBulletX[i] <= playerX + 100 &&
                enemyBulletY[i] >= playerY && enemyBulletY[i] <= playerY + 100)
        {

            if (bossActive)
            {
                if (currentLevel == 5)
                {
                    health = 0;
                }
                else
                {
                    health -= bossDamage[currentLevel - 1];
                }
            }
            else
            {
                health -= enemyDamage[currentLevel - 1];
            }

            for (int j = i; j < enemyBulletCount - 1; j++)
            {
                enemyBulletX[j] = enemyBulletX[j + 1];
                enemyBulletY[j] = enemyBulletY[j + 1];
            }
            enemyBulletCount--;
            i--;
        }

    }

    // Boss
    if (bossActive)
    {
        static int bossDir = 1;
        bossX += bossDir * 2;
        if (bossX <= 0 || bossX >= SCREEN_WIDTH - 300) bossDir = -bossDir;

        if (rand() % 50 == 0)
        {
            int bulletsToShoot = (currentLevel >= 2) ? 6 : bossBulletCount[currentLevel - 1];

            for (int b = 0; b < bulletsToShoot && enemyBulletCount < 50; b++)
            {
                enemyBulletX[enemyBulletCount] = bossX + 150 + (b * 20) - 60;
                enemyBulletY[enemyBulletCount] = bossY;
                enemyBulletCount++;
            }

        }




        if (currentLevel >= 2 && rand() % 200 == 0)
        {
            int healAmount = 3 + (currentLevel * 2);
            bossHealth += healAmount;

            if (bossHealth > bossMaxHealth[currentLevel - 1])
            {
                bossHealth = bossMaxHealth[currentLevel - 1];
            }
        }



        for (int i = 0; i < bulletCount; i++)
        {
            if (bulletX[i] >= bossX && bulletX[i] <= bossX + 300 &&
                    bulletY[i] >= bossY && bulletY[i] <= bossY + 150)
            {

                bossHealth--;


                for (int k = i; k < bulletCount - 1; k++)
                {
                    bulletX[k] = bulletX[k + 1];
                    bulletY[k] = bulletY[k + 1];
                }
                bulletCount--;
                i--;


                if (bossHealth <= 0)
                {
                    bossActive = false;
                    score += 100;

                    if (currentLevel < maxLevel)
                    {
                        currentLevel++;
                        currentWave = 1;
                        spawnEnemies();
                        spawnMeteors();
                    }
                    else
                    {
                        currentState = GAME_OVER;
                        PlaySound("missionpass.wav", NULL, SND_ASYNC);
                        applyMusicVolume();

                    }
                }
            }
        }
    }


    else
    {
        // Enemies
        for (int i = 0; i < 5; i++)
        {
            if (enemyAlive[i] > 0)
            {
                enemyY[i] -= enemySpeed[currentLevel - 1];

                if (enemyY[i] < 0)
                {
                    enemyAlive[i] = 0;
                    health -= enemyDamage[currentLevel - 1];

                    continue;
                }
                if (rand() % 60 == 0 && enemyBulletCount < 50)
                {
                    enemyBulletX[enemyBulletCount] = enemyX[i] + 45;
                    enemyBulletY[enemyBulletCount] = enemyY[i];
                    enemyBulletCount++;
                }
                for (int j = 0; j < bulletCount; j++)
                {
                    if (bulletX[j] >= enemyX[i] && bulletX[j] <= enemyX[i] + 100&&
                            bulletY[j] >= enemyY[i] && bulletY[j] <= enemyY[i] + 100)
                    {
                        enemyAlive[i]--;
                        for (int k = j; k < bulletCount - 1; k++)
                        {
                            bulletX[k] = bulletX[k + 1];
                            bulletY[k] = bulletY[k + 1];
                        }
                        bulletCount--;
                        j--;
                        if (enemyAlive[i] == 0) score += 10;
                    }
                }
            }
        }

        bool waveCleared = true;
        for (int i = 0; i < 5; i++)
        {
            if (enemyAlive[i] > 0)
            {
                waveCleared = false;
                break;
            }
        }

        if (waveCleared)
        {
            currentWave++;
            if (currentWave <= levelWaves[currentLevel - 1])
            {
                spawnEnemies();
            }
            else
            {
                spawnBoss();
            }

        }
    }


    for (int i = 0; i < 2; i++)
    {
        meteorY[i] -= 5;
        if (meteorY[i] < 0)
        {
            meteorY[i] = SCREEN_HEIGHT + rand() % 300;
            meteorX[i] = rand() % (SCREEN_WIDTH - 50);
        }
        if (playerX + 80 > meteorX[i] && playerX < meteorX[i] + 50 &&
                playerY + 80 > meteorY[i] && playerY < meteorY[i] + 50)
        {
            health -= 10;
            meteorY[i] = SCREEN_HEIGHT + rand() % 300;
            meteorX[i] = rand() % (SCREEN_WIDTH - 50);
        }
    }


    if (showFuelTank)
    {
        fuelTankY -= 2;
        if (fuelTankY < 0) showFuelTank = false;
        if (playerX + 80 > fuelTankX && playerX < fuelTankX + 50 &&
                playerY + 80 > fuelTankY && playerY < fuelTankY + 50)
        {
            fuel = 100;
            showFuelTank = false;
        }
    }


    fuelTickTimer++;
    if (fuelTickTimer >= 30)
    {
        fuel--;
        fuelTickTimer = 0;
    }

    fuelTankSpawnTimer++;
    if (!showFuelTank && fuelTankSpawnTimer >= 40 * 60)
    {
        showFuelTank = true;
        fuelTankX = rand() % (SCREEN_WIDTH - 50);
        fuelTankY = SCREEN_HEIGHT;
        fuelTankSpawnTimer = 0;
    }

    if (!showFuelTank && fuelTankSpawnTimer == 1)
    {
        showFuelTank = true;
        fuelTankX = rand() % (SCREEN_WIDTH - 50);
        fuelTankY = SCREEN_HEIGHT;
    }



    if (fuel <= 0)
    {
        timeSinceFuelEmpty++;
        if (timeSinceFuelEmpty >= 5 * 60)   // every 5 seconds
        {
            health -= 10;
            timeSinceFuelEmpty = 0;
        }
    }
    else
    {

        timeSinceFuelEmpty = 0;
    }



    if (score > highScore)
    {
        highScore = score;
        strcpy(highScorer, playerName);
        saveHighScore(score,playerName);
    }
    if (fuel <= 0 || health <= 0)
    {

        FILE *f = fopen("leaderboard.txt", "a");
        if (f)
        {
            fprintf(f, "%d %s\n", score, playerName);
            fclose(f);
        }


        if (score > highScore)
        {
            highScore = score;
            strcpy(highScorer, playerName);
            saveHighScore(score, playerName);
        }

        currentState = GAME_OVER;
        isPaused = false;
        PlaySound("missionpass.wav", NULL, SND_ASYNC);
        applyMusicVolume();

    }


}

void iMouseMove(int mx, int my)
{
    if (currentState == MENU)
    {
        hoveredButtonIndex = -1;
        for (int i = 0; i < 6; i++)
        {
            int bx = buttonX;
            int by = buttonYStart - i * buttonSpacing;

            if (mx >= bx && mx <= bx + buttonWidth &&
                    my >= by && my <= by + buttonHeight)
            {
                hoveredButtonIndex = i;
                break;
            }
        }

    }



    if (currentState == PLAYING && !isPaused)
    {
        playerX = mx - 50;
        if (playerX < 0) playerX = 0;
        if (playerX > SCREEN_WIDTH - 100) playerX = SCREEN_WIDTH - 100;



    }
}

void iMouse(int button, int state, int mx, int my)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {

           if (showMusicSettings)
    {

       if (mx >= musicOffBtnX && mx <= musicOffBtnX + musicBtnW &&
    my >= musicOffBtnY && my <= musicOffBtnY + musicBtnH) {
    // OFF button clicked
    PlaySound(0, 0, 0);
}

if (mx >= musicOnBtnX && mx <= musicOnBtnX + musicBtnW &&
    my >= musicOnBtnY && my <= musicOnBtnY + musicBtnH) {
    // ON button clicked
    PlaySound("menu.wav", NULL, SND_ASYNC | SND_LOOP);
    }



    }



        if (isPaused)
        {
            if (mx >= pauseBtnX && mx <= pauseBtnX + pauseBtnWidth)
            {
                if (my >= pauseBtnYStart && my <= pauseBtnYStart + pauseBtnHeight)
                {
                    isPaused = false; // Resume
                    return;
                }
                else if (my >= pauseBtnYStart - pauseBtnSpacing &&
                         my <= pauseBtnYStart - pauseBtnSpacing + pauseBtnHeight)
                {
                    saveGame();
                    return;
                }
                else if (my >= pauseBtnYStart - 2 * pauseBtnSpacing &&
                         my <= pauseBtnYStart - 2 * pauseBtnSpacing + pauseBtnHeight)
                {
                    currentState = MENU;
                    isPaused = false;
                    PlaySound("menu.wav", NULL, SND_ASYNC | SND_LOOP);
                    applyMusicVolume();

                    return;
                }
            }
        }


        if (currentState == MENU)
        {
            for (int i = 0; i < 6; i++)
            {
                int bx = buttonX;
                int by = buttonYStart - i * buttonSpacing;

                if (mx >= bx && mx <= bx + buttonWidth &&
                        my >= by && my <= by + buttonHeight)
                {
                    switch (i)
                    {
                    case 0:
                        currentState = NAME_ENTRY;
                        PlaySound(0, 0, 0);
                        break;
                    case 1:
                        loadLeaderboard();
                        currentState = LEADERBOARD;
                        break;
                    case 2:
                        exit(0);
                        break;
                    case 3: loadGame();
                        break;
                    case 4:
                        showHelp = !showHelp;
                        showCredit = false;
                        break;
                 case 5:
    showMusicSettings = true;
    showHelp = false;
    showCredit = false;
    break;

                    }
                }
            }
        }

        if(currentState == PLAYING && !isPaused)
        {

             bulletX[bulletCount] = playerX +57;
            bulletY[bulletCount] = playerY + 90;
            bulletCount++;
             PlaySound("fire.wav", NULL, SND_ASYNC | SND_FILENAME | SND_NOSTOP);

        }

    }
}


void iMouseDrag(int mx, int my) {}
void iMouseWheel(int dir, int mx, int my) {}





int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    srand(time(0));
    loadingStartTime = clock();
    loadHighScore();
    spawnEnemies();
    spawnMeteors();

    iSetTimer(100, []()
    {
        if (currentState == LOADING && (clock() - loadingStartTime) / CLOCKS_PER_SEC >= 1.5)
        {
            currentState = MENU;
            PlaySound("menu.wav", NULL, SND_ASYNC | SND_LOOP);
            applyMusicVolume();

        }
    });

    iSetTimer(17, updateGame);

    iOpenWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Space Shooter Game");
    return 0;
}
