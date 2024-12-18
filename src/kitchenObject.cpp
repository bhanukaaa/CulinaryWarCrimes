#include "classes.h"
#include "constants.h"
#include "globals.h"

#include <iostream>

KitchenObject::KitchenObject(Vector2 initPos) {
    position = initPos;
    inQueue = false;
    occupied = false;
}