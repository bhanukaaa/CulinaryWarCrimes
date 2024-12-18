#include "classes.h"
#include "constants.h"
#include "globals.h"

#include <iostream>

CounterObject::CounterObject(Vector2 initPos) : KitchenObject(initPos) {
    holding = 0;
}

void CounterObject::render() {
    DrawRectangle(position.x + 2, position.y + 2, TILE_SIZE - 4, TILE_SIZE - 4, SKYBLUE);
    DrawRectangle(position.x + 4, position.y + 4, TILE_SIZE - 8, TILE_SIZE - 8, GRAY);
    DrawText(TextFormat("%d", holding), position.x + 6, position.y + 6, 30, BLACK);
}

void CounterObject::update() {}

void CounterObject::tsptJobBegin() {
    std::cout << "COUNTER: tsptJobBegin\n";
}

void CounterObject::tsptJobEnd() {
    std::cout << "COUNTER: tsptJobEnd\n";
    holding++;
    inQueue = false;
}