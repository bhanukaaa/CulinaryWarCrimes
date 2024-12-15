#include "classes.h"
#include "constants.h"

#include <iostream>
#include <queue>

extern std::deque<JobKitchen> jobQueueKitchen;

KitchenObject::KitchenObject(Vector2 initPos) {
    position = initPos;
    inQueue = false;
    occupied = false;
}

void KitchenObject::render() {
    DrawRectangle(position.x, position.y, TILE_SIZE - 4, TILE_SIZE - 4, WHITE);
}

void KitchenObject::update() {
    if (!inQueue && !occupied) {
        jobQueueKitchen.push_back(JobKitchen(position, this));
        progress = 100;
        occupied = false;
        inQueue = true;
        std::cout << "Added to Job Queue\n";
    }

    if (!inQueue && progress <= 0) {
        progress = 100;
        std::cout << "Progress Complete\n";
    }
}