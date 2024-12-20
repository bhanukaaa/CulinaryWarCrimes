#include "classes.h"
#include "constants.h"
#include "globals.h"

#include <iostream>

KitchenObj::KitchenObj(Vector2 initPos) {
    position = initPos;
    inQueue = false;
    occupied = false;
}