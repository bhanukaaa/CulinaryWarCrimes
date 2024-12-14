#include "classes.h"
#include "constants.h"

#include <thread>
#include <raymath.h>

extern std::vector<std::unique_ptr<KitchenObject>> objectsKitchen;
extern short tileArray[MAP_WIDTH / TILE_SIZE][MAP_HEIGHT / TILE_SIZE];

ChefNPC::ChefNPC(Vector2& initPos) : StaffNPC(initPos) {
    position = initPos;
}

void ChefNPC::renderNPC() {
    DrawPoly(position, 8, NPC_RADIUS, 5, BLACK);
    DrawPoly(position, 8, NPC_RADIUS - 2, 5, RAYWHITE);
}

void ChefNPC::jobUpdate() {
    if (currTarget.x == -1) {
        float minDist = __FLT_MAX__;
        KitchenObject* closest;
        for (auto& object : objectsKitchen) {
            if (Cooker* cooker = dynamic_cast<Cooker*>(object.get())) {
                if (cooker->occupied) continue;
                float dist = Vector2Distance(cooker->position, position);
                if (dist < minDist) {
                    closest = cooker;
                    minDist = dist;
                }
            }
        }

        if (minDist != __FLT_MAX__) {
            currTarget = closest->position;
            currObject = closest;
            currTarget.x += TILE_SIZE / 2;
            currTarget.y += TILE_SIZE / 2;

            currObject->occupied = true;
        }
    }
    
    if (currTarget.x != -1 && currentPath.empty() && !CheckCollisionPointRec(position, (Rectangle) {currTarget.x - TILE_SIZE / 2, currTarget.y - TILE_SIZE / 2, TILE_SIZE, TILE_SIZE})) {
        pathFind(currTarget);
    }
}
