#include "classes.h"
#include "constants.h"
#include "globals.h"

#include <iostream>

FridgeObj::FridgeObj(Vector2 initPos) : KitchenObj(initPos) {
    balance -= 1200;
}

void FridgeObj::render() {
    DrawRectangle(position.x + 2, position.y + 2, TILE_SIZE - 4, TILE_SIZE - 4, WHITE);
    DrawText("F", position.x + 5, position.y + 5, 30, BLACK);
}

void FridgeObj::update() {}

void FridgeObj::tsptJobBegin() {
    std::cout << "FRIDGE: tsptJobBegin\n";
}

void FridgeObj::tsptJobEnd() {
    std::cout << "FRIDGE: tsptJobEnd\n";
    inQueue = false;
}