#include "classes.h"
#include "constants.h"

#include <iostream>
#include <vector>
#include <thread>
#include <queue>

using std::vector;
using std::unique_ptr;
using std::deque;

extern vector<unique_ptr<KitchenObject>> objectsKitchen;
extern deque<KitchenJob> jobQueueKitchen;
extern deque<TransportJob> transportQueueKitchen;

CookerObject::CookerObject(Vector2 initPos) : KitchenObject(initPos) {
    position = initPos;
    progress = 100;
    cooked = 0;
    loaded = 0;
}

void CookerObject::render() {
    DrawRectangle(position.x + 2, position.y + 2, TILE_SIZE - 4, TILE_SIZE - 4, WHITE);
    DrawRectangle(position.x + 5, position.y + 12, TILE_SIZE - 10, TILE_SIZE - 17, GRAY);
    if (cooked) DrawRectangle(position.x + 5, position.y + 6, TILE_SIZE - 10, 6, BROWN);
    if (loaded) DrawRectangle(position.x + 5, position.y + 9, TILE_SIZE - 10, 3, PINK);
}

void CookerObject::update() {
    if (inQueue) return;

    if (loaded == 0 && cooked == 0) {
        // empty cooker, bring ingredients
        FridgeObject* closestFridge = nullptr;
        for (auto& obj : objectsKitchen) {
            if (FridgeObject* frg = dynamic_cast<FridgeObject*>(obj.get())) {
                closestFridge = frg;
                break;
            }
        }
        if (closestFridge) {
            transportQueueKitchen.push_back(TransportJob(
                closestFridge->position, closestFridge,
                position, this
            ));
            inQueue = true;
        }
    }

    if (loaded > 0 && cooked == 0 && progress <= 0) {
        // progress complete
        cooked++;
        loaded = 0;
    }

    if (cooked > 0) {
        // output ready, move out
        CounterObject* closestCounter = nullptr;
        for (auto& obj : objectsKitchen) {
            if (CounterObject* cnt = dynamic_cast<CounterObject*>(obj.get())) {
                closestCounter = cnt;
                break;
            }
        }
        if (closestCounter) {
            transportQueueKitchen.push_back(TransportJob(
                position, this,
                closestCounter->position, closestCounter
            ));
            inQueue = true;
        }
    }
}

void CookerObject::tsptJobBegin() {
    std::cout << "COOKER: tsptJobBegin\n";
    cooked = 0;
}

void CookerObject::tsptJobEnd() {
    std::cout << "COOKER: tsptJobEnd\n";
    loaded++;
    jobQueueKitchen.push_back(KitchenJob(position, this));
    progress = 100;
    inQueue = false;
}