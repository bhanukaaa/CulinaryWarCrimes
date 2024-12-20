#include "classes.h"
#include "constants.h"

TransportJob::TransportJob(KitchenObj* srcPtr, KitchenObj* delivPtr) {
    targetObj = srcPtr;
    deliveryObj = delivPtr;
    delivered = false;
}