#include "constants.h"
#include "classes.h"
#include "globals.h"

#include <raymath.h>
#include <algorithm>
#include <iostream>

using std::vector;
using std::priority_queue;

BaseNPC::BaseNPC(Vector2& initPos) {
    position = initPos;
    velocity = {0, 0};
    currTarget = {-1, -1};
}

void BaseNPC::renderNPC() {
    DrawPoly(position, 8, NPC_RADIUS, 0, DARKBLUE);
}

void BaseNPC::updateNPC() {
    if (!currentPath.empty()) { // following path
        Vector2 targetPosition = currentPath.front();
        targetPosition.x *= TILE_SIZE;
        targetPosition.y *= TILE_SIZE;
        targetPosition.x += HALF_TILE_SIZE;
        targetPosition.y += HALF_TILE_SIZE;
        Vector2 direction = Vector2Normalize(Vector2Subtract(targetPosition, position));

        velocity = Vector2Scale(direction, 100.0f);
        if (Vector2Distance(position, targetPosition) < 5.0f)
            currentPath.erase(currentPath.begin());
        if (currentPath.empty()) velocity = Vector2Scale(velocity, 0.1);
    }
    else if (currTarget.x != -1) { // target set, no path
        Vector2 targetPos = {currTarget.x + HALF_TILE_SIZE, currTarget.y + HALF_TILE_SIZE}; 
        if (Vector2Distance(targetPos, position) > 10) {
            Vector2 direction = Vector2Normalize(Vector2Subtract(targetPos, position));
            acceleration = Vector2Scale(direction, GetRandomValue(10, 30));
        } else {
            acceleration.x = GetRandomValue(-10, 10);
            acceleration.y = GetRandomValue(-10, 10);
        }
        velocity = Vector2Add(velocity, Vector2Scale(acceleration, GetFrameTime()));
        velocity = Vector2Scale(velocity, 0.99);
    }
    else { // free will
        velocity = Vector2Add(velocity, Vector2Scale(acceleration, GetFrameTime()));
        acceleration.x = GetRandomValue(-100, 100);
        acceleration.y = GetRandomValue(-100, 100);
    }

    Vector2 newPos = Vector2Add(position, Vector2Scale(velocity, GetFrameTime()));
    bool colliding = false;

    int tileCoordsX = floor(position.x / TILE_SIZE);
    int tileCoordsY = floor(position.y / TILE_SIZE);
    for (int xOff = -1; xOff < 2; xOff++) {
        for (int yOff = -1; yOff < 2; yOff++) {
            if (xOff == 0 && yOff == 0) continue;
            if (tileArray[tileCoordsX + xOff][tileCoordsY + yOff] == 0) continue;
            Rectangle tile = {(float) (tileCoordsX + xOff) * TILE_SIZE, (float) (tileCoordsY + yOff) * TILE_SIZE, TILE_SIZE, TILE_SIZE};
            if (CheckCollisionCircleRec(newPos, NPC_RADIUS, tile)) {
                colliding = true;
                velocity = Vector2Invert(velocity);
                break;
            }
        }
    }

    if (newPos.x + NPC_RADIUS > MAP_WIDTH || newPos.y + NPC_RADIUS > MAP_HEIGHT || newPos.x - NPC_RADIUS < 0 || newPos.y - NPC_RADIUS < 0) {
        velocity = Vector2Invert(velocity);
        colliding = true;
    }

    if (!colliding) position = newPos;
    else if (!currentPath.empty()) pathFind(currTarget);
}

struct Node {
    Vector2 position;
    float gCost;
    float hCost;
    float fCost() { return gCost + hCost; }
    Node* parent;

    Node(Vector2 pos) : position(pos), gCost(0), hCost(0), parent(nullptr) {}
};

float BaseNPC::heuristic(Vector2& a, Vector2& b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

void BaseNPC::pathFind(Vector2& target) {
    auto compare = [](Node* a, Node* b) {
        return (a->gCost + a->hCost) > (b->gCost + b->hCost);
    };

    priority_queue<Node*, vector<Node*>, decltype(compare)> openList(compare);
    vector<Node*> closedList;
    vector<Vector2> path;

    Vector2 startTile = {floor(position.x / TILE_SIZE), floor(position.y / TILE_SIZE)};
    Vector2 targetTile = {floor(target.x / TILE_SIZE), floor(target.y / TILE_SIZE)};

    if (tileArray[(int) targetTile.x][(int) targetTile.y] !=  0) return;

    Node* startNode = new Node(startTile);
    Node* targetNode = new Node(targetTile);

    openList.push(startNode);

    while (!openList.empty()) {
        Node* current = openList.top();
        openList.pop();

        if (current->position.x == targetNode->position.x && current->position.y == targetNode->position.y) {
            while (current != nullptr) {
                path.push_back(current->position);
                current = current->parent;
            }
            std::reverse(path.begin(), path.end());
            currentPath = path;
            currTarget = target;
            return;
        }

        closedList.push_back(current);

        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (dx == 0 && dy == 0) continue;
                Vector2 neighbor = {current->position.x + dx, current->position.y + dy};

                if (neighbor.x < 0 || neighbor.x >= MAP_WIDTH_TILE || neighbor.y < 0 || neighbor.y >= MAP_HEIGHT_TILE) continue;
                if (tileArray[(int) neighbor.x][(int) neighbor.y] != 0) continue;
                if (dx + dy != 1 && dx + dy != -1) { // diagonal cell
                    if (tileArray[(int) neighbor.x][(int) current->position.y] != 0) continue;
                    if (tileArray[(int) current->position.x][(int) neighbor.y] != 0) continue;
                }

                bool inClosed = false;
                for (Node* n : closedList) {
                    if (n->position.x == neighbor.x && n->position.y == neighbor.y) {
                        inClosed = true;
                        break;
                    }
                }

                if (inClosed) continue;

                Node* neighborNode = new Node(neighbor);
                neighborNode->gCost = current->gCost + 1;
                neighborNode->hCost = heuristic(neighbor, targetTile);
                neighborNode->parent = current;

                bool inOpen = false;
                vector<Node*> tempNodes;

                while (!openList.empty()) {
                    Node* node = openList.top();
                    openList.pop();
                    if (node->position.x == neighbor.x && node->position.y == neighbor.y)
                        inOpen = true;
                    tempNodes.push_back(node);
                }

                for (Node* node : tempNodes)
                    openList.push(node);

                if (!inOpen) openList.push(neighborNode);
            }
        }
    }
    currTarget = {-1, -1};
    currentPath.clear();
}