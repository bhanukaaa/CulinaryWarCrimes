#include "constants.h"
#include "classes.h"
#include "game.h"

using std::vector;
using std::unique_ptr;

extern Camera2D camera;
extern vector<Vector2> blocks;
extern short tileArray[MAP_WIDTH_TILE][MAP_HEIGHT_TILE];
extern vector<unique_ptr<KitchenNPC>> staffKitchen;
extern vector<unique_ptr<KitchenObject>> objectsKitchen;

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
    staffKitchen.push_back(std::make_unique<ChefNPC>(worldMousePosition));
}

void npcDeletion(int& balance) {
    Vector2 worldMousePosition = GetScreenToWorld2D(GetMousePosition(), camera);
    for (auto s = staffKitchen.begin(); s != staffKitchen.end();) {
        if (CheckCollisionPointCircle(worldMousePosition, (*s)->position, NPC_RADIUS)) {
            balance += 750;
            s = staffKitchen.erase(s);
        } else ++s;
    }
}

void objectPlacement(int& balance, int& selectedID) {
    Vector2 worldMousePosition = GetScreenToWorld2D(GetMousePosition(), camera);
    int tileCoordsX = floor(worldMousePosition.x / TILE_SIZE);
    int tileCoordsY = floor(worldMousePosition.y / TILE_SIZE);

    switch (selectedID) {
        case 0:
            objectsKitchen.push_back(std::make_unique<CookerObject>((Vector2) {(float) TILE_SIZE * tileCoordsX, (float) TILE_SIZE * tileCoordsY}));
            balance -= 750;
            break;
        case 1:
            objectsKitchen.push_back(std::make_unique<FridgeObject>((Vector2) {(float) TILE_SIZE * tileCoordsX, (float) TILE_SIZE * tileCoordsY}));
            balance -= 1200;
            break;
        case 2:
            objectsKitchen.push_back(std::make_unique<CounterObject>((Vector2) {(float) TILE_SIZE * tileCoordsX, (float) TILE_SIZE * tileCoordsY}));
            balance -= 600;
            break;
    }
}

void objectDeletion(int& balance) {
    Vector2 worldMousePosition = GetScreenToWorld2D(GetMousePosition(), camera);
    worldMousePosition.x -= HALF_TILE_SIZE;
    worldMousePosition.y -= HALF_TILE_SIZE;
    for (auto o = objectsKitchen.begin(); o != objectsKitchen.end();) {
        if (CheckCollisionPointCircle(worldMousePosition, (*o)->position, HALF_TILE_SIZE)) {
            balance += 100;
            o = objectsKitchen.erase(o);
        } else ++o;
    }
}

void selectionChange(short& uiMode, int& selectedID) {
    if (IsKeyPressed(KEY_E)) selectedID += 1;
    if (IsKeyPressed(KEY_Q)) selectedID -= 1;
    if (selectedID < 0) selectedID = 0;

    if (uiMode == UI_MODE_OBJECT) {
        if (selectedID > 2) selectedID = 2;
    }
}