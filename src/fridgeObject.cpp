#include "classes.h"
#include "constants.h"
#include "globals.h"

#include <iostream>

FridgeObject::FridgeObject(Vector2 initPos) : KitchenObject(initPos) {}

void FridgeObject::render() {
    DrawRectangle(position.x + 2, position.y + 2, TILE_SIZE - 4, TILE_SIZE - 4, WHITE);
    DrawText("F", position.x + 5, position.y + 5, 30, BLACK);
}

void FridgeObject::update() {}

void FridgeObject::tsptJobBegin() {
    std::cout << "FRIDGE: tsptJobBegin\n";
}

void FridgeObject::tsptJobEnd() {
    std::cout << "FRIDGE: tsptJobEnd\n";
    inQueue = false;
}