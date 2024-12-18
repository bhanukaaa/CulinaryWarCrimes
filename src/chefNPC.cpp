#include "constants.h"
#include "classes.h"
#include "globals.h"

#include <raymath.h>
#include <iostream>

using std::vector;
using std::unique_ptr;
using std::deque;

ChefNPC::ChefNPC(Vector2& initPos) : KitchenNPC(initPos) {}

void ChefNPC::renderNPC() {
    DrawPoly(position, 8, NPC_RADIUS, 5, BLACK);
    DrawPoly(position, 8, NPC_RADIUS - 2, 5, RAYWHITE);
}

void ChefNPC::jobUpdate() {
    if (!currentJob) {
        bool tspJobSet = false;
        for (auto& tsp : transportQueueKitchen) {
            if (tsp.inProgress) continue;
            currentJob = &tsp;
            tspJobSet = true;
            tsp.inProgress = true;
            break;
        }

        if (!tspJobSet) {
            for (auto& job : jobQueueKitchen) {
                if (job.inProgress) continue;
                currentJob = &job;
                job.inProgress = true;
                break;
            }
        }

        if (!currentJob) return;
        pathFind(currentJob->targetObject->position);
    }

    if (currentPath.empty() && CheckCollisionPointRec(position, (Rectangle) {currTarget.x, currTarget.y, TILE_SIZE, TILE_SIZE})) {
        // path finding complete and currently on target
        if (TransportJob* job = dynamic_cast<TransportJob*>(currentJob)) {
            if (!job->delivered) { // reached source object
                job->targetObject->tsptJobBegin();
                job->delivered = true;
                pathFind(job->deliveryObject->position);
            }
            else { // reached destination object
                job->deliveryObject->tsptJobEnd();
                job->active = false;
                resetJob();
            }
        }
        else if (BasicJob* job = dynamic_cast<BasicJob*>(currentJob)) {
            if (job->progress > 0) job->progress -= 0.5f;
            else {
                job->targetObject->basicJobEnd();
                job->active = false;
                resetJob();
            }
        }
    }
}
