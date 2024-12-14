#include "constants.h"
#include "classes.h"

#include <raymath.h>
#include <algorithm>
#include <queue>

extern short tileArray[MAP_WIDTH / TILE_SIZE][MAP_HEIGHT / TILE_SIZE];

struct Node {
    Vector2 position;
    float gCost;
    float hCost;
    float fCost() { return gCost + hCost; }
    Node* parent;

    Node(Vector2 pos) : position(pos), gCost(0), hCost(0), parent(nullptr) {}
};

BaseNPC::BaseNPC(Vector2& initPos) {
    position = initPos;
    velocity = {(float) GetRandomValue(-1, 1), -10};
    currTarget = {-1, -1};
}

void BaseNPC::renderNPC() {
    DrawPoly(position, 8, NPC_RADIUS, 0, DARKBLUE);
}

float BaseNPC::heuristic(Vector2& a, Vector2& b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

void BaseNPC::pathFind(Vector2& target) {
    auto compare = [](Node* a, Node* b) {
        return (a->gCost + a->hCost) > (b->gCost + b->hCost);
    };

    std::priority_queue<Node*, std::vector<Node*>, decltype(compare)> openList(compare);
    std::vector<Node*> closedList;
    std::vector<Vector2> path;

    Vector2 startTile = {floor(position.x / TILE_SIZE), floor(position.y / TILE_SIZE)};
    Vector2 targetTile = {floor(target.x / TILE_SIZE), floor(target.y / TILE_SIZE)};

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
            return;
        }

        closedList.push_back(current);

        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (dx == 0 && dy == 0) continue;

                Vector2 neighbor = {current->position.x + dx, current->position.y + dy};

                if (neighbor.x < 0 || neighbor.x >= MAP_WIDTH / TILE_SIZE || neighbor.y < 0 || neighbor.y >= MAP_HEIGHT / TILE_SIZE) continue;
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
                std::vector<Node*> tempNodes;

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
    currentPath.clear();
}

void BaseNPC::updateNPC(std::vector<Vector2>& surrounding) {
    if (!currentPath.empty()) {
        Vector2 targetPosition = currentPath.front();
        targetPosition.x *= TILE_SIZE;
        targetPosition.y *= TILE_SIZE;
        targetPosition.x += TILE_SIZE / 2;
        targetPosition.y += TILE_SIZE / 2;
        Vector2 direction = Vector2Normalize(Vector2Subtract(targetPosition, position));

        velocity = Vector2Scale(direction, 100.0f);
        if (Vector2Distance(position, targetPosition) < 5.0f)
            currentPath.erase(currentPath.begin());
        if (currentPath.empty()) velocity = {0, 0};
    }
    else if (currTarget.x != -1) {
        if (Vector2Distance(currTarget, position) > 10) {
            Vector2 direction = Vector2Normalize(Vector2Subtract(currTarget, position));
            acceleration = Vector2Scale(direction, GetRandomValue(10, 30));
        } else {
            acceleration.x = GetRandomValue(-5, 5);
            acceleration.y = GetRandomValue(-5, 5);
        }
        velocity = Vector2Add(velocity, Vector2Scale(acceleration, GetFrameTime()));
    }
    else {
        velocity = Vector2Add(velocity, Vector2Scale(acceleration, GetFrameTime()));
        acceleration.x = GetRandomValue(-100, 100);
        acceleration.y = GetRandomValue(-100, 100);
    }

    Vector2 newPos = Vector2Add(position, Vector2Scale(velocity, GetFrameTime()));
    bool colliding = false;
    for (size_t s = 0; s < surrounding.size(); s++) {
        if (CheckCollisionCircleRec(newPos, NPC_RADIUS, (Rectangle) {surrounding[s].x * TILE_SIZE, surrounding[s].y * TILE_SIZE, TILE_SIZE, TILE_SIZE})) {
            colliding = true;
            velocity = Vector2Invert(velocity);
            break;
        }
    }

    if (newPos.x + NPC_RADIUS > MAP_WIDTH || newPos.y + NPC_RADIUS > MAP_HEIGHT || newPos.x - NPC_RADIUS < 0 || newPos.y - NPC_RADIUS < 0) {
        velocity = Vector2Invert(velocity);
        colliding = true;
    }

    if (!colliding) position = newPos;
}
