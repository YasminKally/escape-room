#include <stdio.h>
#include <raylib.h>

int main(void){
    SetConfigFlags(FLAG_FULLSCREEN_MODE);

    const int SCREENWIDTH = 800;
    const int SCREENHEIGHT = 450;

    InitWindow(SCREENWIDTH, SCREENHEIGHT, "loading screen");

    bool loadingComplete = false;
    float totalTime = 0.1 * 60;
    float currentTime = 0;

    SetTargetFPS(60);

    while(!WindowShouldClose()){

        if(!loadingComplete){
            currentTime += GetFrameTime();
            if(currentTime >= totalTime){
                loadingComplete = true;
            }
        }

        BeginDrawing();
            ClearBackground(BLACK);

            if (!loadingComplete) {
                DrawText("LOADING...", SCREENWIDTH / 2 - MeasureText("LOADING...", 40) / 2, SCREENHEIGHT / 2 - 20, 40, DARKGRAY);
                DrawRectangle(SCREENWIDTH / 2 - 300, SCREENHEIGHT / 2 + 40, (currentTime / totalTime) * 600, 40, PINK);
                DrawRectangleLines(SCREENWIDTH / 2 - 300, SCREENHEIGHT / 2 + 40, 600, 40, PINK);
            } else {
                DrawText("LOADING COMPLETE", SCREENWIDTH / 2 - MeasureText("LOADING COMPLETE", 40) / 2, SCREENHEIGHT / 2 - 20, 40, RAYWHITE);
            }
        EndDrawing();
    }

    return 0;
}