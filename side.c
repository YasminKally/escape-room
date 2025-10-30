#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <raylib.h>
#include "fixfont.h"

#define MAX_INPUT_CHARS 8

static const char* passwords[] = {"EVA1973", "CONTROL5", "KEY3A9"};

int main(int argc, char** argv){
    // filter CLI boot setting
    if(argc < 2){
        printf("missing boot setting. aborting...");
        getchar();
        return 1;
    }
    int correctPassword = atoi(argv[1]);
    if(correctPassword > 2 || correctPassword < 1){
        printf("invalid password index. aborting...");
        getchar();
        return 1;
    }
    SetConfigFlags(FLAG_FULLSCREEN_MODE);

    char title[128];
    sprintf(title, "upload server #%d", correctPassword);
    correctPassword -= 1;

    InitWindow(0, 0, title);
    mono = LoadFont("monofont.ttf");
    int screenWidth = GetMonitorWidth(0);
    int screenHeight = GetMonitorHeight(0);
    SetWindowSize(screenWidth, screenHeight);

    Rectangle inputBox = { screenWidth / 2 - 500, screenHeight - 80, 1000, 60 };

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
    float warningTimer = 0;
    float badEndingTimer = 0;
    float goodEndingTimer = 0;

    SetTargetFPS(60);
    SetExitKey(0);

    while(!WindowShouldClose()){
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
            if(!strcmp(inputBuffer, passwords[correctPassword])){
                solved = true;
            } else {
                inputCount = 0;
                inputBuffer[0] = '\0';
                missedPassword = true;
                attempts += 1;
                warningTimer = 3;
            }
        }
        BeginDrawing();
            ClearBackground(BLACK);

            if ((attempts < 4 || missedPassword) && !solved) {
                // loading and timer rendering
                typeText(title, screenWidth,
                    screenHeight / 2 - 230, 80, RAYWHITE);
                char triesBuffer[128];
                sprintf(triesBuffer, "attemps left: %d", 4 - attempts);
                // inputBuffer box rendering
                if(missedPassword){
                    typeText(triesBuffer, screenWidth,
                        screenHeight / 2 - 120, 80, RED);
                     typeText("wrong password", screenWidth,
                        screenHeight - 110, 60, RED);
                    // timer for wrong time
                    warningTimer -= GetFrameTime();
                    if(warningTimer <= 0){
                        warningTimer = 0;
                        missedPassword = false;
                    }
                } else {
                    typeText(triesBuffer, screenWidth,
                        screenHeight / 2 - 120, 80, RAYWHITE);

                    typeText("Enter the code to cancel:", screenWidth,
                        screenHeight - 160, 60, RAYWHITE);
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
                typeText("CODE NOT CRACKED. SERVER FINISHED UPLOAD", screenWidth,
                    screenHeight / 2 - 20, 40, RED);
                if(badEndingTimer >= 3){
                    switch(correctPassword){
                    case 0:
                        typeText("1st final password digits locked. good luck", screenWidth,
                            screenHeight - 180, 60, RED);
                        break;
                    case 1:
                        typeText("2nd final password digits locked. good luck", screenWidth,
                            screenHeight - 180, 60, RED);
                        break;
                    case 2:
                        typeText("3rd final password digits locked. good luck", screenWidth,
                            screenHeight - 180, 60, RED);
                        break;
                    case 3:
                        typeText("4th final password digits locked. good luck", screenWidth,
                            screenHeight - 180, 60, RED);
                        break;
                    }
                }
            } else {
                typeText("CODE CRACKED. SERVER UPLOAD CANCELLED", screenWidth,
                    screenHeight / 2 - 20, 40, RAYWHITE);
                goodEndingTimer += GetFrameTime();
                if(goodEndingTimer >= 3){
                    switch(correctPassword){
                    case 0:
                        typeText("1st final password digits are 29", screenWidth,
                            screenHeight - 180, 60, GREEN);
                        break;
                    case 1:
                        typeText("1st final password digits are 08", screenWidth,
                            screenHeight - 180, 60, GREEN);
                        break;
                    case 2:
                        typeText("1st final password digits are 19", screenWidth,
                            screenHeight - 180, 60, GREEN);
                        break;
                    case 3:
                        typeText("1st final password digits are 97", screenWidth,
                            screenHeight - 180, 60, GREEN);
                        break;
                    }
                }
            }
        EndDrawing();
    }
    CloseWindow();
    UnloadFont(mono);
    return 0;
}