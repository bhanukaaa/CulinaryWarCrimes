#include "classes.h"
#include "constants.h"

#include <raymath.h>
#include <thread>
#include <queue>

using std::vector;
using std::unique_ptr;
using std::deque;

extern vector<unique_ptr<KitchenObject>> objectsKitchen;
extern short tileArray[MAP_WIDTH_TILE][MAP_HEIGHT_TILE];
extern deque<JobKitchen> jobQueueKitchen;

ChefNPC::ChefNPC(Vector2& initPos) : StaffNPC(initPos) {
    position = initPos;
}

void ChefNPC::renderNPC() {
    DrawPoly(position, 8, NPC_RADIUS, 5, BLACK);
    DrawPoly(position, 8, NPC_RADIUS - 2, 5, RAYWHITE);
}

void ChefNPC::jobUpdate() {
    if (currTarget.x == -1) {
        for (auto& job : jobQueueKitchen) {
            if (!job.active) continue;
            currObject = job.object;
            currTarget = job.position;
            currTarget.x += HALF_TILE_SIZE;
            currTarget.y += HALF_TILE_SIZE;
            currObject->occupied = true;
            job.active = false;
            break;
        }
    }

    if (currTarget.x != -1 && currentPath.empty()) {
        if (!CheckCollisionPointRec(position, (Rectangle) {currTarget.x - HALF_TILE_SIZE, currTarget.y - HALF_TILE_SIZE, TILE_SIZE, TILE_SIZE})) {
            pathFind(currTarget);
        } else {
            currObject->progress -= 0.5;
        }
    }
}
