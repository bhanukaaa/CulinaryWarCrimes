#include "classes.h"

KitchenJob::KitchenJob(const Vector2& targetPos, KitchenObject* targetObj) {
    active = true;
    dest = targetPos;
    destObject = targetObj;
}