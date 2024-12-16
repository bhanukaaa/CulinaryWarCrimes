#ifndef GAME_H
#define GAME_H

#include <raylib.h>
#include <raymath.h>
#include <vector>
#include <thread>
#include <queue>
#include <iostream>

void cameraMovement(short, short);
void place(short, short);
void blockPlacement();
void boxPlacement();
void blockDeletion();
void npcPlacement();
void npcDeletion(int&);
void objectPlacement(int&, int&);
void objectDeletion(int&);
void selectionChange(short&, int&);

#endif