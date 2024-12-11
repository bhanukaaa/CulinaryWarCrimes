#include "classes.h"
#include "constants.h"

KitchenObject::KitchenObject(Vector2 initPos) {
    position = initPos;
}

void KitchenObject::render() {
    DrawRectangle(position.x, position.y, TILE_SIZE - 4, TILE_SIZE - 4, WHITE);
}