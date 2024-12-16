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
        virtual void tsptJobBegin() {};
        virtual void tsptJobEnd() {};
        virtual void render();
        virtual void update();
};

class CookerObject : public KitchenObject {
    public:
        int loaded;
        int cooked;

        CookerObject(Vector2);
        void tsptJobBegin() override;
        void tsptJobEnd() override;
        void render() override;
        void update() override;
};

class FridgeObject : public KitchenObject {
    public:
        FridgeObject(Vector2);
        void tsptJobBegin() override;
        void tsptJobEnd() override;
        void render() override;
        void update() override;
};

class CounterObject : public KitchenObject {
    public:
        int holding; // temp

        CounterObject(Vector2);
        void tsptJobBegin() override;
        void tsptJobEnd() override;
        void render() override;
        void update() override;
};


// ------------------------------------------------------------------


class Job {
    public:
        Vector2 dest;
        bool active = true;

        virtual void deactivate() { active = false; };
};


class KitchenJob : public Job {
    public:
        KitchenObject* destObject;
        KitchenJob(const Vector2&, KitchenObject*);
};

class TransportJob : public Job {
    public:
        bool inProgress = false;
        bool srcReached = false;
        KitchenObject* destObject;
        KitchenObject* srcObject;
        Vector2 src;
        TransportJob(const Vector2&, KitchenObject*, const Vector2&, KitchenObject*);
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

class KitchenNPC : public BaseNPC {
    public:
        KitchenObject* currObject;
        Job* currentJob;

        KitchenNPC(Vector2&);
        void resetJob();
};

class ChefNPC : public KitchenNPC {
    public:
        ChefNPC(Vector2&);
        void renderNPC() override;
        void jobUpdate();
};

#endif