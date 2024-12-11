#include "classes.h"
#include "constants.h"

#include <thread>
#include <raymath.h>

extern std::vector<std::unique_ptr<KitchenObject>> objectsKitchen;

ChefNPC::ChefNPC(Vector2& initPos) : StaffNPC(initPos) {
    position = initPos;
    targetSet = false;
}

void ChefNPC::renderNPC() {
    DrawPoly(position, 8, NPC_RADIUS, 5, RAYWHITE);
}

void ChefNPC::jobUpdate(short tileArray[MAP_WIDTH / TILE_SIZE][MAP_HEIGHT / TILE_SIZE]) {
    float minDist = __FLT_MAX__;
    Vector2 closest;
    for (auto& object : objectsKitchen) {
        if (Cooker* cooker = dynamic_cast<Cooker*>(object.get())) {
            if (cooker->occupied) continue;
            float dist = Vector2Distance(cooker->position, position);
            if (dist < minDist) {
                closest = cooker->position;
                minDist = dist;
            } 
        }
    }

    if (minDist != __FLT_MAX__) {
        pathFind(closest, tileArray);
        targetSet = true;
        currTarget = closest;
    }
}
