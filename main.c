#include <stdio.h>
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

    Rectangle inputBox = { screenWidth / 2 - 500, screenHeight - 50, 1000, 40 };

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
    
    SetTargetFPS(60);

    while(!WindowShouldClose()){

        // loading nad timer logic
        if(!loadingComplete){
            currentTime += GetFrameTime();
            percentage = (currentTime / totalTime) * 100;
            sprintf(percentageBuffer, "%d/100%%", percentage);

            minLeft = (totalTime - currentTime) / 60;
            secLeft = (int)floor(totalTime - currentTime) % 60;
            sprintf(timeBuffer, "%.2d:%.2d", minLeft, secLeft);
            
            if(currentTime >= totalTime){
                loadingComplete = true;
            }
        }

        // inputBuffer box logic
        key = GetCharPressed();
        tick += GetFrameTime();

        if(tick >= tickTimer){
            flip = !flip;
            tick = 0;
        }

        while(key > 0){
            if((key >= 32) && (key <= 125) && (inputCount < MAX_INPUT_CHARS)){
                inputBuffer[inputCount] = (char)key;
                inputCount++;
            }

            key = GetCharPressed();
        }

        if(IsKeyPressed(KEY_BACKSPACE) && inputCount > 0){
            inputCount--;
            inputBuffer[inputCount] = '\0';
        }

        inputBuffer[inputCount] = '\0';

        BeginDrawing();
            ClearBackground(BLACK);

            if (!loadingComplete) {
                // loading and timer rendering
                DrawText("LOADING...", screenWidth / 2 - MeasureText("LOADING...", 80) / 2, screenHeight / 2 - 210, 80, RAYWHITE);
                DrawText(percentageBuffer, screenWidth / 2 - MeasureText(percentageBuffer, 80) / 2, screenHeight / 2 - 130, 80, RAYWHITE);
                DrawRectangle(screenWidth / 2 - 600, screenHeight / 2 - 40, (currentTime / totalTime) * 1200, 80, PINK);
                DrawRectangleLines(screenWidth / 2 - 600, screenHeight / 2 - 40, 1200, 80, PINK);
                DrawText(timeBuffer, screenWidth / 2 - MeasureText(timeBuffer, 80) / 2, screenHeight / 2 + 60, 80, RAYWHITE);

                // inputBuffer box rendering
                DrawText("Enter the code to cancel:", screenWidth / 2 - MeasureText("Enter the code to cancel:", 60) / 2, screenHeight - 110, 60, RAYWHITE);
                DrawRectangleLines(inputBox.x, inputBox.y, inputBox.width, inputBox.height, PINK);
                DrawText(TextToLower(inputBuffer), inputBox.x + 5, inputBox.y - 4, 40, PINK);
                // underline blink rendering
                if(flip){
                    DrawText("_", inputBox.x + 8 + MeasureText(inputBuffer, 40), inputBox.y + 4, 40, PINK);
                }

            } else {
                DrawText("LOADING COMPLETE", screenWidth / 2 - MeasureText("LOADING COMPLETE", 40) / 2, screenHeight / 2 - 20, 40, RAYWHITE);
            }
        EndDrawing();
    }

    return 0;
}