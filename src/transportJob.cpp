#include "classes.h"
#include "constants.h"

TransportJob::TransportJob(BaseObj* srcPtr, BaseObj* delivPtr) {
    targetObj = srcPtr;
    deliveryObj = delivPtr;
    delivered = false;
}