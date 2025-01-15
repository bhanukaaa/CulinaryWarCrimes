#include "constants.h"
#include "classes.h"
#include "game.h"

using std::vector;
using std::unique_ptr;
using std::deque;
using std::thread;
using std::cout;

// TODO: ------------------------------------------------------

// fix seg fault when deleting object in job queue
// move job assignment and handling to kitchenNPC class
// add static member functions to render previews
// waiter, customer and workflows
// unset job if path finding fails
// split src into subfolders
// move balance updaters upon place/delete in constructor/destructor

// globals ----------------------------------------------------

Camera2D camera;

vector<Vector2> blocks;
short tileArray[MAP_WIDTH_TILE][MAP_HEIGHT_TILE];

vector<unique_ptr<KitchenNPC>> staffKitchen;
vector<unique_ptr<KitchenObj>> objectsKitchen;
vector<unique_ptr<DiningObj>> objectsDining;
vector<unique_ptr<CustomerNPC>> customers;

deque<BasicJob> jobQueueKitchen;
deque<TransportJob> transportQueueKitchen;

int balance;

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

    for (auto& block : blocks)
        DrawRectangle(block.x * TILE_SIZE, block.y * TILE_SIZE, TILE_SIZE, TILE_SIZE, DARKBROWN);

    for (auto& object : objectsKitchen) object->render();
    for (auto& object : objectsDining) object->render();
    for (auto& npc : staffKitchen) npc->render();
    for (auto& cust : customers) cust->render();
}

// logic ------------------------------------------------------

void updateLogic() {
    for (auto& npc : staffKitchen) {
        if (ChefNPC* chef = dynamic_cast<ChefNPC*>(npc.get()))
            chef->jobUpdate();
        npc->updateNPC();
    }

    if (customers.size() < MAX_CUSTOMERS) {
        if (GetRandomValue(0, 500) == 7)
            customers.push_back(std::make_unique<CustomerNPC>());
    }

    for (auto c = customers.begin(); c != customers.end();) {
        if (c->get()->kill)
            c = customers.erase(c);
        else {
            c->get()->update();
            c->get()->updateNPC();
            c++;
        }
    }

    for (auto& object : objectsKitchen) object->update();

    while (!jobQueueKitchen.empty() && !jobQueueKitchen.front().active)
        jobQueueKitchen.pop_front();

    while (!transportQueueKitchen.empty() && !transportQueueKitchen.front().active)
        transportQueueKitchen.pop_front();
}

// ------------------------------------------------------------

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Culinary War Crimes");
    SetExitKey(KEY_NULL);
    SetTargetFPS(60);

    short camMovX, camMovY;
    camera = {0};
    camera.target = (Vector2) {MAP_WIDTH / 2, MAP_HEIGHT - 450};
    camera.offset = (Vector2) {SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    for (int x = 0; x < MAP_WIDTH_TILE; x++)
        for (int y = 0; y < MAP_HEIGHT_TILE; y++)
            tileArray[x][y] = 0;

    short uiMode = 0;
    int selectedID = 0;
    balance = 10000;

    while (!WindowShouldClose()) {
        if (IsKeyReleased(KEY_ESCAPE)) uiMode = UI_MODE_NORMAL;
        if (IsKeyReleased(KEY_B)) uiMode = UI_MODE_BUILD;
        if (IsKeyReleased(KEY_X)) uiMode = UI_MODE_DESTROY;
        if (IsKeyReleased(KEY_H)) uiMode = UI_MODE_STAFF;
        if (IsKeyReleased(KEY_O)) uiMode = UI_MODE_OBJECT;

        if (uiMode == UI_MODE_OBJECT || uiMode == UI_MODE_STAFF)
            selectionChange(uiMode, selectedID); 

        if (IsKeyDown(KEY_BACKSLASH)) balance += 7193; // TEMP

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

        if (uiMode == 3 && IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
            npcPlacement();

        if (uiMode == 3 && IsMouseButtonReleased(MOUSE_BUTTON_RIGHT))
            npcDeletion();

        if (uiMode == 4 && IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
            objectPlacement(selectedID);

        if (uiMode == 4 && IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
            objectDeletion();

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

            drawMainUI(uiMode, selectedID);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}