#include "classes.h"

JobKitchen::JobKitchen(const Vector2& targetPos, KitchenObject* targetObj) {
    active = true;
    position = targetPos;
    object = targetObj;
}