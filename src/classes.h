#ifndef CLASSES_H
#define CLASSES_H

#include "constants.h"

#include <raylib.h>
#include <vector>


// --------------------------------------------------------------


class KitchenObject {
    public:
        Vector2 position;

        KitchenObject(Vector2);
        virtual void render();
};

class Cooker : public KitchenObject {
    public:
        short type;
        bool occupied;
        bool ready;
        Cooker(Vector2);
        void render() override;
};


// ---------------------------------------------------------------


class BaseNPC {
    public:
        Vector2 position;
        Vector2 velocity;
        Vector2 acceleration;
        bool kill = false;
        std::vector<Vector2> currentPath;
        Vector2 currTarget;

        BaseNPC(Vector2&);
        virtual void renderNPC();
        void updateNPC(std::vector<Vector2>&);
        void pathFind(Vector2& target, short tileArray[MAP_WIDTH / TILE_SIZE][MAP_HEIGHT / TILE_SIZE]);
        float heuristic(Vector2&, Vector2&);
};

class StaffNPC : public BaseNPC {
    public:
        StaffNPC(Vector2&);
};

class ChefNPC : public StaffNPC {
    public:
        bool targetSet;
        ChefNPC(Vector2&);
        void renderNPC() override;
        void jobUpdate(short tileArray[MAP_WIDTH / TILE_SIZE][MAP_HEIGHT / TILE_SIZE]);
};


#endif