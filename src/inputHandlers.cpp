#include "constants.h"
#include "classes.h"
#include "globals.h"
#include "game.h"

using std::vector;
using std::unique_ptr;

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
        tileArray[x][y] = -1;
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

    if (tileArray[tileCoordsX][tileCoordsY] < 0) {
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
    if (balance < 0) return;
    Vector2 worldMousePosition = GetScreenToWorld2D(GetMousePosition(), camera);
    int tileCoordsX = floor(worldMousePosition.x / TILE_SIZE);
    int tileCoordsY = floor(worldMousePosition.y / TILE_SIZE);
    if (tileArray[tileCoordsX][tileCoordsY] != 0) return;
    staffKitchen.push_back(std::make_unique<ChefNPC>(worldMousePosition));
}

void npcDeletion() {
    Vector2 worldMousePosition = GetScreenToWorld2D(GetMousePosition(), camera);

    for (auto s = staffKitchen.begin(); s != staffKitchen.end();) {
        if (CheckCollisionPointCircle(worldMousePosition, (*s)->position, NPC_RADIUS)) {
            balance += 750;
            s = staffKitchen.erase(s);
        } else ++s;
    }
}

void objectPlacement(int& selectedID) {
    if (balance < 0) return;
    Vector2 worldMousePosition = GetScreenToWorld2D(GetMousePosition(), camera);
    int tileCoordsX = floor(worldMousePosition.x / TILE_SIZE);
    int tileCoordsY = floor(worldMousePosition.y / TILE_SIZE);
    if (tileArray[tileCoordsX][tileCoordsY] != 0) return;

    switch (selectedID) {
        case 0:
            objectsKitchen.push_back(
                std::make_unique<CookerObj>(
                    (Vector2) {(float) TILE_SIZE * tileCoordsX, (float) TILE_SIZE * tileCoordsY}
            ));
            break;

        case 1:
            objectsKitchen.push_back(
                std::make_unique<FridgeObj>(
                    (Vector2) {(float) TILE_SIZE * tileCoordsX, (float) TILE_SIZE * tileCoordsY}
            ));
            break;

        case 2:
            objectsKitchen.push_back(
                std::make_unique<CounterObj>(
                    (Vector2) {(float) TILE_SIZE * tileCoordsX, (float) TILE_SIZE * tileCoordsY}
            ));
            break;

        case 3:
            objectsDining.push_back(
                std::make_unique<TableObj>(
                    (Vector2) {(float) TILE_SIZE * tileCoordsX, (float) TILE_SIZE * tileCoordsY}
            ));
            break;

        case 4:
            TableObj* linkTable = nullptr;
            for (auto& o : objectsDining) {
                if (TableObj* t = dynamic_cast<TableObj*>(o.get())) {
                    if (t->chairs.size() >= 4) continue;
                    if (Vector2Distance(t->position, (Vector2) {(float) TILE_SIZE * tileCoordsX, (float) TILE_SIZE * tileCoordsY}) <= TILE_SIZE + 1) {
                        linkTable = t;
                        break;
                    }
                }
            }

            if (linkTable != nullptr) {
                objectsDining.push_back(
                    std::make_unique<ChairObj>(
                        (Vector2) {(float) TILE_SIZE * tileCoordsX, (float) TILE_SIZE * tileCoordsY}, linkTable
                ));
                linkTable->addChair(dynamic_cast<ChairObj*>(objectsDining.back().get()));
            }
            break;
    }
    // add smth to tile array
    tileArray[tileCoordsX][tileCoordsY] = 1;
}

void objectDeletion() {
    Vector2 worldMousePosition = GetScreenToWorld2D(GetMousePosition(), camera);
    int tileCoordsX = floor(worldMousePosition.x / TILE_SIZE);
    int tileCoordsY = floor(worldMousePosition.y / TILE_SIZE);
    if (tileArray[tileCoordsX][tileCoordsY] <= 0) return;
    worldMousePosition.x -= HALF_TILE_SIZE;
    worldMousePosition.y -= HALF_TILE_SIZE;
    for (auto o = objectsKitchen.begin(); o != objectsKitchen.end();) {
        if (CheckCollisionPointCircle(worldMousePosition, (*o)->position, HALF_TILE_SIZE)) {
            balance += 100;
            o = objectsKitchen.erase(o);
        } else ++o;
    }
    for (auto o = objectsDining.begin(); o != objectsDining.end();) {
        if (CheckCollisionPointCircle(worldMousePosition, (*o)->position, HALF_TILE_SIZE)) {
            balance += 100;
            o = objectsDining.erase(o);
        } else ++o;
    }
}

void selectionChange(short& uiMode, int& selectedID) {
    if (IsKeyPressed(KEY_E)) selectedID += 1;
    if (IsKeyPressed(KEY_Q)) selectedID -= 1;
    if (selectedID < 0) selectedID = 0;

    if (uiMode == UI_MODE_OBJECT) {
        if (selectedID > 4) selectedID = 4;
    }
}