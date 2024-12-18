#include "constants.h"
#include "classes.h"
#include "globals.h"

TableObject::TableObject(Vector2 initPos) : DiningObject(initPos) {
    seats = 4;
    occupied = 0;
}

void TableObject::render() {
    DrawCircle(position.x + HALF_TILE_SIZE, position.y + HALF_TILE_SIZE, HALF_TILE_SIZE, BLACK);
    DrawCircle(position.x + HALF_TILE_SIZE, position.y + HALF_TILE_SIZE, HALF_TILE_SIZE - 2, BROWN);
}

void TableObject::update() {}