#include "classes.h"
#include "constants.h"

#include <raymath.h>
#include <iostream>
#include <thread>
#include <queue>

using std::vector;
using std::unique_ptr;
using std::deque;

extern vector<unique_ptr<KitchenObject>> objectsKitchen;
extern short tileArray[MAP_WIDTH_TILE][MAP_HEIGHT_TILE];
extern deque<KitchenJob> jobQueueKitchen;
extern deque<TransportJob> transportQueueKitchen;

ChefNPC::ChefNPC(Vector2& initPos) : KitchenNPC(initPos) {
    position = initPos;
    currentJob = nullptr;
}

void ChefNPC::renderNPC() {
    DrawPoly(position, 8, NPC_RADIUS, 5, BLACK);
    DrawPoly(position, 8, NPC_RADIUS - 2, 5, RAYWHITE);
}

void ChefNPC::jobUpdate() {
    if (currTarget.x == -1 && !currentJob) {
        bool tspJobSet = false;
        for (auto& tsp : transportQueueKitchen) {
            if (!tsp.active) continue;
            if (tsp.inProgress) continue;
            currObject = tsp.srcObject;
            currTarget = tsp.src;
            tsp.inProgress = true;
            tspJobSet = true;
            currentJob = &tsp;
            break;
        }

        if (!tspJobSet) {
            for (auto& job : jobQueueKitchen) {
                if (!job.active) continue;
                currObject = job.destObject;
                currTarget = job.dest;
                currObject->occupied = true;
                job.active = false;
                break;
            }
        }
    }

    if (currTarget.x != -1 && currentPath.empty()) {
        if (!CheckCollisionPointRec(position, (Rectangle) {currTarget.x, currTarget.y, TILE_SIZE, TILE_SIZE})) {
            pathFind(currTarget);
        } else {
            if (currentJob) {
                TransportJob* tspJob = dynamic_cast<TransportJob*>(currentJob);
                if (!tspJob->srcReached) {
                    currObject->tsptJobBegin();
                    currTarget = tspJob->dest;
                    currObject = tspJob->destObject;
                    tspJob->srcReached = true;
                } else {
                    currObject->tsptJobEnd();
                    tspJob->active = false;
                    tspJob->srcObject->inQueue = false;
                    resetJob();
                }
            } else {
                currObject->progress -= 0.5f;
                if (currObject->progress <= 0) {
                    currObject->inQueue = false;
                    resetJob();
                }
            }
        }
    }
}
