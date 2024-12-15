#ifndef CLASSES_H
#define CLASSES_H

#include "constants.h"

#include <raylib.h>
#include <vector>


// --------------------------------------------------------------


class KitchenObject {
    public:
        Vector2 position;
        bool occupied;
        bool inQueue;
        float progress;

        KitchenObject(Vector2);
        virtual void render();
        void update();
};

class Cooker : public KitchenObject {
    public:
        short variant;
        Cooker(Vector2);
        void render() override;
};


// ---------------------------------------------------------------


class BaseNPC {
    public:
        Vector2 position;
        Vector2 velocity;
        Vector2 acceleration;
        std::vector<Vector2> currentPath;
        Vector2 currTarget;

        BaseNPC(Vector2&);
        virtual void renderNPC();
        void updateNPC();
        void pathFind(Vector2& target);
        float heuristic(Vector2&, Vector2&);
};

class StaffNPC : public BaseNPC {
    public:
        StaffNPC(Vector2&);
};

class ChefNPC : public StaffNPC {
    public:
        KitchenObject* currObject;
        ChefNPC(Vector2&);
        void renderNPC() override;
        void jobUpdate();
};


// ------------------------------------------------------------------


class Job {
    public:
        Vector2 position;
        bool active = true;
};


class JobKitchen : public Job {
    public:
        KitchenObject* object;
        JobKitchen(const Vector2&, KitchenObject*);
};

#endif