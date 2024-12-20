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
    if (!currJob) {
        bool tspJobSet = false;
        for (auto& tsp : transportQueueKitchen) {
            if (tsp.inProgress) continue;
            currJob = &tsp;
            tspJobSet = true;
            tsp.inProgress = true;
            break;
        }

        if (!tspJobSet) {
            for (auto& job : jobQueueKitchen) {
                if (job.inProgress) continue;
                currJob = &job;
                job.inProgress = true;
                break;
            }
        }

        if (!currJob) return;
        pathFind(currJob->targetObj->position);
    }

    if (currPath.empty() && CheckCollisionPointRec(position, (Rectangle) {currTarget.x, currTarget.y, TILE_SIZE, TILE_SIZE})) {
        // path finding complete and currly on target
        if (TransportJob* job = dynamic_cast<TransportJob*>(currJob)) {
            if (!job->delivered) { // reached source object
                job->targetObj->tsptJobBegin();
                job->delivered = true;
                pathFind(job->deliveryObj->position);
            }
            else { // reached destination object
                job->deliveryObj->tsptJobEnd();
                job->active = false;
                resetJob();
            }
        }
        else if (BasicJob* job = dynamic_cast<BasicJob*>(currJob)) {
            if (job->progress > 0) job->progress -= 0.5f;
            else {
                job->targetObj->basicJobEnd();
                job->active = false;
                resetJob();
            }
        }
    }
}
