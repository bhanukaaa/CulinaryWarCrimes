#include "classes.h"

BasicJob::BasicJob(KitchenObject* targetObj) {
    active = true;
    targetObject = targetObj;
}