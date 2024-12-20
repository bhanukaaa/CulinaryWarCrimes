#include "constants.h"
#include "classes.h"

KitchenNPC::KitchenNPC(Vector2& initPos) : BaseNPC(initPos) {
    currJob = nullptr;
}

void KitchenNPC::resetJob() {
    currJob = nullptr;
    currTarget = {-1, -1};
}