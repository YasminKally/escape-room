#ifndef Color
    #include <raylib.h>
#endif

static Font mono = {0};

void typeText(const char* text, int screenWidth, int y, int size, Color color){
    DrawTextEx(mono, text,
        (Vector2){
            screenWidth / 2 - MeasureTextEx(mono, text, size, size / 4).x / 2,
            y
        }, size, size / 4, color);
}
