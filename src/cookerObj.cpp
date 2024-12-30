#include "classes.h"
#include "constants.h"
#include "globals.h"

#include <raymath.h>
#include <iostream>

using std::vector;
using std::unique_ptr;
using std::deque;

CookerObj::CookerObj(Vector2 initPos) : KitchenObj(initPos) {
    position = initPos;
    cooked = 0;
    loaded = 0;

    balance -= 1000;
}

void CookerObj::render() {
    DrawRectangle(position.x + 2, position.y + 2, TILE_SIZE - 4, TILE_SIZE - 4, WHITE);
    DrawRectangle(position.x + 5, position.y + 12, TILE_SIZE - 10, TILE_SIZE - 17, GRAY);
    if (cooked) DrawRectangle(position.x + 5, position.y + 6, TILE_SIZE - 10, 6, BROWN);
    if (loaded) DrawRectangle(position.x + 5, position.y + 9, TILE_SIZE - 10, 3, PINK);
}

void CookerObj::update() {
    if (inQueue) return;

    if (loaded == 0 && cooked == 0) {
        // empty cooker, bring ingredients
        FridgeObj* closestFridge = nullptr;
        float dist = __FLT_MAX__;
        for (auto& obj : objectsKitchen) {
            if (FridgeObj* frg = dynamic_cast<FridgeObj*>(obj.get())) {
                float frgDist = Vector2Distance(frg->position, position);
                if (frgDist < dist) {
                    closestFridge = frg;
                    dist = frgDist;
                }
            }
        }
        if (closestFridge) {
            transportQueueKitchen.push_back(TransportJob(closestFridge, this));
            inQueue = true;
        }
    }

    if (cooked > 0) {
        // output ready, move out
        CounterObj* closestCounter = nullptr;
        float dist = __FLT_MAX__;
        for (auto& obj : objectsKitchen) {
            if (CounterObj* cnt = dynamic_cast<CounterObj*>(obj.get())) {
                float cntDist = Vector2Distance(cnt->position, position);
                if (cntDist < dist) {
                    closestCounter = cnt;
                    dist = cntDist;
                }
            }
        }
        if (closestCounter) {
            transportQueueKitchen.push_back(TransportJob(this, closestCounter));
            inQueue = true;
        }
    }
}

void CookerObj::tsptJobBegin() {
    std::cout << "COOKER: tsptJobBegin\n";
    cooked = 0;
    inQueue = false;
}

void CookerObj::tsptJobEnd() {
    std::cout << "COOKER: tsptJobEnd\n";
    loaded++;
    jobQueueKitchen.push_back(BasicJob(this));
    inQueue = false;
}

void CookerObj::basicJobEnd() {
    std::cout << "COOKER: basicJobEnd\n";
    cooked++;
    loaded = 0;
    inQueue = false;
}