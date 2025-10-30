#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <raylib.h>
#include "fixfont.h"

#define MAX_INPUT_CHARS 8

static char* happyFace[] = {
    "______________",
    "|            |",
    "| ==\\    /== |",
    "|  0      0  |",
    "|   \\____/   |",
    "==============",
    "|         oo |",
    "|            |",
};

static char* sadFace[] = {
    "______________",
    "|            |",
    "| ===    === |",
    "|  x      x  |",
    "|     /\\     |",
    "==============",
    "|         -- |",
    "|            |",
};

int main(void){
    SetConfigFlags(FLAG_FULLSCREEN_MODE);

    InitWindow(0, 0, "loading screen");
    mono = LoadFont("monofont.ttf");
    int screenWidth = GetMonitorWidth(0);
    int screenHeight = GetMonitorHeight(0);
    SetWindowSize(screenWidth, screenHeight);

    Rectangle inputBox = { screenWidth / 2 - 500, screenHeight - 80, 1000, 60 };

    // loading and timer variables
    bool loadingComplete = false;
    int percentage = 0;
    int minLeft = 0;
    int secLeft = 0;
    float totalTime = 15 * 60;
    float currentTime = 0;
    char percentageBuffer[100];
    char timeBuffer[100];

    // input box variables
    bool flip = false;
    int inputCount = 0;
    int key = 0;
    float tickTimer = 0.8;
    float tick = 0;
    char inputBuffer[MAX_INPUT_CHARS + 1] = "\0";

    // game win state
    bool solved = false;
    bool missedPassword = false;
    int attempts = 0;
    int lineCount = 0;
    float warningTimer = 0;
    float badEndingTimer = 0;
    float goodEndingTimer = 0;

    SetTargetFPS(60);
    SetExitKey(0);

    while(!WindowShouldClose()){
        // loading and timer logic
        if(!loadingComplete && !solved){
            currentTime += GetFrameTime();
            percentage = (currentTime / totalTime) * 100;
            sprintf(percentageBuffer, "%d/100%%", percentage);
        }
        if(currentTime >= totalTime){
            currentTime = totalTime;
            loadingComplete = true;
        }
        minLeft = (totalTime - currentTime) / 60;
        secLeft = (int)floor(totalTime - currentTime) % 60;
        sprintf(timeBuffer, "%.2d:%.2d", minLeft, secLeft);

        // inputBuffer box logic
        if(!missedPassword){
            key = GetCharPressed();
            tick += GetFrameTime();
        }

        if(tick >= tickTimer){
            flip = !flip;
            tick = 0;
        }

        while(key > 0){
            if((key >= 32) && (key <= 125) && (inputCount < MAX_INPUT_CHARS)){
                inputBuffer[inputCount] = toupper(key);
                inputCount++;
            }
            key = GetCharPressed();
        }

        if(IsKeyPressed(KEY_BACKSPACE) && inputCount > 0){
            inputCount--;
        }
        inputBuffer[inputCount] = '\0';

        if(IsKeyPressed(KEY_ENTER) && !missedPassword && inputCount > 0){
            if(!strcmp(inputBuffer, "29081997")){
                solved = true;
            } else {
                inputCount = 0;
                inputBuffer[0] = '\0';
                missedPassword = true;
                attempts += 1;
                currentTime += 2 * 60;
                warningTimer = 3;
            }
        }
        BeginDrawing();
            ClearBackground(BLACK);

            if ((!loadingComplete || missedPassword) && !solved) {
                // loading and timer rendering
                typeText("LOADING...", screenWidth,
                    screenHeight / 2 - 240, 80, RAYWHITE);
                typeText(percentageBuffer, screenWidth,
                    screenHeight / 2 - 150, 80, RAYWHITE);
                // inputBuffer box rendering
                if(missedPassword){
                    char errBuffer[128];
                    sprintf(errBuffer, "SENHA ERRADA. PERDEU %d MINUTOS DO TEMPO", 2);
                    typeText(errBuffer, screenWidth, screenHeight - 110, 40, RED);
                    DrawRectangle(screenWidth / 2 - 500, screenHeight / 2 - 60,
                        (currentTime / totalTime) * 1000, 80, RED);
                    DrawRectangleLines(screenWidth / 2 - 500,
                        screenHeight / 2 - 60, 1000, 80, RED);
                    typeText(timeBuffer, screenWidth,
                        screenHeight / 2 + 40, 80, RED);
                    // timer for wrong time
                    warningTimer -= GetFrameTime();
                    if(warningTimer <= 0){
                        warningTimer = 0;
                        missedPassword = false;
                    }
                } else {
                    DrawRectangle(screenWidth / 2 - 500, screenHeight / 2 - 60,
                        (currentTime / totalTime) * 1000, 80, PINK);
                    DrawRectangleLines(screenWidth / 2 - 500,
                        screenHeight / 2 - 60, 1000, 80, PINK);
                    typeText(timeBuffer, screenWidth,
                        screenHeight / 2 + 40, 80, RAYWHITE);

                    typeText(" Digite o codigo para cancelar:", screenWidth,
                        screenHeight - 160, 40, RAYWHITE);
                    DrawRectangleLines(inputBox.x, inputBox.y, inputBox.width, inputBox.height, PINK);
                    DrawTextEx(mono, TextToLower(inputBuffer),
                        (Vector2){inputBox.x + 5, inputBox.y + 12}, 40, 10, PINK);
                    // underline blink rendering
                    if(flip){
                        float length = inputBox.x + 8 + MeasureTextEx(mono, inputBuffer, 40, 10).x;
                        DrawTextEx(mono, "_", (Vector2){(int)length, inputBox.y + 8}, 40, 10, PINK);
                    }
                }
            } else if(!solved){
                badEndingTimer += GetFrameTime();
                typeText("LOADING COMPLETO", screenWidth,
                    screenHeight / 2 + 140, 40, RAYWHITE);

                int time = badEndingTimer * 1000;
                if((time / 10) % 50 == 0 && lineCount < 8) lineCount += 1;
                for(int t = 0; t < lineCount; t += 1){
                    typeText(happyFace[t], screenWidth,
                    screenHeight / 2 - 420 + 60 * t, 40, RED);
                };

                if(badEndingTimer >= 6){
                    typeText("EVA ONLINE NO MUNDO", screenWidth,
                        screenHeight - 200, 60, RED);
                }
                if(badEndingTimer >= 8){
                    typeText("VOCE PERDEU", screenWidth,
                        screenHeight - 120, 60, RED);
                }
            } else {
                goodEndingTimer += GetFrameTime();
                typeText("LOADING FALHOU", screenWidth,
                    screenHeight / 2 + 140, 40, RAYWHITE);

                if(goodEndingTimer >= 3){
                    int time = goodEndingTimer * 1000;
                    if((time / 10) % 50 == 0 && lineCount < 8) lineCount += 1;
                    for(int t = 0; t < lineCount; t += 1){
                        typeText(sadFace[t], screenWidth,
                        screenHeight / 2 - 420 + 60 * t, 40, GRAY);
                    };
                    if(goodEndingTimer >= 10){
                        typeText("UPLOAD DA EVA CANCELADO", screenWidth,
                            screenHeight - 200, 60, GREEN);
                    }
                    if(goodEndingTimer >= 12){
                        typeText("VOCE VENCEU", screenWidth,
                            screenHeight - 120, 60, GREEN);
                    }
                } else {
                    DrawRectangle(screenWidth / 2 - 500, screenHeight / 2 - 60,
                        (currentTime / totalTime) * 1000, 80, GRAY);
                    DrawRectangleLines(screenWidth / 2 - 500,
                        screenHeight / 2 - 60, 1000, 80, GRAY);
                    typeText(timeBuffer, screenWidth,
                        screenHeight / 2 + 40, 80, GRAY);
                }
            }
        EndDrawing();
    }
    CloseWindow();
    UnloadFont(mono);
    return 0;
}
