#include "classes.h"
#include "constants.h"
#include "globals.h"

CustomerNPC::CustomerNPC() {
    position = {NPC_RADIUS + 2, MAP_HEIGHT - TILE_SIZE * 3};
    currTarget = {MAP_WIDTH, MAP_HEIGHT - TILE_SIZE * 3};
}

void CustomerNPC::render() {
    DrawPoly(position, 3, NPC_RADIUS, 30, BLACK);
    DrawPoly(position, 3, NPC_RADIUS - 2, 30, RED);
    DrawCircle(position.x, position.y - 18, 9, BLACK);
    DrawCircle(position.x, position.y - 18, 8, {255, 229, 180, 255});
}

void CustomerNPC::update() {
    if (position.x >= MAP_WIDTH - NPC_RADIUS - 3 && position.y > MAP_HEIGHT - TILE_SIZE * 6) {
        kill = true; return;
    }

    if (state == -1) return;

    if (state == 0) {
        // fresh spawn, find chair

        for (auto& obj : objectsDining) {
            if (ChairObj* chair = dynamic_cast<ChairObj*>(obj.get())) {
                if (!chair->occupied) {
                    currObj = obj.get();
                    pathFind(chair->position);
                    chair->occupied = true;
                    chair->linkedTable->occupied++;
                    state = 1;
                    break;
                }
            }
        }
    }

    else if (state == 1) {
        // travelling to chair

        if (currPath.empty() && CheckCollisionPointRec(position, (Rectangle) {currTarget.x, currTarget.y, TILE_SIZE, TILE_SIZE})) {
            // at chair, wait for food/service

            timer = 200;
            state = 2;
        }
    }

    else if (state == 2) {
        // waiting for food/service

        timer -= 0.1f;
        if (timer < 0) {
            // time out
            if (ChairObj* chair = dynamic_cast<ChairObj*>(currObj)) {
                chair->linkedTable->occupied++;
                chair->occupied = false;
            }
            currTarget = {MAP_WIDTH, MAP_HEIGHT - TILE_SIZE * 3};
            state = -1;
        }
    }
}