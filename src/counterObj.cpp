#include "classes.h"
#include "constants.h"
#include "globals.h"

#include <iostream>

CounterObj::CounterObj(Vector2 initPos) : KitchenObj(initPos) {
    holding = 0;
    balance -= 500;
}

void CounterObj::render() {
    DrawRectangle(position.x + 2, position.y + 2, TILE_SIZE - 4, TILE_SIZE - 4, SKYBLUE);
    DrawRectangle(position.x + 4, position.y + 4, TILE_SIZE - 8, TILE_SIZE - 8, GRAY);
    DrawText(TextFormat("%d", holding), position.x + 6, position.y + 6, 30, BLACK);
}

void CounterObj::update() {}

void CounterObj::tsptJobBegin() {
    std::cout << "COUNTER: tsptJobBegin\n";
}

void CounterObj::tsptJobEnd() {
    std::cout << "COUNTER: tsptJobEnd\n";
    holding++;
    inQueue = false;
}