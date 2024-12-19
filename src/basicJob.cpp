#include "classes.h"

BasicJob::BasicJob(KitchenObject* target) {
    active = true;
    targetObject = target;
}