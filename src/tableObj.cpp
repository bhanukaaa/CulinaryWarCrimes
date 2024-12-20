#include "constants.h"
#include "classes.h"
#include "globals.h"

TableObj::TableObj(Vector2 initPos) : DiningObj(initPos) {
    seats = 4;
    occupied = 0;
}

void TableObj::render() {
    DrawCircle(position.x + HALF_TILE_SIZE, position.y + HALF_TILE_SIZE, HALF_TILE_SIZE, BLACK);
    DrawCircle(position.x + HALF_TILE_SIZE, position.y + HALF_TILE_SIZE, HALF_TILE_SIZE - 2, BROWN);
}

void TableObj::update() {}