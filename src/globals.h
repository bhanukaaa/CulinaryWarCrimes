#ifndef GLOBALS_H
#define GLOBALS_H

#include <raylib.h>
#include <queue>
#include <vector>
#include <memory>

#include "classes.h"
#include "constants.h"

extern Camera2D camera;

extern std::vector<Vector2> blocks;
extern short tileArray[MAP_WIDTH_TILE][MAP_HEIGHT_TILE];

extern std::vector<std::unique_ptr<KitchenNPC>> staffKitchen;
extern std::vector<std::unique_ptr<KitchenObj>> objectsKitchen;
extern std::vector<std::unique_ptr<DiningObj>> objectsDining;

extern std::deque<BasicJob> jobQueueKitchen;
extern std::deque<TransportJob> transportQueueKitchen;

#endif