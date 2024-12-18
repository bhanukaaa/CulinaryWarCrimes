#include "constants.h"
#include "classes.h"

KitchenNPC::KitchenNPC(Vector2& initPos) : BaseNPC(initPos) {
    currentJob = nullptr;
}

void KitchenNPC::resetJob() {
    currObject = nullptr;
    currentJob = nullptr;
    currTarget = {-1, -1};
}