#include "constants.h"
#include "classes.h"
#include "game.h"

using std::vector;
using std::unique_ptr;
using std::deque;
using std::thread;
using std::cout;

// TODO: -----------------------------------------------------

// move job assignment and handling to kitchenNPC class
// fix seg fault when deleting object in job queue
// change tile array rep of walls
// waiter, customer and workflows
// unset job if path finding fails

// globals ----------------------------------------------------

const int screenWidth = 1600;
const int screenHeight = 900;
Camera2D camera;

vector<Vector2> blocks;
short tileArray[MAP_WIDTH_TILE][MAP_HEIGHT_TILE];

vector<unique_ptr<KitchenNPC>> staffKitchen;
vector<unique_ptr<KitchenObject>> objectsKitchen;
vector<unique_ptr<DiningObject>> objectsDining;

deque<BasicJob> jobQueueKitchen;
deque<TransportJob> transportQueueKitchen;

// rendering --------------------------------------------------

void drawWorld() {
    // draw road
    DrawRectangle(0, 0, MAP_WIDTH, MAP_HEIGHT - TILE_SIZE * 7, DARKGREEN);
    DrawRectangle(0, MAP_HEIGHT - TILE_SIZE * 7, MAP_WIDTH, TILE_SIZE, GRAY);
    DrawRectangle(0, MAP_HEIGHT - TILE_SIZE * 6, MAP_WIDTH, TILE_SIZE * 6, DARKGRAY);

    // draw grid
    for (int x = TILE_SIZE; x < MAP_WIDTH; x += TILE_SIZE)
        DrawLine(x, 0, x, MAP_HEIGHT, {0, 0, 0, 40});
    for (int y = TILE_SIZE; y < MAP_HEIGHT; y += TILE_SIZE)
        DrawLine(0, y, MAP_WIDTH, y, {0, 0, 0, 40});

    for (size_t b = 0; b < blocks.size(); b++)
        DrawRectangle(blocks[b].x * TILE_SIZE, blocks[b].y * TILE_SIZE, TILE_SIZE, TILE_SIZE, DARKBROWN);

    for (auto& object : objectsKitchen) object->render();
    for (auto& object : objectsDining) object->render();
    for (auto& npc : staffKitchen) npc->renderNPC();
}

void drawMainUI(short& uiMode, int& balance, int& selectedID) {
    switch (uiMode) {
        case UI_MODE_NORMAL:
            DrawRectangleLinesEx((Rectangle) {0, 0, screenWidth, screenHeight}, 5, BLUE);
            DrawText("Normal", 7, 7, 30, BLACK);
            DrawText("Normal", 5, 5, 30, WHITE);
            break;
        case UI_MODE_BUILD:
            DrawRectangleLinesEx((Rectangle) {0, 0, screenWidth, screenHeight}, 5, YELLOW);
            DrawText("Build", 7, 7, 30, BLACK);
            DrawText("Build", 5, 5, 30, WHITE);
            break;
        case UI_MODE_DESTROY:
            DrawRectangleLinesEx((Rectangle) {0, 0, screenWidth, screenHeight}, 5, RED);
            DrawText("Destroy", 7, 7, 30, BLACK);
            DrawText("Destroy", 5, 5, 30, WHITE);
            break;
        case UI_MODE_STAFF:
            DrawRectangleLinesEx((Rectangle) {0, 0, screenWidth, screenHeight}, 5, BROWN);
            DrawText("Staff", 7, 7, 30, BLACK);
            DrawText("Staff", 5, 5, 30, WHITE);
            break;
        case UI_MODE_OBJECT:
            DrawRectangleLinesEx((Rectangle) {0, 0, screenWidth, screenHeight}, 5, PURPLE);
            DrawText("Object", 7, 7, 30, BLACK);
            DrawText("Object", 5, 5, 30, WHITE);
            DrawText(TextFormat("Selected ID: %d", selectedID), 9, 47, 30, BLACK);
            DrawText(TextFormat("Selected ID: %d", selectedID), 7, 45, 30, WHITE);
            break;
    }

    DrawFPS(10, 80);
    DrawText(TextFormat("$%d", balance), 22, screenHeight - 48, 40, BLACK);
    DrawText(TextFormat("$%d", balance), 19, screenHeight - 51, 40, WHITE);
}

// logic ------------------------------------------------------

void updateLogic() {
    for (auto& npc : staffKitchen) {
        if (ChefNPC* chef = dynamic_cast<ChefNPC*>(npc.get()))
            chef->jobUpdate();
        npc->updateNPC();
    }

    for (auto& object : objectsKitchen) object->update();

    while (!jobQueueKitchen.empty() && !jobQueueKitchen.front().active)
        jobQueueKitchen.pop_front();

    while (!transportQueueKitchen.empty() && !transportQueueKitchen.front().active)
        transportQueueKitchen.pop_front();
}

// ------------------------------------------------------------

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
    int selectedID = 0;
    int balance = 10000;

    while (!WindowShouldClose()) {
        if (IsKeyReleased(KEY_ESCAPE)) uiMode = UI_MODE_NORMAL;
        if (IsKeyReleased(KEY_B)) uiMode = UI_MODE_BUILD;
        if (IsKeyReleased(KEY_X)) uiMode = UI_MODE_DESTROY;
        if (IsKeyReleased(KEY_H)) uiMode = UI_MODE_STAFF;
        if (IsKeyReleased(KEY_O)) uiMode = UI_MODE_OBJECT;

        if (uiMode == UI_MODE_OBJECT || uiMode == UI_MODE_STAFF)
            selectionChange(uiMode, selectedID); 

        if (IsKeyDown(KEY_BACKSLASH)) balance += 7193; // temp

        camera.zoom += ((float) GetMouseWheelMove() * 0.04f);
        if (camera.zoom > 3.0f) camera.zoom = 3.0f;
        else if (camera.zoom < 0.2f) camera.zoom = 0.2f;
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
            objectPlacement(balance, selectedID);

        if (uiMode == 4 && IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
            objectDeletion(balance);

        // temp
        if (uiMode == 0 && IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) {
            Vector2 worldMousePosition = GetScreenToWorld2D(GetMousePosition(), camera);
            vector<thread> threads;
            for (size_t n = 0; n < staffKitchen.size(); n++) {
                threads.emplace_back([&, n]() {
                    staffKitchen[n]->pathFind(worldMousePosition);
                });
            }
            for (auto& t : threads) t.join();
        }

        updateLogic();

        // rendering
        BeginDrawing();
            ClearBackground(BLACK);

            BeginMode2D(camera);
                drawWorld();
            EndMode2D();

            drawMainUI(uiMode, balance, selectedID);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}