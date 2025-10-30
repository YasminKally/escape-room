#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <raylib.h>

#define MAX_INPUT_CHARS 8

static const char* passwords[] = {"EVA1973", "CONTROL5", "SHUTD0NW", "KEY3A9"};

int main(int argc, char** argv){
    // filter CLI boot setting
    if(argc < 2){
        printf("missing boot setting. aborting...");
        getchar();
        return 1;
    }
    int correctPassword = atoi(argv[1]);
    if(correctPassword > 3 || correctPassword < 1){
        printf("invalid password index. aborting...");
        getchar();
        return 1;
    }
    SetConfigFlags(FLAG_FULLSCREEN_MODE);

    char title[128];
    sprintf(title, "upload server #%d", correctPassword);
    correctPassword -= 1;

    InitWindow(0, 0, title);
    int screenWidth = GetMonitorWidth(0);
    int screenHeight = GetMonitorHeight(0);
    SetWindowSize(screenWidth, screenHeight);

    Rectangle inputBox = { screenWidth / 2 - 500, screenHeight - 50, 1000, 45 };

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
                DrawText(title,
                    screenWidth / 2 - MeasureText(title, 80) / 2,
                    screenHeight / 2 - 230, 80, RAYWHITE);
                char triesBuffer[128];
                sprintf(triesBuffer, "attemps left: %d", 4 - attempts);
                // inputBuffer box rendering
                if(missedPassword){
                    DrawText(triesBuffer,
                        screenWidth / 2 - MeasureText(triesBuffer, 80) / 2,
                        screenHeight / 2 - 120, 80, RED);
                     DrawText("wrong password",
                        screenWidth / 2 - MeasureText("wrong password", 60) / 2,
                        screenHeight - 110, 60, RED);
                    // timer for wrong time
                    warningTimer -= GetFrameTime();
                    if(warningTimer <= 0){
                        warningTimer = 0;
                        missedPassword = false;
                    }
                } else {
                    DrawText(triesBuffer,
                        screenWidth / 2 - MeasureText(triesBuffer, 80) / 2,
                        screenHeight / 2 - 120, 80, RAYWHITE);
                    DrawText("Enter the code to cancel:",
                        screenWidth / 2 - MeasureText("Enter the code to cancel:", 60) / 2,
                        screenHeight - 110, 60, RAYWHITE);
                    DrawRectangleLines(inputBox.x, inputBox.y, inputBox.width, inputBox.height, PINK);
                    DrawText(TextToLower(inputBuffer), inputBox.x + 5, inputBox.y + 4, 40, PINK);
                    // underline blink rendering
                    if(flip){
                        DrawText("_", inputBox.x + 8 + MeasureText(inputBuffer, 40), inputBox.y + 8, 40, PINK);
                    }
                }
            } else if(!solved){
                badEndingTimer += GetFrameTime();
                DrawText("CODE NOT CRACKED. SERVER FINISHED UPLOAD",
                    screenWidth / 2 - MeasureText("CODE NOT CRACKED. SERVER FINISHED UPLOAD", 40) / 2,
                    screenHeight / 2 - 20, 40, RED);
                if(badEndingTimer >= 3){
                    switch(correctPassword){
                    case 0:
                        DrawText("1st final password digits locked. good luck",
                            screenWidth / 2 - MeasureText("1st final password digits locked. good luck", 60) / 2,
                            screenHeight - 180, 60, RED);
                        break;
                    case 1:
                        DrawText("2nd final password digits locked. good luck",
                            screenWidth / 2 - MeasureText("2nd final password digits locked. good luck", 60) / 2,
                            screenHeight - 180, 60, RED);
                        break;
                    case 2:
                        DrawText("3rd final password digits locked. good luck",
                            screenWidth / 2 - MeasureText("3rd final password digits locked. good luck", 60) / 2,
                            screenHeight - 180, 60, RED);
                        break;
                    case 3:
                        DrawText("4th final password digits locked. good luck",
                            screenWidth / 2 - MeasureText("4th final password digits locked. good luck", 60) / 2,
                            screenHeight - 180, 60, RED);
                        break;
                    }
                }
            } else {
                DrawText("CODE CRACKED. SERVER UPLOAD CANCELLED",
                    screenWidth / 2 - MeasureText("CODE CRACKED. SERVER UPLOAD CANCELLED", 40) / 2,
                    screenHeight / 2 - 20, 40, RAYWHITE);
                goodEndingTimer += GetFrameTime();
                if(goodEndingTimer >= 3){
                    switch(correctPassword){
                    case 0:
                        DrawText("1st final password digits are 29",
                            screenWidth / 2 - MeasureText("1st final password digits are 29", 60) / 2,
                            screenHeight - 180, 60, GREEN);
                        break;
                    case 1:
                        DrawText("1st final password digits are 08",
                            screenWidth / 2 - MeasureText("1st final password digits are 08", 60) / 2,
                            screenHeight - 180, 60, GREEN);
                        break;
                    case 2:
                        DrawText("1st final password digits are 19",
                            screenWidth / 2 - MeasureText("1st final password digits are 19", 60) / 2,
                            screenHeight - 180, 60, GREEN);
                        break;
                    case 3:
                        DrawText("1st final password digits are 97",
                            screenWidth / 2 - MeasureText("1st final password digits are 97", 60) / 2,
                            screenHeight - 180, 60, GREEN);
                        break;
                    }
                }
            }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}