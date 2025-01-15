#include "constants.h"
#include "globals.h"

#include <raylib.h>

void drawMainUI(short& uiMode, int& selectedID) {
    switch (uiMode) {
        case UI_MODE_NORMAL:
            DrawRectangleLinesEx((Rectangle) {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}, 5, BLUE);
            DrawText("Normal", 7, 7, 30, BLACK);
            DrawText("Normal", 5, 5, 30, WHITE);
            break;
        case UI_MODE_BUILD:
            DrawRectangleLinesEx((Rectangle) {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}, 5, YELLOW);
            DrawText("Build", 7, 7, 30, BLACK);
            DrawText("Build", 5, 5, 30, WHITE);
            break;
        case UI_MODE_DESTROY:
            DrawRectangleLinesEx((Rectangle) {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}, 5, RED);
            DrawText("Destroy", 7, 7, 30, BLACK);
            DrawText("Destroy", 5, 5, 30, WHITE);
            break;
        case UI_MODE_STAFF:
            DrawRectangleLinesEx((Rectangle) {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}, 5, BROWN);
            DrawText("Staff", 7, 7, 30, BLACK);
            DrawText("Staff", 5, 5, 30, WHITE);
            break;
        case UI_MODE_OBJECT:
            DrawRectangleLinesEx((Rectangle) {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}, 5, PURPLE);
            DrawText("Object", 7, 7, 30, BLACK);
            DrawText("Object", 5, 5, 30, WHITE);
            DrawText(TextFormat("Selected ID: %d", selectedID), 9, 47, 30, BLACK);
            DrawText(TextFormat("Selected ID: %d", selectedID), 7, 45, 30, WHITE);
            break;
    }

    DrawFPS(10, 80);
    DrawText(TextFormat("$%d", balance), 22, SCREEN_HEIGHT - 48, 40, BLACK);
    DrawText(TextFormat("$%d", balance), 19, SCREEN_HEIGHT - 51, 40, (balance >= 0 ? WHITE : RED));
}