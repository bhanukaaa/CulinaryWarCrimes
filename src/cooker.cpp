#include "classes.h"
#include "constants.h"

Cooker::Cooker(Vector2 initPos) : KitchenObject(initPos) {
    position = initPos;
    progress = 100;
}

void Cooker::render() {
    DrawRectangle(position.x + 2, position.y + 2, TILE_SIZE - 4, TILE_SIZE - 4, WHITE);
    DrawRectangle(position.x + 5, position.y + 12, TILE_SIZE - 10, TILE_SIZE - 17, GRAY);
}