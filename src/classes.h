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
        bool inQueue; // temp

        KitchenObject(Vector2);
        virtual void tsptJobBegin() {};
        virtual void tsptJobEnd() {};
        virtual void basicJobEnd() {};
        virtual void render() {};
        virtual void update() {};
};

class CookerObject : public KitchenObject {
    public:
        int loaded;
        int cooked;

        CookerObject(Vector2);
        void tsptJobBegin() override;
        void tsptJobEnd() override;
        void basicJobEnd() override;
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


class DiningObject {
    public:
        Vector2 position;

        DiningObject(Vector2);
        virtual void update() {};
        virtual void render() {};
};

class TableObject : public DiningObject {
    public:
        short seats;
        short occupied;

        TableObject(Vector2);
        void update() override;
        void render() override;
};


// ------------------------------------------------------------------


class Job {
    public:
        Vector2 targetPos;
        KitchenObject* targetObject;
        bool active = true;
        bool inProgress = false;

        virtual void deactivate() { active = false; }; // temp, for polymorph
};


class BasicJob : public Job {
    public:
        float progress = 100;
        BasicJob(KitchenObject*);
};

class TransportJob : public Job {
    public:
        bool delivered = false;
        KitchenObject* deliveryObject;
        TransportJob(KitchenObject*, KitchenObject*);
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


// ---------------------------------------------------------


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


// ---------------------------------------------------------


// class CustomerNPC : public BaseNPC {
//     public:
//         CustomerNPC(Vector2);
//         virtual void render();
// };


#endif