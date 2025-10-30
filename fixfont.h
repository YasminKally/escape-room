#ifndef Color
    #include <raylib.h>
#endif

static Font mono = {0};

void typeText(const char* text, int screenWidth, int y, int size, Color color){
    DrawTextEx(mono, text,
        (Vector2){
            screenWidth / 2 - MeasureTextEx(mono, text, size / 2, (size / 4) / 2).x / 2,
            y
        }, size / 2 , (size / 4) / 2, color);
}
