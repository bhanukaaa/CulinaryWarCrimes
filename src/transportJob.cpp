#include "classes.h"
#include "constants.h"

TransportJob::TransportJob(KitchenObject* srcPtr, KitchenObject* delivPtr) {
    targetObject = srcPtr;
    deliveryObject = delivPtr;
    delivered = false;
}