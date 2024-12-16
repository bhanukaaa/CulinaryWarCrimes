#include "classes.h"
#include "constants.h"

TransportJob::TransportJob(const Vector2& srcPos, KitchenObject* srcPtr, const Vector2& destPos, KitchenObject* destPtr) {
    src = srcPos;
    dest = destPos;
    srcObject = srcPtr;
    destObject = destPtr;
    srcReached = false;
}