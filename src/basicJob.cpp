#include "classes.h"

BasicJob::BasicJob(KitchenObj* target) {
    active = true;
    targetObj = target;
}