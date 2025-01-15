#ifndef CLASSES_H
#define CLASSES_H

#include "constants.h"

#include <raylib.h>
#include <vector>


// -------------------------------------------------------------


class BaseObj {
public:
    Vector2 position;

    virtual void tsptJobBegin() {};
    virtual void tsptJobEnd() {};
    virtual void basicJobEnd() {};
};


// --------------------------------------------------------------


class KitchenObj : public BaseObj {
public:
    bool occupied;
    bool inQueue; // temp

    KitchenObj(Vector2);

    virtual void render() {};
    virtual void update() {};
};

class CookerObj : public KitchenObj {
public:
    int loaded;
    int cooked;

    CookerObj(Vector2);

    void render() override;
    void update() override;

    void tsptJobBegin() override;
    void tsptJobEnd() override;
    void basicJobEnd() override;
};

class FridgeObj : public KitchenObj {
public:
    FridgeObj(Vector2);

    void render() override;
    void update() override;

    void tsptJobBegin() override;
    void tsptJobEnd() override;
};

class CounterObj : public KitchenObj {
public:
    int holding; // temp

    CounterObj(Vector2);

    void render() override;
    void update() override;

    void tsptJobBegin() override;
    void tsptJobEnd() override;
};


// ------------------------------------------------------------------


class DiningObj : public BaseObj {
public:
    DiningObj(Vector2);

    virtual void update() {};
    virtual void render() {};
};

class ChairObj;
class TableObj : public DiningObj {
public:
    std::vector<ChairObj*> chairs;
    short state = 0;
    short occupied = 0;

    TableObj(Vector2);

    void update() override;
    void render() override;
    void addChair(ChairObj*);
};

class ChairObj : public DiningObj {
public:
    TableObj* linkedTable = nullptr;
    bool occupied = false;

    ChairObj(Vector2, TableObj*);

    void update() override {};
    void render() override;
};


// ------------------------------------------------------------------


class Job {
public:
    Vector2 targetPos;
    BaseObj* targetObj;
    bool active = true;
    bool inProgress = false;

    virtual void deactivate() { active = false; }; // temp, for polymorph
};


class BasicJob : public Job {
public:
    float progress = 100;

    BasicJob(BaseObj*);
};

class TransportJob : public Job {
public:
    bool delivered = false;
    BaseObj* deliveryObj;

    TransportJob(BaseObj*, BaseObj*);
};


// ---------------------------------------------------------------


class BaseNPC {
public:
    Vector2 position;
    Vector2 velocity;
    Vector2 acceleration;
    Vector2 currTarget;
    std::vector<Vector2> currPath;

    BaseNPC() {};
    BaseNPC(Vector2&);

    virtual void render();
    void updateNPC();

    void pathFind(Vector2&);
    float heuristic(Vector2&, Vector2&);
};


// ---------------------------------------------------------


class KitchenNPC : public BaseNPC {
public:
    Job* currJob;

    KitchenNPC(Vector2&);

    void resetJob();
};

class ChefNPC : public KitchenNPC {
public:
    ChefNPC(Vector2&);

    void render() override;
    void jobUpdate();
};


// ---------------------------------------------------------


class CustomerNPC : public BaseNPC {
public:
    DiningObj* currObj;
    bool kill = false;
    short state = 0;
    float timer = 0;

    CustomerNPC();

    virtual void update();
    virtual void render();
};


#endif