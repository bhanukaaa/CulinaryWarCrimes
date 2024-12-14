#include "constants.h"
#include "classes.h"

#include <raylib.h>
#include <raymath.h>
#include <vector>
#include <iostream>
#include <thread>

const int screenWidth = 1600;
const int screenHeight = 900;

Camera2D camera;
std::vector<Vector2> blocks;
std::vector<std::unique_ptr<StaffNPC>> staff;
std::vector<std::unique_ptr<KitchenObject>> objectsKitchen;
short tileArray[MAP_WIDTH_TILE][MAP_HEIGHT_TILE];

void cameraMovement(short camMovX, short camMovY) {
    camera.target.x += camMovX * 15;
    camera.target.y += camMovY * 15;

    if (camera.target.x < 0) camera.target.x = 0;
    if (camera.target.y < 0) camera.target.y = 0;
    if (camera.target.x > MAP_WIDTH) camera.target.x = MAP_WIDTH;
    if (camera.target.y > MAP_HEIGHT) camera.target.y = MAP_HEIGHT;
}

void place(short x, short y) {
    if (tileArray[x][y] == 0) {
        blocks.push_back((Vector2) {(float) x, (float) y});
        tileArray[x][y] = 1;
    }
}

void blockPlacement() {
    Vector2 worldMousePosition = GetScreenToWorld2D(GetMousePosition(), camera);
    int tileCoordsX = floor(worldMousePosition.x / TILE_SIZE);
    int tileCoordsY = floor(worldMousePosition.y / TILE_SIZE);

    place(tileCoordsX, tileCoordsY);
}

void boxPlacement() {
    static Vector2 first = {-1, -1};

    if (first.x == -1) first = GetScreenToWorld2D(GetMousePosition(), camera);
    else {
        Vector2 second = GetScreenToWorld2D(GetMousePosition(), camera);
        int tileCoordsXF = floor(first.x / TILE_SIZE);
        int tileCoordsYF = floor(first.y / TILE_SIZE);
        int tileCoordsXS = floor(second.x / TILE_SIZE);
        int tileCoordsYS = floor(second.y / TILE_SIZE);
        first = {-1, -1};

        for (int x = std::min(tileCoordsXF, tileCoordsXS); x <= std::max(tileCoordsXF, tileCoordsXS); x++) {
            place(x, tileCoordsYF);
            place(x, tileCoordsYS);
        }
        for (int y = std::min(tileCoordsYF, tileCoordsYS); y <= std::max(tileCoordsYF, tileCoordsYS); y++) {
            place(tileCoordsXF, y);
            place(tileCoordsXS, y);
        }
    }
}

void blockDeletion() {
    Vector2 worldMousePosition = GetScreenToWorld2D(GetMousePosition(), camera);
    int tileCoordsX = floor(worldMousePosition.x / TILE_SIZE);
    int tileCoordsY = floor(worldMousePosition.y / TILE_SIZE);

    if (tileArray[tileCoordsX][tileCoordsY] == 1) {
        tileArray[tileCoordsX][tileCoordsY] = 0;
        short eraseIndex = -1;
        for (size_t b = 0; b < blocks.size(); b++) {
            if (blocks[b].x == tileCoordsX && blocks[b].y == tileCoordsY) {
                eraseIndex = b;
                break;
            }
        }
        blocks.erase(std::next(blocks.begin(), eraseIndex));
    }
}

void npcPlacement() {
    Vector2 worldMousePosition = GetScreenToWorld2D(GetMousePosition(), camera);
    staff.push_back(std::make_unique<StaffNPC>(worldMousePosition));
    staff.push_back(std::make_unique<ChefNPC>(worldMousePosition));
}

void npcDeletion(int& balance) {
    Vector2 worldMousePosition = GetScreenToWorld2D(GetMousePosition(), camera);
    for (auto& npc : staff) {
        if (CheckCollisionPointCircle(worldMousePosition, npc->position, NPC_RADIUS)) {
            npc->kill = true;
            balance += 750;
        }
    }
}

void objectPlacement(int& balance) {
    Vector2 worldMousePosition = GetScreenToWorld2D(GetMousePosition(), camera);
    int tileCoordsX = floor(worldMousePosition.x / TILE_SIZE);
    int tileCoordsY = floor(worldMousePosition.y / TILE_SIZE);

    objectsKitchen.push_back(std::make_unique<Cooker>((Vector2) {(float) TILE_SIZE * tileCoordsX + 2, (float) TILE_SIZE * tileCoordsY + 2}));
    balance -= 750;
}

void drawMainUI(short& uiMode, int& balance) {
    switch (uiMode) {
        case 0:
            DrawRectangleLinesEx((Rectangle) {0, 0, screenWidth, screenHeight}, 5, BLUE);
            break;
        case 1:
            DrawRectangleLinesEx((Rectangle) {0, 0, screenWidth, screenHeight}, 5, YELLOW);
            break;
        case 2:
            DrawRectangleLinesEx((Rectangle) {0, 0, screenWidth, screenHeight}, 5, RED);
            break;
        case 3:
            DrawRectangleLinesEx((Rectangle) {0, 0, screenWidth, screenHeight}, 5, BROWN);
            break;
        case 4:
            DrawRectangleLinesEx((Rectangle) {0, 0, screenWidth, screenHeight}, 5, PURPLE);
            break;
    }

    DrawFPS(10, 10);
    DrawText(TextFormat("$%d", balance), 20, screenHeight - 50, 40, WHITE);
}

void drawWorld() {
    DrawRectangle(0, 0, MAP_WIDTH, MAP_HEIGHT - TILE_SIZE * 7, DARKGREEN);
    DrawRectangle(0, MAP_HEIGHT - TILE_SIZE * 7, MAP_WIDTH, TILE_SIZE, GRAY);
    DrawRectangle(0, MAP_HEIGHT - TILE_SIZE * 6, MAP_WIDTH, TILE_SIZE * 6, DARKGRAY);

    for (int y = 0; y < MAP_HEIGHT / TILE_SIZE; y++) // grid
        for (int x = 0; x < MAP_WIDTH / TILE_SIZE; x++)
            DrawRectangleLines(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, {0, 0, 0, 25});

    for (size_t b = 0; b < blocks.size(); b++)
        DrawRectangle(blocks[b].x * TILE_SIZE, blocks[b].y * TILE_SIZE, TILE_SIZE, TILE_SIZE, DARKBROWN);

    for (auto& object : objectsKitchen) {
        object->render();
    }

    for (auto& npc : staff) {
        npc->renderNPC();
        if (ChefNPC* chef = dynamic_cast<ChefNPC*>(npc.get())) {
            chef->jobUpdate();
        }
    }
}

int main(void) {
    InitWindow(screenWidth, screenHeight, "Culinary War Crimes");
    SetExitKey(KEY_NULL);
    SetTargetFPS(60);

    short camMovX, camMovY;
    camera = {0};
    camera.target = (Vector2) {MAP_WIDTH / 2, MAP_HEIGHT - 450};
    camera.offset = (Vector2) {screenWidth / 2.0f, screenHeight / 2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    for (int x = 0; x < MAP_WIDTH_TILE; x++)
        for (int y = 0; y < MAP_HEIGHT_TILE; y++)
            tileArray[x][y] = 0;

    short uiMode = 0;
    int balance = 10000;

    while (!WindowShouldClose()) {
        if (IsKeyReleased(KEY_ESCAPE)) uiMode = 0;
        if (IsKeyReleased(KEY_B)) uiMode = 1;
        if (IsKeyReleased(KEY_X)) uiMode = 2;
        if (IsKeyReleased(KEY_H)) uiMode = 3;
        if (IsKeyReleased(KEY_O)) uiMode = 4;

        // temp
        if (IsKeyDown(KEY_BACKSLASH)) balance += 7193;

        // updates
        camera.zoom += ((float) GetMouseWheelMove() * 0.02f);
        if (camera.zoom > 3.0f) camera.zoom = 3.0f;
        else if (camera.zoom < 0.3f) camera.zoom = 0.3f;
        camMovX = camMovY = 0;
        if (IsKeyDown(KEY_W)) camMovY -= 1;
        if (IsKeyDown(KEY_A)) camMovX -= 1;
        if (IsKeyDown(KEY_S)) camMovY += 1;
        if (IsKeyDown(KEY_D)) camMovX += 1;
        cameraMovement(camMovX, camMovY);

        if (uiMode == 1 && IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
            blockPlacement();

        if (uiMode == 1 && IsMouseButtonReleased(MOUSE_BUTTON_RIGHT))
            boxPlacement();

        if (uiMode == 2 && IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
            blockDeletion();

        if (uiMode == 3 && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            if (balance >= 1000) {
                npcPlacement();
                balance -= 1000;
            }
        }

        if (uiMode == 3 && IsMouseButtonReleased(MOUSE_BUTTON_RIGHT))
            npcDeletion(balance);

        if (uiMode == 4 && IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
            objectPlacement(balance);

        // temp
        if (uiMode == 0 && IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) {
            Vector2 worldMousePosition = GetScreenToWorld2D(GetMousePosition(), camera);
            std::vector<std::thread> threads;
            for (size_t n = 0; n < staff.size(); n++) {
                threads.emplace_back([&, n]() {
                    staff[n]->pathFind(worldMousePosition);
                });
            }
            for (auto& t : threads) t.join();
        }

        for (auto& npc : staff) {
            int tileCoordsX = floor(npc->position.x / TILE_SIZE);
            int tileCoordsY = floor(npc->position.y / TILE_SIZE);
            std::vector<Vector2> surrounding;
            for (int xOff = -1; xOff < 2; xOff++) {
                for (int yOff = -1; yOff < 2; yOff++) {
                    if (xOff == 0 && yOff == 0) continue;
                    if (tileArray[tileCoordsX + xOff][tileCoordsY + yOff] != 0)
                        surrounding.push_back((Vector2) {(float) tileCoordsX + xOff, (float) tileCoordsY + yOff});
                }
            }
            npc->updateNPC(surrounding);
        }

        // rendering
        BeginDrawing();
            ClearBackground(BLACK);

            BeginMode2D(camera);
                drawWorld();
            EndMode2D();

            drawMainUI(uiMode, balance);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}