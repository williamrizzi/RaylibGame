#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define INPUT_CHARS     7

///Estados de jogo
 enum { MENU, PAUSE, INGAME, CREDITS, TUTORIAL, RANKING, VITORIA, DERROTA } state = MENU;

 enum { LEVEL0, LEVEL1} stage = LEVEL0;

///Resolução da janela
const int screenWidth = 820;
const int screenHeight = 360;

///Condições do jogo
int timeMessage = 40;

///Posição do jogador
int posX;
int posY;

///Atributos do jogador
int level = 0;
int score = 0;
int life = 3;
int ammo = 5;
int moveSpeed = 3;
int hiscore = 0;
int direction = 0;
int killCounter = 0;
bool playSoundWalk = false;
int timerDamage = 0;
bool startTimerDamage = false;

///Atributos tiro/arrow
bool shooting = false;
int arrowX;
int arrowY;
int arrowSpeed = 12;
int ammoTimer = 0;
bool startTimerAmmo = false;

///Atributos faca
bool knifing = false;
bool holdingKnife = true;
int directionKnife = false;
int knifeX;
int knifeY;
int knifeSpeed = 12;
int timerKnife = 0;
bool startTimerGetKnife = false;
int timerGetKnife = 0;

///Atributos do Baú
int chestX = 200,chestY = 100,chestOffSet = 30;
bool chestOpen = false;
int timerChest = 0;
bool startTimer = false, buffSet = false;
bool lifeChest = true, ammoChest = false, scoreChest = false;

///Atributor da porta
int doorX = 710, doorY = 40, doorOffSet = 30;
bool playSoundDoor = false;

///Sprite visual do jogador
Texture2D spritePlayer;
Texture2D spriteRight;
Texture2D spriteLeft;
Texture2D chestOppened;
Texture2D chestClosed;
Texture2D enemy;
Texture2D door;
Texture2D spriteArrowRight;
Texture2D spriteArrowLeft;
Texture2D spriteKnifeRight;
Texture2D spriteKnifeLeft;

///Inimigos
int maxEnemy = 0;
int vida[10];
int enemyX[10];
int enemyY[10];
int enemyTimer = 0;
int enemyMax = 0;
int enemySpeed = 2;
int timerEnemyKill = 0;
bool startTimerEnemy = false;
bool playSoundKill = false;
bool tempEnemy = false;

//Ranking
Rectangle textBox = { 300, 180, 225, 50 };
char name[INPUT_CHARS + 1] = "\0";
int letterCount = 0;

//audio
bool playSoundWin = false;

//Load and save
FILE *fileLevel;

void LoadSprites(){
    ///Sprites do jogador
    spriteRight = LoadTexture("assets/playerright.png");
    spriteLeft = LoadTexture("assets/playerleft.png");
    spritePlayer = spriteRight;
    ///Sprite do tiro
    spriteArrowRight = LoadTexture("assets/arrowRight.png");
    spriteArrowLeft = LoadTexture("assets/arrowLeft.png");
    ///Sprite da faca
    spriteKnifeRight = LoadTexture("assets/knifeRight.png");
    spriteKnifeLeft = LoadTexture("assets/knifeLeft.png");
    ///Sprites de props
    chestOppened = LoadTexture("assets/chestopenned.png");
    chestClosed = LoadTexture("assets/chestclosed.png");
    ///Sprite dos Inimigos
    enemy = LoadTexture("assets/enemy.png");
    door = LoadTexture("assets/door.png");
}

void Unload(){
    UnloadTexture(spriteRight);
    UnloadTexture(spriteLeft);
    UnloadTexture(spriteArrowRight);
    UnloadTexture(spriteArrowLeft);
    UnloadTexture(spriteKnifeRight);
    UnloadTexture(spriteKnifeLeft);
    UnloadTexture(chestOppened);
    UnloadTexture(chestClosed);
    UnloadTexture(enemy);
    UnloadTexture(door);
}

void LoadLevel(FILE *file){
    enemyMax = 0;
    killCounter = 0;
    shooting = false;
    knifing = false;
    holdingKnife = true;
    int list[30][81];
    char aux;
    for (int i=0; i < 30; i++){
        for (int j=0; j < 81; j++){
            fscanf(file, "%c", &aux);
            list[i][j]=aux;
        }
    }
    fclose(file);
    for (int i=0; i < 30; i++){
        for (int j=0; j < 81; j++){
            switch (list[i][j])
            {
                case 80:
                    printf(" P = %3d %3d\n", i+1, j+1); ///porta
                    doorY = (i+1) * 10;
                    doorX = (j+1) * 10;
                    break;
                case 66:
                    printf(" B = %3d %3d\n", i+1, j+1); ///bau
                    int aux;
                    aux = rand() % 3;
                    switch(aux){
                        case 0:
                            lifeChest = true;
                            ammoChest = false;
                            scoreChest = false;
                            break;
                        case 1:
                            lifeChest = false;
                            ammoChest = true;
                            scoreChest = false;
                            break;
                        case 2:
                            lifeChest = false;
                            ammoChest = false;
                            scoreChest = true;
                            break;
                    }
                    chestY = (i+1) * 10;
                    chestX = (j+1) * 10;
                    break;
                case 69:
                    enemyX[enemyMax] = (j+1)*10;
                    enemyY[enemyMax] = (i+1)*10;
                    enemyMax +=1;
                    printf(" E = %3d %3d\n", i+1, j+1); ///enemigo
                    break;
                case 74:
                    printf(" J = %3d %3d\n", i+1, j+1); ///jogador
                    posY = (i+1) * 10;
                    posX = (j+1) * 10;
                    break;
                case 86:
                    printf(" V = %3d %3d\n", i+1, j+1); ///bau vida
                    lifeChest = true;
                    ammoChest = false;
                    scoreChest = false;
                    chestOpen = false;
                    chestY = (i+1) * 10;
                    chestX = (j+1) * 10;
                    break;
                case 77:
                    printf(" M = %3d %3d\n", i+1, j+1); ///bau munição
                    lifeChest = false;
                    ammoChest = true;
                    scoreChest = false;
                    chestOpen = false;
                    chestY = (i+1) * 10;
                    chestX = (j+1) * 10;
                    break;
                case 84:
                    printf(" T = %3d %3d\n", i+1, j+1); ///bau pontos
                    lifeChest = false;
                    ammoChest = false;
                    scoreChest = true;
                    chestOpen = false;
                    chestY = (i+1) * 10;
                    chestX = (j+1) * 10;
                    break;
                case 65:
                    printf(" A = %3d %3d\n", i+1, j+1); ///bau aberto
                    chestOpen = true;
                    chestY = (i+1) * 10;
                    chestX = (j+1) * 10;
                    break;
            }
        }
    }
}

void SaveGame(){
    fileLevel = fopen("save.txt", "w");
    for (int i=0; i < 30; i++){
        for (int j=0; j < 81; j++){
            bool temp = false;
            for(int k = 0; k < enemyMax; k++){
                if ((j == (enemyX[k]/10)) && (i == (enemyY[k]/10))) {fprintf(fileLevel, "E");}
            }
            if (j == 80)fprintf(fileLevel, "\n");
            else if ((j == (posX/10)) && (i == (posY/10))) {fprintf(fileLevel, "J");}
            else if ((j == (doorX/10)) && (i == (doorY/10))) {fprintf(fileLevel, "P");}
            else if ((j == (chestX/10)) && (i == (chestY/10)))
            {
                if (chestOpen == false)
                {
                    if ((lifeChest == true) && (ammoChest == false) && (scoreChest == false)) fprintf(fileLevel, "V");
                    if ((lifeChest == false) && (ammoChest == true) && (scoreChest == false)) fprintf(fileLevel, "M");
                    if ((lifeChest == false) && (ammoChest == false) && (scoreChest == true)) fprintf(fileLevel, "T");
                }
                else fprintf(fileLevel, "A");
            }
            else if (temp == false) fprintf(fileLevel, "-");
        }
    }
    fclose(fileLevel);
}

void LoadShowRanking(){

}

void WriteRanking(){

}

void Player(){
        printf("%d", killCounter);
        printf("%d", enemyMax);

        ///Comando de movimento do Jogador
        if (IsKeyDown(KEY_UP)) {
            playSoundWalk = true;
            if(posY>2){
                posY -= moveSpeed;
            }else if(posY<=1){
                posY = 1;
            }
        }
        if (IsKeyDown(KEY_DOWN)){
            playSoundWalk = true;
            if(posY<290){
                posY += moveSpeed;
            }else if(posY>=291){
                posY = 291;
            }
        }
        if (IsKeyDown(KEY_RIGHT)){
            playSoundWalk = true;
            if(shooting == false && knifing == false){
                direction = 0;
            }
            if(posX<793){
                posX += moveSpeed;
                spritePlayer = spriteRight;
            }else if(posX>=794){
                posX = 794;
                spritePlayer = spriteRight;
            }
        }
        if (IsKeyDown(KEY_LEFT)){
            playSoundWalk = true;
            if(shooting == false && knifing == false){
                direction = 1;
            }
            if(posX>2){
                posX -= moveSpeed;
                spritePlayer = spriteLeft;
            }else if(posX<=1){
                posX = 1;
                spritePlayer = spriteLeft;
            }
        }
        ///Colisor com chest
        if(posX>(chestX-chestOffSet) && posX<(chestX+chestOffSet)){
            if(posY>(chestY-chestOffSet) && posY<(chestY+chestOffSet)){
                if(chestOpen == false){
                    DrawText("Press E do open", 560, 335, 20, BLACK);
                    if (IsKeyPressed(KEY_E)) chestOpen = true;
                }else if(chestOpen == true){
                    if(startTimer == false){
                        DrawText("Chest already open", 560, 335, 20, BLACK);
                    }
                }
            }
        }
        ///Colisor com door
        if(posX>(doorX-doorOffSet) && posX<(doorX+doorOffSet)){
            if(posY>(doorY-doorOffSet) && posY<(doorY+doorOffSet)){
                DrawText("Press E to enter", 560, 335, 20, BLACK);
                if(IsKeyPressed(KEY_E) && killCounter == enemyMax){
                    playSoundDoor = true;
                    if(stage == LEVEL0){
                        fileLevel = fopen("level1.txt", "r");
                        LoadLevel(fileLevel);
                        stage = LEVEL1;
                        level += 1;
                        killCounter = 0;
                    }else if(stage == LEVEL1){
                        state = VITORIA;  /// Condição de vitória
                        playSoundWin = true;
                    }
                }
            }
        }
        ///colisor com inimigo
        for(int i = 0; i < enemyMax; i++){
            if(posX>(enemyX[i]-chestOffSet) && posX<(enemyX[i]+chestOffSet)){
                if(posY>(enemyY[i]-chestOffSet) && posY<(enemyY[i]+chestOffSet)){
                    enemyX[i] = 10000000;
                    enemyY[i] = 10000000;
                    life -= 1;
                    killCounter += 1;
                    playSoundKill = true;
                    startTimerDamage = true;
                }
            }
        }
        ///Colisor com knife
        if(posX>(knifeX-doorOffSet) && posX<(knifeX+doorOffSet)){
            if(posY>(knifeY-doorOffSet) && posY<(knifeY+doorOffSet)){
                if(timerKnife > 4){
                    knifing = false;
                    holdingKnife = true;
                    timerKnife = 0;
                    timerGetKnife = 0;
                    startTimerGetKnife = true;
                }
            }
        }
        if(startTimerGetKnife == true){
            timerGetKnife += 1;
        }
        if(timerGetKnife < timeMessage && startTimerGetKnife == true){
            DrawText("You got a knife.", 560, 335, 20, BLACK);
        }
        else if(timerGetKnife >timeMessage){
            startTimerGetKnife = false;
        }
        ///Morte
        if(life <=0){
            state = DERROTA;  ///Condição de derrota
        }
}

void ThrowArrow(){
    if(shooting == true){
        if(direction == 0){
            arrowX += arrowSpeed;
            DrawTexture(spriteArrowRight, arrowX, arrowY, WHITE);
        }
        else if(direction == 1){
            arrowX -= arrowSpeed;
            DrawTexture(spriteArrowLeft, arrowX, arrowY, WHITE);
        }
        //Collision com inimigo
        for(int i = 0; i < enemyMax; i++){
            if(arrowX>enemyX[i]-doorOffSet && arrowX<enemyX[i]+doorOffSet){
                if(arrowY>enemyY[i]-doorOffSet && arrowY<enemyY[i]+doorOffSet){
                    enemyX[i] = 10000000;
                    enemyY[i] = 10000000;
                    score += 10;
                    shooting = false;
                    timerEnemyKill = 0;
                    startTimerEnemy = true;
                    killCounter += 1;
                    playSoundKill = true;
                }
            }
        }
        //collision com a saida da tela
        if(arrowX<0 || arrowX>800){
            shooting = false;
        }
    }
    else if(shooting == false){
        arrowX = posX;
        arrowY = posY;
    }
    if(startTimerEnemy == true){
        timerEnemyKill += 1;
    }
    if(timerEnemyKill < timeMessage && startTimerEnemy == true){
        DrawText("you killed one.", 560, 335, 20, BLACK);
    }
    else if(timerEnemyKill >timeMessage){
        startTimerEnemy = false;
    }
}

void ThrowKnife(){
    if(knifing == true){
        timerKnife += 1;
        if(direction == 0){
            knifeX += knifeSpeed;
            DrawTexture(spriteKnifeRight, knifeX, knifeY, WHITE);
        }
        else if(direction == 1){
            knifeX -= knifeSpeed;
            DrawTexture(spriteKnifeLeft, knifeX, knifeY, WHITE);
        }
        //collision com inimigo
        for(int i = 0; i < enemyMax; i++){
            if(knifeX>enemyX[i]-doorOffSet && knifeX<enemyX[i]+doorOffSet){
                if(knifeY>enemyY[i]-doorOffSet && knifeY<enemyY[i]+doorOffSet){
                    if(enemyX[i] < posX){
                        directionKnife = false;
                    }else if(enemyX[i] > posX){
                        directionKnife = true;
                    }
                    knifeX = enemyX[i];
                    knifeY = enemyY[i];
                    enemyX[i] = 10000000;
                    enemyY[i] = 10000000;
                    score += 10;
                    shooting = false;
                    knifing = false;
                    holdingKnife = false;
                    timerEnemyKill = 0;
                    startTimerEnemy = true;
                    killCounter += 1;
                    playSoundKill = true;
                }
            }
        }
        //collision com paredes
        if(knifeX>=794){
            knifeX = 794;
            knifing = false;
            holdingKnife = false;
            directionKnife = true;
        }
        if(knifeX<=-5){
            knifeX = -5;
            knifing = false;
            holdingKnife = false;
            directionKnife = false;
        }
    }
    else if(knifing == false){
        if(holdingKnife == true){
            knifeX = posX;
            knifeY = posY;
        }
        else if(holdingKnife == false){
            if(direction == 0){
                if(directionKnife == true){
                    DrawTexture(spriteKnifeRight, knifeX, knifeY, WHITE);
                }
                if(directionKnife == false){
                    DrawTexture(spriteKnifeLeft, knifeX, knifeY, WHITE);
                }
            }
            else if(direction == 1){
                if(directionKnife == true){
                    DrawTexture(spriteKnifeRight, knifeX, knifeY, WHITE);
                }
                if(directionKnife == false){
                    DrawTexture(spriteKnifeLeft, knifeX, knifeY, WHITE);
                }
            }
        }
    }
    if(startTimerEnemy == true){
        timerEnemyKill += 1;
    }
    if(timerEnemyKill < timeMessage && startTimerEnemy == true){
        DrawText("you killed one.", 560, 335, 20, BLACK);
    }
    else if(timerEnemyKill > timeMessage){
        startTimerEnemy = false;
    }
}

void Chest(){
    if(chestOpen == true && startTimer == true){
        timerChest += 1;
        if(lifeChest == true){
            if(buffSet == true){
                life += 1;
                buffSet = false;
            }
            DrawText("Your got one more life", 560, 335, 20, BLACK);
        }else if(ammoChest == true){
            if(buffSet == true){
                ammo += 1;
                buffSet = false;
            }
            DrawText("You got a ammo", 560, 335, 20, BLACK);
        }else if(scoreChest == true){
            if(buffSet == true){
                score += 10;
                buffSet = false;
            }
            DrawText("You got some score", 560, 335, 20, BLACK);
        }
        if(timerChest >100){
            startTimer = false;
            timerChest = 100;
        }
    }
    if(chestOpen == false){
        DrawTexture(chestClosed, chestX, chestY, WHITE);
        startTimer = true;
        buffSet = true;
    }else if(chestOpen == true){
        DrawTexture(chestOppened, chestX, chestY, WHITE);
    }
}

void Enemy(){
    enemyTimer += 1;
    for(int i = 0; i < enemyMax; i++){
        DrawTexture(enemy, enemyX[i], enemyY[i], WHITE);
    }
    if(enemyTimer > 2){
        for(int i = 0; i < enemyMax; i++){
            if(enemyX[i] > posX){
                enemyX[i] -= enemySpeed;
            }else if(enemyX[i] < posX){
                enemyX[i] += enemySpeed;
            }
            if(enemyY[i] > posY){
                enemyY[i] -= enemySpeed;
            }else if(enemyY[i] < posY){
                enemyY[i] += enemySpeed;
            }
        }
        enemyTimer = 0;
    }
}

void InputsMenu(){
    if(state == MENU){
        if (IsKeyDown(KEY_ONE)) {
            fileLevel = fopen("level0.txt", "r");
            LoadLevel(fileLevel);
            stage = LEVEL0;
            state = INGAME;
            score = 0;
            life = 3;
            ammo = 3;
            level = 0;
            killCounter = 0;
        }else if(IsKeyDown(KEY_TWO)){
            fileLevel = fopen("save.txt", "r");
            LoadLevel(fileLevel);
            stage = LEVEL0;
            state = INGAME;
            score = 0;
            life = 3;
            ammo = 3;
            level = 0;
            killCounter = 0;
        }
        else if(IsKeyDown(KEY_THREE)){
            state = TUTORIAL;
        }
        else if(IsKeyDown(KEY_FOUR)){
            state = CREDITS;
        }
    }else if(state == INGAME){
        if (IsKeyPressed(KEY_P)){
            state = PAUSE;
        }
    }
}

void InterfaceMenu(){
    DrawText("1 - New Game", 20, 80, 20, BLACK);
    DrawText("2 - Load Game", 20, 120, 20, BLACK);
    DrawText("3 - Tutorial", 20, 160, 20, BLACK);
    DrawText("4 - Credits", 20, 200, 20, BLACK);
    DrawText("5 - Exit the game", 20, 240, 20, BLACK);
    DrawText("Castle", 420, 60, 80, BLACK);
    DrawText("of", 520, 140, 65, BLACK);
    DrawText("Stein", 460, 215, 80, BLACK);
}

void InterfaceInGame(){
    ///Interface
    DrawText(TextFormat("LEVEL: %d", level), 20, 335, 20, BLACK);
    DrawText(TextFormat("SCORE: %d", score), 130, 335, 20, BLACK);
    DrawText(TextFormat("LIFE: %d", life), 280, 335, 20, BLACK);
    DrawText(TextFormat("AMMO: %d", ammo), 390, 335, 20, BLACK);
}

void Ranking(){
    int key = GetCharPressed();
    while (key > 0){
        if ((key >= 32) && (key <= 125) && (letterCount < INPUT_CHARS))
        {
            name[letterCount] = (char)key;
            letterCount++;
        }
        key = GetCharPressed();
    }
    if (IsKeyPressed(KEY_BACKSPACE))
    {
        letterCount--;
        if (letterCount < 0) letterCount = 0;
        name[letterCount] = '\0';
    }

    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawRectangleRec(textBox, LIGHTGRAY);
    DrawRectangleLines(textBox.x, textBox.y, textBox.width, textBox.height, DARKGRAY);
    DrawText(name, textBox.x + 5, textBox.y + 8, 40, MAROON);
    DrawText(TextFormat("INPUT CHARS: %i/%i", letterCount, INPUT_CHARS), 315, 250, 20, DARKGRAY);
    EndDrawing();
}

int main(void)
{
    InitWindow(screenWidth, screenHeight, "Castle of Stein");

    LoadSprites();

    //Audio System
    InitAudioDevice();
    Sound doorWav = LoadSound("assets/door.wav");
    Sound splashWav = LoadSound("assets/splash.wav");
    Sound winWav = LoadSound("assets/yes.wav");
    Sound walkWav = LoadSound("assets/walk.wav");

    ///Definação de fps
    SetTargetFPS(60);

    while (IsKeyUp(KEY_FIVE))
    {
        switch (state){
            case MENU:
                BeginDrawing();
                ClearBackground(RAYWHITE);
                InterfaceMenu();// AQUI VAI O DRAW DA TELA DE MENU COM OPÇOES;
                InputsMenu();
                score = 0;
                EndDrawing();
                break;
            case PAUSE:
                BeginDrawing();
                ClearBackground(RAYWHITE);
                DrawText("Game paused.", 560, 335, 20, BLACK);
                DrawText("Press X to back to game.", 20, 120, 20, BLACK);
                DrawText("Press S anytime to save the game.", 20, 160, 20, BLACK);
                DrawText("Press Z to back to main menu.", 20, 200, 20, BLACK);
                DrawText("Press number 5 to exit the game.", 20, 240, 20, BLACK);
                DrawText("Castle", 420, 60, 80, BLACK);
                DrawText("of", 520, 140, 65, BLACK);
                DrawText("Stein", 460, 215, 80, BLACK);
                EndDrawing();
                if (IsKeyDown(KEY_X)){
                    state = INGAME;
                }
                if (IsKeyDown(KEY_Z)){
                    state = MENU;
                }
                if (IsKeyDown(KEY_S)){
                    DrawText("Saved game", 560, 335, 20, BLACK);
                    SaveGame();
                }
                break;
            case CREDITS:
                if (IsKeyDown(KEY_ESCAPE)) state = MENU;
                BeginDrawing();
                ClearBackground(RAYWHITE);
                DrawText("Thought and Developed by:", 240, 120, 20, BLACK);
                DrawText("Matheus Stein", 315, 150, 20, BLACK);
                DrawText("Press Z to back", 10, 335, 20, BLACK);
                if (IsKeyDown(KEY_Z)){
                    state = MENU;
                }
                EndDrawing();
                break;
            case TUTORIAL:
                if (IsKeyDown(KEY_ESCAPE)) state = MENU;
                BeginDrawing();
                ClearBackground(RAYWHITE);
                DrawText("Use the ARROW KEYS to move your character.", 20, 60, 20, BLACK);
                DrawTexture(spritePlayer, 530, 50, WHITE);
                DrawText("Use SPACEBAR to shot.", 20, 100, 20, BLACK);
                DrawTexture(spriteArrowRight, 280, 90, WHITE);
                DrawText("Use letter X to throw a knife.", 20, 140, 20, BLACK);
                DrawTexture(spriteKnifeRight, 340, 130, WHITE);
                DrawText("You need to kill all enemies to proceed to next level.", 20, 180, 20, BLACK);
                DrawTexture(enemy, 580, 170, WHITE);
                DrawText("Press letter P to pause game.", 20, 220, 20, BLACK);
                DrawText("Press letter S anytime to save your game.", 20, 260, 20, BLACK);
                DrawText("Press Z to back", 10, 335, 20, BLACK);
                if (IsKeyDown(KEY_Z)){
                    state = MENU;
                }
                EndDrawing();
                break;
            case INGAME:
                Player();
                InputsMenu();
                BeginDrawing();
                ClearBackground(RAYWHITE);
                InterfaceInGame();
                Chest();
                Enemy();
                ThrowArrow();
                ThrowKnife();
                if (IsKeyPressed(KEY_SPACE) && ammo>0 && shooting == false && knifing == false){
                    shooting = !shooting;
                    ammo-=1;
                }
                if (IsKeyDown(KEY_SPACE) && ammo <= 0){
                    startTimerAmmo = true;
                    if(startTimerAmmo == true){
                        ammoTimer += 1;
                    }
                    if(ammoTimer < 1000){
                        DrawText("You have no ammo.", 560, 335, 20, BLACK);
                        startTimerAmmo = false;
                    }
                }
                if (IsKeyPressed(KEY_X) && shooting == false && knifing == false && holdingKnife == true){
                    knifing = !knifing;
                    holdingKnife = false;
                }
                DrawTexture(spritePlayer, posX, posY, WHITE);
                DrawTexture(door, doorX, doorY, WHITE);
                EndDrawing();
                if (IsKeyDown(KEY_S)){
                    DrawText("Saved game", 560, 335, 20, BLACK);
                    SaveGame();
                }
                if(playSoundDoor == true){
                    PlaySound(doorWav);
                    playSoundDoor = false;
                }
                if(playSoundKill == true){
                    PlaySound(splashWav);
                    playSoundKill = false;
                }
                if(IsKeyPressed(KEY_UP) ||IsKeyPressed(KEY_DOWN) ||IsKeyPressed(KEY_RIGHT) ||IsKeyPressed(KEY_LEFT)){
                    PlaySound(walkWav);
                }
                break;
            case VITORIA:
                BeginDrawing();
                ClearBackground(RAYWHITE);
                DrawText("YOU WON!", 300, 80, 50, BLACK);
                DrawText(TextFormat("Score: %d", score), 350, 150, 20, BLACK);
                DrawText("Press X to play again", 300, 200, 20, BLACK);
                DrawText("Press Z to return to menu", 285, 250, 20, BLACK);
                if(IsKeyPressed(KEY_Z)){
                    state = MENU;
                }
                if(IsKeyPressed(KEY_X)){
                    fileLevel = fopen("level0.txt", "r");
                    LoadLevel(fileLevel);
                    stage = LEVEL0;
                    state = INGAME;
                    score = 0;
                    life = 3;
                    ammo = 3;
                    level = 0;
                    killCounter = 0;
                }
                EndDrawing();
                if(playSoundWin == true){
                    PlaySound(winWav);
                    playSoundWin = false;
                }
                break;
            case DERROTA:
                BeginDrawing();
                ClearBackground(RAYWHITE);
                DrawText("YOU LOSE!", 300, 80, 50, BLACK);
                DrawText(TextFormat("Score: %d", score), 350, 150, 20, BLACK);
                DrawText("Press X to play again", 300, 200, 20, BLACK);
                DrawText("Press Z to return to menu", 285, 250, 20, BLACK);
                if(IsKeyPressed(KEY_Z)){
                    state = MENU;
                }
                if(IsKeyPressed(KEY_X)){
                    fileLevel = fopen("level0.txt", "r");
                    LoadLevel(fileLevel);
                    stage = LEVEL0;
                    state = INGAME;
                    score = 0;
                    life = 3;
                    ammo = 3;
                    level = 0;
                    killCounter = 0;
                }
                EndDrawing();
                break;
            case RANKING:
                Ranking();
                break;
        }
    }
    Unload();
    UnloadSound(doorWav);
    UnloadSound(splashWav);
    UnloadSound(winWav);
    UnloadSound(walkWav);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}

bool IsAnyKeyPressed()
{
    bool keyPressed = false;
    int key = GetKeyPressed();

    if ((key >= 32) && (key <= 126)) keyPressed = true;

    return keyPressed;
}
