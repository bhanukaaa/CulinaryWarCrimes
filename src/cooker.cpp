#include "classes.h"
#include "constants.h"

Cooker::Cooker(Vector2 initPos) : KitchenObject(initPos) {
    position = initPos;
    occupied = false;
    ready = false;
}

void Cooker::render() {
    DrawRectangle(position.x, position.y, TILE_SIZE - 4, TILE_SIZE - 4, WHITE);
    DrawRectangle(position.x + 3, position.y + 10, TILE_SIZE - 10, TILE_SIZE - 17, GRAY);
}