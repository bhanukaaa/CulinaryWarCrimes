#include "classes.h"
#include "constants.h"
#include "globals.h"

ChairObj::ChairObj(Vector2 initPos, TableObj* linkTable) : DiningObj(initPos) {
    linkedTable = linkTable;
    balance -= 100;
}

void ChairObj::render() {
    DrawCircle(position.x + HALF_TILE_SIZE, position.y + HALF_TILE_SIZE, HALF_TILE_SIZE - 8, BLACK);
    DrawCircle(position.x + HALF_TILE_SIZE, position.y + HALF_TILE_SIZE, HALF_TILE_SIZE - 9, BROWN);
}