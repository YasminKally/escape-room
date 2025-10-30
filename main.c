#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <raylib.h>

#define MAX_INPUT_CHARS 8

int main(void){
    SetConfigFlags(FLAG_FULLSCREEN_MODE);

    InitWindow(0, 0, "loading screen");
    int screenWidth = GetMonitorWidth(0);
    int screenHeight = GetMonitorHeight(0);
    SetWindowSize(screenWidth, screenHeight);

    Rectangle inputBox = { screenWidth / 2 - 500, screenHeight - 50, 1000, 45 };

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
                DrawText("LOADING...",
                    screenWidth / 2 - MeasureText("LOADING...", 80) / 2,
                    screenHeight / 2 - 210, 80, RAYWHITE);
                DrawText(percentageBuffer,
                    screenWidth / 2 - MeasureText(percentageBuffer, 80) / 2,
                    screenHeight / 2 - 130, 80, RAYWHITE);
                // inputBuffer box rendering
                if(missedPassword){
                    char errBuffer[128];
                    sprintf(errBuffer, "WRONG PASSWORD. LOST %d MINUTES OF TIME", 2);
                    DrawText(errBuffer, screenWidth / 2 - MeasureText(errBuffer, 60) / 2,
                        screenHeight - 110, 60, RED);
                    DrawRectangle(screenWidth / 2 - 600, screenHeight / 2 - 40,
                        (currentTime / totalTime) * 1200, 80, RED);
                    DrawRectangleLines(screenWidth / 2 - 600,
                        screenHeight / 2 - 40, 1200, 80, RED);
                    DrawText(timeBuffer,
                        screenWidth / 2 - MeasureText(timeBuffer, 80) / 2,
                        screenHeight / 2 + 60, 80, RED);
                    // timer for wrong time
                    warningTimer -= GetFrameTime();
                    if(warningTimer <= 0){
                        warningTimer = 0;
                        missedPassword = false;
                    }
                } else {
                    DrawRectangle(screenWidth / 2 - 600, screenHeight / 2 - 40,
                        (currentTime / totalTime) * 1200, 80, PINK);
                    DrawRectangleLines(screenWidth / 2 - 600,
                        screenHeight / 2 - 40, 1200, 80, PINK);
                    DrawText(timeBuffer,
                        screenWidth / 2 - MeasureText(timeBuffer, 80) / 2,
                        screenHeight / 2 + 60, 80, RAYWHITE);
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
                DrawText("LOADING COMPLETE",
                    screenWidth / 2 - MeasureText("LOADING COMPLETE", 40) / 2,
                    screenHeight / 2 - 20, 40, RAYWHITE);
                if(badEndingTimer >= 3){
                    DrawText("EVA IS ONLINE WORLDWIDE. YOU LOST",
                        screenWidth / 2 - MeasureText("EVA IS ONLINE WORLDWIDE. YOU LOST", 60) / 2,
                        screenHeight - 180, 60, RED);
                }
            } else {
                goodEndingTimer += GetFrameTime();
                DrawText("LOADING FAILED",
                    screenWidth / 2 - MeasureText("LOADING FAILED", 40) / 2,
                    screenHeight / 2 - 120, 40, RAYWHITE);
                DrawRectangle(screenWidth / 2 - 600, screenHeight / 2 - 40,
                        (currentTime / totalTime) * 1200, 80, GRAY);
                DrawRectangleLines(screenWidth / 2 - 600,
                    screenHeight / 2 - 40, 1200, 80, GRAY);
                DrawText(timeBuffer,
                    screenWidth / 2 - MeasureText(timeBuffer, 80) / 2,
                    screenHeight / 2 + 60, 80, GRAY);
                DrawText(timeBuffer,
                        screenWidth / 2 - MeasureText(timeBuffer, 80) / 2,
                        screenHeight / 2 + 60, 80, GRAY);
                if(goodEndingTimer >= 3){
                    DrawText("EVA WAS NOT UPLOADED. YOU WON",
                        screenWidth / 2 - MeasureText("EVA WAS NOT UPLOADED. YOU WON", 60) / 2,
                        screenHeight - 180, 60, GREEN);
                }
            }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}