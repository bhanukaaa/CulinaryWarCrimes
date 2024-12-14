#include "classes.h"
#include "constants.h"

#include <raymath.h>
#include <thread>

extern std::vector<std::unique_ptr<KitchenObject>> objectsKitchen;
extern short tileArray[MAP_WIDTH_TILE][MAP_HEIGHT_TILE];

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
            currTarget.x += HALF_TILE_SIZE;
            currTarget.y += HALF_TILE_SIZE;
            currObject->occupied = true;
        }
    }
    
    if (currTarget.x != -1 && currentPath.empty() && !CheckCollisionPointRec(position, (Rectangle) {currTarget.x - HALF_TILE_SIZE, currTarget.y - HALF_TILE_SIZE, TILE_SIZE, TILE_SIZE})) {
        pathFind(currTarget);
    }
}
