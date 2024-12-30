#include "constants.h"
#include "classes.h"
#include "globals.h"

TableObj::TableObj(Vector2 initPos) : DiningObj(initPos) {
    balance -= 400;
}

void TableObj::render() {
    DrawCircle(position.x + HALF_TILE_SIZE, position.y + HALF_TILE_SIZE, HALF_TILE_SIZE, BLACK);
    DrawCircle(position.x + HALF_TILE_SIZE, position.y + HALF_TILE_SIZE, HALF_TILE_SIZE - 2, BROWN);
}


void TableObj::addChair(ChairObj* newChair) {
    chairs.push_back(newChair);
}