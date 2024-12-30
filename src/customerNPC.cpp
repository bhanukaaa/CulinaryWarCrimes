#include "classes.h"
#include "constants.h"
#include "globals.h"

CustomerNPC::CustomerNPC() {
    position = {NPC_RADIUS + 2, MAP_HEIGHT - TILE_SIZE * 3};
    currTarget = {MAP_WIDTH, MAP_HEIGHT - TILE_SIZE * 3};
}

void CustomerNPC::render() {
    DrawPoly(position, 3, NPC_RADIUS, 30, BLACK);
    DrawPoly(position, 3, NPC_RADIUS - 2, 30, RED);
    DrawCircle(position.x, position.y - 18, 9, BLACK);
    DrawCircle(position.x, position.y - 18, 8, {255, 229, 180, 255});
}

void CustomerNPC::update() {
    if (position.x >= MAP_WIDTH - NPC_RADIUS - 3 && position.y > MAP_HEIGHT - TILE_SIZE * 6)
        kill = true;
}