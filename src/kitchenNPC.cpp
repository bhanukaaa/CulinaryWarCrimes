#include "constants.h"
#include "classes.h"

KitchenNPC::KitchenNPC(Vector2& initPos) : BaseNPC(initPos) {
    position = initPos;
}

void KitchenNPC::resetJob() {
    currObject = nullptr;
    currentJob = nullptr;
    currTarget = {-1, -1};
}